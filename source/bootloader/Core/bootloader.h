/**
 * \file            bootloader.h
 * \brief           bootloader core
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
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2022-11-23     Dino         the first version
 * v1.1     2022-12-04     Dino         1. 增加一个记录版本的机制，可选写在 APP 分区
 *                                      2. 修复 AC6 -O0 优化下，无法正常运行 APP 的问题
 *                                      3. 增加长按按键恢复出厂固件的选项
 *                                      4. 将 flash 的擦除粒度配置移至 user_config.h 
 *                                      5. 增加 Main_Satrt() 函数
 *                                      6. 增加是否判断固件包超过分区大小的选项
 * v1.2     2022-12-07     Dino         1. 增加对 STM32L4 的支持
 * v1.3     2022-12-08     Dino         1. 增加固件包可放置在 SPI flash 的功能
 * v1.4     2022-12-21     Dino         1. 修复一些配置选项的编译问题
 *                                      2. 增加更多的 user_config.h 配置参数的合法性判断
 *                                      3. 将 user_config.h 配置参数的合法性判断移动至 app.h
 * v1.5     2023-05-04     Dino         1. 修复在 YModem 协议下，进入需要主机下发固件包的流程时需要主动上发字符 'C' 的问题
 * v1.6     2023-12-10     Dino         1. 改名为 bootloader
 *                                      2. 可移植部分代码分至 bootloader_port.c
 */

#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#include "bootloader_define.h"
#include "firmware_manage.h"
#include "protocol_parser.h"
#include "data_transfer.h"


/* 设备与上位机所处的通讯阶段，如果不满足需求，可修改或增加 */
typedef enum
{
    COMM_STATUS_NONE = 0x00,
    COMM_STATUS_RECV_DATA,                          /* 收到数据 */
    COMM_STATUS_FILE_INFO,                          /* 收到文件的信息 */
    COMM_STATUS_FIRMWARE_HEAD,                      /* 收到固件包头 */
    COMM_STATUS_FIRMWARE_PKG,                       /* 收到固件包体 */
    COMM_STATUS_FILE_DONE,                          /* 文件传输完成 */
    COMM_STATUS_START_UPDATE,                       /* 开始更新固件 */
    COMM_STATUS_CANCEL,                             /* 取消传输 */
    COMM_STATUS_RECV_TIMEOUT,                       /* 接收数据超时 */
    COMM_STATUS_UNKNOWN,                            /* 未知 */

} COMM_STATUS;

/* 应用执行流程 */
typedef enum 
{
    EXE_FLOW_NOTHING = 0x00,
    EXE_FLOW_ACCIDENT_UPDATE,                       /* 意外更新固件流程 */
    EXE_FLOW_NEED_HOST_SEND_FIRMWARE,               /* 需要主机下发固件包 */
    
    EXE_FLOW_FIND_RUNNING_FIRMWARE,                 /* (0) 在各分区查找可运行的固件 */
    EXE_FLOW_WAIT_FIRMWARE,                         /* 等待接收固件包 */
    EXE_FLOW_VERIFY_FIRMWARE_HEAD,                  /* (1) 校验收到的固件包头 */
    EXE_FLOW_ERASE_OLD_FIRMWARE,                    /* 擦除旧固件 */
    EXE_FLOW_ERASE_OLD_FIRMWARE_DONE,               /* 完成擦除旧固件 */
    EXE_FLOW_WRITE_FIRMWARE_HEAD,                   /* 将固件包头写入 flash */
    EXE_FLOW_WRITE_FIRMWARE_HEAD_DONE,              /* 完成固件包头的写入 */
    EXE_FLOW_VERIFY_FIRMWARE_PKG,                   /* (2) 校验固件分包数据的正确性 */
    EXE_FLOW_WRITE_NEW_FIRMWARE,                    /* 写入新的固件分包到分区 */
    EXE_FLOW_WRITE_NEW_FIRMWARE_DONE,               /* 写入新的固件分包完成 */
    EXE_FLOW_UPDATE_FIRMWARE,                       /* (3) 开始更新固件 */
    EXE_FLOW_VERIFY_FIRMWARE,                       /* 校验整个固件包体的数据正确性 */
    EXE_FLOW_VERIFY_FIRMWARE_DONE,                  /* 完成校验整个固件包体的数据正确性 */
    EXE_FLOW_ERASE_APP,                             /* 擦除 APP 分区的固件 */
    EXE_FLOW_UPDATE_TO_APP,                         /* 将其它分区的固件更新到 APP 分区 */
    EXE_FLOW_VERIFY_APP,                            /* 校验 APP 分区固件的数据正确性 */
    EXE_FLOW_UPDATE_TO_APP_DONE,                    /* 校验 APP 分区固件通过后，将剩余数据写入 */
    EXE_FLOW_ERASE_DOWNLOAD,                        /* 擦除 download 分区 */
    EXE_FLOW_ERASE_DOWNLOAD_DONE,                   /* 完成擦除 download 分区 */
    EXE_FLOW_JUMP_TO_APP,                           /* 跳转至 APP 运行 */
    EXE_FLOW_RECOVERY,                              /* 恢复出厂固件 */
    EXE_FLOW_FAILED,                                /* 失败流程 */
    
} BOOT_EXE_FLOW;

