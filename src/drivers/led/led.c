#include "led.h"

void led_toggle(struct led *self)
{
    if(self->state) {
        led_off(self);
    } else {
        led_on(self);
    }
}

void led_on(struct led *self)
{
    LL_GPIO_SetOutputPin(self->port, self->pin);
    self->state = 1;
}

void led_off(struct led *self)
{
    LL_GPIO_ResetOutputPin(self->port, self->pin);
    self->state = 0;
}
