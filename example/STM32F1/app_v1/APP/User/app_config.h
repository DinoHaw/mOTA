/**
 * \file            app_config.h
 * \brief           configuration of the application
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

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* APP �汾 */
#define APP_VERSION_MAIN                    (0x01U) /*!< [15:8] main version */
#define APP_VERSION_SUB                     (0x00U) /*!< [7:0] sub version */
#define APP_VERSION                         ((APP_VERSION_MAIN << 8)    \
                                             |(APP_VERSION_SUB))

/* ������ */
#define RTOS_USING_NONE                     0                   /* ��ʹ��RTOS */
#define RTOS_USING_RTTHREAD                 1                   /* RT-Thread */
#define RTOS_USING_UCOS                     2                   /* uC/OS */


/* ����ѡ�� */
#define ENABLE_ASSERT                       0                   /* �Ƿ�ʹ�ܺ�����ڲ������ */
#define ENABLE_DEBUG_PRINT                  1                   /* �Ƿ�ʹ�ܵ�����Ϣ��ӡ */
#define EANBLE_PRINTF_USING_RTT             0                   /* BSP_Print�����Ƿ�ʹ��SEGGER RTT��Ϊ����˿� */

#define USING_RTOS_TYPE                     RTOS_USING_NONE
#define SEGGER_RTT_PRINTF_TERMINAL          0                   /* SEGGER RTT�Ĵ�ӡ�˿� */
#define MAX_NAME_LEN                        8

#define LED0_PIN                            GET_PIN(B, 5)
#define KEY0_PIN                            GET_PIN(E, 4)

#define FIRMWARE_UPDATE_VAR_ADDR            0x20000000          /* �̼����±�־λ�Ĵ�ŵ�ַ��һ��Ҫ�� bootloader һ�£� */
#define FIRMWARE_UPDATE_MAGIC_WORD          0xA5A5A5A5          /* �̼���Ҫ���µ������ǣ�һ��Ҫ�� bootloader һ�£� */
#define FIRMWARE_RECOVERY_MAGIC_WORD        0x5A5A5A5A          /* ��Ҫ�ָ������̼��������ǣ�һ��Ҫ�� bootloader һ�£� */

#endif