/* 应用状态 */
typedef enum 
{
    BOOT_STATUS_NONE = 0x00,
    BOOT_STATUS_NO_UPDATE,                          /* 无须更新固件的标志 */
    BOOT_STATUS_ENTER_UPDATE_MODE,                  /* 进入固件更新模式的标志 */
    BOOT_STATUS_ACCIDENT_UPDATE,                    /* 意外更新固件中 */
    BOOT_STATUS_UPDATE_SUCCESS,                     /* 更新固件成功 */
    BOOT_STATUS_UPDATE_FAILED,                      /* 更新固件失败 */
    BOOT_STATUS_NO_FIRMWARE,                        /* 无任何可用固件 */
    BOOT_STATUS_NO_APP,                             /* 无 APP 固件 */
    BOOT_STATUS_READ_PART_ERR,                      /* 读取分区失败 */
    BOOT_STATUS_APP_VERIFY_ERR,                     /* APP 固件校验错误 */
    BOOT_STATUS_APP_CAN_NOT_VERIFY,                 /* APP 固件无法校验 */
    BOOT_STATUS_AUTO_UPDATE_FAILED,                 /* 自动更新失败 */
    
} BOOT_STATUS;

/* 固件更新的缩略步骤，用于计算固件更新的进度 */
typedef enum 
{
    STEP_VERIFY_FIRMWARE = 0x00,                    /* 校验接收到的固件包 */
    STEP_ERASE_APP,                                 /* 擦除 APP 分区 */
    STEP_UPDATE_TO_APP,                             /* 更新固件包到 APP 分区 */
    STEP_VERIFY_APP,                                /* 校验 APP 固件 */
    STEP_ERASE_DOWNLOAD,                            /* 擦除 download 分区 */
    
} BOOY_UPDATE_STEP;

/* 固件更新的所有信息 */
struct FIRMWARE_UPDATE_INFO
{
    BOOT_EXE_FLOW       exe_flow;                   /* 应用执行的流程 */
    BOOY_UPDATE_STEP    step;                       /* 固件的更新阶段，便于程序判断 total_progress 的增加系数 */

    PP_CMD_EXE_RESULT   cmd_exe_result;             /* 指令执行结果 */
    PP_CMD_ERR_CODE     cmd_exe_err_code;           /* 指令执行失败时的故障码 */

    bool                is_recovery;                /* 是否正在进行恢复出厂固件的标志位 */
    uint8_t             total_progress;             /* 固件更新的总进度， 0-100 */
};


