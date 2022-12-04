/**
 * \file            bsp_key.c
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

/* Includes ------------------------------------------------------------------*/
#include "bsp_key.h"


/* Private variables ---------------------------------------------------------*/
#define EVENT_CALLBACK(event)   do{                                                                \
                                       if (key_index->Callback[ event ])                           \
                                       {                                                           \
                                           key_index->Callback[ event ]( key_index->id, event );   \
                                       }                                                           \
                                  } while(0)

                                        
/* Private function prototypes -----------------------------------------------*/
static struct BSP_KEY *_key_head;


/* Exported functions ---------------------------------------------------------*/                                        
/**
 * @brief  ���������ʼ��
 * @note   
 * @param[in]  key: �������
 * @param[in]  id: ���� ID������ʶ��ͬ�İ���
 * @param[in]  KEY_GetState: ����״̬��ȡ�ص�����
 * @param[in]  press_level: ��������ʱ�ĵ�ƽ״̬
 * @param[in]  press_time: ���ð��������ļ��ʱ��
 * @retval ִ�н��
 */
#if (KEY_USER_DEFINE_PRESS_TIME)
int8_t BSP_Key_Init(struct BSP_KEY *key, uint8_t id, uint8_t (*KEY_GetState)(void), KEY_LEVEL press_level, uint16_t press_time)
#else
int8_t BSP_Key_Init(struct BSP_KEY *key, uint8_t id, uint8_t (*KEY_GetState)(void), KEY_LEVEL press_level)
#endif
{
    memset(key, 0, sizeof(struct BSP_KEY));

    key->id               = id;
    key->press_level      = press_level;
    key->continuous_value = 0xFF; 
    key->GetState         = KEY_GetState;
#if (KEY_USER_DEFINE_PRESS_TIME)
    key->press_time       = press_time;
#endif
    
    return 0;
}


/**
 * @brief  ��������
 * @note   ÿ��������ʼ����ע�����Ӧ�İ����¼��ص������󣬱�����ñ���������ʹ��
 * @param[in]  key: �������
 * @retval ִ�н��
 */
int8_t BSP_Key_Start(struct BSP_KEY *key)
{
    struct BSP_KEY *key_index = _key_head;
    
    while (key_index)
    {
        if (key_index == key)
            return 1;

        key_index = key_index->next;
    }
    
    key->next = _key_head;
    _key_head  = key;
    
    return 0;
}


/**
 * @brief  ����ͣ��
 * @note   ������������Ҫʹ��ʱ����ɵ��ñ�����
 * @param[in]  key:������� 
 * @retval None
 */
void BSP_Key_Stop(struct BSP_KEY *key)
{
    struct BSP_KEY *key_now = _key_head;
    struct BSP_KEY *key_last;
    
    for (; key_now; key_now = key_now->next)
    {
        if (key_now == key)
        {
            if (key == _key_head)
            {
                _key_head = key->next;
                return;
            }

            if (key->next)
                key_last->next = key->next;
            
            else
                key_last->next = NULL;
        }
        key_last = key_now;
    }
}
    

/**
 * @brief  ע�ᰴ���¼�����ʱ�Ļص�����
 * @note   ����Ҫʹ�ð�����������Ӧ��ע��һ�������¼��Ļص�����
 * @param[in]  key: �������
 * @param[in]  event: �����¼�
 * @param[in]  callback: �����¼��ص�����
 * @retval None
 */
void BSP_Key_Register(struct BSP_KEY *key, KEY_EVENT event, Key_EventCallback callback)
{
    key->Callback[ event ] = callback;
}


/**
 * @brief  ����ҵ������
 * @note   ����Ҫʹ�ð������κι��ܣ�����Ҫ���ñ��������������Ե�ѭ��ִ��
 * @param[in]  ms: ��֪������ÿ�����ٺ��뱻ִ��1��
 * @retval None
 */
