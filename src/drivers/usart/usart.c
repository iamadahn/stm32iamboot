#include "usart.h"
#include "dwt.h"

#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_usart.h>

#include <stddef.h>

static int8_t usart_async_fduplex_init(struct usart_config *usart);

int8_t usart_init(struct usart_config *usart)
{
    if (usart == NULL)
        return -1;

    if (!usart->async_mode)
        return -2; // Not supported
    
    int8_t ret = usart_async_fduplex_init(usart);
    if (ret != 0)
        return ret;

    if (usart->irq_mode) {
        switch ((intptr_t)usart->self) {
            case (intptr_t)USART1:
                NVIC_SetPriority(USART1_IRQn, usart->irq_priority);
                NVIC_EnableIRQ(USART1_IRQn);
                LL_USART_EnableIT_RXNE(USART1);
                break;

            case (intptr_t)USART2:
                NVIC_SetPriority(USART2_IRQn, usart->irq_priority);
                NVIC_EnableIRQ(USART2_IRQn);
                LL_USART_EnableIT_RXNE(USART2);
                break;

            case (intptr_t)USART3:
                NVIC_SetPriority(USART3_IRQn, usart->irq_priority);
                NVIC_EnableIRQ(USART3_IRQn);
                LL_USART_EnableIT_RXNE(USART3);
                break;
        }
    }

    return ret;
}

int8_t usart_async_fduplex_init(struct usart_config *usart)
{
    if (usart->self == NULL)
        return -1;

    switch ((intptr_t)usart->self) {
        case (intptr_t)USART1:
            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
            break;

        case (intptr_t)USART2:
            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
            break;

        case (intptr_t)USART3:
            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
            break;

        default:
            return -1;
            break;
    }

    LL_USART_SetTransferDirection(usart->self, usart->transfer_direction);
    LL_USART_ConfigCharacter(usart->self, usart->data_width, usart->parity, usart->stopbits);
    LL_USART_SetBaudRate(usart->self, usart->apb_bus_freq, usart->baudrate);
    LL_USART_ConfigAsyncMode(usart->self);
    LL_USART_Enable(usart->self);

    (void)usart->self->DR;

    return 0;
}

int8_t usart_block_transmit(struct usart_config *usart, void *data, uint32_t size, uint32_t timeout)
{
    if (data == NULL)
        return -1;

    uint8_t *msg = (uint8_t*)data;
    uint32_t timeout_counter = 0;

    for (uint32_t i = 0; i < size; i++) {
        while (!LL_USART_IsActiveFlag_TXE(usart->self)) {
            dwt_delay(1);
            if (timeout_counter > timeout * 1000)
                return 1;
            timeout_counter++;
        }

        LL_USART_TransmitData8(usart->self, msg[i]);
        while (!LL_USART_IsActiveFlag_TC(usart->self));
    }

    return 0;
}

int8_t usart_block_receive(struct usart_config *usart, void *buf, uint32_t size, uint32_t timeout)
{
    if (buf == NULL)
        return -1;

    uint8_t *msg = (uint8_t*)buf;

    uint32_t timeout_counter = 0;

    for (uint32_t i = 0; i < size; i++) {
        while (!LL_USART_IsActiveFlag_RXNE(usart->self)) {
            dwt_delay(1);
            if (timeout_counter > timeout * 1000)
                return 1;
            timeout_counter++;
        }

        msg[i] = LL_USART_ReceiveData8(usart->self);
    }

    return 0;
}

__attribute__((weak)) void usart1_irq_handler(void){}
__attribute__((weak)) void usart2_irq_handler(void){}
__attribute__((weak)) void usart3_irq_handler(void){}

#ifndef USE_HAL_DRIVER
void USART1_IRQHandler(void)
{
    usart1_irq_handler();
}

void USART2_IRQHandler(void)
{
    usart2_irq_handler();
}

void USART3_IRQHandler(void)
{
    usart3_irq_handler();
}
#endif

