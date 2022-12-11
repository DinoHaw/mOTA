/**
 * \file            firmware_manage.h
 * \brief           firmware package manager
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
 * Version:         v1.0.2
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-23     Dino         the first version
 * 2022-12-07     Dino         修复 STM32L4 写入 flash 的最小单位问题
 * 2022-12-10     Dino         增加对 SPI flash 的支持
 */

#ifndef __FIRMWARE_MANAGE_H__
#define __FIRMWARE_MANAGE_H__

#include "bsp_common.h"
#include "utils.h"

/* fpk: Firmware Package */
#define FPK_LEAST_HANDLE_BYTE           4096
#define FPK_VERSION_SIZE                16
#define FPK_USER_STRING_SIZE            16
#define FPK_PART_NAME_SIZE              16
#define FPK_HEAD_SIZE                   sizeof(struct FPK_HEAD)
#define FPK_IDENTIFIER                  0x006B7066

/* 固件操作的错误代码 */
typedef enum 
{
    FM_ERR_OK                           = 0x00,             
    FM_ERR_NULL_POINTER                 = 0x01,             /* 参数为空指针 */
    FM_ERR_FLASH_NO_EMPTY               = 0x02,             /* flash 非空 */

    FM_ERR_NO_THIS_PART                 = 0x06,             /* 找不到固件包指定的分区 */
    FM_ERR_READ_IS_EMPTY_ERR            = 0x07,             /* 判断分区是否为空时读取错误 */
    FM_ERR_FIRMWARE_OVERSIZE            = 0x08,             /* 固件大小超过分区容量 */
    FM_ERR_FIRMWARE_HEAD_VERIFY_ERR     = 0x09,             /* 固件包头校验错误 */
    FM_ERR_VERIFY_READ_ERR              = 0x0A,             /* 校验固件时读取错误 */
    FM_ERR_RAW_BODY_VERIFY_ERR          = 0x0B,             /* 源固件包体校验错误 */
    FM_ERR_PKG_BODY_VERIFY_ERR          = 0x0C,             /* 打包后的固件包体校验错误 */
    FM_ERR_ERASE_PART_ERR               = 0x0D,             /* 擦除分区错误 */
    FM_ERR_WRITE_FIRST_ADDR_ERR         = 0x0E,             /* 数据写入分区首地址错误 */
    FM_ERR_JUMP_TO_APP_ERR              = 0x0F,             /* 跳转至 APP 时检测到错误 */
    FM_ERR_READ_FIRMWARE_HEAD_ERR       = 0x10,             /* 读取固件包头错误 */
    FM_ERR_UPDATE_READ_ERR              = 0x11,             /* 固件更新至 APP 分区时读取分区错误 */
    FM_ERR_UPDATE_VER_READ_ERR          = 0x12,             /* 更新固件版本信息时读取分区错误 */
    FM_ERR_UPDATE_VER_ERASE_ERR         = 0x13,             /* 更新固件版本信息时擦除分区错误 */
    FM_ERR_UPDATE_VER_WRITE_ERR         = 0x14,             /* 更新固件版本信息时写入分区错误 */
    FM_ERR_WRITE_PART_ERR               = 0x15,             /* 数据写入分区错误 */
    FM_ERR_FAULT_FIRMWARE               = 0x16,             /* 错误的固件包 */
    FM_ERR_NO_DECRYPT_COMPONENT         = 0x1A,             /* 从机没有解密组件，无法解密 */
    FM_ERR_DECRYPT_ERR                  = 0x1B,             /* 固件解密失败 */
    FM_ERR_READ_VER_ERR                 = 0x1C,             /* 读取固件的版本失败 */
    FM_ERR_WRITE_VER_ERR                = 0x1D,             /* 固件的版本写入 APP 分区失败 */
    FM_ERR_VER_AREA_NO_ERASE            = 0x1E,             /* APP 分区的固件版本区域没有擦除 */
    FM_ERR_READ_FLASH_ERR               = 0x1F,             /* 读取 flash 错误 */

} FM_ERR_CODE;

/* 固件写入的方向 */
typedef enum 
{
    FM_DIR_HOST_TO_APP                  = 0x00,             /* 从主机方向将固件写入 APP 分区 */
    FM_DIR_HOST_TO_DOWNLOAD,                                /* 从主机方向将固件写入 download/factory 分区 */
    FM_DIR_DOWNLOAD_TO_APP,                                 /* 从 download/factory 分区方向将固件写入 APP 分区 */

} FM_FIRMWARE_WRITE_DIR;

#pragma pack(1)
/* fpk 固件表头的内容详见《fpk固件包表头信息.xlsx》 */
struct FPK_HEAD
{
    char     name[4];                                       /* fpk 文件标识 */ 
    uint8_t  config[4];                                     /* 配置选项 */
    char     fw_old_ver[FPK_VERSION_SIZE];                  /* 固件旧版本 */
    char     fw_new_ver[FPK_VERSION_SIZE];                  /* 固件新版本 */
    char     user_string[FPK_USER_STRING_SIZE];             /* 用户自定义的字符水印 */
    char     part_name[FPK_PART_NAME_SIZE];                 /* 固件包存放的分区名 */
    uint32_t raw_size;                                      /* 源固件的大小，不包含本表头 */
    uint32_t pkg_size;                                      /* 打包后固件的大小，不包含本表头 */
    uint32_t timestamp;                                     /* 打包时的 unix 时间戳，可转换为年月日时分秒信息 */
    uint32_t raw_crc;                                       /* 源固件的 CRC32 值 */
    uint32_t pkg_crc;                                       /* 打包后固件的 CRC32 值 */
    uint32_t head_crc;                                      /* 本表头的 CRC32 值 */
};
#pragma pack()


void            FM_Init                     (void);
uint8_t         FM_IsEncrypt                (void);
FM_ERR_CODE     FM_IsEmpty                  (const char *part_name);
char *          FM_GetNewFirmwareVersion    (void);
uint32_t        FM_GetRawCRC32              (void);
FM_ERR_CODE     FM_StorageFirmwareHead      (const char *part_name, uint8_t *data);
FM_ERR_CODE     FM_VerifyFirmware           (const char *part_name, uint32_t crc32, uint8_t is_auto_fill);
FM_ERR_CODE     FM_EraseFirmware            (const char *part_name);
FM_ERR_CODE     FM_WriteFirmwareDone        (const char *part_name);
FM_ERR_CODE     FM_WriteFirmwareSubPackage  (const char *part_name, uint8_t *data, uint16_t pkg_size);
FM_ERR_CODE     FM_CheckFirmwareIntegrity   (uint32_t addr);

#if (USING_PART_PROJECT > ONE_PART_PROJECT)
uint8_t         FM_IsNeedAutoUpdate         (void);
char *          FM_GetPartName              (void);
char *          FM_GetOldFirmwareVersion    (void);
uint32_t        FM_GetPackageCRC32          (void);
FM_ERR_CODE     FM_ReadFirmwareHead         (const char *part_name);
FM_ERR_CODE     FM_UpdateToAPP              (const char *from_part_name);
#if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT ||   \
     USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
FM_ERR_CODE     FM_UpdateFirmwareVersion    (const char *part_name);
#endif
#endif

#endif


