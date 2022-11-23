/**
 * \file            dialog.c
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

#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QSettings>

#include "aes.h"
#include "crc.h"


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->labPkgResult->clear();
    setFixedSize(600, 700);
    setWindowTitle("fpk固件打包工具");
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);

    ui->twFirmwareInfo->setColumnWidth(0, 180);
    ui->twFirmwareInfo->setColumnWidth(1, 350);
    ui->twFirmwareInfo->setAlternatingRowColors(true);

    /* 设置密钥、IV和字符水印的输入规则 */
    QRegularExpression key_reg_exp("[0-9A-Fa-f]{32}");
    QRegularExpression iv_reg_exp("[0-9A-Fa-f]{16}");
    QRegularExpression user_str_reg_exp("[0-9A-Za-z ]{28}");
    ui->leKey->setValidator(new QRegularExpressionValidator(key_reg_exp, this));
    ui->leIV->setValidator(new QRegularExpressionValidator(iv_reg_exp, this));
    ui->leUserString->setValidator(new QRegularExpressionValidator(user_str_reg_exp, this));

    readConfigInfo();

    fwPackageJudge();

    qDebug()<<"size of struct FPK_HEAD_INFO: "<<sizeof(struct FPK_HEAD_INFO);
}


Dialog::~Dialog()
{
    writeConfigInfo();
    delete ui;
}


/**
 * @brief 判断“开始打包”按钮是否可启用
 */
void Dialog::fwPackageJudge()
{
    if (!ui->leOpenFwPath->text().isEmpty()
    &&  !ui->leSaveFwPath->text().isEmpty())
    {
        ui->pbStartPkg->setEnabled(true);
    }
    else
    {
        ui->pbStartPkg->setEnabled(false);
    }
}


/**
 * @brief 自动填充保存的固件路径和文件名
 */
void Dialog::savePathAutoFill()
{
    static QRegularExpression reg;

    /* 选中的固件路径非空，且固件的保存路径为空，自动填充 */
    if (!ui->leOpenFwPath->text().isEmpty()
    &&  ui->leSaveFwPath->text().isEmpty())
    {
        QString save_path = ui->leOpenFwPath->text();

        reg = QRegularExpression(".bin");

        save_path.replace(reg, ".fpk");

        ui->leSaveFwPath->setText(save_path);
        QFileInfo file_info(save_path);
        ui->leSaveFwName->setText(file_info.fileName());
    }
}


/**
 * @brief 读取配置文件的所有配置信息并填充进各个控件内
 */
