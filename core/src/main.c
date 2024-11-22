#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "bsp.h"
#include "drivers/led/led.h"
#include "dwt.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_utils.h"
#include "usart.h"
#include "flash.h"
#include "protocol.h"
#include "checksum.h"
#include "error_handlers.h"
#include "string.h"
        

led_t led0 = {
    .port = GPIOC,
    .pin = LL_GPIO_PIN_13,
    .state = 0,
};

int
main(void) {
    /* Perform hardware initialisation */
    bsp_init();  

    DWT_Init();

    usart_t usart = {
        .instance = USART1,
        .baudrate = 115200,
        .periph_clock_freq = SystemCoreClock,
    };
    usart_config(&usart);

    flash_full_erase();

    uint8_t buf[TOTAL_MSG_LENGTH];
    uint32_t number_of_packets = 0;
    while (handshake_receive_serial(&usart, &number_of_packets) != 0) {
        ;
    }
    handshake_send_serial(&usart);
 
    flash_unlock();

    uint32_t addr = 0x8000000 + (1024 * 4);
    for (uint32_t current_packet_number = 0; current_packet_number < number_of_packets; current_packet_number++) {
        usart_blck_receive(&usart, buf, TOTAL_MSG_LENGTH, 1000);
        if (checksum_valid(buf, TOTAL_MSG_LENGTH) != 0) {
            error_handler();
        }

        uint8_t firmware_bytes[FIRMWARE_BYTES_LENGTH];
        memcpy(firmware_bytes, &buf[2], FIRMWARE_BYTES_LENGTH);
        for (uint8_t i = 0; i < FIRMWARE_BYTES_LENGTH; i+=2) {
            uint16_t* data_ptr;
            data_ptr = (uint16_t*)&firmware_bytes[i];
            flash_program(addr, *data_ptr);
            addr+=2;
        }

        if (ack_send_serial(&usart) != 0) {
            error_handler();
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
        led_toggle(&led0);
        LL_mDelay(100);
    }

    return 0;
}

#pragma GCC pop_options
