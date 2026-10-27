/**
 *******************************************************************************
 * @file  usb/usb_dev_hid_custom/source/usb_bsp.h
 * @brief USB 板级接口声明，由 keyboard_usb_board.c 实现，供 USB 底层库调用。
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Add USB core ID select function
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2025, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#ifndef __USB_BSP_H__
#define __USB_BSP_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_lib.h"
#include "ev_hc32f460_lqfp100_v2_bsp.h"

/**
 * @addtogroup HC32F460_DDL_Applications
 * @{
 */

/**
 * @addtogroup USB_Dev_Hid_Custom
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/* 初始化系统时钟及 USB 使用的 48 MHz PLL 时钟。 */
void bsp_init(void);
/* 配置 USB 数据引脚、VBUS 引脚和外设时钟。 */
void usb_pin_init(void);
/* USB 底层初始化所需的微秒、毫秒延时接口。 */
void usb_udelay(const uint32_t usec);
void usb_mdelay(const uint32_t msec);
/* 注册并启用 USB 全局中断。 */
void usb_bsp_nvicconfig(usb_core_instance *pdev);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __USB_BSP_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