void Dialog::readConfigInfo()
{
    QString app_path = QCoreApplication::applicationDirPath();
    QFile file(app_path + "/config.ini");

    qDebug()<<"app_path"<<app_path;

    QSettings *config_file = new QSettings(app_path + "/config.ini", QSettings::IniFormat);
    config_file->beginGroup("FpkConfigInfo");

    /* 打开的固件路径 */
    QString fm_path = config_file->value("FirmwarePath", "C:/Users/Administrator/Desktop").toString();
    ui->leOpenFwPath->setText(fm_path);
    if (fm_path.contains(".bin"))
    {
        QFileInfo fm_file_info(fm_path);
        ui->leOpenFwName->setText(fm_file_info.fileName());
    }

    /* 保存打包固件的路径 */
    QString save_path = config_file->value("SavePath", "C:/Users/Administrator/Desktop").toString();
    ui->leSaveFwPath->setText(save_path);
    if (save_path.contains(".fpk"))
    {
        QFileInfo save_file_info(save_path);
        ui->leSaveFwName->setText(save_file_info.fileName());
    }

    /* 加密选项 */
    unsigned int encrypt_opt = config_file->value("Encrypt", 0).toUInt();
    if (encrypt_opt == 0)
        ui->combEncrypt->setCurrentIndex(0);
    else if (encrypt_opt == 1)
        ui->combEncrypt->setCurrentIndex(1);

    /* 加密密钥 */
    QString key = config_file->value("Key", "0123456789ABCDEF0123456789ABCDEF").toString();
    ui->leKey->setText(key);

    /* 加密IV */
    QString iv = config_file->value("IV", "0123456789ABCDEF").toString();
    ui->leIV->setText(iv);

    /* 字符水印 */
    QString user_str = config_file->value("UserString", "ChinaDino").toString();
    ui->leUserString->setText(user_str);

    /* 固件分区名 */
    unsigned int part_name = config_file->value("PartName", 1).toUInt();
    if (part_name == 0)
        ui->combPartName->setCurrentIndex(0);
    else if (part_name == 1)
        ui->combPartName->setCurrentIndex(1);
    else if (part_name == 2)
        ui->combPartName->setCurrentIndex(2);

    /* 固件版本 */
    unsigned int ver1 = config_file->value("Ver1", 1).toUInt();
    unsigned int ver2 = config_file->value("Ver2", 0).toUInt();
    unsigned int ver3 = config_file->value("Ver3", 0).toUInt();
    unsigned int ver4 = config_file->value("Ver4", 0).toUInt();
    ui->sbVer1->setValue(ver1);
    ui->sbVer2->setValue(ver2);
    ui->sbVer3->setValue(ver3);
    ui->sbVer4->setValue(ver4);

    /* 解析固件的路径 */
    QString pkg_path = config_file->value("PackagePath", "C:/Users/Administrator/Desktop").toString();
    ui->leOpenPkgPath->setText(pkg_path);
    if (pkg_path.contains(".fpk"))
    {
        QFileInfo pkg_file_info(pkg_path);
        ui->leOpenPkgName->setText(pkg_file_info.fileName());
    }

    config_file->endGroup();

    delete config_file;
}


/**
 * @brief 读取配置文件中的指定参数
 * @param para 指定参数名（QString）
 * @return 对应参数的值（QString）
 */
QString Dialog::readConfigInfo(QString para)
{
    QString app_path = QCoreApplication::applicationDirPath();
    QSettings *config_file = new QSettings(app_path + "/config.ini", QSettings::IniFormat);

    config_file->beginGroup("FpkConfigInfo");
    QString value = config_file->value(para, "").toString();
    config_file->endGroup();

    delete config_file;
    return value;
}


/**
 * @brief 所有配置信息写入配置文件
 */
void Dialog::writeConfigInfo()
{
    QString app_path = QCoreApplication::applicationDirPath();
    QSettings *config_file = new QSettings(app_path + "/config.ini", QSettings::IniFormat);

    config_file->beginGroup("FpkConfigInfo");

    if (!ui->leOpenFwPath->text().isEmpty())
        config_file->setValue("FirmwarePath", ui->leOpenFwPath->text());

    if (!ui->leSaveFwPath->text().isEmpty())
        config_file->setValue("SavePath", ui->leSaveFwPath->text());

    config_file->setValue("Encrypt", ui->combEncrypt->currentIndex());

    if (!ui->leKey->text().isEmpty())
        config_file->setValue("Key", ui->leKey->text());

    if (!ui->leIV->text().isEmpty())
        config_file->setValue("IV", ui->leIV->text());

    if (!ui->leUserString->text().isEmpty())
        config_file->setValue("UserString", ui->leUserString->text());

    config_file->setValue("PartName", ui->combPartName->currentIndex());
    config_file->setValue("Ver1", ui->sbVer1->value());
    config_file->setValue("Ver2", ui->sbVer2->value());
    config_file->setValue("Ver3", ui->sbVer3->value());
    config_file->setValue("Ver4", ui->sbVer4->value());

    if (!ui->leOpenPkgPath->text().isEmpty())
        config_file->setValue("PackagePath", ui->leOpenPkgPath->text());

    config_file->endGroup();

    delete config_file;
}


/**
 * @brief 向配置文件写入指定参数和值
 * @param para 参数名
 * @param value 参数的值
 */
void Dialog::writeConfigInfo(QString para, QString value)
{
    QString app_path = QCoreApplication::applicationDirPath();
    QSettings *config_file = new QSettings(app_path + "/config.ini", QSettings::IniFormat);

    config_file->beginGroup("FpkConfigInfo");
    config_file->setValue(para, value);
    config_file->endGroup();

    delete config_file;
}


