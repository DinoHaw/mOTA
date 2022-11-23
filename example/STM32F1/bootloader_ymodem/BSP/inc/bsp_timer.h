/**
 * \file            bsp_timer.h
 * \brief           timer driver
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

#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "bsp_common.h"

#define TIMER_RUN_FOREVER           0

typedef enum
{
    TIMER_TYPE_HARDWARE = 0x01,     /* 硬件 timer ，实时性较强，定时精度较高，占用中断时间 */
    TIMER_TYPE_SOFTWARE,            /* 软件 timer ，实时性较差，定时精度较差，不占用中断时间 */
    
} BSP_TIMER_TYPE;

struct BSP_TIMER
{
    void (*Timeout_Callback)(void *user_data);  /* 回调函数 */

    BSP_TIMER_TYPE  type;               /* timer 类型 */
    
    uint8_t  start          :1;         /* 开启标志位 */
    uint8_t  timeout_flag   :1;         /* 时间到达标志位 */
    uint8_t                 :0;
    uint16_t period;                    /* 用户指定 timer 的执行次数 */
    uint16_t period_temp;               /* 暂存执行次数 */
    uint32_t timeout;                   /* 需要计时的时间，单位为 ms */
    uint32_t time_temp;                 /* 用于计数的变量 */

    void *user_data;                    /* 用户数据 */
    
    struct BSP_TIMER  *next;
};

void BSP_Timer_Init(struct BSP_TIMER *timer,
                    void (*Timeout_Callback)(void *user_data),
                    uint32_t timeout,
                    uint16_t period,
                    BSP_TIMER_TYPE  timer_type);
void BSP_Timer_LinkUserData(struct BSP_TIMER *timer, void *user_data);
void BSP_Timer_Start(struct BSP_TIMER *timer);
void BSP_Timer_Restart(struct BSP_TIMER *timer);
void BSP_Timer_Pause(struct BSP_TIMER *timer);
void BSP_Timer_Detach(struct BSP_TIMER *timer);
/* 建议在中断中调用，能保证时间精度，但任务执行时间不宜过长 */
void BSP_Timer_Handler(uint8_t ms);
/* 当用户选择使用软件定时器时，一定要循环调用该函数 */
void BSP_Timer_SoftTimerTask(void);

#endif

