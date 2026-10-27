/**
 *******************************************************************************
 * @file  iap/iap_ymodem_boot/source/flash.h
 * @brief This file contains all the functions prototypes of the Flash driver.
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
#ifndef __FLASH_H__
#define __FLASH_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_efm.h"
#include "config.h"
#include "wear_leveling_legacy_config.h"
#include "wear_leveling_internal.h"
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Flash definitions */
#define FLASH_BASE                      (EFM_START_ADDR)
#define FLASH_SIZE                      (EFM_END_ADDR + 1U)
#define FLASH_SECTOR_SIZE               (SECTOR_SIZE)

/* FLASH Status */
typedef enum
{ 
  FLASH_COMPLETE = 0,
  FLASH_FAIL = 1
}FLASH_Status;

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
FLASH_Status FLASH_ErasePage(uint32_t u32Addr);
FLASH_Status FLASH_ProgramHalfWord(uint32_t u32Addr, uint16_t Data);
backing_store_int_t FLASH_read_data(uint32_t u32Addr);
#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
