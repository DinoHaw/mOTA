/*
 * File      : fal_cfg.h
 * This file is part of FAL (Flash Abstraction Layer) package
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include "user_config.h"

/* Ĭ������ SFUD */
#define FAL_USING_SFUD_PORT

/* ʹ�ö���õı����ó���ȥ flash �Զ����� */
#define FAL_PART_HAS_TABLE_CFG

#define FAL_ONCHIP_FLASH_DEV_NAME       "onchip_flash"
#define FAL_SPI_FLASH_DEV_NAME          "spi_flash"


/* ������ flash �ڴ���� */
/* ===================== Flash device Configuration ========================= */

extern const struct fal_flash_dev stm32_onchip_flash;
extern struct fal_flash_dev spi_flash1;

#if (IS_ENABLE_SPI_FLASH)
#define FAL_FLASH_DEV_TABLE                 \
{                                           \
    &stm32_onchip_flash,                    \
    &spi_flash1,                            \
}
#else
#define FAL_FLASH_DEV_TABLE     {0}
#define FAL_PART_TABLE          {0}
#endif


/* ���µ�����һ�㲻��Ҫ�ٸĶ� */
/* ====================== Partition Configuration ========================== */

#if (IS_ENABLE_SPI_FLASH && defined(FAL_PART_HAS_TABLE_CFG))
#if (USING_PART_PROJECT == DOUBLE_PART_PROJECT)
/* ˫�������� */
#if (DOWNLOAD_PART_LOCATION == STORE_IN_ONCHIP_FLASH)
/* ����һ�� download ������Ƭ�� */
#define FAL_PART_TABLE                                                      \
{                                                                           \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = "bootload",                                           \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = 0,                                                    \
        .len        = BOOTLOADER_SIZE,                                      \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = APP_PART_NAME,                                        \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = BOOTLOADER_SIZE,                                      \
        .len        = APP_PART_SIZE,                                        \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = DOWNLOAD_PART_NAME,                                   \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = BOOTLOADER_SIZE + APP_PART_SIZE,                      \
        .len        = DOWNLOAD_PART_SIZE,                                   \
        .reserved   = 0,                                                    \
    },                                                                      \
}
#else
/* ���ζ��� download ������ SPI flash */
#define FAL_PART_TABLE                                                      \
{                                                                           \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = "bootload",                                           \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = 0,                                                    \
        .len        = BOOTLOADER_SIZE,                                      \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = APP_PART_NAME,                                        \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = BOOTLOADER_SIZE,                                      \
        .len        = APP_PART_SIZE,                                        \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = DOWNLOAD_PART_NAME,                                   \
        .flash_name = FAL_SPI_FLASH_DEV_NAME,                               \
        .offset     = 0,                                                    \
        .len        = DOWNLOAD_PART_SIZE,                                   \
        .reserved   = 0,                                                    \
    },                                                                      \
}
#endif  /* #if (DOWNLOAD_PART_LOCATION == STORE_IN_ONCHIP_FLASH) */
#else
/* ���������� */
#if (DOWNLOAD_PART_LOCATION == STORE_IN_ONCHIP_FLASH && FACTORY_PART_LOCATION == STORE_IN_SPI_FLASH)
/* ����һ�� download ������Ƭ�ڣ� factory ������ SPI flash */
#define FAL_PART_TABLE                                                      \
{                                                                           \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = "bootload",                                           \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = 0,                                                    \
        .len        = BOOTLOADER_SIZE,                                      \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = APP_PART_NAME,                                        \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = BOOTLOADER_SIZE,                                      \
        .len        = APP_PART_SIZE,                                        \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = DOWNLOAD_PART_NAME,                                   \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = BOOTLOADER_SIZE + APP_PART_SIZE,                      \
        .len        = DOWNLOAD_PART_SIZE,                                   \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = FACTORY_PART_NAME,                                    \
        .flash_name = FAL_SPI_FLASH_DEV_NAME,                               \
        .offset     = 0,                                                    \
        .len        = FACTORY_PART_SIZE,                                    \
        .reserved   = 0,                                                    \
    },                                                                      \
}
#elif (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH && FACTORY_PART_LOCATION == STORE_IN_ONCHIP_FLASH)
/* ���ζ��� download ������ SPI flash�� factory ������Ƭ�� */
#define FAL_PART_TABLE                                                      \
{                                                                           \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = "bootload",                                           \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = 0,                                                    \
        .len        = BOOTLOADER_SIZE,                                      \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = APP_PART_NAME,                                        \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = BOOTLOADER_SIZE,                                      \
        .len        = APP_PART_SIZE,                                        \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = FACTORY_PART_NAME,                                    \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = BOOTLOADER_SIZE + APP_PART_SIZE,                      \
        .len        = FACTORY_PART_SIZE,                                    \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = DOWNLOAD_PART_NAME,                                   \
        .flash_name = FAL_SPI_FLASH_DEV_NAME,                               \
        .offset     = 0,                                                    \
        .len        = DOWNLOAD_PART_SIZE,                                   \
        .reserved   = 0,                                                    \
    },                                                                      \
}
#elif (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH && FACTORY_PART_LOCATION == STORE_IN_SPI_FLASH)
/* �������� download ������ factory �������� SPI flash */
#define FAL_PART_TABLE                                                      \
{                                                                           \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = "bootload",                                           \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = 0,                                                    \
        .len        = BOOTLOADER_SIZE,                                      \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = APP_PART_NAME,                                        \
        .flash_name = FAL_ONCHIP_FLASH_DEV_NAME,                            \
        .offset     = BOOTLOADER_SIZE,                                      \
        .len        = APP_PART_SIZE,                                        \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = DOWNLOAD_PART_NAME,                                   \
        .flash_name = FAL_SPI_FLASH_DEV_NAME,                               \
        .offset     = 0,                                                    \
        .len        = DOWNLOAD_PART_SIZE,                                   \
        .reserved   = 0,                                                    \
    },                                                                      \
    {                                                                       \
        .magic_word = FAL_PART_MAGIC_WORD,                                  \
        .name       = FACTORY_PART_NAME,                                    \
        .flash_name = FAL_SPI_FLASH_DEV_NAME,                               \
        .offset     = DOWNLOAD_PART_SIZE,                                   \
        .len        = FACTORY_PART_SIZE,                                    \
        .reserved   = 0,                                                    \
    },                                                                      \
}
#endif
#endif /* #if (USING_PART_PROJECT == DOUBLE_PART_PROJECT) */

#endif /* #if (IS_ENABLE_SPI_FLASH && defined(FAL_PART_HAS_TABLE_CFG)) */

#endif /* _FAL_CFG_H_ */
