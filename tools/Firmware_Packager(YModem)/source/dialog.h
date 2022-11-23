/**
 * \file            dialog.h
 * \brief           Firmware_Packager(YModem)
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
 * This file is part of Firmware_Packager(YModem).
 *
 * Author:          Dino Haw <347341799@qq.com>
 * Version:         v1.0.0
 */


#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

#define FPK_NAME                        "fpk"
#define FPK_HEAD_NAME_MAX_LEN           4
#define FPK_FIRMWARE_VERSION_MAX_LEN    16
#define FPK_USER_STRING_MAX_LEN         16
#define FPK_PART_NAME_MAX_LEN           16
#define FPK_CONFIG_MAX_LEN              4
#define FPK_HEAD_CRC32_MAX_LEN          4

#define FPK_HEADER_MAX_LEN              1024

/* 加密的最小处理单位 */
#define FPK_ENCRYPT_UNIT                4096

#define AES256_KEY_MAX_LEN              32
#define AES256_IV_MAX_LEN               16

enum TABLE_COLUMN_TYPE
{
    COLUMN_TYPE_PARA = 1000,
    COLUMN_TYPE_VALUE,
};

enum TABLE_ROW_ITEM
{
    ROW_VERSION_TYPE = 0,
    ROW_OLD_VERSION,
    ROW_NEW_VERSION,
    ROW_USER_STRING,
    ROW_PART_NAME,
    ROW_IS_ENCRYPT,
    ROW_RAW_SIZE,
    ROW_PKG_SIZE,
    ROW_FILE_SIZE,
    ROW_PKG_TIME,
    ROW_RAW_CRC32,
    ROW_PKG_CRC32,
    ROW_HEAD_CRC32
};


struct FPK_HEAD_INFO
{
    char        head_name   [FPK_HEAD_NAME_MAX_LEN];
    uint8_t     config_info [FPK_CONFIG_MAX_LEN];
    char        fw_old_ver  [FPK_FIRMWARE_VERSION_MAX_LEN];
    char        fw_new_ver  [FPK_FIRMWARE_VERSION_MAX_LEN];
    char        user_string [FPK_USER_STRING_MAX_LEN];
    char        part_name   [FPK_PART_NAME_MAX_LEN];
    uint32_t    raw_size;
    uint32_t    pkg_size;
    uint32_t    timestamp;
    uint32_t    raw_crc32;
    uint32_t    pkg_crc32;
    uint32_t    head_crc32;
};

class Dialog : public QDialog
{
    Q_OBJECT

private:
    void encrypt(void);
    void fwPackageJudge(void);
    void savePathAutoFill(void);

    void readConfigInfo(void);
    QString readConfigInfo(QString para);
    void writeConfigInfo(void);
    void writeConfigInfo(QString para, QString value);

    bool isNeedToEncrypt(void);

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_combEncrypt_currentIndexChanged(int index);

    void on_pbChooseFw_clicked();

    void on_pbSaveFw_clicked();

    void on_pbStartPkg_clicked();

    void on_leSaveFwPath_textChanged(const QString &arg1);

    void on_leOpenFwPath_textChanged(const QString &arg1);

    void on_leSaveFwPath_editingFinished();

    void on_leOpenFwPath_editingFinished();

    void on_pbChoosePkg_clicked();

    void on_leOpenPkgPath_editingFinished();

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
