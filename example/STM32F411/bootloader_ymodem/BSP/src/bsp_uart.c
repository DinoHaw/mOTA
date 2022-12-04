/**
 * \file            bsp_uart.c
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
 * Version:         v1.0.1
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-23     Dino         the first version
 * 2022-12-04     Dino         优化中断开关
 */


/* Includes ------------------------------------------------------------------*/
#include "bsp_uart.h"


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void UART_CopyDataToUserBuff (struct UART_STRUCT *uart);
static void UART_RxIntHandler       (struct UART_STRUCT *uart);
static void UART_RxIdleHandler      (struct UART_STRUCT *uart);
static void UART_TxHandler          (struct UART_STRUCT *uart);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  UART 组件初始化
 * @note   仅支持已实现的串口
 * @param[in]  id: 串口 ID
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Init(BSP_UART_ID  id)
{
    /* GPIO/UART/DMA 等的初始化代码已由 cubeMX 生成 */

    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;
    
    if (uart->init)
        return BSP_UART_ERR_NO_INIT;

    BSP_UART_Port_Init( uart, 
                        UART_RxIntHandler, 
                        UART_RxIdleHandler, 
                        UART_CopyDataToUserBuff, 
                        UART_TxHandler);
    
    BSP_INT_DIS();
    uart->init = 1;
    BSP_INT_EN();

    return BSP_UART_ERR_OK;
}


/**
 * @brief  使能UART接收数据
 * @note   
 * @param[in]  id: 串口 ID
 * @param[in]  data: 接收的数据池
 * @param[in]  len: 指示接收数据长度的变量
 * @param[in]  max_len: 数据池的最大容量，单位 byte
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_EnableReceive(BSP_UART_ID  id, uint8_t *data, uint16_t *len, uint16_t max_len)
{
    ASSERT(data != NULL && len != NULL && max_len != 0);
    
    BSP_UART_ERR ret;
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);

    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;

    uart->rx_data         = data;
    uart->rx_data_len     = len;
    uart->rx_data_max_len = max_len;

    ret = BSP_UART_Port_EnableReceive(uart);
    
    BSP_INT_DIS();
    uart->rx_init = 1;
    BSP_INT_EN();

    return ret;
}


/**
 * @brief  禁止 UART 接收数据
 * @note   
 * @param[in]  id: 串口 ID
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_DisableReceive(BSP_UART_ID  id)
{
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;
    
    BSP_INT_DIS();
    uart->rx_init = 0;
    BSP_INT_EN();
    
    return BSP_UART_Port_DisableReceive(uart);
}


#if (USING_RTOS_TYPE)
/**
 * @brief  等待 UART 接收到一帧数据
 * @note   
 * @param[in]  id: 串口 ID
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_WaitForData(BSP_UART_ID  id)
{
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;
    
    return BSP_UART_Port_RxLock(uart);
}
#endif


/**
 * @brief  判断 UART 是否收到了一帧数据
 * @note   该接口为轮询的方式
 * @param[in]  id: 串口 ID
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_IsFrameEnd(BSP_UART_ID  id)
{
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL)  
        return BSP_UART_ERR_NOT_FOUND; 

//    BSP_INT_DIS();
    if (uart->idle_flag == 0)
    {    
//        BSP_INT_EN();
        return BSP_UART_ERR_NO_RECV_FRAME;
    }
    else
    {
        uart->idle_flag = 0;
//        BSP_INT_EN();
        return BSP_UART_ERR_OK;
    }
}


/**
 * @brief  挂载一个用户自定义的数据到 UART 对象上
 * @note   
 * @param[in]  id: 串口 ID
 * @param[in]  user_data: 用户自定义数据
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_LinkUserData(BSP_UART_ID  id, void *user_data)
{
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL) 
        return BSP_UART_ERR_NOT_FOUND;
    
    uart->user_data = user_data;
    
    return BSP_UART_ERR_OK;
}


/**
 * @brief  设置 UART 的发送完成指示回调函数
 * @note   
 * @param[in]  id: 串口 ID
 * @param[in]  TX_Complete: 函数指针
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_SetTxIndicate(BSP_UART_ID  id, uint8_t (*TX_Complete)(struct UART_STRUCT *uart))
{
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;
    
    uart->TX_Complete = TX_Complete;
    
    return BSP_UART_ERR_OK;
}


/**
 * @brief  从 UART 发出一些数据
 * @note   若使用 RTOS ，则 BSP_UART_Send、 BSP_UART_SendBlocking 均不能在中断中使用
 * @param[in]  id: 串口ID
 * @param[in]  data: 要发送的数据池
 * @param[in]  len: 要发送的数据长度，单位 byte
 * @param[in]  timeout: 发送最大超时等待时间，单位 ms
 * @retval BSP_UART_ERR 
 */
#if (USING_RTOS_TYPE)
BSP_UART_ERR  BSP_UART_Send(BSP_UART_ID  id, const uint8_t *data, uint16_t len)
#else
BSP_UART_ERR  BSP_UART_Send(BSP_UART_ID  id, const uint8_t *data, uint16_t len, uint16_t timeout)
#endif
{
    ASSERT(data != NULL && len != 0);
    
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;

    if (uart->init == 0)
        return BSP_UART_ERR_NO_INIT;
    
#if (USING_RTOS_TYPE)
    BSP_UART_Port_TxLock(uart);

    return BSP_UART_Port_Send(uart, data, len, 0);
#else
    return BSP_UART_Port_Send(uart, data, len, timeout);
#endif
}


