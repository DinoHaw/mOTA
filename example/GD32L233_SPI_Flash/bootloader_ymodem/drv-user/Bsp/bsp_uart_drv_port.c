/**
 * \file            bsp_uart_drv_port.c
 * \brief           bsp uart driver port file
 */

/*
 * Copyright (c) 2024
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
 * Author:          wade任
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2024-01-08     wade任       the first version
 */

/* Includes ------------------------------------------------------------------*/
#include "bsp_uart_drv_port.h"
#include "bsp_usart1_drv.h"
#include "bsp_common.h"


/* Private variables ---------------------------------------------------------*/
struct UART_STRUCT uart1;
struct UART_STRUCT *uart = &uart1;


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  UART 组件初始化
 * @note   仅支持已实现的串口
 * @param[in]  id: 串口 ID
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Init(BSP_UART_ID  id)
{
    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;
    
    if (uart->is_init)
        return BSP_UART_ERR_NO_INIT;
    
    bsp_usart1_hardware_init();	/* 接口初始化 */
    
    BSP_Printf("-----------------------------\r\n");
    BSP_Printf("[ %s ]\r\n", __func__);
    BSP_Printf("id: 1 \r\n");
    BSP_Printf("name: USART1 \r\n");
    BSP_Printf("rx_buff: 0x%.8p\r\n", &uart->rx_buff[0]);
    BSP_Printf("uart size: %d byte\r\n", BSP_UART_BUFF_SIZE);
    BSP_Printf("-----------------------------\r\n\r\n");
    
    __IRQ_SAFE {
        uart->is_init = true;
    }

    return BSP_UART_ERR_OK;
}

/**
 * @brief  使能 UART 接收数据
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
    
    BSP_UART_ERR ret = BSP_UART_ERR_OK;

    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;

    uart->rx_data         = data;
    uart->rx_data_len     = len;
    uart->rx_data_max_len = max_len;

    usart_interrupt_enable(COM_USART1, USART_INT_RBNE);
    usart_interrupt_enable(COM_USART1, USART_INT_IDLE);
    
    __IRQ_SAFE {
        uart->is_rx_init = true;
    }

    return ret;
}

/**
 * @brief  禁止 UART 接收数据
 * @note   
 * @param[in]  uart: UART 对象
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_DisableReceive(struct UART_STRUCT *uart)
{
    usart_interrupt_disable(COM_USART1, USART_INT_RBNE);
    usart_interrupt_disable(COM_USART1, USART_INT_IDLE);
    
    return BSP_UART_ERR_OK;
}

/**
 * @brief  控制 UART 发送一帧数据
 * @note   
 * @param[in]  uart: UART 对象
 * @param[in]  data: 要发送的数据
 * @param[in]  len: 要发送的数据长度，单位 byte。最大长度: 65535 byte
 * @param[in]  timeout: 最大处理超时时间，单位 ms。最大指定时间: 65535 ms
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_Send(struct UART_STRUCT *uart, const uint8_t *data, uint16_t len, uint16_t timeout)
{
    bsp_usart1_send_string((char *)data, len);
    
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
BSP_UART_ERR  BSP_UART_Send(BSP_UART_ID  id, const uint8_t *data, uint16_t len, uint16_t timeout)
{
    if (uart == NULL)
        return BSP_UART_ERR_NOT_FOUND;

    if (uart->is_init == false)
        return BSP_UART_ERR_NO_INIT;
    
    return BSP_UART_Port_Send(uart, data, len, timeout);
}

/**
 * @brief  判断 UART 是否收到了一帧数据
 * @note   该接口为轮询的方式
 * @param[in]  id: 串口 ID
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_IsFrameEnd(BSP_UART_ID  id)
{
    if (uart == NULL)  
        return BSP_UART_ERR_NOT_FOUND; 

    if (uart->is_idle_int == false)
    {    
        return BSP_UART_ERR_NO_RECV_FRAME;
    }
    else
    {
        uart->is_idle_int = false;
        return BSP_UART_ERR_OK;
    }
}

/**
 * @brief  清除用户传入的 UART 数据缓存池
 * @note   
 * @param[in]  id: 串口 ID
 * @retval BSP_UART_ERR 
 */
BSP_UART_ERR  BSP_UART_ClearUserBuff(BSP_UART_ID  id)
{
    if (uart == NULL) 
        return BSP_UART_ERR_NOT_FOUND;
    
    __IRQ_SAFE
    {
        if (uart->rx_data_len) {
            *(uart->rx_data_len) = 0;
        }
    }
    
    return BSP_UART_ERR_OK;
}

/**
 * @brief  获取 DMA 当前的计数
 * @note   
 * @param[in]  uart: UART 对象
 * @retval 计数值
 */
inline uint32_t BSP_UART_Port_GetDmaCounter(struct UART_STRUCT *uart)
{
    return dma_transfer_number_get(DMA_CH0);
}


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

void USART1_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RT)) 
    {
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RT);
    }
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)) 
    {
        uart->rx_data[ *(uart->rx_data_len) ] = (uint8_t)usart_data_receive(USART1);
        *(uart->rx_data_len) += 1;
        
        if (*(uart->rx_data_len) == uart->rx_data_max_len)
        {
            *(uart->rx_data_len) = 0;
        }
    }
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE)) 
    {
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);
        uart->is_idle_int = true;
        UART_CopyDataToUserBuff(uart);
    }
}
