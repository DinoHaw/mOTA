/**
 * \file            ymodem.c
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

#include "ymodem.h"
#include "crc.h"
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QIODevice>
#include <QDebug>


/**
 * @brief 构造函数
 */
YModem::YModem()
{
    sendStop();

    connect(&_send_timer, &QTimer::timeout, this, &YModem::sendFileHandler);
    connect(&_recv_timer, &QTimer::timeout, this, &YModem::receiveDataTimeout);
}


/**
 * @brief 发送文件
 * @param file_path 文件所在的路径
 * @return 执行结果
 */
YMODEM_ERR YModem::sendFile(QString file_path)
{
    assert(file_path != "");

    /* 一些错误检查 */
//    if (YModemSend == nullptr || YModemReceive == nullptr)
//        return YMODEM_ERR_NO_INIT;

    QFile file(file_path);
    QFileInfo file_info(file_path);

    if (file.exists() == false)
        return YMODEM_ERR_FILE_NO_EXIST;

    if (file.open(QIODevice::ReadOnly) == false)
        return YMODEM_ERR_OPEN_FILE_ERR;

    /* 暂存文件源数据 */
    QDataStream file_stream(&file);
    int add_byte_num = 0;
    int size = file.size();
    if (size % YMODEM_STX_DATA_MAX_LEN)
    {
        add_byte_num = YMODEM_STX_DATA_MAX_LEN - (size % YMODEM_STX_DATA_MAX_LEN);
        size += add_byte_num;
        qDebug()<<"add byte num: "<<add_byte_num;
        qDebug()<<"need send size: "<<size;
    }

    /* 将文件读入数组 */
    _file_raw_data = new char[file.size() + add_byte_num];
    int read_len = file_stream.readRawData(_file_raw_data, file.size());
    if (read_len == -1)
    {
        delete[] _file_raw_data;
        _file_raw_data = nullptr;
        return YMODEM_ERR_READ_FILE_ERR;
    }

    /* 最后一个未足YMODEM_STX_DATA_MAX_LEN的包的数据全部填充为0x1A */
    if (add_byte_num)
        memset((uint8_t *)&_file_raw_data[file.size()], YMODEM_FILL_BYTE, add_byte_num);

    /* 暂存文件名和文件大小 */
    memset(_file_size_str, 0, sizeof(_file_size_str));
    char *str = new char[sizeof(_file_size_str)];
    memset(str, 0, sizeof(_file_size_str));

    snprintf(str, sizeof(_file_size_str), "%lld", file.size());
    int len = strlen(str);
    strncpy(_file_size_str, str, len);
    qDebug()<<"file size: "<<_file_size_str<<" ("<<len<<")";

    memset(_file_name, 0, sizeof(_file_name));
    strncpy(_file_name, file_info.fileName().toLatin1(), YMODEM_FILE_NAME_MAX_LEN - len - 1);
    qDebug()<<"file name: "<<_file_name;

    /* 开始发送文件 */
    _send_step = YMODEM_STEP_SEND_FILE_INFO;
    _need_send_size = size;
    _send_timer.start(1);
    _recv_timer.start(YMODEM_RECV_DATA_MAX_TIME);

    file.close();
    delete []str;

    return YMODEM_ERR_OK;
}


/**
 * @brief 停止发送
 */
void YModem::sendStop()
{
    _send_timer.stop();
    _recv_timer.stop();

    _pkt_num        = 0;
    _sended_size    = 0;
    _need_send_size = 0;
    _send_step      = YMODEM_STEP_NONE;
    if (_file_raw_data)
    {
        delete []_file_raw_data;
        _file_raw_data = nullptr;
    }
}


/*********************************** private function ***********************************/
/**
 * @brief 发送文件的循环处理函数
 */
