/**
 * \file            app.c
 * \brief           application
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


/* Includes ------------------------------------------------------------------*/
#include "app.h"


/* Private variables ---------------------------------------------------------*/
/* 固件更新的标志位，该标志位不能被清零 */
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE)
    #if defined(__IS_COMPILER_ARM_COMPILER_5__)
    volatile uint64_t update_flag __attribute__((at(FIRMWARE_UPDATE_VAR_ADDR), zero_init));

    #elif defined(__IS_COMPILER_ARM_COMPILER_6__)
        #define __INT_TO_STR(x)     #x
        #define INT_TO_STR(x)       __INT_TO_STR(x)
        volatile uint64_t update_flag __attribute__((section(".bss.ARM.__at_" INT_TO_STR(FIRMWARE_UPDATE_VAR_ADDR))));

    #else
        #error "variable placement not supported for this compiler."
    #endif
#endif

static struct BSP_KEY    _key0;         /* 按键对象 */
static struct BSP_TIMER  _timer_led;    /* LED的闪烁timer */


/* Private function prototypes -----------------------------------------------*/
static void _APP_Reset(void);
static uint8_t _Key0_GetLevel(void);
static void _Key_EventCallback(uint8_t id, KEY_EVENT  event);
static void _Timer_LedFlashCallback(void *user_data);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  外设初始化前的一些处理
 * @note   执行到此处，内核时钟已初始化
 * @retval None
 */
void System_Init(void)
{
    /* 设置中断向量表后，开启总中断 */
    extern int Image$$ER_IROM1$$Base;
    BSP_INT_DIS();
    SCB->VTOR = (uint32_t)&Image$$ER_IROM1$$Base;
    BSP_INT_EN();
}


/**
 * @brief  应用初始化
 * @note   此时外设已经初始化完毕
 * @retval None
 */
void APP_Init(void)
{
#if (ENABLE_DEBUG_PRINT)
    #if (EANBLE_PRINTF_USING_RTT)
    /* 配置通道0，下行配置 */
    SEGGER_RTT_ConfigDownBuffer(SEGGER_RTT_PRINTF_TERMINAL, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    SEGGER_RTT_SetTerminal(SEGGER_RTT_PRINTF_TERMINAL);
    #else
    BSP_UART_Init( BSP_UART1 );
    #endif

    uint32_t hal_version = HAL_GetHalVersion();

    BSP_Printf("\r\n[APP] DinoHaw\r\n");
    BSP_Printf("UID: %.8X %.8X %.8X\r\n", HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2());
    BSP_Printf("APP Version: V%d.%d\r\n", APP_VERSION_MAIN, APP_VERSION_SUB);
    BSP_Printf("HAL Version: V%d.%d.%d.%d\r\n", (hal_version >> 24), (uint8_t)(hal_version >> 16), (uint8_t)(hal_version >> 8), (uint8_t)hal_version);
#endif
    
    /* BSP初始化 */
    BSP_Timer_Init( &_timer_led, 
                    _Timer_LedFlashCallback, 
                    500,
                    TIMER_RUN_FOREVER, 
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_led);

    BSP_Key_Init(&_key0, KEY0_PIN, _Key0_GetLevel, KEY_PRESS_HIGH);
    BSP_Key_Register(&_key0, KEY_CLICK, _Key_EventCallback);
    BSP_Key_Register(&_key0, KEY_DBLCLICK, _Key_EventCallback);
    BSP_Key_Start(&_key0);
}


/**
 * @brief  应用开始运行
 * @note   APP_Running 本身就在一个 while 循环内
 * @retval None
 */
void APP_Running(void)
{
    uint16_t time = 0;
    uint32_t count = 0;
    
    while (1)
    {
        BSP_Key_Handler(2);
        
        if (time++ == 500)
        {
            time = 0;
            BSP_Printf("[%d] hello human\r\n", count++);
        }
        
        BSP_Delay(2);
    }
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  APP 软复位的处理函数
 * @note   
 * @retval None
 */
static inline void _APP_Reset(void)
{
    HAL_NVIC_SystemReset();
}


/**
 * @brief  按键的事件处理
 * @note   
 * @param[in]  id: 按键的 ID
 * @param[in]  event: 按键的事件
 * @retval None
 */
static void _Key_EventCallback(uint8_t id, KEY_EVENT  event)
{
    BSP_Printf("[ key ] You just press the button[%d], event: %d\r\n\r\n", id, event);
    switch (event)
    {
        case KEY_CLICK:
        {
            update_flag = FIRMWARE_UPDATE_MAGIC_WORD;
            _APP_Reset();
            break;
        }
        case KEY_DBLCLICK:
        {
            update_flag = FIRMWARE_RECOVERY_MAGIC_WORD;
            _APP_Reset();
            break;
        }
        default: break;
    }
}


/**
 * @brief  读取 key0 的电平值
 * @note   
 * @retval 电平值
 */
static uint8_t _Key0_GetLevel(void)
{
    return BSP_GPIO_Read(_key0.id);
}


/**
 * @brief  LED闪烁周期任务
 * @note   
 * @param[in]  user_data: 用户数据
 * @retval None
 */
static void _Timer_LedFlashCallback(void *user_data)
{
    BSP_GPIO_Toggle( LED0_PIN );
}


/**
 * @brief  参数检查错误时的处理函数
 * @note   
 * @param[in]  func: 错误发生的所在函数
 * @param[in]  line: 错误发生的所在c文件的行数
 * @retval None
 */
void Assert_Failed(uint8_t *func, uint32_t line)
{
    BSP_Printf("\r\n[ error ]: %s(%d)\r\n\r\n", func, line);
    while (1);
}





