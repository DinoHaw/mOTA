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
 * 2022-12-04     Dino         1. ����һ����¼�汾�Ļ��ƣ���ѡд�� APP ����
 *                             2. �޸� AC6 -O0 �Ż��£��޷��������� APP ������
 *                             3. ���ӳ��������ָ������̼���ѡ��
 *                             4. �� flash �Ĳ��������������� user_config.h 
 *                             5. ���� Main_Satrt() ����
 *                             6. �����Ƿ��жϹ̼�������������С��ѡ��
 * 2022-12-07     Dino         ���Ӷ� STM32L4 ��֧��
 * 2022-12-08     Dino         ���ӹ̼����ɷ����� SPI flash �Ĺ���
 */

#ifndef __APP_H__
#define __APP_H__

#include "firmware_manage.h"
#include "protocol_parser.h"
#include "data_transfer.h"
#include "utils.h"

/* Ӧ��ִ������ */
typedef enum 
{
    EXE_FLOW_NOTHING = 0x00,
    EXE_FLOW_ACCIDENT_UPDATE,                       /* ������¹̼����� */
    EXE_FLOW_NEED_HOST_SEND_FIRMWARE,               /* ��Ҫ�����·��̼��� */
    
    EXE_FLOW_FIND_RUNNING_FIRMWARE,                 /* (0) �ڸ��������ҿ����еĹ̼� */
    EXE_FLOW_WAIT_FIRMWARE,                         /* �ȴ����չ̼��� */
    EXE_FLOW_VERIFY_FIRMWARE_HEAD,                  /* (1) У���յ��Ĺ̼���ͷ */
    EXE_FLOW_ERASE_OLD_FIRMWARE,                    /* �����ɹ̼� */
    EXE_FLOW_ERASE_OLD_FIRMWARE_DONE,               /* ��ɲ����ɹ̼� */
    EXE_FLOW_WRITE_FIRMWARE_HEAD,                   /* ���̼���ͷд�� flash */
    EXE_FLOW_WRITE_FIRMWARE_HEAD_DONE,              /* ��ɹ̼���ͷ��д�� */
    EXE_FLOW_VERIFY_FIRMWARE_PKG,                   /* (2) У��̼��ְ����ݵ���ȷ�� */
    EXE_FLOW_WRITE_NEW_FIRMWARE,                    /* д���µĹ̼��ְ������� */
    EXE_FLOW_WRITE_NEW_FIRMWARE_DONE,               /* д���µĹ̼��ְ���� */
    EXE_FLOW_UPDATE_FIRMWARE,                       /* (3) ��ʼ���¹̼� */
    EXE_FLOW_VERIFY_FIRMWARE,                       /* У�������̼������������ȷ�� */
    EXE_FLOW_VERIFY_FIRMWARE_DONE,                  /* ���У�������̼������������ȷ�� */
    EXE_FLOW_ERASE_APP,                             /* ���� APP �����Ĺ̼� */
    EXE_FLOW_UPDATE_TO_APP,                         /* �����������Ĺ̼����µ� APP ���� */
    EXE_FLOW_VERIFY_APP,                            /* У�� APP �����̼���������ȷ�� */
    EXE_FLOW_UPDATE_TO_APP_DONE,                    /* У�� APP �����̼�ͨ���󣬽�ʣ������д�� */
    EXE_FLOW_ERASE_DOWNLOAD,                        /* ���� download ���� */
    EXE_FLOW_ERASE_DOWNLOAD_DONE,                   /* ��ɲ��� download ���� */
    EXE_FLOW_JUMP_TO_APP,                           /* ��ת�� APP ���� */
    EXE_FLOW_RECOVERY,                              /* �ָ������̼� */
    EXE_FLOW_FAILED,                                /* ʧ������ */
    
} BOOT_EXE_FLOW;

/* Ӧ��״̬ */
typedef enum 
{
    BOOT_STATUS_NONE = 0x00,
    BOOT_STATUS_NO_UPDATE,                          /* ������¹̼��ı�־ */
    BOOT_STATUS_ENTER_UPDATE_MODE,                  /* ����̼�����ģʽ�ı�־ */
    BOOT_STATUS_ACCIDENT_UPDATE,                    /* ������¹̼��� */
    BOOT_STATUS_UPDATE_SUCCESS,                     /* ���¹̼��ɹ� */
    BOOT_STATUS_UPDATE_FAILED,                      /* ���¹̼�ʧ�� */
    BOOT_STATUS_NO_FIRMWARE,                        /* ���κο��ù̼� */
    BOOT_STATUS_NO_APP,                             /* �� APP �̼� */
    BOOT_STATUS_READ_PART_ERR,                      /* ��ȡ����ʧ�� */
    BOOT_STATUS_APP_VERIFY_ERR,                     /* APP �̼�У����� */
    BOOT_STATUS_APP_CAN_NOT_VERIFY,                 /* APP �̼��޷�У�� */
    BOOT_STATUS_AUTO_UPDATE_FAILED,                 /* �Զ�����ʧ�� */
    
} BOOT_STATUS;

/* �̼����µ����Բ��裬���ڼ���̼����µĽ��� */
typedef enum 
{
    STEP_VERIFY_FIRMWARE = 0x00,                    /* У����յ��Ĺ̼��� */
    STEP_ERASE_APP,                                 /* ���� APP ���� */
    STEP_UPDATE_TO_APP,                             /* ���¹̼����� APP ���� */
    STEP_VERIFY_APP,                                /* У�� APP �̼� */
    STEP_ERASE_DOWNLOAD,                            /* ���� download ���� */
    
} BOOY_UPDATE_STEP;

/* �̼����µ�������Ϣ */
struct FIRMWARE_UPDATE_INFO
{
    BOOT_STATUS            status;                  /* ��¼ bootloader ������״̬ */
    BOOT_EXE_FLOW          exe_flow;                /* Ӧ��ִ�е����� */
    BOOY_UPDATE_STEP       step;                    /* �̼��ĸ��½׶Σ����ڳ����ж� total_progress ������ϵ�� */

    PP_CMD_EXE_RESULT      cmd_exe_result;          /* ָ��ִ�н�� */
    PP_CMD_ERR_CODE        cmd_exe_err_code;        /* ָ��ִ��ʧ��ʱ�Ĺ����� */

    uint8_t start           :1;                     /* ��ʼ�̼��������̵ı�־ */
    uint8_t is_recovery     :1;                     /* �Ƿ����ڽ��лָ������̼��ı�־λ */
    uint8_t                 :0;
    
    uint8_t total_progress;                         /* �̼����µ��ܽ��ȣ� 0-100 */
};

#endif