void YModem::sendFileHandler()
{
    static uint8_t err_count;

    if (emit receive(&_recv_buff[0], 0, (60 * 1000)) != 0)
    {
        _recv_timer.start(YMODEM_RECV_DATA_MAX_TIME);

        switch ((uint8_t)_send_step)
        {
            /* 发送文件信息 */
            case YMODEM_STEP_SEND_FILE_INFO:
            {
                if (_recv_buff[0] == YMODEM_C)
                {
                    err_count = 0;
                    sendFileInfo();
                    _send_step = YMODEM_STEP_WAIT_FIRST_PKG_ACK;
                    emit progressCallback(YMODEM_STAGE_WAIT_C, 0);
                }
                break;
            }
            /* 发送完第一个SOH包后等待ACK */
            case YMODEM_STEP_WAIT_FIRST_PKG_ACK:
            {
                if (_recv_buff[0] == YMODEM_ACK)
                {
                    err_count  = 0;
                    _send_step = YMODEM_STEP_SEND_FILE_FIRST;
                    goto _YMODEM_STEP_SEND_FILE_FIRST;
                }
                else if (_recv_buff[0] == YMODEM_NAK)
                {
                    sendFileInfo();

                    /* 错误计数检查 */
                    if (++err_count == YMODEM_ERR_MAX_TIME)
                    {
                        sendStop();
                        emit resultCallback(YMODEM_ERR_SLAVER_REPLY_ERR);
                    }
                }
                break;
            }
            /* 首次发送文件内容 */
            case YMODEM_STEP_SEND_FILE_FIRST:
                 _YMODEM_STEP_SEND_FILE_FIRST:
            {
                if (_recv_buff[0] == YMODEM_C ||
                    (_recv_buff[0] == YMODEM_ACK && _recv_buff[1] == YMODEM_C))
                {
                    err_count = 0;
                    ++_pkt_num;
                    sendFileData((uint8_t *)&_file_raw_data[_sended_size], YMODEM_STX_SIZE);
                    _send_step = YMODEM_STEP_SEND_FILE;
                }
                break;
            }
            /* 发送文件内容 */
            case YMODEM_STEP_SEND_FILE:
            {
                if (_recv_buff[0] == YMODEM_ACK)
                {
                    err_count = 0;
                    ++_pkt_num;
                    _sended_size += YMODEM_STX_DATA_MAX_LEN;
                    emit progressCallback(YMODEM_STAGE_SEND_FILE, 100 * _sended_size / _need_send_size);
                }
                else
                {
                    /* 错误计数检查 */
                    if (++err_count == YMODEM_ERR_MAX_TIME)
                    {
                        sendStop();
                        emit resultCallback(YMODEM_ERR_SLAVER_REPLY_ERR);
                    }
                }

                /* 分包发送文件 */
                if (_sended_size < _need_send_size)
                    sendFileData((uint8_t *)&_file_raw_data[_sended_size], YMODEM_STX_SIZE);
                else
                {
                    sendPackage(YMODEM_EOT);
                    _send_step = YMODEM_STEP_WAIT_EOT_NAK;
                }
                break;
            }
            /* 等待第一个EOT的NAK回复 */
            case YMODEM_STEP_WAIT_EOT_NAK:
            {
                if (_recv_buff[0] == YMODEM_NAK
                ||  _recv_buff[0] == YMODEM_ACK)
                {
                    err_count  = 0;
                    _pkt_num   = 0;
                    _send_step = YMODEM_STEP_WAIT_EOT_ACK;
                }
                else
                {
                    /* 错误计数检查 */
                    if (++err_count == YMODEM_ERR_MAX_TIME)
                    {
                        sendStop();
                        emit resultCallback(YMODEM_ERR_SLAVER_REPLY_ERR);
                    }
                }

                /* 发送EOT */
                sendPackage(YMODEM_EOT);
                break;
            }
            /* 等待第二个EOT的ACK回复 */
            case YMODEM_STEP_WAIT_EOT_ACK:
            {
                if (_recv_buff[0] == YMODEM_ACK)
                {
                    err_count  = 0;
                    _send_step = YMODEM_STEP_SEND_NULL_PKG;
                    goto _YMODEM_STEP_SEND_NULL_PKG;
                }
                else
                {
                    /* 错误计数检查 */
                    if (++err_count == YMODEM_ERR_MAX_TIME)
                    {
                        sendStop();
                        emit resultCallback(YMODEM_ERR_SLAVER_REPLY_ERR);
                    }
                }

                /* 发送EOT */
                sendPackage(YMODEM_EOT);
                break;
            }
            /* 发送一个空包 */
            case YMODEM_STEP_SEND_NULL_PKG:
                 _YMODEM_STEP_SEND_NULL_PKG:
            {
                if (_recv_buff[0] == YMODEM_C ||
                    (_recv_buff[0] == YMODEM_ACK && _recv_buff[1] == YMODEM_C))
                {
                    /* 发一个空的SOH包 */
                    err_count = 0;
                    sendFileData(nullptr, YMODEM_SOH_SIZE);
                    _send_step = YMODEM_STEP_WAIT_NULL_PKG_ACK;
                }
                else
                {
                    /* 错误计数检查 */
                    if (++err_count == YMODEM_ERR_MAX_TIME)
                    {
                        sendStop();
                        emit resultCallback(YMODEM_ERR_SLAVER_REPLY_ERR);
                    }
                }
                break;
            }
            /* 等待发送空包的ACK回复 */
            case YMODEM_STEP_WAIT_NULL_PKG_ACK:
            {
                sendStop();
                emit progressCallback(YMODEM_STAGE_SEND_FINISHED, 100);

                if (_recv_buff[0] == YMODEM_ACK)
                {
                    emit resultCallback(YMODEM_ERR_OK);
                }
                else
                {
                    emit resultCallback(YMODEM_ERR_SLAVER_REPLY_ERR);
                }
                break;
            }
        }
        memset(_recv_buff, 0, sizeof(_recv_buff));
    }
}