void Bootloader_Init(void);
void Bootloader_Loop(void);


/* 一些配置参数的错误检查 */
#if (ONCHIP_FLASH_SIZE == 0)
#error "The ONCHIP_FLASH_SIZE cannot be 0."
#endif

#if (APP_PART_SIZE == 0)
#error "The APP_PART_SIZE cannot be 0."
#endif

#if (USING_PART_PROJECT > ONE_PART_PROJECT)
    #if (DOWNLOAD_PART_SIZE == 0)
    #error "The DOWNLOAD_PART_SIZE cannot be 0."
    #endif
#endif

#if (USING_PART_PROJECT > DOUBLE_PART_PROJECT)
    #if (FACTORY_PART_SIZE == 0)
    #error "The FACTORY_PART_SIZE cannot be 0."
    #endif
#endif

#if (USING_PART_PROJECT < ONE_PART_PROJECT || USING_PART_PROJECT > TRIPLE_PART_PROJECT)
#error "The USING_PART_PROJECT option is out of range."
#endif

#if (USING_IS_NEED_UPDATE_PROJECT < USING_HOST_CMD_UPDATE ||    \
     USING_IS_NEED_UPDATE_PROJECT > USING_APP_SET_FLAG_UPDATE)
#error "The USING_IS_NEED_UPDATE_PROJECT option is out of range."
#endif

#if (USING_AUTO_UPDATE_PROJECT < DO_NOT_AUTO_UPDATE || USING_AUTO_UPDATE_PROJECT > VERSION_WRITE_TO_APP)
#error "The USING_AUTO_UPDATE_PROJECT option is out of range."
#endif

#if (USING_APP_SAFETY_CHECK_PROJECT < DO_NOT_CHECK || USING_APP_SAFETY_CHECK_PROJECT > DO_NOT_DO_ANYTHING)
#error "The USING_APP_SAFETY_CHECK_PROJECT option is out of range."
#endif

#if (FACTORY_NO_FIRMWARE_SOLUTION < JUMP_TO_APP || FACTORY_NO_FIRMWARE_SOLUTION > WAIT_FOR_NEW_FIRMWARE)
#error "The FACTORY_NO_FIRMWARE_SOLUTION option is out of range."
#endif

#ifndef WAIT_HOST_DATA_MAX_TIME
#error "The WAIT_HOST_DATA_MAX_TIME undefined."
#endif

#if (WAIT_HOST_DATA_MAX_TIME == 0)
#error "The WAIT_HOST_DATA_MAX_TIME cannot be 0."
#endif

#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE)
    #if (FIRMWARE_UPDATE_MAGIC_WORD == 0)
    #error "The FIRMWARE_UPDATE_MAGIC_WORD cannot be 0."
    #endif
    #if (FIRMWARE_RECOVERY_MAGIC_WORD == 0)
    #error "The FIRMWARE_RECOVERY_MAGIC_WORD cannot be 0."
    #endif
    #if (BOOTLOADER_RESET_MAGIC_WORD == 0)
    #error "The BOOTLOADER_RESET_MAGIC_WORD cannot be 0."
    #endif
#endif

#if (ONCHIP_FLASH_ONCE_WRITE_BYTE == 0)
#error "The ONCHIP_FLASH_ONCE_WRITE_BYTE cannot be 0."
#endif

#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
    #if (FACTORY_FIRMWARE_BUTTON_TIME == 0)
    #error "The FACTORY_FIRMWARE_BUTTON_TIME cannot be 0."
    #endif
#endif

#if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT)
    #if (ONCHIP_FLASH_ERASE_GRANULARITY == 0)
    #error "The ONCHIP_FLASH_ERASE_GRANULARITY cannot be 0."
    #endif
#endif

#if (IS_ENABLE_SPI_FLASH)
    #if (SPI_FLASH_ERASE_GRANULARITY == 0)
    #error "The SPI_FLASH_ERASE_GRANULARITY cannot be 0."
    #endif
