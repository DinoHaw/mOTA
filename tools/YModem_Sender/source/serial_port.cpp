/**
 * \file            serial_port.c
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


#include "serial_port.h"
#include <QDebug>
#include <QRegularExpression>


/**
 * @brief 构造函数
 * @param scan_interval 串口扫描间隔时间，单位：ms
 */
Serial_Port::Serial_Port(qint32 scan_interval)
{
    _scan_timer  = new QTimer(this);
    _serial_port = new QSerialPort(this);

    _serial_port->setTextModeEnabled(true);
    _scan_timer->start(scan_interval);

    connect(_serial_port, &QSerialPort::readyRead, this, &Serial_Port::readyRead);
    connect(_scan_timer, &QTimer::timeout, this, &Serial_Port::scan_timeout_handler);
}


/**
 * @brief 某串口设备是否已经打开
 * @param serial_name 串口名
 * @return true：打开。false：未打开
 */
bool Serial_Port::isOpen(QString serial_name)
{
//    if (_serial_port->portName() == nullptr)
//        return false;

    QString port_str;
    int start_index = serial_name.indexOf("COM");
    int end_index = serial_name.indexOf(" ");
    port_str = serial_name.mid(start_index, end_index - start_index);

    QSerialPort port;
    port.setPortName(port_str);
    qDebug() << "isOpen: " << port_str;

//    if (_serial_port->portName() != port_str)
//        return false;

    return port.isOpen();
}


/**
 * @brief 获取所有串口列表
 * @return 串口列表
 */
QStringList * Serial_Port::portList()
{
    return &_port_list;
}


/**
 * @brief 获取当前打开的串口配置信息
 * @return 串口配置信息
 */
SerialPortConfigInfo * Serial_Port::portConfigInfo()
{
    if (_serial_port->portName() == nullptr)
        return nullptr;

    if (_serial_port->isOpen() == false)
        return nullptr;

    _port_info.baud_rate = _serial_port->baudRate();
    _port_info.data_bits = _serial_port->dataBits();
    _port_info.flow_ctrl = _serial_port->flowControl();
    _port_info.parity    = _serial_port->parity();
    _port_info.stop_bits = _serial_port->stopBits();

    return &_port_info;
}


/**
 * @brief 打开串口
 * @param serial_name 串口名
 * @param config_info 配置信息
 * @return true：成功。false：失败
 */
bool Serial_Port::open(QString serial_name, SerialPortConfigInfo config_info)
{
    if (_port_list.contains(serial_name) == false)
        return false;

    if (isOpen(serial_name) == true)
        return true;

    if (_serial_port->isOpen() == true)
        _serial_port->close();

    QString port_str;
    int start_index = serial_name.indexOf("COM");
    int end_index = serial_name.indexOf(" ");
    port_str = serial_name.mid(start_index, end_index - start_index);
    qDebug() << "port name: " << serial_name;
    qDebug() << "select port: " << port_str;

    _serial_port->setPortName(port_str);
    _serial_port->setBaudRate(config_info.baud_rate);
    _serial_port->setDataBits(config_info.data_bits);
    _serial_port->setFlowControl(config_info.flow_ctrl);
    _serial_port->setParity(config_info.parity);
    _serial_port->setStopBits(config_info.stop_bits);

    return _serial_port->open(QIODevice::ReadWrite);
}


/**
 * @brief 关闭串口
 */
void Serial_Port::close()
{
    if (_serial_port->isOpen() == true)
        _serial_port->close();
}


/**
 * @brief 向已打开的串口写数据
 * @param data 源数据
 * @param len 源数据长度
 * @return 实际已写的数据长度
 */
qint64 Serial_Port::write(const char *data, qint64 len)
{
    if (data == nullptr || len == 0)
        return -1;

    if (_serial_port->isOpen() == true)
        return _serial_port->write(data, len);

    return 0;
}


/**
 * @brief 读取已打开串口设备的全部缓存数据
 * @return 串口数据
 */
QByteArray Serial_Port::readAll()
{
    if (_serial_port->isOpen() == true)
        return _serial_port->readAll();

    return 0;
}


/*********************************** private function ***********************************/
/**
 * @brief 定时扫描串口设备的函数
 */
void Serial_Port::scan_timeout_handler()
{
    /* 串口错误检查 */
    if (_serial_port->isOpen() == true)
    {
        if (_serial_port->error() != QSerialPort::NoError)
            emit portError();
    }
    /* 扫描串口列表 */
    scanPortList();
}


/**
 * @brief 扫描串口设备
 */
void Serial_Port::scanPortList()
{
    /* 扫描串口列表 */
    bool update = false;        /* 是否需要更新串口列表的标志位 */
    QStringList ports_name;     /* 所有扫描到的串口名 */

    foreach (const QSerialPortInfo port_info, QSerialPortInfo::availablePorts())
    {
        QString str = "";
        str = port_info.portName() + " (" + port_info.description() + ")";
        if (_port_list.contains(str) == false)
            update = true;
        ports_name.append(str);
    }

    /* 判断刷到的串口列表与旧列表是否一致 */
    /* 若旧串口列表为空 或 新旧串口列表不一致 */
    if (update || ports_name.count() != _port_list.count())
    {
        /* 将旧串口列表修改为新串口列表 */
        _port_list.clear();
        _port_list.append(ports_name);
        qDebug() << "port list changed";
        qDebug() << _port_list;

        /* 发送portChanged信号 */
        emit portChanged();
    }
}
