#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f1xx.h"

#ifndef FLASH_PAGES
    #error "You must provide number of flash pages with define in CMakeLists.txt"
#endif

/** @defgroup FLASHEx_Voltage_Range FLASH Voltage Range
  * @{
  */ 
#define FLASH_VOLTAGE_RANGE_1        0x00000000U  /*!< Device operating range: 1.8V to 2.1V                */
#define FLASH_VOLTAGE_RANGE_2        0x00000001U  /*!< Device operating range: 2.1V to 2.7V                */
#define FLASH_VOLTAGE_RANGE_3        0x00000002U  /*!< Device operating range: 2.7V to 3.6V                */
#define FLASH_VOLTAGE_RANGE_4        0x00000003U  /*!< Device operating range: 2.7V to 3.6V + External Vpp */
/**
  * @}
  */

/** @defgroup FLASHEx_Type_Program 
  * @{
  */ 
#define FLASH_TYPEPROGRAM_HALFWORD    0x00000001U  /*!< Program a half-word (16-bit) at a specified address   */
#define FLASH_TYPEPROGRAM_WORD        0x00000002U  /*!< Program a word (32-bit) at a specified address        */
#define FLASH_TYPEPROGRAM_DOUBLEWORD  0x00000003U  /*!< Program a double word (64-bit) at a specified address */
/**
  * @}
  */

/** @defgroup FLASH_Flag_definition FLASH Flag definition
  * @brief Flag definition
  * @{
  */
#define FLASH_FLAG_EOP              FLASH_SR_EOP            /*!< FLASH End of Operation flag               */
#define FLASH_FLAG_WRPERR           FLASH_SR_WRPRTERR         /*!< FLASH Write protected error flag          */
#if defined(FLASH_SR_RDERR)
#define FLASH_FLAG_RDERR            FLASH_SR_RDERR          /*!< Read Protection error flag (PCROP)        */
#endif /* FLASH_SR_RDERR */
#define FLASH_FLAG_BSY              FLASH_SR_BSY            /*!< FLASH Busy flag                           */
/**
  * @}
  */

/** @defgroup FLASH_Error_Code FLASH Error Code
  * @brief    FLASH Error Code 
  * @{
  */ 
#define FLASH_ERROR_NONE            0x00000000U    /*!< No error                      */
#define FLASH_ERROR_RD              0x00000001U    /*!< Read Protection error         */
#define FLASH_ERROR_PGS             0x00000002U    /*!< Programming Sequence error    */
#define FLASH_ERROR_PGP             0x00000004U    /*!< Programming Parallelism error */
#define FLASH_ERROR_PGA             0x00000008U    /*!< Programming Alignment error   */
#define FLASH_ERROR_WRP             0x00000010U    /*!< Write protection error        */
#define FLASH_ERROR_OPERATION       0x00000020U    /*!< Operation Error               */
/**
  * @}
  */

/** @defgroup FLASH_Interrupt_definition FLASH Interrupt definition
  * @brief FLASH Interrupt definition
  * @{
  */
#define FLASH_IT_EOP                FLASH_CR_EOPIE          /*!< End of FLASH Operation Interrupt source */
#define FLASH_IT_ERR                0x02000000U             /*!< Error Interrupt source                  */
/**
  * @}
  */

/** @defgroup FLASH_Program_Parallelism FLASH Program Parallelism
  * @{
  */
#define FLASH_PSIZE_BYTE            0x00000000U
#define FLASH_PSIZE_HALF_WORD       0x00000100U
#define FLASH_PSIZE_WORD            0x00000200U
#define FLASH_PSIZE_DOUBLE_WORD     0x00000300U
#define CR_PSIZE_MASK               0xFFFFFCFFU

/* Flash access keys */
#define FLASH_OPT_KEY1              0x08192A3BU
#define FLASH_OPT_KEY2              0x4C5D6E7FU

void flash_lock(void);
uint8_t flash_unlock(void);
uint8_t flash_ready_wait(uint16_t timeout);
uint8_t flash_page_erase(uint32_t page);
void flash_full_erase(void);
uint8_t flash_program(uint32_t addr, uint64_t data);

#endif