#if (USING_RTOS_TYPE)
/**
 * @brief  从 UART 发出一些数据（阻塞式，直到数据发送完毕或超时）
 * @note   若使用 RTOS ，则 BSP_UART_Send、 BSP_UART_SendBlocking 均不能在中断中使用
 * @param[in]  id: 串口 ID
 * @param[in]  data: 要发送的数据池
 * @param[in]  len: 要发送的数据长度，单位 byte
 * @param[in]  timeout: 发送最大超时等待时间，单位 ms
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_SendBlocking(BSP_UART_ID  id, const uint8_t *data, uint16_t len, uint16_t timeout)
{
    ASSERT(data != NULL && len != 0);
    
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;

    if (uart->init == 0)
        return BSP_UART_ERR_NO_INIT;

    return BSP_UART_Port_Send(uart, data, len, timeout);
}
#endif



/**
 * @brief  清除用户传入的 UART 数据缓存池
 * @note   
 * @param[in]  id: 串口 ID
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_ClearUserBuff(BSP_UART_ID  id)
{
    struct UART_STRUCT *uart = BSP_UART_Port_GetHandle(id);
    
    if (uart == NULL) 
        return BSP_UART_ERR_NOT_FOUND;
    
    BSP_INT_DIS();
    
    if (uart->rx_data_len)
        *(uart->rx_data_len) = 0;
    
    BSP_INT_EN();
    
    return BSP_UART_ERR_OK;
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  将 DMA 缓存池的 UART 数据搬运到用户数据池中
 * @note   
 * @param[in]  uart: UART 对象
 * @retval None
 */
static void UART_CopyDataToUserBuff(struct UART_STRUCT *uart)
{
    if (*(uart->rx_data_len) >= uart->rx_data_max_len)
        return;

//    BSP_INT_DIS();
    
    uint8_t  *user_buff     = &uart->rx_data[0];
    uint16_t *user_buff_len = (uint16_t *)(uart->rx_data_len);
    uint16_t new_pos        = uart->rx_buff_max_len - BSP_UART_Port_GetDmaCounter(uart);   /* 计算缓冲区的接收字节数 */
    uint16_t recv_len       = 0;

    if (new_pos != uart->old_pos)                   /* 收到新的数据 */
    {
        if (new_pos > uart->old_pos)                /* 线性模式 */
        {
            recv_len = new_pos - uart->old_pos;     /* 计算本次接收到的字节数 */
            
            /* 数据超出用户 buff ，只能接收用户 buff 剩余空间的长度数据 */
            if ((*user_buff_len) + recv_len > uart->rx_data_max_len)
            {
                recv_len = uart->rx_data_max_len - *user_buff_len;
            }
            
            memcpy(&user_buff[*user_buff_len], &uart->rx_buff[ uart->old_pos ], recv_len);
            (*user_buff_len) += recv_len;    /* 设置偏移量 */
        }
        else    /* 溢出模式 */
        {
            /* 先处理未溢出的部分 */
            recv_len = uart->rx_buff_max_len - uart->old_pos;
            
            /* 数据超出用户 buff ，只能接收用户 buff 剩余空间的长度数据 */
            if (*user_buff_len + recv_len > uart->rx_data_max_len)
            {
                recv_len = uart->rx_data_max_len - *user_buff_len;
            }
            
            memcpy(&user_buff[*user_buff_len], &uart->rx_buff[ uart->old_pos ], recv_len);
            *user_buff_len += recv_len;    /* 设置偏移量 */
            
            /* 再处理溢出的部分 */
            recv_len = new_pos;
            
            if (recv_len != 0)
            {
                /* 数据超出用户 buff ，只能接收用户 buff 剩余空间的长度数据 */
                if (*user_buff_len + recv_len > uart->rx_data_max_len)
                {
                    recv_len = uart->rx_data_max_len - *user_buff_len;
                }
                
                memcpy(&user_buff[*user_buff_len], &uart->rx_buff[0], recv_len);
                (*user_buff_len) += recv_len;    /* 设置偏移量 */
            }
        }
    }
    uart->old_pos = new_pos;

    if (uart->old_pos >= uart->rx_buff_max_len) 
    {
        uart->old_pos = 0;
    }
    
//    BSP_INT_EN();
}


/**
 * @brief  串口接收到单字节数据的中断处理函数
 * @note   
 * @param[in]  uart: UART 对象
 * @retval None
 */
static void UART_RxIntHandler(struct UART_STRUCT *uart)
{
    if (uart->rx_init == 0)
        return;
    
    uart->rx_data[ *(uart->rx_data_len) ] = (uint8_t)BSP_UART_Port_GetOneByte(uart);
    *(uart->rx_data_len) += 1;
    
    if (*(uart->rx_data_len) == uart->rx_data_max_len)
    {
        *(uart->rx_data_len) = 0;
    }
}


/**
 * @brief  串口发生空闲中断的处理函数
 * @note   
 * @param[in]  uart: UART 对象
 * @retval None
 */
static void UART_RxIdleHandler(struct UART_STRUCT *uart)
{
    if (uart->rx_init == 0)
        return;
    
//    BSP_INT_DIS();
    uart->idle_flag = 1;
//    BSP_INT_EN();

    if (uart->handle.hdmarx)
        UART_CopyDataToUserBuff(uart);
    
    BSP_UART_Port_RxUnlock(uart);
}


/**
 * @brief  串口发送完成的中断处理函数
 * @note   
 * @param[in]  uart: UART 对象
 * @retval None
 */
static void UART_TxHandler(struct UART_STRUCT *uart)
{
    BSP_UART_Port_TxUnlock(uart);
        
    if (uart->TX_Complete)
        uart->TX_Complete(uart);
}




