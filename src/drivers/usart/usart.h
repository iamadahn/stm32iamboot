#ifndef USART_H_
#define USART_H_

#include "stm32f1xx_ll_usart.h"

typedef struct {
    USART_TypeDef* instance;
    uint32_t baudrate;
    uint32_t periph_clock_freq;
} usart_t;

void usart_config(usart_t* usart);
uint8_t usart_blck_transmit(usart_t* usart, void* data, uint32_t size, uint16_t timeout);
uint8_t usart_blck_receive(usart_t* usart, void* buf, uint32_t size, uint16_t timeout);

#endif
