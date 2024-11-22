#include "bsp.h"
#include "rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_usart.h"

void
bsp_init(void) {

    /* Configure RCC to use HSE and get 72 MHz */
    rcc_config();

    NVIC_SetPriorityGrouping(3U);
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

    /* Enable LEDS on GPIOC port*/
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_InitTypeDef uart1_pins = {
        .Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_UP,
    };

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_GPIO_Init(GPIOA, &uart1_pins);

    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_FLOATING);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
}
