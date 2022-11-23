/**
 * \file            mainwindow.c
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontMetrics>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(700, 300);
    setWindowTitle("YModem发送器");

    ui->labFileSize->clear();

    _labAuther = new QLabel;
    _labAuther->setText(" Design by Dino 2022 ");
    ui->statusbar->addWidget(_labAuther);

    _labVersion = new QLabel;
    _labVersion->setText(" V1.0.0 ");
    ui->statusbar->addWidget(_labVersion);

    _serial_config.data_bits = QSerialPort::Data8;
    _serial_config.flow_ctrl = QSerialPort::NoFlowControl;
    _serial_config.parity    = QSerialPort::NoParity;
    _serial_config.stop_bits = QSerialPort::OneStop;

    connect(&_serial_port, &Serial_Port::portChanged, this, &MainWindow::serialPortChanged);
    connect(&_ymodem, &YModem::send, this, &MainWindow::YModemSend);
    connect(&_ymodem, &YModem::receive, this, &MainWindow::YModemReceive);
    connect(&_ymodem, &YModem::progressCallback, this, &MainWindow::sendFileProgress);
    connect(&_ymodem, &YModem::resultCallback, this, &MainWindow::sendFileFinished);
}


MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief 打开串口
 * @param serial_name 串口名
 */
void MainWindow::openSerialPort(QString serial_name)
{
    /* 取得波特率 */
    _serial_config.baud_rate = ui->combBaud->currentText().trimmed().toUInt();

    /* 打开串口 */
    if (_serial_port.open(serial_name, _serial_config) == false)
    {
        QMessageBox::warning(this, "错误提示", "打开COM口失败，可能被占用或不存在");
        return;
    }

    /* 成功打开串口，设置标志位和信息 */
    _is_serial_open = true;
    _opened_serial_name = serial_name;

    /* 改变按钮文字和颜色 */
    QPalette palette(ui->pbOpenSerial->palette());
    palette.setColor(QPalette::ButtonText, Qt::darkGreen);
    ui->pbOpenSerial->setPalette(palette);
    ui->pbOpenSerial->setText("关闭串口");
}


/**
 * @brief 关闭串口
 */
void MainWindow::closeSerialPort()
{
    _serial_port.close();

    _is_serial_open = false;
    _opened_serial_name = "";

    QPalette palette(ui->pbOpenSerial->palette());
    palette.setColor(QPalette::ButtonText, Qt::black);
    ui->pbOpenSerial->setPalette(palette);
    ui->pbOpenSerial->setText("打开串口");
}


/**
 * @brief 串口设备发生变动
 * @note 槽函数，连接 serial_port 的 portChanged 信号
 */
void MainWindow::serialPortChanged()
{
    int width = 0;
    int max_width = 0;
    QStringList *serial_list = _serial_port.portList();     /* 获取新的串口设备列表 */
    QFontMetrics font_metrics(ui->combSerialName->fontMetrics());

    /* 逐个检测列表占用最大的宽度 */
    foreach (QString str, *serial_list)
    {
        width = font_metrics.boundingRect(str).width();
        if (width > max_width)
            max_width = width;
    }

    /* 把串口设备信息添加进下拉列表框中 */
    ui->combSerialName->view()->setMinimumWidth(max_width + 10);
    ui->combSerialName->addItems(*serial_list);
}


/**
 * @brief YModem发送数据接口
 * @note 槽函数，连接 ymodem 的 send 信号
 * @param data 源数据
 * @param size 源数据大小
 * @param timeout 发送超时时间
 */
void MainWindow::YModemSend(uint8_t *data, size_t size, uint16_t timeout)
{
    Q_UNUSED(timeout);

    _serial_port.write((const char *)data, size);
}


/**
 * @brief YModem接收数据接口
 * @note 槽函数，连接 ymodem 的 receive 信号
 * @param data 数据接收池
 * @param need_size 需要接收的数据长度
 * @param timeout 接收超时时间
 * @return 实际接收到的数据长度
 */
size_t MainWindow::YModemReceive(uint8_t *data, size_t need_size, uint16_t timeout)
{
    Q_UNUSED(need_size);
    Q_UNUSED(timeout);

    QByteArray byte_arr = _serial_port.readAll();
    memcpy(data, byte_arr.data(), byte_arr.size());

    if (byte_arr.size() > 0)
    {
        QString str = "";
        qDebug() << "YModemReceive: " << byte_arr.size();
        for (int i = 0; i < byte_arr.size(); i++)
            str += QString::asprintf("%.2X ", data[i]);
        qDebug() << str;
    }

    return byte_arr.size();
}


