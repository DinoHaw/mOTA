/**
 * \file            bsp_uart.h
 * \brief           UART driver
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

#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "bsp_common.h"
#include "bsp_uart_config.h"

#if (USING_RTOS_TYPE)
#define UART_INIT_PARA(x)                       \
{                                               \
    .id              = BSP_UART##x,             \
    .rx_buff         = _uart##x##_buff,         \
    .name            = "_u"#x,                  \
    .rx_buff_max_len = BSP_UART_BUFF_SIZE,      \
}
#else
#define UART_INIT_PARA(x)                       \
{                                               \
    .id              = BSP_UART##x,             \
    .rx_buff         = _uart##x##_buff,          \
    .rx_buff_max_len = BSP_UART_BUFF_SIZE,      \
}
#endif

#define UART(x)             _uart##x

#define UART_CREATE(x)      static uint8_t _uart##x##_buff[BSP_UART_BUFF_SIZE];     \
                            static struct UART_STRUCT _uart##x = UART_INIT_PARA(x);

/* ������ */
typedef enum 
{
#if (BSP_USING_UART1)
    BSP_UART1    = 0x01,
#endif
#if (BSP_USING_UART2)
    BSP_UART2    = 0x02,
#endif
#if (BSP_USING_UART2_RE)
    BSP_UART2_RE = 0x82,
#endif
#if (BSP_USING_UART3)
    BSP_UART3    = 0x03,
#endif
#if (BSP_USING_UART3_RE)
    BSP_UART3_RE = 0x83,
#endif
#if (BSP_USING_UART4)
    BSP_UART4    = 0x04,
#endif
#if (BSP_USING_UART5)
    BSP_UART5    = 0x05,
#endif
#if (BSP_USING_UART6)
    BSP_UART6    = 0x06,
#endif
} BSP_UART_ID;

typedef enum 
{
    BSP_UART_ERR_OK                 = 0x00U,
    BSP_UART_ERR_COMM_ERR           = 0x01U,        /* ͨѶ����Դ�� HAL �⣩ */
    BSP_UART_ERR_BUSY               = 0x02U,        /* UART ����æµ��Դ�� HAL �⣩ */
    BSP_UART_ERR_TIMEOUT            = 0x03U,        /* ����ʱ�䳬ʱ��Դ�� HAL �⣩ */
    BSP_UART_ERR_NOT_FOUND          = 0x04U,        /* δ�ҵ���Ӧ�� UART ���� */
    BSP_UART_ERR_LOCK_INIT_ERR      = 0x05U,        /* ͨѶ�ź�����ʼ��ʧ�� */
    BSP_UART_ERR_LOCK_ERR           = 0x06U,        /* ͨѶ�ź�����ʧ�� */
    BSP_UART_ERR_UNLOCK_ERR         = 0x07U,        /* ͨѶ�źŽ���ʧ�� */
    BSP_UART_ERR_NO_RECV_FRAME      = 0x08U,        /* ��δ�յ�һ֡���������� */
    BSP_UART_ERR_NO_INIT            = 0x09U,        /* ʹ�õ� UART ����δ��ʼ�� */
    BSP_UART_ERR_NAME_DUPLICATE     = 0x0AU,        /* UART ���������ظ� */

} BSP_UART_ERR;

struct UART_STRUCT
{
    UART_HandleTypeDef      handle;

#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    /* for RT-Thread */
    struct rt_semaphore     rx_sem;
    struct rt_semaphore     tx_lock;
#elif (USING_RTOS_TYPE == RTOS_USING_UCOS)
    /* for uC/OS */
    OS_SEM     rx_sem;
    OS_SEM     tx_lock;
#endif
    
    /* ����Ψһ��ʶ��Ϣ */
    const uint8_t id;
#if (USING_RTOS_TYPE)
    const char name[ MAX_NAME_LEN ];
#endif

    /* �û��������ݣ���Ҫ�û����루�������棩����һ�����������ȫ��ʱ������д��������� */
    uint8_t  *rx_data;
    uint16_t * volatile rx_data_len;
    uint16_t rx_data_max_len;
    
