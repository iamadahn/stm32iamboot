#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "bsp.h"
#include "dwt.h"
#include "drivers/gpio/gpio.h"
#include "drivers/led/led.h"
#include "drivers/usart/usart.h"
#include "drivers/flash/flash.h"
#include "iamboot/protocol.h"
#include "iamboot/checksum.h"

#include <stm32f1xx_ll_utils.h>

#include <string.h>

int main(void)
{
    /* Perform hardware initialisation */
    bsp_init();  

    struct led led_heartbeat = {
        .port = GPIOC,
        .pin = LL_GPIO_PIN_13,
        .state = 0,
    };

    gpio_output_init(led_heartbeat.port, led_heartbeat.pin);

    struct usart_config usart_dev = {
        .self = USART1,
        .apb_bus_freq = 64000000,
        .baudrate = 115200,
        .transfer_direction = LL_USART_DIRECTION_TX_RX,
        .data_width = LL_USART_DATAWIDTH_8B,
        .parity = LL_USART_PARITY_NONE,
        .stopbits = LL_USART_STOPBITS_1,
        .async_mode = 0,
        .irq_mode = 0,
    };
    usart_init(&usart_dev);

    flash_full_erase();

    uint8_t buf[TOTAL_MSG_LENGTH];
    uint32_t number_of_packets = 0;
    while (iamboot_handshake_serial_rx(&usart_dev, &number_of_packets, 100) != 0) {
        led_toggle(&led_heartbeat);
    }
    iamboot_handshake_serial_tx(&usart_dev, &number_of_packets, 1000);
 
    flash_unlock();

    uint32_t addr = 0x8000000 + (1024 * 4);
    for (uint32_t current_packet_number = 0; current_packet_number < number_of_packets; current_packet_number++) {
        usart_block_receive(&usart_dev, buf, TOTAL_MSG_LENGTH, 1000);
        if (checksum_valid(buf, TOTAL_MSG_LENGTH) != 0) {
            while (1);
        }

        uint8_t firmware_bytes[FIRMWARE_BYTES_LENGTH];
        memcpy(firmware_bytes, &buf[2], FIRMWARE_BYTES_LENGTH);
        for (uint8_t i = 0; i < FIRMWARE_BYTES_LENGTH; i+=2) {
            uint16_t* data_ptr;
            data_ptr = (uint16_t*)&firmware_bytes[i];
            flash_program(addr, *data_ptr);
            addr+=2;
        }

        if (iamboot_ack_serial_tx(&usart_dev) != 0) {
            while (1);
        }
    }

    flash_lock();


    /*

    uint32_t app_start_address = *(uint32_t*)(0x8004000 + 4);

    typedef void (*boot_jump)(void);
    boot_jump app_start;

    LL_mDelay(100);
    __disable_irq();

    app_start = (boot_jump)app_start_address;
    __set_MSP(*(uint32_t*) app_start_address - 4);
    app_start();
    
    */

    while (1) {
        led_toggle(&led_heartbeat);
        LL_mDelay(100);
    }

    return 0;
}

int8_t iamboot_serial_tx(void *pv_arg, void *buf, uint32_t len, uint32_t timeout_ms)
{
    if (pv_arg == NULL)
        return -1;

    struct usart_config *usart_dev = (struct usart_config *)pv_arg;

    int8_t ret = usart_block_transmit(usart_dev, buf, len, timeout_ms);
    return ret;
}

int8_t iamboot_serial_rx(void *pv_arg, void *buf, uint32_t len, uint32_t timeout_ms)
{
    if (pv_arg == NULL)
        return -1;

    struct usart_config *usart_dev = (struct usart_config *)pv_arg;

    int8_t ret = usart_block_receive(usart_dev, buf, len, timeout_ms);
    return ret;
}
#pragma GCC pop_options
