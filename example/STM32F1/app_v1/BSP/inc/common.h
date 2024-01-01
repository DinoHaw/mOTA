/**
 * \file            common.h
 * \brief           the common file of this project
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
 * v1.1     2022-12-04     Dino         增加 __IS_COMPILER_ARM_COMPILER__
 * v1.2     2023-12-19     Dino         1. 引入 perf_counter 库
 */

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

/* 配置文件 */
#include "app_config.h"
#include "bsp_config.h"
#include "bootloader_config.h"

/* 用户库 */
#include "main.h"

/* 工具库 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/* RTOS */
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
#include "rtthread.h"
#include "rthw.h"
#elif (USING_RTOS_TYPE == RTOS_USING_UCOS)
#include "os.h"
#include "bsp_rtos.h"
#endif

/* Component */
#if (EANBLE_PRINTF_USING_RTT)
#include "SEGGER_RTT.h"
#endif
#include "perf_counter.h"


#if (ENABLE_ASSERT)
extern void Assert_Failed(uint8_t *func, uint32_t line);
#define ASSERT(expr)            ((expr) ? (void)0U : Assert_Failed((uint8_t *)__func__, __LINE__))
#else
#define ASSERT(expr)            ((void)0U)
#endif


#endif
