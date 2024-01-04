/**
 * \file            user.c
 * \brief           user application
 */

/*
 * Copyright (c) 2023 Dino Haw
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
 * v1.0     2023-12-10     Dino         the first version
 */

/* Includes ------------------------------------------------------------------*/
#include "user.h"
#include "bootloader.h"


/* Private variables ---------------------------------------------------------*/
static struct BSP_TIMER _timer_led;     /* LED 的闪烁 timer */


/* Private function prototypes -----------------------------------------------*/
static void _Timer_LedFlashCallback(void *user_data);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  外设初始化前的一些处理
 * @note   执行到此处，内核时钟已初始化
 * @retval None
 */
void System_Init(void)
{
    
}


/**
 * @brief  应用初始化
 * @note   此时外设已初始化完毕
 * @retval None
 */
void APP_Init(void)
{
#if (ENABLE_DEBUG_PRINT)
    #if (EANBLE_PRINTF_USING_RTT)
    /* 配置通道 0 ，下行配置 SEGGER_RTT_MODE_NO_BLOCK_SKIP */
    SEGGER_RTT_ConfigDownBuffer(SEGGER_RTT_PRINTF_TERMINAL, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    SEGGER_RTT_SetTerminal(SEGGER_RTT_PRINTF_TERMINAL);
    #else
    BSP_UART_Init( BSP_UART1 );
    #endif
#endif

    BSP_Board_Init();
    
    /* BSP 初始化 */
    BSP_Timer_Init( &_timer_led, 
                    _Timer_LedFlashCallback, 
                    100,
                    TIMER_RUN_FOREVER, 
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_led);

    Bootloader_Init();
}


/**
 * @brief  应用运行
 * @note   APP_Running 本身就在一个 while(1) 内
 * @retval None
 */
void APP_Running(void)
{
    while(1)
    {
        Bootloader_Loop();
    }
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  LED 闪烁周期任务
 * @note   
 * @param[in]  user_data: 用户数据
 * @retval None
 */
static void _Timer_LedFlashCallback(void *user_data)
{
    HAL_GPIO_TogglePin(G_LED_GPIO_Port, G_LED_Pin);
}
