/**
 * \file            protocol_parser.h
 * \brief           protocol parser
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
 * Version:         v1.0.0
 */

#ifndef __PROTOCOL_PARSER_H__
#define __PROTOCOL_PARSER_H__

#include "bsp_common.h"
#include "firmware_manage.h"

/* YModem 协议 */
#define YMODEM_SOH                  0x01
#define YMODEM_STX                  0x02
#define YMODEM_EOT                  0x04
#define YMODEM_ACK                  0x06
#define YMODEM_NAK                  0x15
#define YMODEM_CAN                  0x18

#define YMODEM_FRAME_FIXED_LEN      (5)
#define YMODEM_SOH_DATA_LEN         (128)
#define YMODEM_STX_DATA_LEN         (1024)
#define YMODEM_SOH_FRAME_LEN        (YMODEM_FRAME_FIXED_LEN + YMODEM_SOH_DATA_LEN)
#define YMODEM_STX_FRAME_LEN        (YMODEM_FRAME_FIXED_LEN + YMODEM_STX_DATA_LEN)
#define YMODEM_C                    'C'

/* 协议包数据最大长度定义，因以下两个宏定义在 app.c 中也使用，因此不建议修改宏定义名称，更改宏定义内容即可 */
#define PP_FIRMWARE_PKG_SIZE        YMODEM_STX_DATA_LEN
#define PP_MSG_BUFF_SIZE            YMODEM_STX_FRAME_LEN

typedef enum 
{
    YMODEM_FLOW_NONE = 0x00,
    YMODEM_FLOW_START,
    YMODEM_FLOW_FIRST_EOT,
    YMODEM_FLOW_SECOND_EOT,
    YMODEM_FLOW_ASK,
    YMODEM_FLOW_SUCCESS,
    YMODEM_FLOW_FAILED,
    YMODEM_FLOW_CANCEL,

} YMODEM_EXE_FLOW;

#pragma pack(1)
union HOST_MESSAGE
{
    uint8_t raw_data[ PP_MSG_BUFF_SIZE ];      /* 数据缓存池，接收到来自上位机的原始数据 */
    
    struct
    {
        uint8_t header;         /* start of header */
        uint8_t pkt_num;        /* packet number */
        uint8_t not_pkt_num;    /* the invert of packet number's bit */
        uint8_t data[];         /* packet data */
    } pkg;
};
#pragma pack()


/************ 以下 enum struct typedef 和函数不建议删除， enum struct 内部数据需根据协议进行修改 *************/

