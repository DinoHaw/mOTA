/**
 * \file            serial_port.h
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


#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H


#include <QWidget>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>


struct SerialPortConfigInfo
{
    qint32                      baud_rate;
    QSerialPort::DataBits       data_bits;
    QSerialPort::Parity         parity;
    QSerialPort::StopBits       stop_bits;
    QSerialPort::FlowControl    flow_ctrl;
};


class Serial_Port : public QWidget
{
    Q_OBJECT

public:
    Serial_Port(qint32 scan_interval = 1000);

    bool isOpen(QString serial_name = nullptr);

    QStringList * portList();
    SerialPortConfigInfo * portConfigInfo();

    bool open(QString serial_name = nullptr, SerialPortConfigInfo config_info =
                                             {.baud_rate = 9600,
                                              .data_bits = QSerialPort::Data8,
                                              .parity    = QSerialPort::NoParity,
                                              .stop_bits = QSerialPort::OneStop,
                                              .flow_ctrl = QSerialPort::NoFlowControl});
    void close();
    qint64 write(const char *data = nullptr, qint64 len = 0);
    QByteArray readAll();

signals:
    void readyRead();
    void portError();
    void portChanged();

private slots:
    void scan_timeout_handler();

private:
    void scanPortList();

    QStringList             _port_list;
    SerialPortConfigInfo    _port_info;
    QTimer                 *_scan_timer;
    QSerialPort            *_serial_port;
};


#endif // SERIAL_PORT_H
