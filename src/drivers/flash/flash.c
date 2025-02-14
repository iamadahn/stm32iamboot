#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "stdint.h"
#include "flash.h"
#include "dwt.h"

static uint8_t flash_program_halfword(uint32_t addr, uint16_t data);
static uint8_t flash_is_locked(void);
static uint8_t flash_is_busy(void);

static uint8_t
flash_program_halfword(uint32_t addr, uint16_t data) {
    if (flash_is_locked())
        return 1;

    while (flash_is_busy());

    FLASH->CR |= FLASH_CR_PG;

    *(__IO uint16_t*) addr = data;

    while (flash_is_busy());

    FLASH->CR &= ~(FLASH_CR_PG);

    if (FLASH->SR & FLASH_SR_WRPRTERR) {
        FLASH->SR |= FLASH_SR_WRPRTERR;
        return 1;
    }
    

    if (FLASH->SR & FLASH_SR_PGERR) {
        FLASH->SR |= FLASH_SR_PGERR;
        return 1;
    }

    return 0;
}

static uint8_t flash_is_locked(void)
{
    return FLASH->CR & FLASH_CR_LOCK;
}

static uint8_t flash_is_busy(void)
{
    return FLASH->SR & FLASH_SR_BSY;
}

void
flash_lock(void) {
    FLASH->CR |= FLASH_CR_LOCK;
}

uint8_t
flash_unlock(void) {
    if (!flash_is_locked()) {
        return 0;
    }

    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    return flash_is_locked();
}

uint8_t
flash_ready_wait(uint16_t timeout) {
    uint32_t timeout_counter = 0, timeout_usec = timeout * 1000;
    
    while ((FLASH->SR & FLASH_FLAG_BSY) != RESET) {
        dwt_delay(1);
        if (++timeout_counter == timeout_usec) {
            return 1;
        }
    }

    /* Chech Flash End of Operation flag and reset it if necessary */
    if ((FLASH->SR & FLASH_FLAG_EOP) != RESET) {
        FLASH->SR = FLASH_FLAG_EOP;
    }

    return 0;
}

void
flash_full_erase(void) {
   for (uint8_t i = 4; i < FLASH_PAGES; i++) {
       flash_page_erase(i);
   } 
}

uint8_t
flash_page_erase(uint32_t page) {
    if (flash_unlock() == 1) {
        return 1;
    }

    while (flash_is_busy());

    FLASH->CR |= FLASH_CR_PER;

    FLASH->AR = FLASH_APP_START_ADDR + page * FLASH_PAGE_SIZE;
    FLASH->CR |= FLASH_CR_STRT;

    while (flash_is_busy());

    FLASH->CR &= ~FLASH_CR_PER;

    return 0; 
}
       
uint8_t
flash_program(uint32_t addr, uint64_t data) {
    uint8_t error_status = 0;

    error_status = flash_ready_wait(50000);

    if (!error_status) {
        flash_program_halfword(addr, (uint16_t)data);

        error_status = flash_ready_wait(50000);

        FLASH->CR &= (~FLASH_CR_PG);
    }

    return error_status;
}

#pragma GCC pop_options
