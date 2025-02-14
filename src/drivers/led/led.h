#ifndef LED_H_
#define LED_H_

#include <stm32f1xx_ll_gpio.h>

struct led {
    GPIO_TypeDef* port;
    uint32_t pin;
    uint8_t state;
};

void led_toggle(struct led *self);
void led_on(struct led *self);
void led_off(struct led *self);

#endif
