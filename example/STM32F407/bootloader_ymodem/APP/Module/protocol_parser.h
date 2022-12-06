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

/* YModem Э�� */
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

/* Э���������󳤶ȶ��壬�����������궨���� app.c ��Ҳʹ�ã���˲������޸ĺ궨�����ƣ����ĺ궨�����ݼ��� */
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
    uint8_t raw_data[ PP_MSG_BUFF_SIZE ];      /* ���ݻ���أ����յ�������λ����ԭʼ���� */
    
    struct
    {
        uint8_t header;         /* start of header */
        uint8_t pkt_num;        /* packet number */
        uint8_t not_pkt_num;    /* the invert of packet number's bit */
        uint8_t data[];         /* packet data */
    } pkg;
};
#pragma pack()


/************ ���� enum struct typedef �ͺ���������ɾ���� enum struct �ڲ����������Э������޸� *************/

/* ָ��ִ�н���Ĺ��ϴ��� */
typedef enum 
{
    PP_ERR_OK                   = 0x00,     /* �޴��� */
    PP_ERR_UNKOWN_ERR           = 0x01,     /* δ֪���� */

    /* Э����� */
    PP_ERR_DUPLICATE_FRAME      = 0x02,     /* �ظ�֡ */
    PP_ERR_OMISSION_FRAME       = 0x03,     /* ��֡����©֡ */
    PP_ERR_PKT_NUM_ERR          = 0x04,     /* �������кŴ��� */
    PP_ERR_FRAME_LENGTH_ERR     = 0x05,     /* ֡���ȴ��� */
    PP_ERR_FRAME_VERIFY_ERR     = 0x06,     /* ֡У����� */
    PP_ERR_HEADER_ERR           = 0x07,     /* header ���� */
    PP_ERR_EXE_FLOW_ERR         = 0x08,     /* ִ�����̴���δ��Э�������յ����ݰ� */

    /* ҵ����� */
    PP_ERR_NO_FACTORY_FIRMWARE,             /* û�пɹ��ָ������Ĺ̼� */
    PP_ERR_NO_THIS_PART,                    /* �Ҳ����̼���ָ���ķ��� */
    PP_ERR_READ_IS_EMPTY_ERR,               /* �жϷ����Ƿ�Ϊ��ʱ��ȡ���� */
    PP_ERR_FIRMWARE_OVERSIZE,               /* �̼���С������������ */
    PP_ERR_FIRMWARE_HEAD_VERIFY_ERR,        /* �̼���ͷУ����� */
    PP_ERR_VERIFY_READ_ERR,                 /* У��̼�ʱ��ȡ���� */
    PP_ERR_RAW_BODY_VERIFY_ERR,             /* Դ�̼�����У����� */
    PP_ERR_PKG_BODY_VERIFY_ERR,             /* �����Ĺ̼�����У����� */
    PP_ERR_ERASE_PART_ERR,                  /* ������������ */
    PP_ERR_WRITE_FIRST_ADDR_ERR,            /* ����д������׵�ַ���� */
    PP_ERR_JUMP_TO_APP_ERR,                 /* ��ת�� APP ʱ��⵽���� */
    PP_ERR_READ_FIRMWARE_HEAD_ERR,          /* ��ȡ�̼���ͷ���� */
    PP_ERR_UPDATE_READ_ERR,                 /* �̼������� APP ����ʱ��ȡ�������� */
    PP_ERR_UPDATE_VER_READ_ERR,             /* ���¹̼��汾��Ϣʱ��ȡ�������� */
    PP_ERR_UPDATE_VER_ERASE_ERR,            /* ���¹̼��汾��Ϣʱ������������ */
    PP_ERR_UPDATE_VER_WRITE_ERR,            /* ���¹̼��汾��Ϣʱд��������� */
    PP_ERR_WRITE_PART_ERR,                  /* ����д��������� */
    PP_ERR_FAULT_FIRMWARE,                  /* ����Ĺ̼��� */
    PP_ERR_CAN_NOT_PLACE_IN_APP,            /* �ӻ������˶��������ʱ����ָ���̼��������� APP ���� */
    PP_ERR_DOES_NOT_EXIST_DOWNLOAD,         /* ������ download ���� */
    PP_ERR_DOES_NOT_EXIST_FACTORY,          /* ������ factory ���� */
    PP_ERR_NO_DECRYPT_COMPONENT,            /* �ӻ�û�н���������޷����� */
    PP_ERR_DECRYPT_ERR,                     /* �̼�����ʧ�� */

} PP_CMD_ERR_CODE;

/* Э�������ָ�� */
typedef enum  
{
    PP_CMD_NONE                 = 0x00,
    PP_CMD_SOH                  = YMODEM_SOH,
    PP_CMD_STX                  = YMODEM_STX,
    PP_CMD_EOT                  = YMODEM_EOT,
    PP_CMD_CAN                  = YMODEM_CAN,

} PP_CMD;

/* Э��ָ���ִ�н�� */
typedef enum 
{
    PP_RESULT_OK                = 0x00,     /* ִ�гɹ� */
    PP_RESULT_PROCESS           = 0x01,     /* ִ���� */
    PP_RESULT_FAILED            = 0x02,     /* ִ��ʧ�ܣ����� */
    PP_RESULT_CANCEL            = 0x03,     /* ִ��ʧ�ܣ�ȡ������ */

} PP_CMD_EXE_RESULT;

/* ��Э�������������ѡ�� */
typedef enum 
{
    PP_CONFIG_NONE              = 0x00,
    PP_CONFIG_RESET             = 0x01,
    PP_CONFIG_ENABLE_RECV_CMD   = 0x02,
    
} PP_CONFIG_PARA;

/* ��Ҫ�����������λ�������� */
struct PP_DEV_TX_PKG
{
    uint8_t response;
};


/* typedef of function */
typedef void (*PP_Send_t)(uint8_t *data, uint16_t len, uint32_t timeout);
typedef void (*PP_HeartbeatCallback_t)(uint8_t *data, uint16_t *data_len);
typedef void (*PP_PrepareCallback_t)(PP_CMD cmd, uint8_t *data, uint16_t data_len);
typedef void (*PP_ReplyCallback_t)(PP_CMD cmd, PP_CMD_EXE_RESULT *cmd_exe_result, uint8_t *data, uint16_t *data_len);


/* �������� */
void PP_Init(PP_Send_t               Send, 
             PP_HeartbeatCallback_t  HeartbeatCallback,
             PP_PrepareCallback_t    PrepareCallback, 
             PP_ReplyCallback_t      Set_ResponseInfo);
PP_CMD_ERR_CODE     PP_Handler  (uint8_t *data, uint16_t len);
void                PP_Config   (PP_CONFIG_PARA  para, void *value);

#endif
