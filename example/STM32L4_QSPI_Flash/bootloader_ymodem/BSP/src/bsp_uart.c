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
 * 2022-12-04     Dino         �Ż��жϿ���
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
 * @brief  UART �����ʼ��
 * @note   ��֧����ʵ�ֵĴ���
 * @param[in]  id: ���� ID
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Init(BSP_UART_ID  id)
{
    /* GPIO/UART/DMA �ȵĳ�ʼ���������� cubeMX ���� */

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
 * @brief  ʹ��UART��������
 * @note   
 * @param[in]  id: ���� ID
 * @param[in]  data: ���յ����ݳ�
 * @param[in]  len: ָʾ�������ݳ��ȵı���
 * @param[in]  max_len: ���ݳص������������λ byte
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
 * @brief  ��ֹ UART ��������
 * @note   
 * @param[in]  id: ���� ID
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
 * @brief  �ȴ� UART ���յ�һ֡����
 * @note   
 * @param[in]  id: ���� ID
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
 * @brief  �ж� UART �Ƿ��յ���һ֡����
 * @note   �ýӿ�Ϊ��ѯ�ķ�ʽ
 * @param[in]  id: ���� ID
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
 * @brief  ����һ���û��Զ�������ݵ� UART ������
 * @note   
 * @param[in]  id: ���� ID
 * @param[in]  user_data: �û��Զ�������
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
 * @brief  ���� UART �ķ������ָʾ�ص�����
 * @note   
 * @param[in]  id: ���� ID
 * @param[in]  TX_Complete: ����ָ��
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
 * @brief  �� UART ����һЩ����
 * @note   ��ʹ�� RTOS ���� BSP_UART_Send�� BSP_UART_SendBlocking ���������ж���ʹ��
 * @param[in]  id: ����ID
 * @param[in]  data: Ҫ���͵����ݳ�
 * @param[in]  len: Ҫ���͵����ݳ��ȣ���λ byte
 * @param[in]  timeout: �������ʱ�ȴ�ʱ�䣬��λ ms
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
 * @brief  �� UART ����һЩ���ݣ�����ʽ��ֱ�����ݷ�����ϻ�ʱ��
 * @note   ��ʹ�� RTOS ���� BSP_UART_Send�� BSP_UART_SendBlocking ���������ж���ʹ��
 * @param[in]  id: ���� ID
 * @param[in]  data: Ҫ���͵����ݳ�
 * @param[in]  len: Ҫ���͵����ݳ��ȣ���λ byte
 * @param[in]  timeout: �������ʱ�ȴ�ʱ�䣬��λ ms
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
 * @brief  ����û������ UART ���ݻ����
 * @note   
 * @param[in]  id: ���� ID
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
 * @brief  �� DMA ����ص� UART ���ݰ��˵��û����ݳ���
 * @note   
 * @param[in]  uart: UART ����
 * @retval None
 */
static void UART_CopyDataToUserBuff(struct UART_STRUCT *uart)
{
    if (*(uart->rx_data_len) >= uart->rx_data_max_len)
        return;

//    BSP_INT_DIS();
    
    uint8_t  *user_buff     = &uart->rx_data[0];
    uint16_t *user_buff_len = (uint16_t *)(uart->rx_data_len);
    uint16_t new_pos        = uart->rx_buff_max_len - BSP_UART_Port_GetDmaCounter(uart);   /* ���㻺�����Ľ����ֽ��� */
    uint16_t recv_len       = 0;

    if (new_pos != uart->old_pos)                   /* �յ��µ����� */
    {
        if (new_pos > uart->old_pos)                /* ����ģʽ */
        {
            recv_len = new_pos - uart->old_pos;     /* ���㱾�ν��յ����ֽ��� */
            
            /* ���ݳ����û� buff ��ֻ�ܽ����û� buff ʣ��ռ�ĳ������� */
            if ((*user_buff_len) + recv_len > uart->rx_data_max_len)
            {
                recv_len = uart->rx_data_max_len - *user_buff_len;
            }
            
            memcpy(&user_buff[*user_buff_len], &uart->rx_buff[ uart->old_pos ], recv_len);
            (*user_buff_len) += recv_len;    /* ����ƫ���� */
        }
        else    /* ���ģʽ */
        {
            /* �ȴ���δ����Ĳ��� */
            recv_len = uart->rx_buff_max_len - uart->old_pos;
            
            /* ���ݳ����û� buff ��ֻ�ܽ����û� buff ʣ��ռ�ĳ������� */
            if (*user_buff_len + recv_len > uart->rx_data_max_len)
            {
                recv_len = uart->rx_data_max_len - *user_buff_len;
            }
            
            memcpy(&user_buff[*user_buff_len], &uart->rx_buff[ uart->old_pos ], recv_len);
            *user_buff_len += recv_len;    /* ����ƫ���� */
            
            /* �ٴ�������Ĳ��� */
            recv_len = new_pos;
            
            if (recv_len != 0)
            {
                /* ���ݳ����û� buff ��ֻ�ܽ����û� buff ʣ��ռ�ĳ������� */
                if (*user_buff_len + recv_len > uart->rx_data_max_len)
                {
                    recv_len = uart->rx_data_max_len - *user_buff_len;
                }
                
                memcpy(&user_buff[*user_buff_len], &uart->rx_buff[0], recv_len);
                (*user_buff_len) += recv_len;    /* ����ƫ���� */
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
 * @brief  ���ڽ��յ����ֽ����ݵ��жϴ�����
 * @note   
 * @param[in]  uart: UART ����
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
 * @brief  ���ڷ��������жϵĴ�����
 * @note   
 * @param[in]  uart: UART ����
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
 * @brief  ���ڷ�����ɵ��жϴ�����
 * @note   
 * @param[in]  uart: UART ����
 * @retval None
 */
static void UART_TxHandler(struct UART_STRUCT *uart)
{
    BSP_UART_Port_TxUnlock(uart);
        
    if (uart->TX_Complete)
        uart->TX_Complete(uart);
}