/**
 * @brief 发送文件的过程处理回调函数
 * @note 槽函数，连接 ymodem 的 progressCallback 信号
 * @param stage 发送文件所处的阶段
 * @param progress 发送文件的总进度（0-100）
 */
void MainWindow::sendFileProgress(YMODEM_SEND_STAGE stage, uint8_t progress)
{
    qDebug()<<"stage: "<<stage;
    ui->progressBar->setValue(progress);
}


/**
 * @brief 发送结束回调函数
 * @note 槽函数，连接 ymodem 的 resultCallback 信号
 * @param err_code 发送文件的结果代码
 */
void MainWindow::sendFileFinished(YMODEM_ERR err_code)
{
    qDebug()<<"error: sendFileFinished: "<<err_code;

    ui->combSerialName->setEnabled(true);
    ui->combBaud->setEnabled(true);
    ui->pbOpenSerial->setEnabled(true);
    ui->pbOpenFiles->setEnabled(true);
    ui->pbSendFiles->setEnabled(true);

    if (err_code == YMODEM_ERR_OK)
    {
        QMessageBox::information(this, "提示", "文件发送成功");
    }
    else
    {
        QMessageBox::warning(this, "错误提示", "文件发送错误，错误代码 YMODEM_ERR：0x" + QString::asprintf("%.2X", err_code));
    }
}


/**
 * @brief 串口设备下拉列表框选择的项目改变
 * @param index 对应的项目
 */
void MainWindow::on_combSerialName_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    ui->progressBar->setValue(0);

    if (_is_serial_open == false)
        return;

    /* 已经有打开的COM，则先把已打开的COM口关闭 */
    if (_opened_serial_name != "")
    {
        closeSerialPort();
    }

    /* 再打开新选择的COM口 */
    on_pbOpenSerial_clicked();
}


/**
 * @brief 波特率下拉列表框选择的项目改变
 * @param index 对应的项目
 */
void MainWindow::on_combBaud_currentIndexChanged(int index)
{
    on_combSerialName_currentIndexChanged(index);
}


/**
 * @brief 单击：打开串口按钮
 */
void MainWindow::on_pbOpenSerial_clicked()
{
    ui->progressBar->setValue(0);

    if (ui->combSerialName->currentText().isEmpty())
    {
        QMessageBox::warning(this, "错误提示", "请选择一个COM口");
        return;
    }

    /* 打开COM口 */
    if (_is_serial_open == false)
    {
        openSerialPort(ui->combSerialName->currentText());
    }
    /* 关闭COM口 */
    else
    {
        closeSerialPort();
    }
}


/**
 * @brief 单击：打开文件
 */
void MainWindow::on_pbOpenFiles_clicked()
{
    /* 选择一个文件 */
    QString open_path;
    QString default_path = "C:/Users/Administrator/Desktop";

    ui->progressBar->setValue(0);

    if (ui->leFilePath->text().isEmpty() == false)
        open_path = ui->leFilePath->text();
    else
        open_path = default_path;

    QString file_path = QFileDialog::getOpenFileName(this, "选择一个文件", open_path, "所有文件(*.*)");

    if (file_path.isEmpty())
        return;

    QFileInfo file_info(file_path);

    ui->leFilePath->setText(file_path);
    ui->leFileName->setText(file_info.fileName());
    ui->labFileSize->setText(QString::number((double)file_info.size() / 1024, 'f', 2) + " kB");
}


/**
 * @brief 单击：发送文件
 */
void MainWindow::on_pbSendFiles_clicked()
{
    if (_is_serial_open == false)
    {
        QMessageBox::warning(this, "错误提示", "未打开串口");
        return;
    }

    if (ui->leFilePath->text().isEmpty() == true)
    {
        QMessageBox::warning(this, "错误提示", "文件路径不能为空");
        return;
    }

    QFile file(ui->leFilePath->text());
    if (file.exists() == false)
    {
        QMessageBox::warning(this, "错误提示", "文件路径或文件不存在");
        return;
    }

    /* 清除串口的缓存数据 */
    _serial_port.readAll();

    /* 关闭控件 */
    ui->progressBar->setValue(0);
    ui->combSerialName->setEnabled(false);
    ui->combBaud->setEnabled(false);
    ui->pbOpenSerial->setEnabled(false);
    ui->pbOpenFiles->setEnabled(false);
    ui->pbSendFiles->setEnabled(false);

    /* 按YModem协议发送 */
    YMODEM_ERR err = _ymodem.sendFile(ui->leFilePath->text());
    if (err != YMODEM_ERR_OK)
        qDebug()<<"error: on_pbOpenFiles_clicked: "<<err;
}