#endif

#if (USING_PART_PROJECT == ONE_PART_PROJECT)
    #if (BOOTLOADER_SIZE + APP_PART_SIZE) > ONCHIP_FLASH_SIZE
    #error "BOOTLOADER_SIZE + APP_PART_SIZE size over than ONCHIP_FLASH_SIZE."
    #endif

#elif (USING_PART_PROJECT == DOUBLE_PART_PROJECT)
    #if (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH)
        #if (BOOTLOADER_SIZE + APP_PART_SIZE) > ONCHIP_FLASH_SIZE
        #error "BOOTLOADER_SIZE + APP_PART_SIZE size over than ONCHIP_FLASH_SIZE."
        #endif
        #if (DOWNLOAD_PART_SIZE) > SPI_FLASH_SIZE
        #error "DOWNLOAD_PART_SIZE size over than SPI_FLASH_SIZE."
        #endif
    #else
        #if (BOOTLOADER_SIZE + APP_PART_SIZE + DOWNLOAD_PART_SIZE) > ONCHIP_FLASH_SIZE
        #error "BOOTLOADER_SIZE + APP_PART_SIZE + DOWNLOAD_PART_SIZE size over than ONCHIP_FLASH_SIZE."
        #endif
    #endif

#elif (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
    #if (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH) &&       \
        (FACTORY_PART_LOCATION  == STORE_IN_SPI_FLASH)
        #if (BOOTLOADER_SIZE + APP_PART_SIZE) > ONCHIP_FLASH_SIZE
        #error "BOOTLOADER_SIZE + APP_PART_SIZE size over than ONCHIP_FLASH_SIZE."
        #endif
        #if (DOWNLOAD_PART_SIZE + FACTORY_PART_SIZE) > SPI_FLASH_SIZE
        #error "DOWNLOAD_PART_SIZE + FACTORY_PART_SIZE size over than SPI_FLASH_SIZE."
        #endif

    #elif (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH) &&     \
          (FACTORY_PART_LOCATION  == STORE_IN_ONCHIP_FLASH)
        #if (BOOTLOADER_SIZE + APP_PART_SIZE + FACTORY_PART_SIZE) > ONCHIP_FLASH_SIZE
        #error "BOOTLOADER_SIZE + APP_PART_SIZE + FACTORY_PART_SIZE size over than ONCHIP_FLASH_SIZE."
        #endif
        #if (DOWNLOAD_PART_SIZE) > SPI_FLASH_SIZE
        #error "DOWNLOAD_PART_SIZE size over than SPI_FLASH_SIZE."
        #endif

    #elif (DOWNLOAD_PART_LOCATION == STORE_IN_ONCHIP_FLASH) &&  \
          (FACTORY_PART_LOCATION  == STORE_IN_SPI_FLASH)
        #if (BOOTLOADER_SIZE + APP_PART_SIZE + DOWNLOAD_PART_SIZE) > ONCHIP_FLASH_SIZE
        #error "BOOTLOADER_SIZE + APP_PART_SIZE + DOWNLOAD_PART_SIZE size over than ONCHIP_FLASH_SIZE."
        #endif
        #if (FACTORY_PART_SIZE) > SPI_FLASH_SIZE
        #error "FACTORY_PART_SIZE size over than SPI_FLASH_SIZE."
        #endif
    #else
        #if (BOOTLOADER_SIZE + APP_PART_SIZE + DOWNLOAD_PART_SIZE + FACTORY_PART_SIZE) > ONCHIP_FLASH_SIZE
        #error "BOOTLOADER_SIZE + APP_PART_SIZE + DOWNLOAD_PART_SIZE + FACTORY_PART_SIZE size over than ONCHIP_FLASH_SIZE."
        #endif
    #endif
#endif  /* #if (USING_PART_PROJECT == ONE_PART_PROJECT) */

#endif  /* __BOOTLOADER_H__ */


