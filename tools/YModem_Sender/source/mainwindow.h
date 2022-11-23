/**
 * \file            mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "serial_port.h"
#include "ymodem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void openSerialPort(QString serial_name);
    void closeSerialPort();
    void YModemSend(uint8_t *data, size_t size, uint16_t timeout);
    size_t YModemReceive(uint8_t *data, size_t need_size, uint16_t timeout);

    QLabel *_labAuther;
    QLabel *_labVersion;

    YModem _ymodem;
    Serial_Port _serial_port;

    bool _is_serial_open = false;
    QString _opened_serial_name = "";

    SerialPortConfigInfo _serial_config;

private slots:
    void serialPortChanged();

    void sendFileProgress(YMODEM_SEND_STAGE stage, uint8_t progress);

    void sendFileFinished(YMODEM_ERR err_code);

    void on_pbOpenSerial_clicked();

    void on_combSerialName_currentIndexChanged(int index);

    void on_combBaud_currentIndexChanged(int index);

    void on_pbOpenFiles_clicked();

    void on_pbSendFiles_clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
