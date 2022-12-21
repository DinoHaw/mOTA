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
 * 2022-12-07     Dino         �޸� STM32L4 д�� flash ����С��λ����
 * 2022-12-10     Dino         ���Ӷ� SPI flash ��֧��
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

/* �̼������Ĵ������ */
typedef enum 
{
    FM_ERR_OK                           = 0x00,             
    FM_ERR_NULL_POINTER                 = 0x01,             /* ����Ϊ��ָ�� */
    FM_ERR_FLASH_NO_EMPTY               = 0x02,             /* flash �ǿ� */

    FM_ERR_NO_THIS_PART                 = 0x06,             /* �Ҳ����̼���ָ���ķ��� */
    FM_ERR_READ_IS_EMPTY_ERR            = 0x07,             /* �жϷ����Ƿ�Ϊ��ʱ��ȡ���� */
    FM_ERR_FIRMWARE_OVERSIZE            = 0x08,             /* �̼���С������������ */
    FM_ERR_FIRMWARE_HEAD_VERIFY_ERR     = 0x09,             /* �̼���ͷУ����� */
    FM_ERR_VERIFY_READ_ERR              = 0x0A,             /* У��̼�ʱ��ȡ���� */
    FM_ERR_RAW_BODY_VERIFY_ERR          = 0x0B,             /* Դ�̼�����У����� */
    FM_ERR_PKG_BODY_VERIFY_ERR          = 0x0C,             /* �����Ĺ̼�����У����� */
    FM_ERR_ERASE_PART_ERR               = 0x0D,             /* ������������ */
    FM_ERR_WRITE_FIRST_ADDR_ERR         = 0x0E,             /* ����д������׵�ַ���� */
    FM_ERR_JUMP_TO_APP_ERR              = 0x0F,             /* ��ת�� APP ʱ��⵽���� */
    FM_ERR_READ_FIRMWARE_HEAD_ERR       = 0x10,             /* ��ȡ�̼���ͷ���� */
    FM_ERR_UPDATE_READ_ERR              = 0x11,             /* �̼������� APP ����ʱ��ȡ�������� */
    FM_ERR_UPDATE_VER_READ_ERR          = 0x12,             /* ���¹̼��汾��Ϣʱ��ȡ�������� */
    FM_ERR_UPDATE_VER_ERASE_ERR         = 0x13,             /* ���¹̼��汾��Ϣʱ������������ */
    FM_ERR_UPDATE_VER_WRITE_ERR         = 0x14,             /* ���¹̼��汾��Ϣʱд��������� */
    FM_ERR_WRITE_PART_ERR               = 0x15,             /* ����д��������� */
    FM_ERR_FAULT_FIRMWARE               = 0x16,             /* ����Ĺ̼��� */
    FM_ERR_NO_DECRYPT_COMPONENT         = 0x1A,             /* �ӻ�û�н���������޷����� */
    FM_ERR_DECRYPT_ERR                  = 0x1B,             /* �̼�����ʧ�� */
    FM_ERR_READ_VER_ERR                 = 0x1C,             /* ��ȡ�̼��İ汾ʧ�� */
    FM_ERR_WRITE_VER_ERR                = 0x1D,             /* �̼��İ汾д�� APP ����ʧ�� */
    FM_ERR_VER_AREA_NO_ERASE            = 0x1E,             /* APP �����Ĺ̼��汾����û�в��� */
    FM_ERR_READ_FLASH_ERR               = 0x1F,             /* ��ȡ flash ���� */

} FM_ERR_CODE;

/* �̼�д��ķ��� */
typedef enum 
{
    FM_DIR_HOST_TO_APP                  = 0x00,             /* ���������򽫹̼�д�� APP ���� */
    FM_DIR_HOST_TO_DOWNLOAD,                                /* ���������򽫹̼�д�� download/factory ���� */
    FM_DIR_DOWNLOAD_TO_APP,                                 /* �� download/factory �������򽫹̼�д�� APP ���� */

} FM_FIRMWARE_WRITE_DIR;

#pragma pack(1)
/* fpk �̼���ͷ�����������fpk�̼�����ͷ��Ϣ.xlsx�� */
struct FPK_HEAD
{
    char     name[4];                                       /* fpk �ļ���ʶ */ 
    uint8_t  config[4];                                     /* ����ѡ�� */
    char     fw_old_ver[FPK_VERSION_SIZE];                  /* �̼��ɰ汾 */
    char     fw_new_ver[FPK_VERSION_SIZE];                  /* �̼��°汾 */
    char     user_string[FPK_USER_STRING_SIZE];             /* �û��Զ�����ַ�ˮӡ */
    char     part_name[FPK_PART_NAME_SIZE];                 /* �̼�����ŵķ����� */
    uint32_t raw_size;                                      /* Դ�̼��Ĵ�С������������ͷ */
    uint32_t pkg_size;                                      /* �����̼��Ĵ�С������������ͷ */
    uint32_t timestamp;                                     /* ���ʱ�� unix ʱ�������ת��Ϊ������ʱ������Ϣ */
    uint32_t raw_crc;                                       /* Դ�̼��� CRC32 ֵ */
    uint32_t pkg_crc;                                       /* �����̼��� CRC32 ֵ */
    uint32_t head_crc;                                      /* ����ͷ�� CRC32 ֵ */
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


