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

/* 定义项 */
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
    BSP_UART_ERR_COMM_ERR           = 0x01U,        /* 通讯错误（源自 HAL 库） */
    BSP_UART_ERR_BUSY               = 0x02U,        /* UART 外设忙碌（源自 HAL 库） */
    BSP_UART_ERR_TIMEOUT            = 0x03U,        /* 处理时间超时（源自 HAL 库） */
    BSP_UART_ERR_NOT_FOUND          = 0x04U,        /* 未找到对应的 UART 对象 */
    BSP_UART_ERR_LOCK_INIT_ERR      = 0x05U,        /* 通讯信号锁初始化失败 */
    BSP_UART_ERR_LOCK_ERR           = 0x06U,        /* 通讯信号上锁失败 */
    BSP_UART_ERR_UNLOCK_ERR         = 0x07U,        /* 通讯信号解锁失败 */
    BSP_UART_ERR_NO_RECV_FRAME      = 0x08U,        /* 还未收到一帧完整的数据 */
    BSP_UART_ERR_NO_INIT            = 0x09U,        /* 使用的 UART 对象还未初始化 */
    BSP_UART_ERR_NAME_DUPLICATE     = 0x0AU,        /* UART 对象命名重复 */

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
    
    /* 串口唯一标识信息 */
    const uint8_t id;
#if (USING_RTOS_TYPE)
    const char name[ MAX_NAME_LEN ];
#endif

    /* 用户串口数据，需要用户传入（二级缓存），当一级缓存半满和全满时，将会写入二级缓存 */
    uint8_t  *rx_data;
    uint16_t * volatile rx_data_len;
    uint16_t rx_data_max_len;
    
    /* 一些标志位 */
    volatile uint8_t init            :1;        /* 串口组件初始化标志位 */
    volatile uint8_t rx_init         :1;        /* 串口组件的接收功能初始化标志位 */
//    volatile uint8_t user_buff_full  :1;        /* 暂无使用 */
    volatile uint8_t idle_flag       :1;        /* 是否发生空闲中断的标志位 */
    volatile uint8_t                 :0;
    
    /* 串口数据一级缓存（由 DMA 无条件写入） */
    const uint8_t  *rx_buff;
    const uint16_t rx_buff_max_len;

    /* 环形缓存的“写”位置 */
    uint16_t old_pos;
    
    /* 回调函数 */
    uint8_t (*RX_Indicate)(struct UART_STRUCT *uart);
    uint8_t (*TX_Complete)(struct UART_STRUCT *uart);

    void *user_data;
};

typedef void (*UART_Callback_t)(struct UART_STRUCT *uart);


/**
 * BSP UART 用户接口 
 */
#if (USING_RTOS_TYPE)
BSP_UART_ERR    BSP_UART_WaitForData        (BSP_UART_ID  id);
#endif
BSP_UART_ERR    BSP_UART_Init               (BSP_UART_ID  id);
BSP_UART_ERR    BSP_UART_EnableReceive      (BSP_UART_ID  id, uint8_t *data, uint16_t *len, uint16_t max_len);
BSP_UART_ERR    BSP_UART_DisableReceive     (BSP_UART_ID  id);
BSP_UART_ERR    BSP_UART_LinkUserData       (BSP_UART_ID  id, void *user_data);
/* 注意！！！若使用 RTOS ，则 BSP_UART_Send 、 BSP_UART_SendBlocking 均不能在中断中使用 */
#if (USING_RTOS_TYPE)
BSP_UART_ERR    BSP_UART_Send               (BSP_UART_ID  id, const uint8_t *data, uint16_t len);
BSP_UART_ERR    BSP_UART_SendBlocking       (BSP_UART_ID  id, const uint8_t *data, uint16_t len, uint16_t timeout);
#else
BSP_UART_ERR    BSP_UART_Send               (BSP_UART_ID  id, const uint8_t *data, uint16_t len, uint16_t timeout);
#endif
/* 注意！！！若使用 RTOS ，则 BSP_Printf 不能在中断中使用 */
#if (ENABLE_DEBUG_PRINT && EANBLE_PRINTF_USING_RTT == 0)
void            BSP_Printf                  (const char *fmt, ...);
#endif
BSP_UART_ERR    BSP_UART_SetTxIndicate      (BSP_UART_ID  id, uint8_t (*TX_Complete)(struct UART_STRUCT *uart));
BSP_UART_ERR    BSP_UART_ClearUserBuff      (BSP_UART_ID  id);
BSP_UART_ERR    BSP_UART_IsFrameEnd         (BSP_UART_ID  id);


/**
 * BSP UART 移植接口，不对外使用
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


/* 通讯锁 */
BSP_UART_ERR    BSP_UART_Port_LockInit          (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_RxLock            (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_RxUnlock          (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_TxLock            (struct UART_STRUCT *uart);
BSP_UART_ERR    BSP_UART_Port_TxUnlock          (struct UART_STRUCT *uart);


#endif


