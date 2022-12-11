/**
 * \file            data_transfer_port.h
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
 * Version:         v1.0.1
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-23     Dino         the first version
 * 2022-12-04     Dino         增加断帧检测
 */

#ifndef __DATA_TRANSFER_PORT_H__
#define __DATA_TRANSFER_PORT_H__

#include "bsp_common.h"
#include "data_transfer.h"

/* 是否使能断帧检测 */
#define DT_ENABLE_BROKEN_FRAME_DETECT   0

#define BROKEN_FRAME_INTERVAL_TIME      100         /* 断帧间隔时间判断，单位 ms */

void    DT_Port_Init            (struct DATA_TRANSFER *xfer);
void    DT_Port_SendData        (struct DATA_TRANSFER *xfer, uint8_t *data, uint32_t len);
uint8_t DT_Port_IsRecvData      (struct DATA_TRANSFER *xfer);
void    DT_Port_ClearRecvBuff   (struct DATA_TRANSFER *xfer);

#endif