/**
 * @brief 发送文件信息
 */
void YModem::sendFileInfo()
{
    uint8_t *buff = new uint8_t[YMODEM_STX_DATA_MAX_LEN];

    memset(buff, 0, YMODEM_STX_DATA_MAX_LEN);
    memcpy(&buff[0], _file_name, strlen(_file_name));
    memcpy(&buff[strlen(_file_name) + 1], _file_size_str, strlen(_file_size_str));
    sendPackage(YMODEM_SOH, buff);

    delete[] buff;
}


/**
 * @brief 发送文件的数据
 * @param data 源数据
 * @param size_type 包的大小类型
 */
void YModem::sendFileData(uint8_t *data, YMODEM_SIZE_TYPE size_type)
{
    if (size_type == YMODEM_SOH_SIZE)
        sendPackage(YMODEM_SOH, data);

    else if (size_type == YMODEM_STX_SIZE)
        sendPackage(YMODEM_STX, data);
}


/**
 * @brief 发送一个数据包
 * @param header YModem头
 * @param data 源数据
 */
void YModem::sendPackage(YMODEM_HEADER header, uint8_t *data)
{
    uint16_t pkg_len;

    if (header == YMODEM_SOH || header == YMODEM_STX)
    {
        uint16_t data_len;

        if (header == YMODEM_SOH)
        {
            pkg_len  = YMODEM_SOH_MAX_LEN;
            data_len = YMODEM_SOH_DATA_MAX_LEN;
        }
        else
        {
            pkg_len  = YMODEM_STX_MAX_LEN;
            data_len = YMODEM_STX_DATA_MAX_LEN;
        }
        memset(&_send_buff[0], 0, sizeof(_send_buff));
        _send_buff[0] = header;
        _send_buff[1] = _pkt_num;
        _send_buff[2] = ~_pkt_num;
        if (data)
            memcpy(&_send_buff[3], data, data_len);
        else
            memset(&_send_buff[3], 0, data_len);
        uint16_t crc16 = CRC::crc16_xmodem(&_send_buff[3], data_len);
        qDebug() << "CRC16: " << QString::asprintf("%.4X", crc16);
        _send_buff[pkg_len - 2] = crc16 >> 8;
        _send_buff[pkg_len - 1] = crc16;
    }
    else
    {
        _send_buff[0] = header;
        pkg_len = 1;
    }

    QString str = "";
    qDebug() << "send buff: ";
    for (int i = 0; i < pkg_len; i++)
        str += QString::asprintf("%.2X ", _send_buff[i]);
    qDebug() << str;

    emit send(&_send_buff[0], pkg_len, (60 * 1000));
}


/**
 * @brief 超时未收到数据的处理函数
 */
void YModem::receiveDataTimeout()
{
    sendStop();
    emit resultCallback(YMODEM_ERR_RECV_DATA_TIMEOUT);
}


