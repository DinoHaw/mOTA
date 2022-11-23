/**
 * \file            data_transfer.h
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

#ifndef __DATA_TRANSFER_H__
#define __DATA_TRANSFER_H__

#include "bsp_common.h"

typedef enum
{
    DT_RESULT_RECV_FRAME_DATA           = 0x00,         /* 收到了一帧数据 */
    DT_RESULT_NO_DATA                   = 0x01,         /* 未收到数据 */
    DT_RESULT_JUST_RECV_DATA            = 0x02,         /* 刚收到一帧数据（可能是断帧） */
    DT_RESULT_WAIT_BROKEN_FRAME_DECTECT = 0x03,         /* 等待断帧判断 */
    
} DT_RECV_DATA_RESULT;

struct DATA_TRANSFER
{
    uint8_t  if_id;
    
    uint8_t  *rx_buff;
    uint16_t *rx_len;
    uint32_t rx_buff_size;
};


void DT_Init(struct DATA_TRANSFER *xfer, 
             uint8_t  if_id, 
             uint8_t  *buff, 
             uint16_t *len, 
             uint32_t buff_size);
void DT_Send(struct DATA_TRANSFER *xfer, uint8_t *data, uint32_t len);
DT_RECV_DATA_RESULT  DT_PollingReceive(struct DATA_TRANSFER *xfer);
void DT_ClearBuff(struct DATA_TRANSFER *xfer);


#endif