    /* һЩ��־λ */
    volatile uint8_t init            :1;        /* ���������ʼ����־λ */
    volatile uint8_t rx_init         :1;        /* ��������Ľ��չ��ܳ�ʼ����־λ */
//    volatile uint8_t user_buff_full  :1;        /* ����ʹ�� */
    volatile uint8_t idle_flag       :1;        /* �Ƿ��������жϵı�־λ */
    volatile uint8_t                 :0;
    
    /* ��������һ�����棨�� DMA ������д�룩 */
    const uint8_t  *rx_buff;
    const uint16_t rx_buff_max_len;

    /* ���λ���ġ�д��λ�� */
    uint16_t old_pos;
    
    /* �ص����� */
    uint8_t (*RX_Indicate)(struct UART_STRUCT *uart);
    uint8_t (*TX_Complete)(struct UART_STRUCT *uart);

    void *user_data;
};

typedef void (*UART_Callback_t)(struct UART_STRUCT *uart);


/**
 * BSP UART �û��ӿ� 
 */
#if (USING_RTOS_TYPE)
BSP_UART_ERR    BSP_UART_WaitForData        (BSP_UART_ID  id);
#endif
BSP_UART_ERR    BSP_UART_Init               (BSP_UART_ID  id);
BSP_UART_ERR    BSP_UART_EnableReceive      (BSP_UART_ID  id, uint8_t *data, uint16_t *len, uint16_t max_len);
BSP_UART_ERR    BSP_UART_DisableReceive     (BSP_UART_ID  id);
BSP_UART_ERR    BSP_UART_LinkUserData       (BSP_UART_ID  id, void *user_data);
/* ע�⣡������ʹ�� RTOS ���� BSP_UART_Send �� BSP_UART_SendBlocking ���������ж���ʹ�� */
#if (USING_RTOS_TYPE)
BSP_UART_ERR    BSP_UART_Send               (BSP_UART_ID  id, const uint8_t *data, uint16_t len);
BSP_UART_ERR    BSP_UART_SendBlocking       (BSP_UART_ID  id, const uint8_t *data, uint16_t len, uint16_t timeout);
#else
BSP_UART_ERR    BSP_UART_Send               (BSP_UART_ID  id, const uint8_t *data, uint16_t len, uint16_t timeout);
#endif
/* ע�⣡������ʹ�� RTOS ���� BSP_Printf �������ж���ʹ�� */
#if (ENABLE_DEBUG_PRINT && EANBLE_PRINTF_USING_RTT == 0)
void            BSP_Printf                  (const char *fmt, ...);
#endif
BSP_UART_ERR    BSP_UART_SetTxIndicate      (BSP_UART_ID  id, uint8_t (*TX_Complete)(struct UART_STRUCT *uart));
BSP_UART_ERR    BSP_UART_ClearUserBuff      (BSP_UART_ID  id);
BSP_UART_ERR    BSP_UART_IsFrameEnd         (BSP_UART_ID  id);


/**
 * BSP UART ��ֲ�ӿڣ�������ʹ��
 */
void            BSP_UART_Port_Init              ( struct UART_STRUCT *uart, 
                                                  UART_Callback_t rx_callback, 
                                                  UART_Callback_t rx_idle_callback, 
                                                  UART_Callback_t dma_rx_callback, 
                                                  UART_Callback_t dma_tx_callback);
BSP_UART_ERR    BSP_UART_Port_EnableReceive     (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_DisableReceive    (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_Send              (struct UART_STRUCT *uart, const uint8_t *data, uint16_t len, uint16_t timeout);
uint32_t        BSP_UART_Port_GetDmaCounter     (struct UART_STRUCT *uart);
uint32_t        BSP_UART_Port_GetOneByte        (struct UART_STRUCT *uart);
struct UART_STRUCT *BSP_UART_Port_GetHandle     (BSP_UART_ID  id);


/* ͨѶ�� */
BSP_UART_ERR    BSP_UART_Port_LockInit          (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_RxLock            (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_RxUnlock          (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_TxLock            (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_TxUnlock          (struct UART_STRUCT *uart);


#endif