/* 指令执行结果的故障代码 */
typedef enum 
{
    PP_ERR_OK                   = 0x00,     /* 无错误 */
    PP_ERR_UNKOWN_ERR           = 0x01,     /* 未知错误 */

    /* 协议错误 */
    PP_ERR_DUPLICATE_FRAME      = 0x02,     /* 重复帧 */
    PP_ERR_OMISSION_FRAME       = 0x03,     /* 跳帧或遗漏帧 */
    PP_ERR_PKT_NUM_ERR          = 0x04,     /* 正反序列号错误 */
    PP_ERR_FRAME_LENGTH_ERR     = 0x05,     /* 帧长度错误 */
    PP_ERR_FRAME_VERIFY_ERR     = 0x06,     /* 帧校验错误 */
    PP_ERR_HEADER_ERR           = 0x07,     /* header 错误 */
    PP_ERR_EXE_FLOW_ERR         = 0x08,     /* 执行流程错误，未按协议流程收到数据包 */

    /* 业务错误 */
    PP_ERR_NO_FACTORY_FIRMWARE,             /* 没有可供恢复出厂的固件 */
    PP_ERR_NO_THIS_PART,                    /* 找不到固件包指定的分区 */
    PP_ERR_READ_IS_EMPTY_ERR,               /* 判断分区是否为空时读取错误 */
    PP_ERR_FIRMWARE_OVERSIZE,               /* 固件大小超过分区容量 */
    PP_ERR_FIRMWARE_HEAD_VERIFY_ERR,        /* 固件包头校验错误 */
    PP_ERR_VERIFY_READ_ERR,                 /* 校验固件时读取错误 */
    PP_ERR_RAW_BODY_VERIFY_ERR,             /* 源固件包体校验错误 */
    PP_ERR_PKG_BODY_VERIFY_ERR,             /* 打包后的固件包体校验错误 */
    PP_ERR_ERASE_PART_ERR,                  /* 擦除分区错误 */
    PP_ERR_WRITE_FIRST_ADDR_ERR,            /* 数据写入分区首地址错误 */
    PP_ERR_JUMP_TO_APP_ERR,                 /* 跳转至 APP 时检测到错误 */
    PP_ERR_READ_FIRMWARE_HEAD_ERR,          /* 读取固件包头错误 */
    PP_ERR_UPDATE_READ_ERR,                 /* 固件更新至 APP 分区时读取分区错误 */
    PP_ERR_UPDATE_VER_READ_ERR,             /* 更新固件版本信息时读取分区错误 */
    PP_ERR_UPDATE_VER_ERASE_ERR,            /* 更新固件版本信息时擦除分区错误 */
    PP_ERR_UPDATE_VER_WRITE_ERR,            /* 更新固件版本信息时写入分区错误 */
    PP_ERR_WRITE_PART_ERR,                  /* 数据写入分区错误 */
    PP_ERR_FAULT_FIRMWARE,                  /* 错误的固件包 */
    PP_ERR_CAN_NOT_PLACE_IN_APP,            /* 从机限制了多分区方案时不能指定固件包放置于 APP 分区 */
    PP_ERR_DOES_NOT_EXIST_DOWNLOAD,         /* 不存在 download 分区 */
    PP_ERR_DOES_NOT_EXIST_FACTORY,          /* 不存在 factory 分区 */
    PP_ERR_NO_DECRYPT_COMPONENT,            /* 从机没有解密组件，无法解密 */
    PP_ERR_DECRYPT_ERR,                     /* 固件解密失败 */

} PP_CMD_ERR_CODE;

/* 协议包含的指令 */
typedef enum  
{
    PP_CMD_NONE                 = 0x00,
    PP_CMD_SOH                  = YMODEM_SOH,
    PP_CMD_STX                  = YMODEM_STX,
    PP_CMD_EOT                  = YMODEM_EOT,
    PP_CMD_CAN                  = YMODEM_CAN,

} PP_CMD;

/* 协议指令的执行结果 */
typedef enum 
{
    PP_RESULT_OK                = 0x00,     /* 执行成功 */
    PP_RESULT_PROCESS           = 0x01,     /* 执行中 */
    PP_RESULT_FAILED            = 0x02,     /* 执行失败，重试 */
    PP_RESULT_CANCEL            = 0x03,     /* 执行失败，取消传输 */

} PP_CMD_EXE_RESULT;

/* 对协议析构层的配置选项 */
typedef enum 
{
    PP_CONFIG_NONE              = 0x00,
    PP_CONFIG_RESET             = 0x01,
    PP_CONFIG_ENABLE_RECV_CMD   = 0x02,
    
} PP_CONFIG_PARA;

/* 需要打包发送至上位机的数据 */
struct PP_DEV_TX_PKG
{
    uint8_t response;
};


/* typedef of function */
typedef void (*PP_Send_t)(uint8_t *data, uint16_t len, uint32_t timeout);
typedef void (*PP_HeartbeatCallback_t)(uint8_t *data, uint16_t *data_len);
typedef void (*PP_PrepareCallback_t)(PP_CMD cmd, uint8_t *data, uint16_t data_len);
typedef void (*PP_ReplyCallback_t)(PP_CMD cmd, PP_CMD_EXE_RESULT *cmd_exe_result, uint8_t *data, uint16_t *data_len);


/* 函数定义 */
void PP_Init(PP_Send_t               Send, 
             PP_HeartbeatCallback_t  HeartbeatCallback,
             PP_PrepareCallback_t    PrepareCallback, 
             PP_ReplyCallback_t      Set_ResponseInfo);
PP_CMD_ERR_CODE     PP_Handler  (uint8_t *data, uint16_t len);
void                PP_Config   (PP_CONFIG_PARA  para, void *value);

#endif
