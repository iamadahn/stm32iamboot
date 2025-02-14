#include "bsp.h"
#include "rcc.h"
#include "dwt.h"

#include <stm32f1xx_ll_system.h>

void bsp_init(void)
{
    /* Setting flash memory latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

    /* Veryfing that flash memory latency was set correctly */
    if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2) {
        while (1);
    }

    rcc_config();

    NVIC_SetPriorityGrouping(3U);
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

    dwt_init();
}
