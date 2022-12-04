/**
 * \file            bsp_config.h
 * \brief           the configuration file of BSP
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

#ifndef __BSP_CONFIG_H__
#define __BSP_CONFIG_H__

/* UART��� */
#define BSP_PRINTF_BUFF_SIZE                256                 /* BSP_Print����ʱ��������С */
#define BSP_PRINTF_HANDLE                   UART(1)             /* ����ִ��BSP_Print��UART��� */

#define BSP_UART_BUFF_SIZE                  64                 /* UART����һ������Ĵ�С */

#define BSP_USING_UART1                     1
#define BSP_USING_UART2                     0
#define BSP_USING_UART2_RE                  0
#define BSP_USING_UART3                     0
#define BSP_USING_UART3_RE                  0
#define BSP_USING_UART4                     0
#define BSP_USING_UART5                     0
#define BSP_USING_UART6                     0

#endif
