/**
 * \file            main.c
 * \brief           main application
 */

/*
 * Copyright (c) 2024
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
 * Author:          wade任
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2024-01-08     wade任       the first version
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "gd32l23x.h"
#include "systick.h"

#include "bsp_common.h"

/* USER CODE BEGIN PFP */
extern void System_Init(void);
extern void APP_Init(void);
extern void APP_Running(void);

int main(void)
{
    systick_config();

    bsp_usart1_hardware_init();
    bsp_debug_hardware_init();

    System_Init();

    APP_Init();
    while (1)
    {
        APP_Running();
    }
}

/* 重定向 c 库函数 printf 到串口，重定向后可使用 printf 函数 */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);   /* 发送字符 */
    /* 检查是否发送完毕 */
    while (usart_flag_get(USART0, USART_FLAG_TC) == RESET) {} 
    return (ch);
}
