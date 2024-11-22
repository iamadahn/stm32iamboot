#include "usart.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_utils.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "dwt.h"

void
usart_config(usart_t* usart) {
    LL_USART_SetTransferDirection(usart->instance, LL_USART_DIRECTION_TX_RX);
    LL_USART_ConfigCharacter(usart->instance, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
    LL_USART_SetBaudRate(usart->instance, usart->periph_clock_freq, usart->baudrate); 
    LL_USART_Enable(usart->instance);
}

uint8_t
usart_blck_transmit(usart_t* usart, void* data, uint32_t size, uint16_t timeout) {
    if (data == NULL) {
        return 1;
    }

    uint8_t* msg = (uint8_t*)data;

    uint32_t timeout_counter = 0, timeout_usec = timeout * 1000;

    for (uint32_t i = 0; i < size; i++) {

        while (!LL_USART_IsActiveFlag_TXE(USART1)) {
            DWT_Delay(1);
            if (++timeout_counter == timeout_usec) {
                return 1;
            }
        }

        LL_USART_TransmitData8(USART1, msg[i]);
    }

    return 0;
}

uint8_t
usart_blck_receive(usart_t* usart, void* buf, uint32_t size, uint16_t timeout) {
    if (buf == NULL) {
        return 1;
    }

    uint8_t* msg = (uint8_t*)buf;

    uint32_t timeout_counter = 0, timeout_usec = timeout * 1000;

    for (uint32_t i = 0; i < size; i++) {
        while (!LL_USART_IsActiveFlag_RXNE(USART1)) {
            DWT_Delay(1);
            if (++timeout_counter == timeout_usec) {
                return 1;
            }
        }
        msg[i] = LL_USART_ReceiveData8(USART1);
    }

    return 0;
}
