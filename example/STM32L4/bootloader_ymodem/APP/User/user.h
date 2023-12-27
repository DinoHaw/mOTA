/**
 * \file            user.h
 * \brief           configuration of the user application
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
 * v1.1     2022-12-04     Dino         增加 VERSION_WRITE_TO_APP
 * v1.2     2023-12-10     Dino         1. 改名为 user
 *                                      2. 剥离 bootloader 部分
 */

#ifndef __USER_H__
#define __USER_H__

/* 定义项 */
#define RTOS_USING_NONE                     0                   /* 不使用 RTOS */
#define RTOS_USING_RTTHREAD                 1                   /* RT-Thread */
#define RTOS_USING_UCOS                     2                   /* uC/OS */

/* 配置选项 */
#define ENABLE_ASSERT                       0                   /* 是否使能函数入口参数检查 */
#define ENABLE_DEBUG_PRINT                  1                   /* 是否使能调试信息打印 */
#define EANBLE_PRINTF_USING_RTT             1                   /* BSP_Print 函数是否使用 SEGGER RTT 作为输出端口 */

#define USING_RTOS_TYPE                     RTOS_USING_NONE
#define SEGGER_RTT_PRINTF_TERMINAL          0                   /* SEGGER RTT 的打印端口 */
#define MAX_NAME_LEN                        8

#define SOC_SERIES_STM32L4

#endif
