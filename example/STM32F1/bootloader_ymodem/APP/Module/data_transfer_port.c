/**
 * \file            data_transfer_port.c
 * \brief           portable file of the transfer data layer
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
#include "data_transfer_port.h"


/* Private variables ---------------------------------------------------------*/
static struct BSP_TIMER _timer_frame_detect;


/* Private function prototypes -----------------------------------------------*/
static void _Timeout_FrameDetect(void *user_data);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  �ײ����ݳ�ʼ���ӿ�
 * @note   
 * @param[in]  xfer: ���ݽӿڶ���
 * @retval None
 */
void DT_Port_Init(struct DATA_TRANSFER *xfer)
{
    BSP_UART_Init((BSP_UART_ID)xfer->if_id);
    BSP_UART_LinkUserData((BSP_UART_ID)xfer->if_id, xfer);
    BSP_UART_EnableReceive( (BSP_UART_ID)xfer->if_id,
                            xfer->rx_buff, 
                            xfer->rx_len,
                            xfer->rx_buff_size);

    BSP_Timer_Init( &_timer_frame_detect, 
                    _Timeout_FrameDetect, 
                    BROKEN_FRAME_INTERVAL_TIME, 
                    1, 
                    TIMER_TYPE_HARDWARE);    
}


/**
 * @brief  �ײ����ݷ��ͽӿ�
 * @note   
 * @param[in]  xfer: ���ݽӿڶ���
 * @param[in]  data: Ҫ���͵�����
 * @param[in]  len: Ҫ���͵����ݳ��ȣ���λ byte
 * @retval None
 */
inline void DT_Port_SendData(struct DATA_TRANSFER *xfer, uint8_t *data, uint32_t len)
{
    BSP_UART_Send((BSP_UART_ID)xfer->if_id, data, len, 0xFFFF);
}


/**
 * @brief  �ײ����ݲ�ѯ�Ƿ���յ�һ֡���ݵĽӿ�
 * @note   
 * @param[in]  xfer: ���ݽӿڶ���
 * @retval �鿴 BSP_UART_IsFrameEnd
 */
inline uint8_t DT_Port_IsRecvData(struct DATA_TRANSFER *xfer)
{
    return BSP_UART_IsFrameEnd((BSP_UART_ID)xfer->if_id);
}


/**
 * @brief  �ײ����ݻ�������ӿ�
 * @note   
 * @param[in]  xfer: ���ݽӿڶ���
 * @retval None
 */
inline void DT_Port_ClearRecvBuff(struct DATA_TRANSFER *xfer)
{
    BSP_UART_ClearUserBuff((BSP_UART_ID)xfer->if_id);
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  ����֡��ⳬʱ����ص�����
 * @note   
 * @param[in]  user_data: �û�����
 * @retval None
 */
static void _Timeout_FrameDetect(void *user_data)
{
    BSP_Printf("frame detect clock time up!\r\n");
}