void BSP_Key_Handler(uint8_t ms)
{
    uint8_t i = 0;
    uint8_t times = 0xFF;
    struct BSP_KEY *key_index = _key_head;
    
    for (; i < (8 - KEY_CONSECUTIVE_READ_TIME); i++)
        times >>= 1;
    
    for (; key_index; key_index = key_index->next)
    {
        /* ��ȡ������ƽֵ */
        key_index->continuous_value = (key_index->continuous_value << 1) | key_index->GetState();
            
        /* ��ȡ�������»���״̬ */
        if ((key_index->continuous_value & times) == 0x00)
        {
            if (key_index->press_level == KEY_PRESS_LOW)
                key_index->key_state = KEY_DOWN;
            else
                key_index->key_state = KEY_UP;
        }
        else if((key_index->continuous_value & times) == times) 
        {
            if (key_index->press_level == KEY_PRESS_HIGH)
                key_index->key_state = KEY_DOWN;
            else
                key_index->key_state = KEY_UP;
        }
        
        /* ����״̬�� */
        switch (key_index->state)
        {
            case 0:
            {
                if (key_index->key_state == KEY_DOWN)   /* �״ΰ��� */
                {
                    key_index->event = KEY_PRESS;
                    EVENT_CALLBACK( KEY_PRESS );
                    key_index->state = 1;
                }
                break;
            }
            case 1:
            {
                if (key_index->key_state == KEY_UP)     /* ���º�ſ� */
                {
                    key_index->tick = 0;
                #if (KEY_CLICK_EVENT_WHEN_DBLCLICK) 
                    key_index->event = KEY_CLICK;
                    EVENT_CALLBACK( KEY_CLICK );
                #endif
                    key_index->state = 2;
                }
                else if (key_index->key_state == KEY_DOWN)   /* �����ڵ�һ�ΰ��µ�״̬ */
                {
                    key_index->tick += ms;
                    
                #if (KEY_USER_DEFINE_PRESS_TIME)
                    if (key_index->tick >= key_index->press_time)   /* ��һ�ΰ���ʱ���ѳ�����������ʱ�� */
                #else
                    if (key_index->tick >= KEY_LONG_PRESS_TIME)     /* ��ʱδ���� */
                #endif
                    {
                        key_index->tick  = 0;
                        key_index->event = KEY_LONG_PRESS;
                        EVENT_CALLBACK( KEY_LONG_PRESS );
                        key_index->state = 4;
                    }
                }
                break;
            }
            case 2:     /* ����Ƿ�˫�� */
            {
                if (key_index->key_state == KEY_DOWN)       /* �������ٴΰ��� */
                {
                    if (KEY_DBLCLICK_PRESS_VALID)           /* �ڶ��ΰ�������������־ */
                    {
                        key_index->event = KEY_DBLCLICK;
                        EVENT_CALLBACK( KEY_DBLCLICK );
                    }
                    key_index->tick  = 0;
                    key_index->state = 3;
                }
                else if (key_index->key_state == KEY_UP)    /* δ���� */
                {
                    key_index->tick += ms;
                    
                    if (key_index->tick >= KEY_DBLCLICK_TIME)   /* ������ʱδ�ٰ��£��¼�����  */
                    {
                    #if (KEY_CLICK_EVENT_WHEN_DBLCLICK == 0) 
                        key_index->event = KEY_CLICK;
                        EVENT_CALLBACK( KEY_CLICK );
                    #endif
                        key_index->tick  = 0;
                        key_index->state = 0;
                        key_index->event = KEY_NONE_EVENT;
                    }
                }
                break;
            }
            case 3:     /* ˫���¼����ڶ����Ѱ��� */
            {
                if (key_index->key_state == KEY_UP)         /* �ڶ����ѷſ����� */
                {
                    if (KEY_DBLCLICK_PRESS_VALID == 0)      /* �ڶ��ΰ��·ſ���Ŵ�����־ */
                    {
                        key_index->event = KEY_DBLCLICK;
                        EVENT_CALLBACK( KEY_DBLCLICK );
                    }
                    key_index->tick  = 0;
                    key_index->state = 0;
                    key_index->event = KEY_NONE_EVENT;
                }
                break;
            }
            case 4:     /* ��ʱ�䰴�ţ�����˴�ʱ�����ص��¼��Ѿ���ִ����ϣ� */
            {
                if (key_index->key_state == KEY_DOWN)       /* ��ʱ���ɱ����� */
                {
                    key_index->tick += ms;

                #if (KEY_LONG_PRESS_CONTINU_TRIG_TIME)
                    /* ��Ҫ�������ִ��� */
                    if (key_index->tick >= KEY_LONG_PRESS_CONTINU_TRIG_TIME)
                    {
                        key_index->tick  = 0;
                        key_index->event = KEY_LONG_PRESS_HOLD;
                        EVENT_CALLBACK( KEY_LONG_PRESS_HOLD );
                    }
                #endif
                }
                else if (key_index->key_state == KEY_UP)    /* �û��ѷſ����������̽��� */
                {
                    key_index->tick  = 0;
                    key_index->state = 0;
                    key_index->event = KEY_NONE_EVENT;
                }
                break;
            }
            default :break;
        }
    }
}


#if (KEY_TEST)
#include "bsp_gpio.h"
#include "bsp_uart.h"

struct BSP_KEY s_key;
struct BSP_KEY c_key;
struct BSP_KEY sc_key;

static uint8_t S_Key_GetState(void)
{
    return BSP_GPIO_Read(s_key.id);
}

static uint8_t C_Key_GetState(void)
{
    return BSP_GPIO_Read(c_key.id);
}

/* ��ϰ��� */
static uint8_t SC_Key_GetState(void)
{
    if (BSP_GPIO_Read(s_key.id) || BSP_GPIO_Read(c_key.id))
        return GPIO_HIGH;
    else
        return GPIO_LOW;
}

static void Key_PressCallback(uint8_t id, uint8_t event)
{
    BSP_Printf("[ key ] You just press the button[%d], event: %d\r\n\r\n", id, event);
}

void Key_Test(void)
{
/*    BSP_Printf("size: %d\r\n\r\n", sizeof(func_key)); */
    
    BSP_Key_Init(&s_key, GET_PIN(B, 8), S_Key_GetState, KEY_PRESS_LOW);
    BSP_Key_Init(&c_key, GET_PIN(B, 9), C_Key_GetState, KEY_PRESS_LOW);
    BSP_Key_Init(&sc_key, 0, SC_Key_GetState, KEY_PRESS_LOW);
    
    BSP_Key_Register(&s_key, KEY_PRESS, Key_PressCallback);
    BSP_Key_Register(&s_key, KEY_CLICK, Key_PressCallback);
    BSP_Key_Register(&s_key, KEY_DBLCLICK, Key_PressCallback);
    BSP_Key_Register(&s_key, KEY_LONG_PRESS, Key_PressCallback);
    BSP_Key_Register(&c_key, KEY_PRESS, Key_PressCallback);
    BSP_Key_Register(&c_key, KEY_CLICK, Key_PressCallback);
    BSP_Key_Register(&c_key, KEY_DBLCLICK, Key_PressCallback);
    BSP_Key_Register(&c_key, KEY_LONG_PRESS, Key_PressCallback);
    
    BSP_Key_Register(&sc_key, KEY_CLICK, Key_PressCallback);
    BSP_Key_Register(&sc_key, KEY_DBLCLICK, Key_PressCallback);
    
    BSP_Key_Start(&s_key);
    BSP_Key_Start(&c_key);
    BSP_Key_Start(&sc_key);
}
#endif