/**
 * @brief 读取是否需要加密
 * @return true：加密。false：不加密
 */
bool Dialog::isNeedToEncrypt()
{
    if (ui->combEncrypt->currentIndex() == 0)
        return false;
    else
        return true;
}


/**
 * @brief 当加密算法选项改变时
 * @param index 新的索引
 */
void Dialog::on_combEncrypt_currentIndexChanged(int index)
{
    if (index == 1)
    {
        ui->leKey->setEnabled(true);
        ui->leIV->setEnabled(true);
    }
    else
    {
        ui->leKey->setEnabled(false);
        ui->leIV->setEnabled(false);
    }
}


/**
 * @brief 当“选择固件”按钮按下时
 */
void Dialog::on_pbChooseFw_clicked()
{
    /* 选择一个固件 */
    QString open_path;
    QString default_path = "C:/Users/Administrator/Desktop";

    if (!ui->leOpenFwPath->text().isEmpty())
        open_path = ui->leOpenFwPath->text();
    else
    {
        open_path = readConfigInfo("FirmwarePath");

        if (open_path == "")
            open_path = default_path;
    }

    QString fm_path = QFileDialog::getOpenFileName(this, "选择一个bin固件", open_path, "bin文件(*.bin)");
    QFileInfo file_info(fm_path);

    if (fm_path.isEmpty())
        return;

    ui->leOpenFwPath->setText(fm_path);
    ui->leOpenFwName->setText(file_info.fileName());

    savePathAutoFill();
    fwPackageJudge();
}


/**
 * @brief 当“保存路径”按钮按下时
 */
void Dialog::on_pbSaveFw_clicked()
{
    /* 选择保存打包固件的路径 */
    QString open_path;
    QString default_path = "C:/Users/Administrator/Desktop";
    static QRegularExpression reg = QRegularExpression(".bin");

    if (!ui->leSaveFwPath->text().isEmpty())
        open_path = ui->leSaveFwPath->text();
    else
    {
        open_path = readConfigInfo("SavePath");

        if (open_path == "")
            open_path = default_path;
    }

    if (!ui->leOpenFwPath->text().isEmpty())
    {
        QFileInfo file_info(ui->leOpenFwPath->text());
        qDebug()<<"file_info: "<<file_info.exists();

        if (file_info.exists() && ui->leOpenFwPath->text().contains(".bin"))
        {
            if (open_path.at(open_path.length()-1) != '/')
                open_path += "/";
            open_path += file_info.fileName();
            open_path.remove(reg);
        }
    }
    QString fpk_path = QFileDialog::getSaveFileName(this, "保存为", open_path, "fpk文件(*.fpk)");
    QFileInfo file_info(fpk_path);

    if (fpk_path.isEmpty())
        return;

    ui->leSaveFwPath->setText(fpk_path);
    ui->leSaveFwName->setText(file_info.fileName());

    fwPackageJudge();
}


/**
 * @brief 当选择的固件路径文本被改变时
 * @param arg1 新的文本
 */
void Dialog::on_leOpenFwPath_textChanged(const QString &arg1)
{
    /* 编辑了选择固件的路径 */
    Q_UNUSED(arg1);

    fwPackageJudge();
}


/**
 * @brief 当选择的固件路径文本编辑完成时
 */
void Dialog::on_leOpenFwPath_editingFinished()
{
    QString fm_path = ui->leOpenFwPath->text();
    QFileInfo file_info(fm_path);

    if (fm_path.contains(".bin") && file_info.exists())
    {
        QFileInfo file_info(fm_path);
        ui->leOpenFwName->setText(file_info.fileName());
    }
    else
        ui->leOpenFwName->clear();

    savePathAutoFill();
}


/**
 * @brief 当保存的固件路径文本被改变时
 * @param arg1 新的文本
 */
