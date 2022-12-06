/**
 * \file            bsp_timer.c
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

/* Includes ------------------------------------------------------------------*/
#include "bsp_timer.h"


/* Private variables ---------------------------------------------------------*/
#define TIMER_ONE_SHOT      0x04
#define TIMER_PERIODIC      0x08

static struct BSP_TIMER *_timer_head;


/* Private function prototypes -----------------------------------------------*/
static void _Timer_Add(struct BSP_TIMER *timer);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  timer ��ʼ��
 * @note   �������� timer ����ʱҲ�ɵ��ñ��������޳�ͻ
 * @param[in]  timer: timer ����
 * @param[in]  Timeout_Callback: timer ��ʱ�ص�����
 * @param[in]  timeout: ��ʱʱ�䣬��λ ms
 * @param[in]  period: ִ�д�����0: ����ִ��
 * @param[in]  timer_type: timer ���ͣ���ѡ��Ӳ�������
 * @retval None
 */
void BSP_Timer_Init(struct BSP_TIMER *timer,
                    void (*Timeout_Callback)(void *user_data),
                    uint32_t timeout,
                    uint16_t period,
                    BSP_TIMER_TYPE  timer_type)
{
    ASSERT(timer != NULL);

    timer->period           = period;
    timer->timeout          = timeout;
    timer->type             = timer_type;
    timer->Timeout_Callback = Timeout_Callback;

    timer->time_temp   = 0;
    timer->period_temp = period;

    if (period == 0)
        timer->type |= TIMER_PERIODIC;
    else
        timer->type |= TIMER_ONE_SHOT;
    
    _Timer_Add(timer);
}


/**
 * @brief  �����û�����
 * @note   ��ѡ���Ҫ���ڻص����������û�������
 * @param[in]  timer: timer ����
 * @param[in]  user_data: �û�����
 * @retval None
 */
void BSP_Timer_LinkUserData(struct BSP_TIMER *timer, void *user_data)
{
    ASSERT(timer != NULL);

    timer->user_data = user_data;
}


/**
 * @brief  ���� timer
 * @note   ������ timer ��̳���һ��״̬������ת
 * @param[in]  timer: timer ����
 * @retval None
 */
void BSP_Timer_Start(struct BSP_TIMER *timer)
{
    ASSERT(timer != NULL);

    timer->start = 1;
}


/**
 * @brief  ���¿��� timer
 * @note   ���¿����Ὣ timer �Ĳ�����λ����ʼ״̬����ת���������ٴε��� BSP_Timer_Init
 * @param[in]  timer: timer ����
 * @retval None
 */
void BSP_Timer_Restart(struct BSP_TIMER *timer)
{
    ASSERT(timer != NULL);
    
    timer->start     = 0;
    timer->time_temp = 0;
    timer->period    = timer->period_temp;
    timer->start     = 1;
}


/**
 * @brief  ��ͣ timer ����
 * @note   ֻ����ͣ�� timer ���������ϣ��ظ�ֻ����� BSP_Timer_Start �� BSP_Timer_Restart
 * @param[in]  timer: timer ����
 * @retval None
 */
void BSP_Timer_Pause(struct BSP_TIMER *timer)
{
    ASSERT(timer != NULL);

    timer->start = 0;
    timer->time_temp = 0;

    return;
}


/**
 * @brief  ���� timer
 * @note   ������������и� timer ���ָ���Ҫ���µ��� BSP_Timer_Init
 * @param[in]  timer: timer ����
 * @retval None
 */
void BSP_Timer_Detach(struct BSP_TIMER *timer)
{
    struct BSP_TIMER **now_target;
    struct BSP_TIMER *entry;

    ASSERT(timer != NULL);

    for (now_target = &_timer_head; *now_target; )
    {
        entry = *now_target;

        if (entry == timer)
        {
            *now_target = entry->next;
            return;
        }
        else 
            now_target = &entry->next;         
    }

    return;
}


/**
 * @brief  timer ������
 * @note   ���뱻ѭ�����ã���������жϺ�����
 * @param[in]  ms: �����õļ��ʱ�䣬��λ ms���Ƽ� 1ms ��
 * @retval None
 */
void BSP_Timer_Handler(uint8_t ms)
{
    struct BSP_TIMER *timer;

    for (timer = _timer_head; timer != NULL; timer = timer->next) 
    {
        if (timer->start)
        {
            timer->time_temp += ms;
            if (timer->time_temp >= timer->timeout)
            {
                timer->time_temp = 0;

                if ((timer->type & TIMER_TYPE_HARDWARE) != 0)
                {
                    if (timer->Timeout_Callback)
                        timer->Timeout_Callback( timer->user_data );
                    
                    if ((timer->type & TIMER_ONE_SHOT) != 0)
                    {
                        --timer->period;
                        if (timer->period == 0)
                            BSP_Timer_Pause(timer);
                    }
                }
                else
                    timer->timeout_flag = 1;
            }
        }
    }
}


/**
 * @brief  �����ʱ��������
 * @note   ʹ�ò�ѯ�ı�־λ��ʽ�����û��� timer ��ʼ��ʱѡ���� TIMER_TYPE_SOFTWARE ������뱻���ϵ���
 * @retval None
 */
void BSP_Timer_SoftTimerTask(void)
{
    struct BSP_TIMER *timer;

    for (timer = _timer_head; timer != NULL; timer = timer->next) 
    {
        if (timer->timeout_flag)
        {
            timer->timeout_flag = 0;
            
            if (timer->Timeout_Callback)
                timer->Timeout_Callback( timer->user_data );

            if ((timer->type & TIMER_ONE_SHOT) != 0)
            {
                --timer->period;
                if (timer->period == 0)
                    BSP_Timer_Pause(timer);
            }
        }
    }
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  ����ʱ��������ӽ���������
 * @note   
 * @param[in]  timer: timer ����
 * @retval None
 */
static void _Timer_Add(struct BSP_TIMER *timer)
{
    struct BSP_TIMER *target;
    
    for (target = _timer_head; target != NULL; target = target->next)
    {
        if (target == timer)
            return;
    }

    timer->next = _timer_head;
    _timer_head  = timer;
}

