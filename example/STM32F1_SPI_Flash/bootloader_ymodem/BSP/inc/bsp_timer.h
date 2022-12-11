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
    TIMER_TYPE_HARDWARE = 0x01,     /* Ӳ�� timer ��ʵʱ�Խ�ǿ����ʱ���Ƚϸߣ�ռ���ж�ʱ�� */
    TIMER_TYPE_SOFTWARE,            /* ��� timer ��ʵʱ�Խϲ��ʱ���Ƚϲ��ռ���ж�ʱ�� */
    
} BSP_TIMER_TYPE;

struct BSP_TIMER
{
    void (*Timeout_Callback)(void *user_data);  /* �ص����� */

    BSP_TIMER_TYPE  type;               /* timer ���� */
    
    uint8_t  start          :1;         /* ������־λ */
    uint8_t  timeout_flag   :1;         /* ʱ�䵽���־λ */
    uint8_t                 :0;
    uint16_t period;                    /* �û�ָ�� timer ��ִ�д��� */
    uint16_t period_temp;               /* �ݴ�ִ�д��� */
    uint32_t timeout;                   /* ��Ҫ��ʱ��ʱ�䣬��λΪ ms */
    uint32_t time_temp;                 /* ���ڼ����ı��� */

    void *user_data;                    /* �û����� */
    
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
/* �������ж��е��ã��ܱ�֤ʱ�侫�ȣ�������ִ��ʱ�䲻�˹��� */
void BSP_Timer_Handler(uint8_t ms);
/* ���û�ѡ��ʹ�������ʱ��ʱ��һ��Ҫѭ�����øú��� */
void BSP_Timer_SoftTimerTask(void);

#endif

