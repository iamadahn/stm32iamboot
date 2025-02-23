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

#pragma GCC push_options
#pragma GCC optimize ("O0")
static void start_application(void);

static void start_application(void)
{
    typedef void (*jump)(void);
    jump jump_to_app;

    LL_mDelay(100);
    __disable_irq();

    uint32_t *app_address = (uint32_t *)FLASH_APP_START_ADDR;
    jump_to_app = (jump)*(app_address + 1);
    __set_MSP(*app_address);
    SCB->VTOR = FLASH_APP_START_ADDR;
    __enable_irq();
    jump_to_app();
}
#pragma GCC pop_options

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
    gpio_usart_tx_init(GPIOA, LL_GPIO_PIN_9);
    gpio_usart_rx_init(GPIOA, LL_GPIO_PIN_10);

    struct usart_config usart_dev = {
        .self = USART1,
        .apb_bus_freq = 64000000,
        .baudrate = 115200,
        .transfer_direction = LL_USART_DIRECTION_TX_RX,
        .data_width = LL_USART_DATAWIDTH_8B,
        .parity = LL_USART_PARITY_NONE,
        .stopbits = LL_USART_STOPBITS_1,
        .async_mode = 1,
        .irq_mode = 0,
    };
    usart_init(&usart_dev);


    int8_t ret = -1;
    uint32_t number_of_packets = 0;
    for (uint8_t i = 0; i < 10; i++) {
        if ((ret = iamboot_handshake_serial_rx(&usart_dev, &number_of_packets, 100)) != 0)  
            led_toggle(&led_heartbeat);
        else
            break;
    }

    if (ret != 0)
        start_application();

    flash_full_erase();

    iamboot_handshake_serial_tx(&usart_dev, &number_of_packets, 1000);
 
    flash_unlock();

    uint8_t buf[TOTAL_MSG_LENGTH];
    uint32_t addr = FLASH_BOOT_START_ADDR + (1024 * 4);
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

    bsp_deinit();

    start_application();

    while (1) {
        led_toggle(&led_heartbeat);
        LL_mDelay(500);
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

