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
 * @brief  timer 初始化
 * @note   重新配置 timer 参数时也可调用本函数，无冲突
 * @param[in]  timer: timer 对象
 * @param[in]  Timeout_Callback: timer 超时回调函数
 * @param[in]  timeout: 超时时间，单位 ms
 * @param[in]  period: 执行次数。0: 无限执行
 * @param[in]  timer_type: timer 类型，可选择硬件和软件
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
 * @brief  链接用户数据
 * @note   可选项，主要用于回调函数传递用户的数据
 * @param[in]  timer: timer 对象
 * @param[in]  user_data: 用户数据
 * @retval None
 */
void BSP_Timer_LinkUserData(struct BSP_TIMER *timer, void *user_data)
{
    ASSERT(timer != NULL);

    timer->user_data = user_data;
}


/**
 * @brief  开启 timer
 * @note   开启后 timer 会继承上一个状态继续运转
 * @param[in]  timer: timer 对象
 * @retval None
 */
void BSP_Timer_Start(struct BSP_TIMER *timer)
{
    ASSERT(timer != NULL);

    timer->start = 1;
}


/**
 * @brief  重新开启 timer
 * @note   重新开启会将 timer 的参数复位到初始状态后运转，而不必再次调用 BSP_Timer_Init
 * @param[in]  timer: timer 对象
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
 * @brief  暂停 timer 运行
 * @note   只是暂停， timer 还在链表上，回复只需调用 BSP_Timer_Start 或 BSP_Timer_Restart
 * @param[in]  timer: timer 对象
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
 * @brief  分离 timer
 * @note   分离后，链表不再有该 timer ，恢复需要重新调用 BSP_Timer_Init
 * @param[in]  timer: timer 对象
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
 * @brief  timer 处理函数
 * @note   必须被循环调用，建议放在中断函数内
 * @param[in]  ms: 被调用的间隔时间，单位 ms（推荐 1ms ）
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
 * @brief  软件定时器处理函数
 * @note   使用查询的标志位方式，若用户对 timer 初始化时选择了 TIMER_TYPE_SOFTWARE ，则必须被不断调用
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
 * @brief  将定时器对象添加进管理链表
 * @note   
 * @param[in]  timer: timer 对象
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