void Dialog::on_leSaveFwPath_textChanged(const QString &arg1)
{
    /* 编辑了打包后固件的保存路径 */
    Q_UNUSED(arg1);

    fwPackageJudge();
}


/**
 * @brief 当保存的固件路径文本编辑完成时
 */
void Dialog::on_leSaveFwPath_editingFinished()
{
    /* 路径可以修改，但打包后的固件必须是fpk后缀 */
    QString save_path = ui->leSaveFwPath->text();

    /* 此处修改为从字符串尾处算起，遇到的第一个‘.’，这段字符替换为.fpk更好 */
    if (save_path.contains(".fpk"))
    {
        QFileInfo file_info(save_path);
        ui->leSaveFwName->setText(file_info.fileName());
    }
    else
        ui->leSaveFwName->clear();
}

/**
 * @brief 当“开始打包”按钮按下时
 */
void Dialog::on_pbStartPkg_clicked()
{
    /*点击开始打包的按钮*/
    QString     fm_path = ui->leOpenFwPath->text();
    QString     fpk_path = ui->leSaveFwPath->text();
    QDir        fpk_dir(fpk_path);
    QFile       fpk_file(fpk_path);
    QFileInfo   fpk_file_info(fpk_path);
    QFile       fm_file(fm_path);

    ui->labPkgResult->clear();

    /* 一些错误提示 */
    if (!fm_file.exists())
    {
        QMessageBox::warning(this, "错误提示", "选择的固件或路径不存在，请检查");
        return;
    }

    if (!fpk_dir.exists(fpk_file_info.path()))
    {
        QMessageBox::warning(this, "错误提示", "选择保存的路径异常，请检查");
        return;
    }

    if (fpk_path.at(fpk_path.length()-1) == '/')
    {
        QMessageBox::warning(this, "错误提示", "请指定打包后保存的固件名");
        return;
    }

    if (ui->leOpenFwPath->text().isEmpty())
    {
        QMessageBox::warning(this, "错误提示", "选择的固件路径不能为空");
        return;
    }
    if (ui->leSaveFwPath->text().isEmpty())
    {
        QMessageBox::warning(this, "错误提示", "保存的固件路径不能为空");
        return;
    }
    if (ui->combEncrypt->currentIndex() == 1)
    {
        if (ui->leKey->text().trimmed().length() < 32)
        {
            QMessageBox::warning(this, "错误提示", "加密密钥的长度少于32位");
            return;
        }

        if (ui->leIV->text().trimmed().length() < 16)
        {
            QMessageBox::warning(this, "错误提示", "加密IV的长度少于16位");
            return;
        }

        if (ui->leKey->text().isEmpty()
        ||  ui->leIV->text().isEmpty())
        {
            QMessageBox::warning(this, "错误提示", "加密密钥和加密IV不能为空");
            return;
        }
    }

    if (ui->sbVer1->value() == 0
    &&  ui->sbVer2->value() == 0
    &&  ui->sbVer3->value() == 0
    &&  ui->sbVer4->value() == 0)
    {
        QMessageBox::warning(this, "错误提示", "固件版本不能全为0");
        return;
    }

    if (fpk_file.exists())
    {
        int button = QMessageBox::warning(this, "文件覆盖提示", "保存的路径已经存在该文件，是否要覆盖？", QMessageBox::Yes, QMessageBox::No);
        if (button == QMessageBox::No)
            return;
    }

    /* 打开固件和需打包的固件 */
    if (!fm_file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "错误提示", "固件打开失败，请检查选中的固件路径");
        return;
    }

    if (!fpk_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::warning(this, "错误提示", "固件打包失败，可能是保存的路径有误");
        return;
    }

    /* 按最小处理单位依次读取固件并进行加密，之后写入暂存区，等待最后打包 */
    int need_read_size = FPK_ENCRYPT_UNIT;
    int read_size = 0;              /* 每次从固件包读出的字节数 */
    int lack_size = 0;              /* 固件包不足32字节的部分 */
    unsigned int read_pos = 0;      /* 记录读取源固件的位置，也可用作最终读取到的固件字节数 */
    char encrypt_data[FPK_ENCRYPT_UNIT] = {0};                      /* 分包加密的暂存区 */
    char *fm_temp = new char[fm_file.size() + FPK_ENCRYPT_UNIT];    /* 暂存源固件，便于计算CRC32值 */
    char *fpk_temp = new char[fm_file.size() + FPK_ENCRYPT_UNIT];   /* 打包后的固件包，打包后的大小可能超过源固件大小 */
    uint8_t *key_str = new uint8_t[ui->leKey->text().length()];
    uint8_t *iv_str = new uint8_t[ui->leIV->text().length()];
    qDebug()<<"int max: "<<INT_MAX;
    qDebug()<<"uint32_t max: "<<__UINT32_MAX__;

    memset(fm_temp, 0, fm_file.size() + FPK_ENCRYPT_UNIT);
    memset(fpk_temp, 0, fm_file.size() + FPK_ENCRYPT_UNIT);
    memcpy(key_str, ui->leKey->text().toLatin1(), AES256_KEY_MAX_LEN);
    memcpy(iv_str, ui->leIV->text().toLatin1(), AES256_IV_MAX_LEN);

    Aes aes(key_str, iv_str);

    QDataStream fm_data_steam(&fm_file);
    QDataStream fpk_data_stream(&fpk_file);

    fm_data_steam.setVersion(QDataStream::Qt_6_2);
    fpk_data_stream.setVersion(QDataStream::Qt_6_2);

    qDebug()<<"fm_file size: "<<fm_file.size();
    for (read_pos = 0; read_pos < fm_file.size(); )
    {
        /* 最后一个分包不足最小单位时，按实际大小处理 */
        if (fm_file.size() - read_pos < FPK_ENCRYPT_UNIT)
            need_read_size = fm_file.size() - read_pos;

        memset(encrypt_data, 0, FPK_ENCRYPT_UNIT);
        read_size = fm_data_steam.readRawData(encrypt_data, need_read_size);
        memcpy(&fm_temp[read_pos], encrypt_data, read_size);

        if (read_size == need_read_size)
        {
            if (isNeedToEncrypt())
            {
                if (read_size % 32 != 0)
                {
                    lack_size = 32 - read_size % 32;
                    memset(&encrypt_data[read_size], lack_size, lack_size);
                    read_size += lack_size;
                }
                aes.CBC_encrypt_buffer(reinterpret_cast<uint8_t *>(&encrypt_data), read_size);
                memcpy(&fpk_temp[read_pos], encrypt_data, read_size);
            }
            qDebug()<<"read_size: "<<read_size;
            read_pos += read_size;
        }
    }
    qDebug()<<"read_pos: "<<read_pos;

    /* 获取源固件的CRC32值和加密后的CRC32值 */
    uint32_t raw_fm_crc = 0;
    uint32_t encrypt_fm_crc = 0;
    raw_fm_crc = CRC::crc32(reinterpret_cast<uint8_t *>(fm_temp), fm_file.size());
    if (isNeedToEncrypt())
        encrypt_fm_crc = CRC::crc32(reinterpret_cast<uint8_t *>(fpk_temp), read_pos);
    qDebug()<<"raw_fm_crc: "<<QString::asprintf("%.8X", raw_fm_crc);
    qDebug()<<"encrypt_fm_crc: "<<QString::asprintf("%.8X", encrypt_fm_crc);

    /* 固件表头信息打包 */
    FPK_HEAD_INFO fpk_head;
    uint8_t *p_fpk_head = reinterpret_cast<uint8_t *>(&fpk_head);
    memset(p_fpk_head, 0, sizeof(FPK_HEAD_INFO));

    memcpy(fpk_head.head_name, FPK_NAME, FPK_HEAD_NAME_MAX_LEN);
    fpk_head.config_info[0] = 0x00;
    fpk_head.fw_new_ver[0] = ui->sbVer1->value();
    fpk_head.fw_new_ver[1] = ui->sbVer2->value();
    fpk_head.fw_new_ver[2] = ui->sbVer3->value();
    fpk_head.fw_new_ver[3] = ui->sbVer4->value();
    char str_temp[FPK_USER_STRING_MAX_LEN] = "";
    memcpy(str_temp, ui->leUserString->text().toLatin1(), ui->leUserString->text().length());
    memcpy(fpk_head.user_string, str_temp, FPK_USER_STRING_MAX_LEN);
    memset(str_temp, 0, sizeof(str_temp));
    memcpy(str_temp, ui->combPartName->currentText().toLatin1(), ui->combPartName->currentText().length());
    memcpy(fpk_head.part_name, str_temp, FPK_PART_NAME_MAX_LEN);
    if (ui->combEncrypt->currentIndex() == 0)
        fpk_head.config_info[1] = 0x00;
    else
        fpk_head.config_info[1] = 0x01;
    fpk_head.raw_size  = fm_file.size();
    fpk_head.pkg_size  = read_pos;
    fpk_head.timestamp = QDateTime::currentSecsSinceEpoch();
    fpk_head.raw_crc32 = raw_fm_crc;
    if (isNeedToEncrypt())
        fpk_head.pkg_crc32 = encrypt_fm_crc;
    else
        fpk_head.pkg_crc32 = raw_fm_crc;
    fpk_head.head_crc32 = CRC::crc32(p_fpk_head, sizeof(FPK_HEAD_INFO) - FPK_HEAD_CRC32_MAX_LEN);

    /* 固件打包，糅合表头和打包后的固件包体 */
    char *fpk_head_data = new char[FPK_HEADER_MAX_LEN];
    /* 此处专为ymodem协议设计，故意将固件包头扩大至1024字节。单分区时，有效的固件包体才需要被写入flash，
     * 而固件包头部分并不需要写入flash，读取固件包头部分，其余填充部分丢弃即可。原因是ymodem协议是按1024字节发包，
     * 为了便于资源有限的MCU处理，降低MCU处理数据包的复杂度，故意将固件包头扩大至1024字节。
     * 若自定义的协议没有发固件包头的指令，则建议将固件包头扩大至协议帧的最小数据单位，如YModem的STX是1024字节，
     * 从而方便设备处理，而不至于给设备增加更多的RAM和计算要求 */
    memset(fpk_head_data, 0, FPK_HEADER_MAX_LEN);
    memcpy(fpk_head_data, p_fpk_head, sizeof(FPK_HEAD_INFO));
    fpk_data_stream.writeRawData(fpk_head_data, FPK_HEADER_MAX_LEN);

    /* 根据加密选项写入不同处理结果的固件 */
    if (isNeedToEncrypt())
        fpk_data_stream.writeRawData(fpk_temp, read_pos);
    else
        fpk_data_stream.writeRawData(fm_temp, fm_file.size());

    fm_file.close();
    fpk_file.close();

    ui->labPkgResult->setText("打包成功");
    QMessageBox::information(this, "打包结果", "打包成功！");

    delete []key_str;
    delete []iv_str;
    delete []fm_temp;
    delete []fpk_temp;
    delete []fpk_head_data;
}



