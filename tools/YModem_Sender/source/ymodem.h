/**
 * \file            ymodem.h
 * \brief           YModem_Sender
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
 * This file is part of YModem_Sender.
 *
 * Author:          Dino Haw <347341799@qq.com>
 * Version:         v1.0.0
 */

#ifndef YMODEM_H
#define YMODEM_H

#include <QObject>
#include <QTimer>

#define YMODEM_RECV_DATA_MAX_TIME       (15 * 1000)
#define YMODEM_ERR_MAX_TIME             5

/* 宏定义 */
#define YMODEM_RECV_MAX_LEN             8
#define YMODEM_FIXED_LEN                5
#define YMODEM_SOH_DATA_MAX_LEN         128
#define YMODEM_STX_DATA_MAX_LEN         1024
#define YMODEM_FILE_SIZE_MAX_LEN        YMODEM_SOH_DATA_MAX_LEN
#define YMODEM_FILE_NAME_MAX_LEN        YMODEM_SOH_DATA_MAX_LEN
#define YMODEM_SOH_MAX_LEN              (YMODEM_FIXED_LEN + YMODEM_SOH_DATA_MAX_LEN)
#define YMODEM_STX_MAX_LEN              (YMODEM_FIXED_LEN + YMODEM_STX_DATA_MAX_LEN)
#define YMODEM_FILL_BYTE                0x1A

/* 提供给用户的YModem发送文件的阶段 */
enum YMODEM_SEND_STAGE
{
    YMODEM_STAGE_NONE,
    YMODEM_STAGE_WAIT_C,                /* 等待从机的'C' */
    YMODEM_STAGE_SEND_FILE,             /* 发送文件阶段 */
    YMODEM_STAGE_SEND_FINISHED,         /* 发送完成阶段 */
};

/* 用于程序标志数据包的类型 */
enum YMODEM_SIZE_TYPE
{
    YMODEM_SOH_SIZE = 0x01,             /* SOH包 */
    YMODEM_STX_SIZE = 0x02,             /* STX包 */
};

/* 错误代码 */
enum YMODEM_ERR
{
    YMODEM_ERR_OK,
    YMODEM_ERR_SENDING,                 /*  */
    YMODEM_ERR_NO_INIT,                 /* 未初始化 */
    YMODEM_ERR_FILE_NO_EXIST,           /* 文件不存在 */
    YMODEM_ERR_OPEN_FILE_ERR,           /* 打开文件失败 */
    YMODEM_ERR_READ_FILE_ERR,           /* 读取文件失败 */
    YMODEM_ERR_RECV_DATA_TIMEOUT,       /* 超时未收到数据 */
    YMODEM_ERR_SLAVER_REPLY_ERR,        /* 从机响应错误 */
};

/* 用于程序记录的流程状态机 */
enum YMOEDM_SEND_STEP
{
    YMODEM_STEP_NONE,
    YMODEM_STEP_SEND_FILE_INFO,         /* 发送文件信息 */
    YMODEM_STEP_WAIT_FIRST_PKG_ACK,     /* 发送完第一个SOH包后等待ACK */
    YMODEM_STEP_SEND_FILE_FIRST,        /* 首次发送文件内容 */
    YMODEM_STEP_SEND_FILE,              /* 发送文件内容 */
    YMODEM_STEP_WAIT_EOT_NAK,           /* 等待第一个EOT的NAK回复 */
    YMODEM_STEP_WAIT_EOT_ACK,           /* 等待第二个EOT的ACK回复 */
    YMODEM_STEP_SEND_NULL_PKG,          /* 发送一个空包 */
    YMODEM_STEP_WAIT_NULL_PKG_ACK,      /* 等待发送空包的ACK回复 */
};

/* YModem的标准header */
enum YMODEM_HEADER
{
    YMODEM_SOH = 0x01,
    YMODEM_STX = 0x02,
    YMODEM_EOT = 0x04,
    YMODEM_ACK = 0x06,
    YMODEM_NAK = 0x15,
    YMODEM_CAN = 0x18,
    YMODEM_C   = 'C',
};

/* 类本体 */
class YModem : public QObject
{
    Q_OBJECT

public:
    YModem();

    YMODEM_ERR  sendFile    (QString file_path);
    void        sendStop    ();

signals:
    /* 发送数据 */
    void send(uint8_t *data, size_t size, uint16_t timeout);
    /* 接收数据 */
    size_t receive(uint8_t *data, size_t need_size, uint16_t timeout);
    /* 发送文件进度的通知 */
    void progressCallback(YMODEM_SEND_STAGE stage, uint8_t progress);
    /* 发送文件完成的通知 */
    void resultCallback(YMODEM_ERR err_code);

private:
    char                   *_file_raw_data = nullptr;
    uint8_t                 _pkt_num;
    size_t                  _sended_size;
    size_t                  _need_send_size;
    char                    _file_size_str[YMODEM_FILE_SIZE_MAX_LEN];
    char                    _file_name[YMODEM_FILE_NAME_MAX_LEN];
    uint8_t                 _send_buff[YMODEM_STX_MAX_LEN];
    uint8_t                 _recv_buff[YMODEM_RECV_MAX_LEN];

    YMOEDM_SEND_STEP        _send_step = YMODEM_STEP_NONE;

    QTimer                  _send_timer;
    QTimer                  _recv_timer;

    void sendPackage(YMODEM_HEADER header, uint8_t *data = nullptr);
    void sendFileData(uint8_t *data, YMODEM_SIZE_TYPE size_type = YMODEM_STX_SIZE);
    void sendFileInfo();
    void sendFileHandler();
    void receiveDataTimeout();
};

#endif // YMODEM_H
