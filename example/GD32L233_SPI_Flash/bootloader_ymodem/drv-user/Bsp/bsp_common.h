/**
 * \file            bsp_common.h
 * \brief           the common file of BSP
 */

/*
 * Copyright (c) 2022 Dino Haw
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of mOTA - The Over-The-Air technology component for MCU.
 *
 * Author:          Dino Haw <347341799@qq.com>
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2022-11-23     Dino         the first version
 * v1.1     2022-12-04     Dino         1. 增加长按按键恢复出厂固件的选项
 *                                      2. 修改中断开启与关闭接口
 * v1.2     2023-12-19     Dino         1. 删除多余的宏
 * v1.3     2024-01-08     wade任       1. 修改包含的头文件
 */

#ifndef __BSP_COMMON_H__
#define __BSP_COMMON_H__

#include "common.h"

#include "bsp_board.h"
#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
#include "bsp_key.h"
#endif
#include "bsp_uart_drv_port.h"
#include "bsp_timer.h"
#if (IS_ENABLE_SPI_FLASH == 0)
#include "bsp_flash_drv.h"
#endif
#include "bsp_flash_drv.h"
#include "bsp_usart1_drv.h"
#include "bsp_debug_drv.h"
#include "bsp_fmc_drv.h"

#define BSP_VERSION_MAIN                    (0x01U) /*!< [15:8] main version */
#define BSP_VERSION_SUB                     (0x00U) /*!< [ 7:0] sub version */
#define BSP_VERSION                         ((BSP_VERSION_MAIN << 8)    \
                                            |(BSP_VERSION_SUB))

#define BSP_Delay(ms)                       HAL_Delay(ms)

#define BSP_INT_ENTER()
#define BSP_INT_EXIT()

#define BSP_INT_EN()                        __enable_irq()
#define BSP_INT_DIS()                       __disable_irq()

#if (ENABLE_DEBUG_PRINT)
    #if (EANBLE_PRINTF_USING_RTT)
    #define BSP_Printf(...)                 SEGGER_RTT_printf(SEGGER_RTT_PRINTF_TERMINAL, __VA_ARGS__)
    #endif
#else
    #define BSP_Printf(...)
#endif

#endif
