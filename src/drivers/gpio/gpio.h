#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f1xx_ll_gpio.h"

#include <stdint.h>

int8_t gpio_output_init(GPIO_TypeDef *port, uint32_t pin_number);
int8_t gpio_usart_tx_init(GPIO_TypeDef *port, uint32_t pin_number);
int8_t gpio_usart_rx_init(GPIO_TypeDef *port, uint32_t pin_number);

#endif
