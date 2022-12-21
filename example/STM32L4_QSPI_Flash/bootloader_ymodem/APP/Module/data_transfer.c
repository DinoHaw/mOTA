/**
 * \file            data_transfer.c
 * \brief           transfer low layer data
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
#include "data_transfer.h"
#include "data_transfer_port.h"


/* ���ݴ����: ����ײ����ݵ��շ������ϲ��ṩ��ʼ�������͡����ա��Ƿ���յ���һ֡���ݵĽӿ� */
/* �ϲ���ָ�����������õĻ���������������С������ָʾ��������ǰ���յ����ݵı��� */


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  ���ݴ�����ʼ��
 * @note   
 * @param[in]  xfer: ������ƿ����
 * @param[in]  if_id: ����ӿ� ID
 * @param[in]  buff: ���ڽ������ݵĻ���أ���λ byte
 * @param[in]  len: ָʾ���յ������ݳ��ȣ���λ byte
 * @param[in]  buff_size: ���ݳ������������λ byte
 * @retval None
 */
void DT_Init(struct DATA_TRANSFER *xfer, 
             uint8_t  if_id, 
             uint8_t  *buff, 
             uint16_t *len, 
             uint32_t buff_size)
{
    ASSERT(xfer != NULL);
    
    xfer->if_id        = if_id;
    xfer->rx_buff      = buff;
    xfer->rx_len       = len;
    xfer->rx_buff_size = buff_size;
  
    DT_Port_Init(xfer);
}


/**
 * @brief  ���ݷ��ͽӿ�
 * @note   
 * @param[in]  xfer: ������ƿ����
 * @param[in]  data: Ҫ���͵�����
 * @param[in]  len: Ҫ���͵����ݳ��ȣ���λ byte
 * @retval None
 */
void DT_Send(struct DATA_TRANSFER *xfer, uint8_t *data, uint32_t len)
{
    DT_Port_SendData(xfer, data, len);
}


/**
 * @brief  ����Ƿ���յ�һ֡���ݵ���ѯ�ӿ�
 * @note   
 * @param[in]  xfer: ������ƿ����
 * @retval DT_RECV_DATA_RESULT
 */
DT_RECV_DATA_RESULT  DT_PollingReceive(struct DATA_TRANSFER *xfer)
{
    DT_RECV_DATA_RESULT  state = DT_RESULT_NO_DATA;
    
    if (DT_Port_IsRecvData(xfer) == 0)
    {
        state = DT_RESULT_RECV_FRAME_DATA;
    }
    
    return state;
}


/**
 * @brief  ������ڽ������ݵ� buff
 * @note   
 * @param[in]  xfer: ������ƿ���� 
 * @retval None
 */
inline void DT_ClearBuff(struct DATA_TRANSFER *xfer)
{
    DT_Port_ClearRecvBuff(xfer);
}











