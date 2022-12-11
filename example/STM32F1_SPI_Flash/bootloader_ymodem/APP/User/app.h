/**
 * \file            app.h
 * \brief           application
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
 * Version:         v1.0.3
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-23     Dino         the first version
 * 2022-12-04     Dino         1. 增加一个记录版本的机制，可选写在 APP 分区
 *                             2. 修复 AC6 -O0 优化下，无法正常运行 APP 的问题
 *                             3. 增加长按按键恢复出厂固件的选项
 *                             4. 将 flash 的擦除粒度配置移至 user_config.h 
 *                             5. 增加 Main_Satrt() 函数
 *                             6. 增加是否判断固件包超过分区大小的选项
 * 2022-12-07     Dino         增加对 STM32L4 的支持
 * 2022-12-08     Dino         增加固件包可放置在 SPI flash 的功能
 */

#ifndef __APP_H__
#define __APP_H__

#include "firmware_manage.h"
#include "protocol_parser.h"
#include "data_transfer.h"
#include "utils.h"

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
    BOOT_STATUS            status;                  /* 记录 bootloader 所处的状态 */
    BOOT_EXE_FLOW          exe_flow;                /* 应用执行的流程 */
    BOOY_UPDATE_STEP       step;                    /* 固件的更新阶段，便于程序判断 total_progress 的增加系数 */

    PP_CMD_EXE_RESULT      cmd_exe_result;          /* 指令执行结果 */
    PP_CMD_ERR_CODE        cmd_exe_err_code;        /* 指令执行失败时的故障码 */

    uint8_t start           :1;                     /* 开始固件更新流程的标志 */
    uint8_t is_recovery     :1;                     /* 是否正在进行恢复出厂固件的标志位 */
    uint8_t                 :0;
    
    uint8_t total_progress;                         /* 固件更新的总进度， 0-100 */
};

#endif


