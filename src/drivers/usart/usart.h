#ifndef USART_H_
#define USART_H_

#include <stm32f1xx_ll_usart.h>

struct usart_config {
    USART_TypeDef *self;
    uint32_t apb_bus_freq;
    uint32_t baudrate;
    uint32_t transfer_direction;
    uint32_t data_width;
    uint32_t parity;
    uint32_t stopbits;
    uint32_t irq_priority;
    uint8_t async_mode;
    uint8_t irq_mode;
};

int8_t usart_init(struct usart_config *usart);
int8_t usart_block_transmit(struct usart_config *usart, void *data, uint32_t size, uint32_t timeout);
int8_t usart_block_receive(struct usart_config *usart, void *buf, uint32_t size, uint32_t timeout);

void usart1_irq_handler(void);
void usart2_irq_handler(void);
void usart3_irq_handler(void);

#endif