/************************************* 固件包解析的内容 *****************************************/

/* 选中固件后即开始解析 */
/**
 * @brief 当“选择固件包”按钮按下时
 */
void Dialog::on_pbChoosePkg_clicked()
{
    /* 先打开一个fpk固件包 */
    QString open_path;
    QString default_path = "C:/Users/Administrator/Desktop";

    if (!ui->leOpenPkgPath->text().isEmpty())
        open_path = ui->leOpenPkgPath->text();
    else
    {
        open_path = readConfigInfo("PackagePath");

        if (open_path == "")
            open_path = default_path;
    }
    QString pkg_path = QFileDialog::getOpenFileName(this, "打开一个fpk固件包", open_path, "fpk固件包(*.fpk)");

    if (pkg_path.isEmpty())
        return;

    /* 显示路径文本 */
    ui->leOpenPkgPath->setText(pkg_path);
    QFileInfo pkg_file_info(pkg_path);
    ui->leOpenPkgName->setText(pkg_file_info.fileName());

    /* 一些错误判断 */
    if (pkg_file_info.fileName().contains(".fpk", Qt::CaseInsensitive) == false)
    {
        QMessageBox::warning(this, "错误提示", "所选的文件非fpk固件，请重新选择");
        return;
    }

    QFile pkg_file(pkg_path);
    if (!pkg_file.exists())
    {
        QMessageBox::warning(this, "错误提示", "选择的固件包或路径不存在，请检查");
        return;
    }

    if (ui->leOpenPkgPath->text().isEmpty())
    {
        QMessageBox::warning(this, "错误提示", "选择的固件路径不能为空");
        return;
    }

    if (pkg_file.open(QIODevice::ReadOnly) == false)
    {
        QMessageBox::warning(this, "错误提示", "固件包打开失败");
        return;
    }

    /* 开始固件解析 */
    QDataStream pkg_data_stream(&pkg_file);

    char *pkg_data = new char[sizeof (FPK_HEAD_INFO)];
    char *temp_str = new char[sizeof (FPK_HEAD_INFO)];

    memset(pkg_data, 0, sizeof (FPK_HEAD_INFO));
    memset(temp_str, 0, sizeof (FPK_HEAD_INFO));
    pkg_data_stream.readRawData(pkg_data, sizeof(FPK_HEAD_INFO));

    FPK_HEAD_INFO *p_pkg_head = (FPK_HEAD_INFO *)pkg_data;
    QTableWidgetItem *item = nullptr;

    /* 版本类型 */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    if (p_pkg_head->config_info[0] == 0x00)
        item->setText("数值型");
    else if (p_pkg_head->config_info[0] == 0x01)
        item->setText("字符型");
    else
        item->setText("");
    ui->twFirmwareInfo->setItem(ROW_VERSION_TYPE, 1, item);

    /* 版本信息 */
    if (p_pkg_head->config_info[0] == 0x00)
    {
        /* int */
        QString str;
        item = new QTableWidgetItem(COLUMN_TYPE_VALUE);

        /* 旧固件版本 */
        if (p_pkg_head->fw_old_ver[0] == 0
        &&  p_pkg_head->fw_old_ver[1] == 0
        &&  p_pkg_head->fw_old_ver[2] == 0
        &&  p_pkg_head->fw_old_ver[3] == 0)
        {
            item->setText("未知");
            ui->twFirmwareInfo->setItem(ROW_OLD_VERSION, 1, item);
        }
        else
        {
            str.append(QString::asprintf("V%d.%d.%d.%d",
                                         (uint8_t)(p_pkg_head->fw_old_ver[0]),
                                         (uint8_t)(p_pkg_head->fw_old_ver[1]),
                                         (uint8_t)(p_pkg_head->fw_old_ver[2]),
                                         (uint8_t)(p_pkg_head->fw_old_ver[3])));
            item->setText(str);
            ui->twFirmwareInfo->setItem(ROW_OLD_VERSION, 1, item);
        }

        /* 新固件版本 */
        str = "";
        item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
        str.append(QString::asprintf("V%d.%d.%d.%d",
                                     (uint8_t)(p_pkg_head->fw_new_ver[0]),
                                     (uint8_t)(p_pkg_head->fw_new_ver[1]),
                                     (uint8_t)(p_pkg_head->fw_new_ver[2]),
                                     (uint8_t)(p_pkg_head->fw_new_ver[3])));
        item->setText(str);
        ui->twFirmwareInfo->setItem(ROW_NEW_VERSION, 1, item);
    }
    else if (p_pkg_head->config_info[0] == 0x01)
    {
        /* string */
        item = new QTableWidgetItem(COLUMN_TYPE_VALUE);

        /* 旧固件版本 */
        if (p_pkg_head->fw_old_ver[0] == 0
        &&  p_pkg_head->fw_old_ver[1] == 0
        &&  p_pkg_head->fw_old_ver[2] == 0
        &&  p_pkg_head->fw_old_ver[3] == 0)
        {
            item->setText("未知");
            ui->twFirmwareInfo->setItem(ROW_OLD_VERSION, 1, item);
        }
        else
        {
            strncpy(temp_str, p_pkg_head->fw_old_ver, FPK_FIRMWARE_VERSION_MAX_LEN);
            item->setText(temp_str);
            ui->twFirmwareInfo->setItem(ROW_OLD_VERSION, 1, item);
        }

        /* 新固件版本 */
        memset(temp_str, 0, sizeof (FPK_HEAD_INFO));
        item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
        strncpy(temp_str, p_pkg_head->fw_new_ver, FPK_FIRMWARE_VERSION_MAX_LEN);
        item->setText(temp_str);
        ui->twFirmwareInfo->setItem(ROW_NEW_VERSION, 1, item);
    }

    /* 字符水印 */
    memset(temp_str, 0, sizeof (FPK_HEAD_INFO));
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    strncpy(temp_str, p_pkg_head->user_string, FPK_USER_STRING_MAX_LEN);
    item->setText(temp_str);
    ui->twFirmwareInfo->setItem(ROW_USER_STRING, 1, item);

    /* 固件存放的分区名 */
    memset(temp_str, 0, sizeof (FPK_HEAD_INFO));
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    strncpy(temp_str, p_pkg_head->part_name, FPK_PART_NAME_MAX_LEN);
    item->setText(temp_str);
    ui->twFirmwareInfo->setItem(ROW_PART_NAME, 1, item);

    /* 固件是否有加密 */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    if ((p_pkg_head->config_info[1]) == 0x00)
        item->setText("无加密");
    else if ((p_pkg_head->config_info[1]) == 0x01)
        item->setText("AES256加密");
    else
        item->setText("");
    ui->twFirmwareInfo->setItem(ROW_IS_ENCRYPT, 1, item);

    /* 源固件大小 */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    item->setText(QString::asprintf("%d byte", p_pkg_head->raw_size));
    ui->twFirmwareInfo->setItem(ROW_RAW_SIZE, 1, item);

    /* 打包后的固件大小 */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    item->setText(QString::asprintf("%d byte", p_pkg_head->pkg_size));
    ui->twFirmwareInfo->setItem(ROW_PKG_SIZE, 1, item);

    /* 文件大小（含表头） */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    item->setText(QString::asprintf("%lld byte", pkg_file_info.size()));
    ui->twFirmwareInfo->setItem(ROW_FILE_SIZE, 1, item);

    /* 固件打包的时间 */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    QDateTime date_time = QDateTime::fromSecsSinceEpoch(p_pkg_head->timestamp);
    QString date_time_str = date_time.toString("yyyy-MM-dd hh:mm:ss");
    item->setText(date_time_str);
    ui->twFirmwareInfo->setItem(ROW_PKG_TIME, 1, item);

    /* 源固件包的CRC32值 */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    item->setText(QString::asprintf("%.8X", p_pkg_head->raw_crc32));
    ui->twFirmwareInfo->setItem(ROW_RAW_CRC32, 1, item);

    /* 打包后固件包的CRC32值 */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    item->setText(QString::asprintf("%.8X", p_pkg_head->pkg_crc32));
    ui->twFirmwareInfo->setItem(ROW_PKG_CRC32, 1, item);

    /* 固件表头的CRC32值 */
    item = new QTableWidgetItem(COLUMN_TYPE_VALUE);
    item->setText(QString::asprintf("%.8X", p_pkg_head->head_crc32));
    ui->twFirmwareInfo->setItem(ROW_HEAD_CRC32, 1, item);

    delete []pkg_data;
    delete []temp_str;
    pkg_file.close();
}


/**
 * @brief 选择的固件包路径文本编辑完成时
 */
void Dialog::on_leOpenPkgPath_editingFinished()
{
    QString pkg_path = ui->leOpenPkgPath->text();
    if (pkg_path.contains(".fpk"))
    {
        QFileInfo file_info(pkg_path);
        ui->leOpenPkgName->setText(file_info.fileName());
    }
}



