/**
 * \file            bsp_key.h
 * \brief           universal key driver
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

#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "bsp_common.h"

#define KEY_TEST                            0

/* 常用配置项 */
#define KEY_CLICK_EVENT_WHEN_DBLCLICK       0       /* 设定当双击按键时是否还触发单击事件。1：双击时依旧触发单击事件；0：双击时不触发单击事件，但会触发双击事件 */
#define KEY_DBLCLICK_PRESS_VALID            0       /* 按键双击时，选择第二次按下时立即触发事件，或是第二次按下时放开按键后才触发事件 */
                                                    /* 1：第二次按下时立即触发事件；0：第二次按下并放开按键后才触发事件 */
#define KEY_LONG_PRESS_TIME                 FACTORY_FIRMWARE_BUTTON_TIME    /* 长按间隔时间，单位：ms */
#define KEY_DBLCLICK_TIME                   200     /* 双击间隔时间。单位：ms */

/* 非常用配置项 */
#define KEY_USER_DEFINE_PRESS_TIME          0       /* 用户想针对每个按键设定不同的长按触发时间，为 0 时表示全部按键的长按触发时间使用 KEY_LONG_PRESS_TIME 参数 */
                                                    /* 为 1 时表示用户想单独为每个按键设置长按事件的触发时间 */
#define KEY_LONG_PRESS_CONTINU_TRIG_TIME    0       /* 一直长按按键， KEY_LONG_PRESS_TIME 个单位时间后，此参数表示的是用户持续按下按键每隔一段时间触发一次 KEY_LONG_PRESS_HOLD 事件 */
                                                    /* 为 0 时表示不管用户一直按下按键持续多长时间，都只触发一次 KEY_LONG_PRESS 事件，直到用户放开按键为止，才能继续下个事件的操作 */
#define KEY_CONSECUTIVE_READ_TIME           4       /* 连续读取键值次数，此参数用于实现非阻塞式的按键消抖，最大值：8 */
                                                    /* 该值会影响读取最终键值和触发事件的时间 */
                                                    /* 按键的单击事件触发时间 = KEY_CONSECUTIVE_READ_TIME * 传入BSP_Key_Handler函数的参数值，单位：ms */
                                                    /* 其他按键事件基于以上时间叠加 */

typedef enum
{
    KEY_PRESS_LOW  = 0x00,
    KEY_PRESS_HIGH = 0xFF

} KEY_LEVEL;

typedef enum 
{
    KEY_UP = 0,
    KEY_DOWN

} KEY_STATE;

typedef enum 
{
    KEY_PRESS           = 0,        /* 刚按下（每个按键事件都会触发该事件，是否要处理该事件由用户选择是否注册该事件的回调函数） */
    KEY_CLICK           = 1,        /* 单击 */
    KEY_DBLCLICK        = 2,        /* 双击 */
    KEY_LONG_PRESS      = 3,        /* 长按 */
    KEY_LONG_PRESS_HOLD = 4,        /* 长按保持 */
    KEY_EVENT_NUM,                  /* 事件总数 */
    KEY_NONE_EVENT                  /* 默认无事件 */

} KEY_EVENT;


typedef void (*Key_EventCallback)(uint8_t id, KEY_EVENT  event);


struct BSP_KEY
{
    KEY_EVENT  event;          /* 按键事件记录 */
    KEY_LEVEL  press_level;    /* 记录按下按键是什么电平（高或低） */
    KEY_STATE  key_state;      /* 记录当前按键状态（按下或弹起） */
    
    uint8_t id;                     /* 提供给用户的按键 id，由于有些用户可能想将所有的按键回调都写成一个函数接口，此举方便用户程序识别为是哪个按键触发了事件 */
                                    /* 由数据类型可见，一个按键接口函数中最大支持256个按键 id */
    uint8_t state;                  /* 状态机 */
    uint8_t continuous_value;       /* 按键连续采样缓存，一共8位深度，用于存取一个按键不同时刻的按键状态，最大是8次 */
    
    uint16_t tick;                  /* 用于计时的临时变量 */
#if (KEY_USER_DEFINE_PRESS_TIME)
    uint16_t press_time;            /* 长按间隔时间 */
#endif
    
    uint8_t (*GetState)(void);                      /* 按键电平状态获取函数，需要用户实现 */
    Key_EventCallback  Callback[ KEY_EVENT_NUM ];   /* 按键回调函数，由按键事件触发，需要用户实现 */
    
    struct BSP_KEY *next;        /* 简单的单向链表 */
};


#if (KEY_USER_DEFINE_PRESS_TIME)
int8_t BSP_Key_Init(struct BSP_KEY *key, uint8_t id, uint8_t (*KEY_GetState)(void), KEY_LEVEL press_level, uint16_t press_time);
#else
int8_t BSP_Key_Init(struct BSP_KEY *key, uint8_t id, uint8_t (*KEY_GetState)(void), KEY_LEVEL press_level);
#endif
int8_t BSP_Key_Start(struct BSP_KEY *key);
void   BSP_Key_Register(struct BSP_KEY *key, KEY_EVENT event, Key_EventCallback callback);
void   BSP_Key_Handler(uint8_t ms);

#if (KEY_TEST)
void Key_Test(void);
#endif

#endif

