#include "gpio.h"

#include <stm32f1xx_ll_bus.h>

#include <stddef.h>

static int8_t gpio_port_clock_enable(GPIO_TypeDef *port);

int8_t gpio_output_init(GPIO_TypeDef *port, uint32_t pin_number)
{
    if (port == NULL)
        return -1;

    if (gpio_port_clock_enable(port) != 0)
        return -2;

    LL_GPIO_SetPinMode(port, pin_number, LL_GPIO_MODE_OUTPUT);
    return 0;
}

int8_t gpio_usart_tx_init(GPIO_TypeDef *port, uint32_t pin_number)
{
    if (port == NULL)
        return -1;

    if (gpio_port_clock_enable(port) != 0)
        return -2;

    LL_GPIO_SetPinMode(port, pin_number, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(port, pin_number, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(port, pin_number, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(port, pin_number, LL_GPIO_PULL_UP);

    return 0;
}

int8_t gpio_usart_rx_init(GPIO_TypeDef *port, uint32_t pin_number)
{
    if (port == NULL)
        return -1;

    if (gpio_port_clock_enable(port) != 0)
        return -2;
    
    LL_GPIO_SetPinMode(port, pin_number, LL_GPIO_MODE_FLOATING);
    LL_GPIO_SetPinSpeed(port, pin_number, LL_GPIO_SPEED_FREQ_HIGH);

    return 0;
}

static int8_t gpio_port_clock_enable(GPIO_TypeDef *port)
{
    switch ((intptr_t)port) {
        case (intptr_t)GPIOA:
            if (!LL_APB2_GRP1_IsEnabledClock(LL_APB2_GRP1_PERIPH_GPIOA))
                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
            break;

        case (intptr_t)GPIOB:
            if (!LL_APB2_GRP1_IsEnabledClock(LL_APB2_GRP1_PERIPH_GPIOB))
                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
            break;

        case (intptr_t)GPIOC:
            if (!LL_APB2_GRP1_IsEnabledClock(LL_APB2_GRP1_PERIPH_GPIOC))
                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
            break;

        case (intptr_t)GPIOD:
            if (!LL_APB2_GRP1_IsEnabledClock(LL_APB2_GRP1_PERIPH_GPIOD))
                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
            break;

        case (intptr_t)GPIOE:
            if (!LL_APB2_GRP1_IsEnabledClock(LL_APB2_GRP1_PERIPH_GPIOE))
                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOE);
            break;

        default:
            return -2;
            break;
    }
    
    return 0;
}
