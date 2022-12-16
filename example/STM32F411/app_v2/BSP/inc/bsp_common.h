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
 * Version:         v1.0.0
 */

#ifndef __BSP_COMMON_H__
#define __BSP_COMMON_H__

#include "common.h"

#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_timer.h"
#include "bsp_key.h"

#define BSP_VERSION_MAIN                    (0x01U) /*!< [15:8] main version */
#define BSP_VERSION_SUB                     (0x00U) /*!< [ 7:0] sub version */
#define BSP_VERSION                         ((BSP_VERSION_MAIN << 8)    \
                                            |(BSP_VERSION_SUB))

#define BSP_Delay(ms)                       HAL_Delay(ms)

#define BSP_INT_ENTER()
#define BSP_INT_EXIT()

#define BSP_INT_EN()                        __enable_irq()
#define BSP_INT_DIS()                       __disable_irq()

#define BSP_UART_ENABLE_RX(UARTx)           UARTx.Instance->CR1 |= (uint32_t)0x0004
#define BSP_UART_DISABLE_RX(UARTx)          UARTx.Instance->CR1 &= (~(uint32_t)0x0004)


#endif
