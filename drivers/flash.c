/**
 *******************************************************************************
 * @file  iap/iap_ymodem_boot/source/flash.c
 * @brief This file provides firmware functions to manage the Flash driver.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "flash.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 * @brief  Erase flash sector.
 * @param  u32Addr                      Flash address
 * @param  u32Size                      Firmware size (0: current address sector)
 * @retval An en_result_t enumeration value:
 *           - LL_OK: Erase succeeded
 *           - LL_ERR: Erase timeout
 *           - LL_ERR_INVD_PARAM: The parameters is invalid.
 */
FLASH_Status FLASH_ErasePage(uint32_t u32Addr)
{
    int32_t result;

    if (u32Addr < WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS ||
        u32Addr >= WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS + WEAR_LEVELING_BACKING_SIZE ||
        (u32Addr % FLASH_SECTOR_SIZE) != 0U) {
        return FLASH_FAIL;
    }

    EFM_REG_Unlock();
    while (SET != EFM_GetStatus(EFM_FLAG_RDY)) {
        ;
    }
    EFM_FWMC_Cmd(ENABLE);
    result = EFM_SectorErase(u32Addr);
    EFM_FWMC_Cmd(DISABLE);
    EFM_REG_Lock();

    return result == LL_OK ? FLASH_COMPLETE : FLASH_FAIL;
}
/**
 * @brief  Write data to flash.
 * @param  u32Addr                      Flash address
 * @param  pu8Buff                      Pointer to the buffer to be written
 * @param  u32Len                       Buffer length
 * @retval int32_t:
 *           - LL_OK: Program successful.
 *           - LL_ERR_INVD_PARAM: The parameters is invalid.
 *           - LL_ERR_NOT_RDY: EFM if not ready.
 *           - LL_ERR_ADDR_ALIGN: Address alignment error
 */
FLASH_Status FLASH_ProgramHalfWord(uint32_t u32Addr, uint16_t Data)
{
    int32_t result;

    if ((u32Addr < WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS) ||
        (u32Addr > WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS
                    + WEAR_LEVELING_BACKING_SIZE - sizeof(uint16_t)) ||
        ((u32Addr % sizeof(uint16_t)) != 0U)) {
        return FLASH_FAIL;
    }
    uint32_t f_write_value;
    uint32_t aligned_address = u32Addr & (~0x3);
    uint32_t offset = u32Addr % 4;

    if (offset == 0) {
        // 写入低 2 个字节
        f_write_value = 0xffff0000 | Data;
    } else if (offset == 2) {
        // 写入高 2 个字节
        f_write_value = 0x0000ffff | ((uint32_t)Data << 16);
    }

    EFM_REG_Unlock();
    while (SET != EFM_GetStatus(EFM_FLAG_RDY)) {
        ;
    }
    EFM_FWMC_Cmd(ENABLE);
    result = EFM_ProgramWord(aligned_address, f_write_value);
    EFM_FWMC_Cmd(DISABLE);
    EFM_REG_Lock();

    return result == LL_OK ? FLASH_COMPLETE : FLASH_FAIL;
}

/**
 * @brief  Read data from flash.
 * @param  u32Addr                      Flash address
 * @param  pu8Buff                      Pointer to the buffer to be reading
 * @param  u32Len                       Buffer length
 * @retval int32_t:
 *           - LL_OK: Read data succeeded
 *           - LL_ERR_INVD_PARAM: The parameters is invalid
 *           - LL_ERR_ADDR_ALIGN: Address alignment error
 */

backing_store_int_t FLASH_read_data(uint32_t u32Addr)
{
    if ((u32Addr < WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS) ||
        (u32Addr > (WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS + WEAR_LEVELING_BACKING_SIZE - sizeof(backing_store_int_t))) ||
        ((u32Addr % sizeof(backing_store_int_t)) != 0U)) {
        return (backing_store_int_t)0xFFFFU;
    }

    return *(volatile const backing_store_int_t *)(uintptr_t)u32Addr;
}
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
