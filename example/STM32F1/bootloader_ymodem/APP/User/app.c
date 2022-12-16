/**
 * \file            app.c
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
 * Version:         v1.0.2
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
 */


/* Includes ------------------------------------------------------------------*/
#include "app.h"


/* Private variables ---------------------------------------------------------*/
/* �̼����µı�־λ���ñ�־λ���ܱ����� */
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE)
    #if defined(__IS_COMPILER_ARM_COMPILER_5__)
    volatile uint64_t update_flag __attribute__((at(FIRMWARE_UPDATE_VAR_ADDR), zero_init));

    #elif defined(__IS_COMPILER_ARM_COMPILER_6__)
        #define __INT_TO_STR(x)     #x
        #define INT_TO_STR(x)       __INT_TO_STR(x)
        volatile uint64_t update_flag __attribute__((section(".bss.ARM.__at_" INT_TO_STR(FIRMWARE_UPDATE_VAR_ADDR))));

    #else
        #error "variable placement not supported for this compiler."
    #endif
#endif

static uint8_t  _is_firmware_head;                          /* �Ƿ�Ϊ�̼���ͷ�ı�־λ */
static uint16_t _dev_rx_len;                                /* ָʾ���������յ�������������λ byte */
static uint8_t  _dev_rx_buff[PP_MSG_BUFF_SIZE + 16];        /* �豸�ײ����ݽ��ջ����� */
static uint8_t  _fw_head_data[FPK_HEAD_SIZE];               /* �����ݴ�̼���ͷ */
static uint8_t  _fw_sub_pkg_data[PP_FIRMWARE_PKG_SIZE];     /* �ݴ�̼����� */
static uint16_t _fw_sub_pkg_len;                            /* ��¼�̼������С�����������ֽڵ����ݳ��� */
static uint32_t _stack_addr;                                /* APP ջ����ַ */
static uint32_t _reset_handler;                             /* APP reset handler ��ַ */

static struct BSP_TIMER _timer_led;                         /* LED ����˸ timer */
static struct BSP_TIMER _timer_wait_data;                   /* ����������ݷ��ͳ�ʱ�� timer */
#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
static struct BSP_TIMER _timer_key;                         /* ���ڰ���ɨ��� timer */
static struct BSP_KEY   _recovery_key;                      /* ���ڻָ������̼��İ��� */  
#endif

static struct DATA_TRANSFER         _data_if;               /* ���ݴ���Ľӿ� */
static struct FIRMWARE_UPDATE_INFO  _fw_update_info;        /* �̼����µ���Ϣ��¼ */


/* Private function prototypes -----------------------------------------------*/
static void         _Host_Data_Handler              (void);
static void         _UART_SendData                  (uint8_t *data, uint16_t len, uint32_t timeout);
static void         _Timer_HostDataTimeoutCallback  (void *user_data);
static void         _Timer_LedFlashCallback         (void *user_data);
static void         _Bootloader_JumpToAPP           (void);
static void         _Bootloader_SetExeFlow          (BOOT_EXE_FLOW  flow);
static void         _Bootloader_ProtocolDataHandle  (PP_CMD  cmd, uint8_t *data, uint16_t data_len);
static void         _Bootloader_SetReplyInfo        (PP_CMD  cmd, PP_CMD_EXE_RESULT  *cmd_exe_result, uint8_t *data, uint16_t *data_len);
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
static uint8_t      _Firmware_Check                 (void);
static void         _Firmware_CheckAndHandle        (void);
static FM_ERR_CODE  _Firmware_AutoUpdate            (const char *part_name);
#endif
#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
static uint8_t      _Key_GetLevel                   (void);
static void         _Key_EventCallback              (uint8_t id, KEY_EVENT  event);
static void         _Timer_ScanKeyCallback          (void *user_data);
#endif


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  ���� main ��������Ҫ����ִ�еĴ���
 * @note   Ϊ�����̶ȵļ��ٶ� APP ��Ӱ�죬��������� SystemInit() ������һ��ִ��
 * @retval None
 */
void Main_Start(void)
{
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE)
    if (update_flag == BOOTLOADER_RESET_MAGIC_WORD)
        _Bootloader_JumpToAPP();
#endif
}


/**
 * @brief  �����ʼ��ǰ��һЩ����
 * @note   ִ�е��˴����ں�ʱ���ѳ�ʼ��
 * @retval None
 */
void System_Init(void)
{
    BSP_Board_Init();

#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE)
    /* ���̼����±�־λ */
    if (update_flag != FIRMWARE_UPDATE_MAGIC_WORD)
    {
        if (update_flag == FIRMWARE_RECOVERY_MAGIC_WORD)
            _Bootloader_SetExeFlow(EXE_FLOW_RECOVERY);
        else
            _Bootloader_SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
    }
#endif

    /* TODO: �˴��� ASSERT ���ʵ�ַ�ʽӦΪ����׶μ��ɱ�������δ�ҵ����� */
    ASSERT(USING_PART_PROJECT >= ONE_PART_PROJECT && USING_PART_PROJECT <= TRIPLE_PART_PROJECT);
    ASSERT(sizeof(AES256_KEY) != 32);
    ASSERT(sizeof(AES256_IV) != 16);

    ASSERT(USING_AUTO_UPDATE_PROJECT >= DO_NOT_AUTO_UPDATE          \
    &&     USING_AUTO_UPDATE_PROJECT <= MODIFY_DOWNLOAD_PART_PROJECT);

    ASSERT(USING_APP_SAFETY_CHECK_PROJECT >= DO_NOT_CHECK       \
    &&     USING_APP_SAFETY_CHECK_PROJECT <= DO_NOT_DO_ANYTHING);

    ASSERT(ONCHIP_FLASH_ERASE_GRANULARITY > 0);
    ASSERT(SPI_FLASH_ERASE_GRANULARITY > 0);

    ASSERT(ONCHIP_FLASH_SIZE > 0);
    ASSERT(APP_PART_SIZE > 0);
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
    ASSERT(DOWNLOAD_PART_SIZE > 0);
#endif
#if (USING_PART_PROJECT > DOUBLE_PART_PROJECT)
    ASSERT(FACTORY_PART_SIZE > 0);
#endif
}


/**
 * @brief  Ӧ�ó�ʼ��
 * @note   ��ʱ�����Ѿ���ʼ�����
 * @retval None
 */
void APP_Init(void)
{
#if (ENABLE_DEBUG_PRINT)
    #if (EANBLE_PRINTF_USING_RTT)
    /* ����ͨ�� 0 ���������� */
    SEGGER_RTT_ConfigDownBuffer(SEGGER_RTT_PRINTF_TERMINAL, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    SEGGER_RTT_SetTerminal(SEGGER_RTT_PRINTF_TERMINAL);
    #else
    BSP_UART_Init( BSP_UART1 );
    #endif

    uint32_t hal_version = HAL_GetHalVersion();

    BSP_Printf("[OTA Component] DinoHaw\r\n");
    BSP_Printf("UID: %.8X %.8X %.8X\r\n", HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2());
    BSP_Printf("bootloader Version: V%d.%d\r\n", BOOT_VERSION_MAIN, BOOT_VERSION_SUB);
    BSP_Printf("HAL Version: V%d.%d.%d.%d\r\n", (hal_version >> 24), (uint8_t)(hal_version >> 16), (uint8_t)(hal_version >> 8), (uint8_t)hal_version);
    #if (ENABLE_SPI_FLASH)
    BSP_Printf("FAL Version: V%s\r\n", FAL_SW_VERSION);
    #endif
#endif

#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
    BSP_Key_Init(&_recovery_key, 0, _Key_GetLevel, FACTORY_FIRMWARE_BUTTON_PRESS);
    BSP_Key_Register(&_recovery_key, KEY_LONG_PRESS, _Key_EventCallback);
    BSP_Key_Start(&_recovery_key);
    
    BSP_Timer_Init( &_timer_key, 
                    _Timer_ScanKeyCallback, 
                    2,
                    TIMER_RUN_FOREVER, 
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_key);
#endif
    
    /* BSP ��ʼ�� */
    BSP_Timer_Init( &_timer_led, 
                    _Timer_LedFlashCallback, 
                    100,
                    TIMER_RUN_FOREVER, 
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_led);

#if (WAIT_HOST_DATA_MAX_TIME)
    /* ��ʼ״̬Ϊ�ȴ������Ƿ��·����ݵĶ�ʱ�� */
    BSP_Timer_Init( &_timer_wait_data,
                    _Timer_HostDataTimeoutCallback,
                    WAIT_HOST_DATA_MAX_TIME,
                    TIMER_RUN_FOREVER,
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_wait_data);
#endif

    /* �����ʼ�� */
    DT_Init(&_data_if, BSP_UART1, _dev_rx_buff, &_dev_rx_len, PP_MSG_BUFF_SIZE + 16);
    PP_Init(_UART_SendData, NULL, _Bootloader_ProtocolDataHandle, _Bootloader_SetReplyInfo);
    FM_Init();
}


/**
 * @brief  Ӧ�ÿ�ʼ����
 * @note   APP_Running �������һ�� while ѭ����
 * @retval None
 */
void APP_Running(void)
{
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
    /* ��¼��ǰ�����Ĺ̼����� */
    char *part_name = NULL;
#endif

    while (1)
    {
        /* �������ݴ����� */
        _Host_Data_Handler();

        /* Ӧ��ִ������״̬�� */
        switch (_fw_update_info.exe_flow)
        {
        #if (USING_PART_PROJECT > ONE_PART_PROJECT)
            /* ������¹̼� �� ���¹̼������еȴ�����ָ�ʱ �� ����ʧ��ʱ �Ĵ����߼� */
            case EXE_FLOW_FIND_RUNNING_FIRMWARE:
            {
                _Firmware_CheckAndHandle();
                break;
            }
        #else
            case EXE_FLOW_FIND_RUNNING_FIRMWARE:
            case EXE_FLOW_JUMP_TO_APP:
            {
                if (FM_CheckFirmwareIntegrity(APP_ADDRESS) == FM_ERR_OK)
                    _Bootloader_JumpToAPP();
                else
                    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE);
                break;
            }
        #endif
            /* �ݴ��У��̼���ͷ�����̣���ʱ���ѽ���̼����µĿ�ʼ�׶� */
            case EXE_FLOW_VERIFY_FIRMWARE_HEAD:
            {
                _fw_update_info.cmd_exe_err_code = PP_ERR_OK;
                
                /* ȡ���̼���ͷ�еķ����� */
                struct FPK_HEAD *p_fpk_head = (struct FPK_HEAD *)_fw_head_data;
                part_name = p_fpk_head->part_name;

            #if (USING_PART_PROJECT > ONE_PART_PROJECT)
                #if (ENABLE_AUTO_CORRECT_PART)
                /* �Զ����̼���ָ���� APP ��������Ϊ download ���� */
                if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                    part_name = DOWNLOAD_PART_NAME;
                #else
                /* �̼���ָ���Ĵ�ŷ����Ϸ��Լ�飬���������ʱ������ָ������ APP ���� */
                if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_CAN_NOT_PLACE_IN_APP;
                    break;
                }
                #endif
            #endif
                
                /* ���̼����Ƿ񳬹��˷�����С */
            #if (ENABLE_CHECK_FIRMWARE_SIZE)
                #if (USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
                if (p_fpk_head->raw_size > (APP_PART_SIZE - FPK_VERSION_SIZE))
                #else
                if (p_fpk_head->raw_size > APP_PART_SIZE)
                #endif
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_FIRMWARE_OVERSIZE;
                    break;
                }
                
                #if (USING_PART_PROJECT == DOUBLE_PART_PROJECT)
                if (p_fpk_head->pkg_size > DOWNLOAD_PART_SIZE)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_FIRMWARE_OVERSIZE;
                    break;
                }
                #elif (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
                if (strncmp(part_name, DOWNLOAD_PART_NAME, MAX_NAME_LEN) == 0)
                {
                    if (p_fpk_head->pkg_size > DOWNLOAD_PART_SIZE)
                    {
                        _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                        _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                        _fw_update_info.cmd_exe_err_code = PP_ERR_FIRMWARE_OVERSIZE;
                        break;
                    }
                }
                else if (strncmp(part_name, FACTORY_PART_NAME, MAX_NAME_LEN) == 0)
                {
                    #if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
                    if (p_fpk_head->pkg_size > FACTORY_PART_SIZE)
                    {
                        _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                        _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                        _fw_update_info.cmd_exe_err_code = PP_ERR_FIRMWARE_OVERSIZE;
                        break;
                    }
                    #else
                    /* ˫�����ķ��������ܴ��� factory ���� */
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_DOES_NOT_EXIST_FACTORY;
                    break;
                    #endif
                }
                #endif
            #endif  /* #if (ENABLE_CHECK_FIRMWARE_SIZE) */
                
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                /* �򵥷����ķ������ӹ̼���ָ���ķ�������˲���Ҫ�жϷ����治���� */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_StorageFirmwareHead(APP_PART_NAME, _fw_head_data);
            #else  
                /* �ݴ�̼���ͷ */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_StorageFirmwareHead(part_name, _fw_head_data);
            #endif
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_ERASE_OLD_FIRMWARE);
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                break;
            }
            /* ������: ���� APP �̼��������: ����Ҫ����̼��ķ������� */
            case EXE_FLOW_ERASE_OLD_FIRMWARE:
            {
                /* �жϷ����Ƿ�Ϊ�� */
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_IsEmpty(APP_PART_NAME);
            #else
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_IsEmpty(part_name);
            #endif
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                    _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                    _Bootloader_SetExeFlow(EXE_FLOW_ERASE_OLD_FIRMWARE_DONE);
                #else
                    _Bootloader_SetExeFlow(EXE_FLOW_WRITE_FIRMWARE_HEAD);
                #endif
                    break;
                }
                else if (_fw_update_info.cmd_exe_err_code != FM_ERR_FLASH_NO_EMPTY)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                    break;
                }
                
                /* ������Ϊ��ʱ�Ų��� */
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(APP_PART_NAME);
            #else
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(part_name);
            #endif
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                    _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                    _Bootloader_SetExeFlow(EXE_FLOW_ERASE_OLD_FIRMWARE_DONE);
                #else
                    _Bootloader_SetExeFlow(EXE_FLOW_WRITE_FIRMWARE_HEAD);
                #endif
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                break;
            }
            #if (USING_PART_PROJECT > ONE_PART_PROJECT)
            /* ���̼���ͷд�� flash */
            case EXE_FLOW_WRITE_FIRMWARE_HEAD:
            {
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareSubPackage(part_name, _fw_head_data, FPK_HEAD_SIZE);
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_WRITE_FIRMWARE_HEAD_DONE);
                    _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                break;
            }
            #endif
            /* У���յ��Ĺ̼��ְ���������ȷ�ԡ�˵�������·��˹̼��ְ� */
            case EXE_FLOW_VERIFY_FIRMWARE_PKG:
            {
                /* �� YModem Э���ѽ���У�飬�����������������ɣ�ֱ�ӽ�����һ�� */
                _Bootloader_SetExeFlow(EXE_FLOW_WRITE_NEW_FIRMWARE);
                break;
            }
            /* ���̼��ְ�д������� */
            case EXE_FLOW_WRITE_NEW_FIRMWARE:
            {
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareSubPackage(APP_PART_NAME, &_fw_sub_pkg_data[0], _fw_sub_pkg_len);
            #else
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareSubPackage(part_name, &_fw_sub_pkg_data[0], _fw_sub_pkg_len);
            #endif
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_WRITE_NEW_FIRMWARE_DONE);
                    _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                break;
            }
            /* �̼��ѽ�����ϡ���ʱ�������׵�ַ�� 4 ���ֽ�д�� */
            case EXE_FLOW_UPDATE_FIRMWARE:
            {
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareDone(APP_PART_NAME);
            #else
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareDone(part_name);
            #endif
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _fw_update_info.total_progress = 0;
                    _Bootloader_SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE);
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                break;
            }
            /* У�����غõĹ̼��������ݵ���ȷ�ԡ�������: ִ�гɹ��� 100% ��ɣ������: ִ�гɹ������ 25% */
            /* �������ʱ��Ҳ��������̿�ʼ */
            case EXE_FLOW_ACCIDENT_UPDATE:
            case EXE_FLOW_VERIFY_FIRMWARE:
            {
                _fw_update_info.step = STEP_VERIFY_FIRMWARE;
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.step = STEP_ERASE_DOWNLOAD;
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 0);
            #else
                /* ȡ���̼���ͷ�еķ����� */
                part_name = FM_GetPartName();
                
                #if (ENABLE_AUTO_CORRECT_PART)
                    /* �Զ����̼���ָ���� APP ��������Ϊ download ���� */
                    if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                        part_name = DOWNLOAD_PART_NAME;
                #else
                    /* �̼���ָ���Ĵ�ŷ����Ϸ��Լ�飬���������ʱ������ָ������ APP ���� */
                    if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                    {
                        _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                        _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                        _fw_update_info.cmd_exe_err_code = PP_ERR_CAN_NOT_PLACE_IN_APP;
                        break;
                    }
                #endif
                /* У��̼� */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(part_name, FM_GetPackageCRC32(), 0);
            #endif
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                    _fw_update_info.total_progress = 100;
                    _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                    _Bootloader_SetExeFlow(EXE_FLOW_JUMP_TO_APP);
                #else
                    _fw_update_info.total_progress = 20;
                    _Bootloader_SetExeFlow(EXE_FLOW_ERASE_APP);
                #endif
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                BSP_Printf("%s: step 1 !!!\r\n", __func__);
                break;
            }
            #if (USING_PART_PROJECT > ONE_PART_PROJECT)
            /* ���� APP �����Ĺ̼���׼��д���µĹ̼� */
            case EXE_FLOW_ERASE_APP:
            {
                _fw_update_info.step = STEP_ERASE_APP;
                /* �ж� APP �����Ƿ�Ϊ�� */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_IsEmpty(APP_PART_NAME);
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _fw_update_info.total_progress = 40;
                    _Bootloader_SetExeFlow(EXE_FLOW_UPDATE_TO_APP);
                    break;
                }
                else if (_fw_update_info.cmd_exe_err_code != FM_ERR_FLASH_NO_EMPTY)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                    break;
                }
                
                /* Ϊ��ʱ�Ų������� */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(APP_PART_NAME);
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _fw_update_info.total_progress = 40;
                    _Bootloader_SetExeFlow(EXE_FLOW_UPDATE_TO_APP);
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                BSP_Printf("%s: step 2 !!!\r\n", __func__);
                break;
            }
            /* ���µĹ̼�д�� APP �����У��߽��ܱ�д�� */
            case EXE_FLOW_UPDATE_TO_APP:
            {
                _fw_update_info.step = STEP_UPDATE_TO_APP;
                
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_UpdateToAPP(part_name);   
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _fw_update_info.total_progress = 60;
                    _Bootloader_SetExeFlow(EXE_FLOW_VERIFY_APP);
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                BSP_Printf("%s: step 3 !!!\r\n", __func__);
                break;
            }
            /* У�� APP �����Ĺ̼�������ȷ�� */
            case EXE_FLOW_VERIFY_APP:
            {
                _fw_update_info.step = STEP_VERIFY_APP;
                /* ���ʱ APP �������׵�ַ������δд�룬�����Ҫ�� FM_VerifyFirmware �Զ�����Խ���У�� */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 1);
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _fw_update_info.total_progress = 80;
                    _Bootloader_SetExeFlow(EXE_FLOW_UPDATE_TO_APP_DONE);
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                BSP_Printf("%s: step 4 !!!\r\n", __func__);
                break;
            }
            /* У�� APP �����̼�ͨ���󣬽�ʣ������д�� */
            case EXE_FLOW_UPDATE_TO_APP_DONE:
            {
            #if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT ||   \
                 USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
                /* ���¼�¼�Ĺ̼��汾 */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_UpdateFirmwareVersion(part_name);
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
            #endif
                
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareDone(APP_PART_NAME);
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_ERASE_DOWNLOAD);
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                break;
            }
            /* �����ݴ�̼��ķ���������� factory ����������Ҫ���� */
            case EXE_FLOW_ERASE_DOWNLOAD:
            {
                _fw_update_info.step = STEP_ERASE_DOWNLOAD;

                if (strncmp(part_name, FACTORY_PART_NAME, MAX_NAME_LEN) == 0)
                    _fw_update_info.cmd_exe_err_code = PP_ERR_OK;
                else
                {
                #if (USING_AUTO_UPDATE_PROJECT == ERASE_DOWNLOAD_PART_PROJECT)
                    _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(part_name);
                #else
                    _fw_update_info.cmd_exe_err_code = PP_ERR_OK;
                #endif
                }
                    
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _fw_update_info.total_progress = 100;
                    _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                    _fw_update_info.status         = BOOT_STATUS_UPDATE_SUCCESS;
                    _Bootloader_SetExeFlow(EXE_FLOW_JUMP_TO_APP);
                }
                else
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
                BSP_Printf("%s: step 5 !!!\r\n", __func__);
                break;
            }
            /* ��ת�� APP */
            case EXE_FLOW_JUMP_TO_APP:
            {
                if (FM_CheckFirmwareIntegrity(APP_ADDRESS) == FM_ERR_OK)
                    _Bootloader_JumpToAPP();
                else
                    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE);
                break;
            }
            #endif  /* #if (USING_PART_PROJECT > ONE_PART_PROJECT) */
            /* �����·��Ļָ������̼���ָ�� �� APP ��������Ҫ�ָ������̼�����Ϣ */
            case EXE_FLOW_RECOVERY:
            {
            #if (USING_PART_PROJECT != TRIPLE_PART_PROJECT)
                #if (FACTORY_NO_FIRMWARE_SOLUTION == JUMP_TO_APP)
                    _Bootloader_SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
                #else
                    _Bootloader_SetExeFlow(EXE_FLOW_NOTHING);
                #endif
                _fw_update_info.cmd_exe_result   = PP_RESULT_FAILED;
                _fw_update_info.cmd_exe_err_code = PP_ERR_DOES_NOT_EXIST_FACTORY;
            #else
                /* ��� factory �Ƿ��п��õĹ̼� */
                if ((_Firmware_Check() & 0x01) == 0)
                {
                #if (FACTORY_NO_FIRMWARE_SOLUTION == JUMP_TO_APP)
                    _Bootloader_SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
                #else
                    _Bootloader_SetExeFlow(EXE_FLOW_NOTHING);
                #endif
                    _fw_update_info.cmd_exe_result   = PP_RESULT_FAILED;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_NO_FACTORY_FIRMWARE;
                    break;
                }

                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_ReadFirmwareHead(FACTORY_PART_NAME);
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE);
                }
                else
                {
                #if (FACTORY_NO_FIRMWARE_SOLUTION == JUMP_TO_APP)
                    _Bootloader_SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
                #else
                    _Bootloader_SetExeFlow(EXE_FLOW_NOTHING);
                #endif
                    _fw_update_info.cmd_exe_result = PP_RESULT_FAILED;
                }
            #endif
                break;
            }
            /* �̼�����ʧ�ܵĴ����߼� */
            case EXE_FLOW_FAILED:
            {
                _is_firmware_head      = 0;
                _fw_update_info.start  = 0;
                _fw_update_info.step   = STEP_VERIFY_FIRMWARE;
                _fw_update_info.status = BOOT_STATUS_UPDATE_FAILED;
                _Bootloader_SetExeFlow(EXE_FLOW_NOTHING);
                break;
            }
            default: break;
        }
    }
}


/**
 * @brief  �̼�д��ʱ�Ļص�����
 * @note   
 * @param[in]  progress: ���ȣ�0-10000��
 * @retval None
 */
void Firmware_OperateCallback(uint16_t progress)
{
    /* ���¹̼����½��Ȱٷֱ� */
    _fw_update_info.total_progress = (_fw_update_info.step * 20) + (progress / 500);

    /* �������������ݣ���ֹ���� */
    _Host_Data_Handler();
    BSP_Printf("%s: progress: %d\r\n", __func__, _fw_update_info.total_progress);
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  ���ݽ��մ�����
 * @note   
 * @retval None
 */
static void _Host_Data_Handler(void)
{
    /* ��ѯ��ʽ����ֹӦ������ */
    if (DT_PollingReceive(&_data_if) == DT_RESULT_RECV_FRAME_DATA)
    {
    #if (WAIT_HOST_DATA_MAX_TIME)
        BSP_Timer_Restart(&_timer_wait_data);
    #endif

        /* ����Э��������Ĵ������������յ���һ֡���ݵ��� */
        if (PP_Handler(_dev_rx_buff, _dev_rx_len) != PP_ERR_OK)
        {
            PP_Config(PP_CONFIG_RESET, NULL);
            BSP_Printf("uart recv len: %d\r\n", _dev_rx_len);
        }
        _dev_rx_len = 0;
    }
    else
        PP_Handler(NULL, 0);
}


/**
 * @brief  �������Э���������������
 * @note   ͨ��Э���ָ��������ó����ִ������
 * @param[in]  cmd: ���յ���ָ��
 * @param[in]  data: ���յ��Ĳ���
 * @param[in]  data_len: ���յ��Ĳ������ȣ���λ byte
 * @retval None
 */
static void _Bootloader_ProtocolDataHandle(PP_CMD cmd, uint8_t *data, uint16_t data_len)
{
    static uint8_t eot_flag = 0;
    
    _fw_update_info.cmd_exe_result = PP_RESULT_PROCESS;

    switch ((uint8_t)cmd)
    {
        case PP_CMD_SOH:
        case PP_CMD_STX:
        {
        #if (WAIT_HOST_DATA_MAX_TIME && USING_IS_NEED_UPDATE_PROJECT == USING_HOST_CMD_UPDATE)
            /* ���� timer Ϊ�ȴ������·��̼����Ķ�ʱ�� */
            BSP_Timer_Pause(&_timer_wait_data);
            BSP_Timer_Init( &_timer_wait_data, 
                            _Timer_HostDataTimeoutCallback, 
                            (60 * 1000), 
                            TIMER_RUN_FOREVER, 
                            TIMER_TYPE_HARDWARE);
            BSP_Timer_Start(&_timer_wait_data);
        #endif
            
            /* ���һ�� SOH ������֡ */
            if (eot_flag && cmd == PP_CMD_SOH)
            {
                eot_flag = 0;
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                _Bootloader_SetExeFlow(EXE_FLOW_UPDATE_FIRMWARE);
                break;
            }

            /* ��������֡�Ĵ��� */
            _fw_update_info.status = BOOT_STATUS_ENTER_UPDATE_MODE;

            /* �������� */
            _fw_sub_pkg_len = data_len;
            memcpy(&_fw_sub_pkg_data[0], &data[0], _fw_sub_pkg_len);
            BSP_Printf("_fw_sub_pkg_len: %d\r\n", _fw_sub_pkg_len);

            /* ��һ�� SOH ����֡�������ļ������ļ���С����Ϣ */
            if (cmd == PP_CMD_SOH
            &&  _fw_update_info.start == 0)
            {
                _fw_update_info.start = 1;
                char *file_name = (char *)&_fw_sub_pkg_data[0];
                char *file_size = (char *)&_fw_sub_pkg_data[strlen(file_name) + 1];
                BSP_Printf("file name: %s\r\n", file_name);
                BSP_Printf("file size: %d\r\n", atol(file_size));
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                break;
            }
            
            /* �̼���ͷ���������� */
            if (_is_firmware_head == 0)
            {
                _is_firmware_head = 1;
                memcpy(&_fw_head_data[0], &data[0], FPK_HEAD_SIZE);
                _Bootloader_SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE_HEAD);
            }
            /* �̼����� */
            else
                _Bootloader_SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE_PKG);
            break;
        }
        case PP_CMD_EOT:
        {
            eot_flag = 1;
            _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            break;
        }
        case PP_CMD_CAN:
        {
            eot_flag = 0;
            _is_firmware_head = 0;
            _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            _Bootloader_SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
            break;
        }
        default:
        {
            _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            break;
        }
    }
}


/**
 * @brief  �ظ�ָ���ִ�������Э����Զ����
 * @note   ���ݵ�ǰִ�е�ָ���ִ����������Ͷ�Ӧ����
 * @param[in]   cmd: ����ִ�е�ָ��
 * @param[out]  cmd_exe_result: ָ��ִ�н��
 * @param[out]  data: ��Ҫ��Ӧ������
 * @param[out]  data_len: ��Ҫ��Ӧ�����ݳ��ȣ���λ byte
 * @retval None
 */
static void _Bootloader_SetReplyInfo(PP_CMD cmd, PP_CMD_EXE_RESULT *cmd_exe_result, uint8_t *data, uint16_t *data_len)
{
    *cmd_exe_result = _fw_update_info.cmd_exe_result;
    
    if (_fw_update_info.cmd_exe_result == PP_RESULT_CANCEL
    ||  _fw_update_info.cmd_exe_result == PP_RESULT_FAILED)
        BSP_Printf("cmd_exe_err_code: %.2X\r\n", _fw_update_info.cmd_exe_err_code);
}


#if (USING_PART_PROJECT > ONE_PART_PROJECT)
/**
 * @brief  �Զ����¹̼��Ĵ�����
 * @note   
 * @param[in]  part_name: �̼����ڵķ�����
 * @retval FM_ERR_CODE
 */
static FM_ERR_CODE  _Firmware_AutoUpdate(const char *part_name)
{
    static uint8_t value = 0;
    FM_ERR_CODE  result = FM_ERR_OK;
    
    /* ����Ϊ����������¹̼���״̬ */
    _fw_update_info.status = BOOT_STATUS_ACCIDENT_UPDATE;
    /* ��ֹ����������ָ�� */
    PP_Config(PP_CONFIG_ENABLE_RECV_CMD, &value);
    
    /* ��ȡ�̼���ͷ */
    result = FM_ReadFirmwareHead(part_name);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
        return result;
    }
    
    /* У��̼���ȷ�� */
    result = FM_VerifyFirmware(part_name, FM_GetPackageCRC32(), 0);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
        return result;
    }
    
    /* ��� APP �����Ƿ�Ϊ�գ���Ϊ��ʱ�������� */
    result = FM_IsEmpty(APP_PART_NAME);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: app is not empty\r\n", __func__);
        result = FM_EraseFirmware(APP_PART_NAME);
    }
    
    /* ���̼������� APP ���� */
    result = FM_UpdateToAPP(part_name);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
        return result;
    }
    
    /* У�� APP �����Ĺ̼� */
    result = FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 1);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
        return result;
    }
    
#if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT ||   \
     USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
    /* ��¼�̼��汾�� flash �ķ���������¼�¼�Ĺ̼��汾 */
    result = FM_UpdateFirmwareVersion(part_name);
#endif

    /* ��ʣ�������д�� APP ���� */
    result = FM_WriteFirmwareDone(APP_PART_NAME);
    
#if (USING_AUTO_UPDATE_PROJECT == ERASE_DOWNLOAD_PART_PROJECT)
    /* ���� download �����ķ�������ֱ�Ӳ��� */
    result = FM_EraseFirmware(part_name);
#endif
    
    if (result != FM_ERR_OK)
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
    
    return result;
}


/**
 * @brief  �̼��ļ��ʹ�����
 * @note   �ϵ�ʱ��� �� ����ʧ��ʱ��飬ִ�е����ȼ���������ָ������ȷ����������Ҫ���¹̼�ʱ�ſ�ִ��
 * @retval None
 */
static void _Firmware_CheckAndHandle(void)
{
    uint8_t is_try_auto_update = 0;
    uint8_t fw_sta = _Firmware_Check();
    
    /* �������Ĵ����߼��ϸ��ӡ�Ϊ�����ִ��Ч�ʣ�������Դռ�ã�ʹ���� goto ���Ķ�ʱ�����ϡ��̼����ʹ�����ơ��ĵ��鿴 */

    /* û���κο��õĹ̼� */
    if (fw_sta == 0)
    {
        _fw_update_info.status = BOOT_STATUS_NO_FIRMWARE;
        _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    }
    
#if (USING_AUTO_UPDATE_PROJECT)
    /* download �����й̼� */
    if ((fw_sta & 0x02) == 0x02)
    {
    #if (USING_AUTO_UPDATE_PROJECT == ERASE_DOWNLOAD_PART_PROJECT)
        /* �������Ǽ�⵽ download �����п��ù̼���ʼ�Զ����� */
        if (_Firmware_AutoUpdate(DOWNLOAD_PART_NAME))
        {
            is_try_auto_update = 1;
            goto __check_app;
        }
        else
            goto __jump_to_app;
    #elif (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT || \
           USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
        /* �������Ǽ�� download �����̼���ͷ�İ汾��Ϣ�����ж��Ƿ���Ҫ�Զ����� */
        /* ���̼���ͷ */
        _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_ReadFirmwareHead(DOWNLOAD_PART_NAME);
        if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
            goto __check_app;
        
        /* ��Ҫ�Զ����� */
        if (FM_IsNeedAutoUpdate())
        {
            if (_Firmware_AutoUpdate(DOWNLOAD_PART_NAME))
            {
                is_try_auto_update = 1;
                goto __check_app;
            }
            else
                goto __jump_to_app;
        }
    #endif
    }
#endif
    
__check_app:
    /* APP �����й̼� */
    if ((fw_sta & 0x04) == 0x04)
    {
    #if (USING_APP_SAFETY_CHECK_PROJECT)
        /* ���� APP �̼���� �� download �����й̼� */
        if ((fw_sta & 0x02) == 0x02)
        {
            /* ���̼���ͷ */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_ReadFirmwareHead(DOWNLOAD_PART_NAME);

        #if (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
             USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
            /* ���� APP �̼������Զ����·���������ת����� download �����̼� */
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                goto __check_download;
        #else
            /* δ���� APP �̼������Զ����·������򲻼���ִ�У������� */
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
            {
                /* ����Ϊ��ȡ download ����ʧ�ܵ�״̬ */
                _fw_update_info.status = BOOT_STATUS_READ_PART_ERR;
                _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                return;
            }
        #endif
            
            /* У�� APP �̼� */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 0);
        #if (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
             USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
            /* ���� APP �̼������Զ����·���������ת����� download �����̼� */
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                goto __check_download;
        #else
            /* δ���� APP �̼������Զ����·������򲻼���ִ�У������� */
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
            {
                /* ����Ϊ APP �̼�У��ʧ�ܵ�״̬ */
                _fw_update_info.status = BOOT_STATUS_APP_VERIFY_ERR;
                _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                return;
            }
        #endif
            
            /* ����һ����˵�� APP У��ɹ� */
            goto __jump_to_app;
        }
        #if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
        /* ���� APP �̼������Զ����·��� �� download �����޹̼� */
        else
        {
            /* factory �����й̼� */
            if ((fw_sta & 0x01) == 0x01)
            {
                /* ���̼���ͷ */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_ReadFirmwareHead(FACTORY_PART_NAME);

            #if (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
                 USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
                /* ���� APP �̼������Զ����·���������ת����� factory �����̼� */
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                    goto __check_factory;
            #else
                /* δ���� APP �̼������Զ����·������򲻼���ִ�У������� */
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                {
                    /* ����Ϊ��ȡ factory ����ʧ�ܵ�״̬ */
                    _fw_update_info.status = BOOT_STATUS_READ_PART_ERR;
                    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                    return;
                }
            #endif
                
                /* У�� APP �̼� */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 0);

            #if (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
                 USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
                /* ���� APP �̼������Զ����·���������ת����� factory �����̼� */
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                    goto __check_factory;
            #else
                /* δ���� APP �̼������Զ����·������򲻼���ִ�У������� */
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                {
                    /* ����Ϊ APP �̼�У��ʧ�ܵ�״̬ */
                    _fw_update_info.status = BOOT_STATUS_APP_VERIFY_ERR;
                    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                    return;
                }
            #endif
                
                /* ����һ����˵�� APP У��ɹ� */
                goto __jump_to_app;
            }
            /* ���� APP �̼���� �� download �����޹̼� �� factory �����޹̼� */
            else
            {
            #if (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
                goto __jump_to_app;
            #else
                /* ����Ϊ APP �̼�Ϊ�޷�����״̬ */
                _fw_update_info.status = BOOT_STATUS_APP_CAN_NOT_VERIFY;
                _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                return;
            #endif
            }
        }
        #else
            /* �������������� */
            /* ���� APP �̼���� �� download �����޹̼� */
            #if (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
            goto __jump_to_app;
            #else
            /* ����Ϊ APP �̼�Ϊ�޷�����״̬ */
            _fw_update_info.status = BOOT_STATUS_APP_CAN_NOT_VERIFY;
            _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
            return;
            #endif
        #endif  /* #if (USING_PART_PROJECT == TRIPLE_PART_PROJECT) */
    #else
        /* APP �����й̼� �� δ���� APP �̼���� */
        goto __jump_to_app;
    #endif  /* #if (USING_APP_SAFETY_CHECK_PROJECT) */
    }
    #if (USING_AUTO_UPDATE_PROJECT == DO_NOT_AUTO_UPDATE)
    /* APP �����޹̼� �� δ�����Զ����� */
    else
    {
        /* ����Ϊ APP �̼��쳣��״̬ */
        _fw_update_info.status = BOOT_STATUS_NO_APP;
        _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    }
    #endif
    
__check_download:
    /* ִ�е��˴�����ζ���Ѿ��������������
     * APP û�й̼� �� У�鲻ͨ�� �� APP ���Զ�����ʧ��
     */
    /* ��δִ�й��Զ����� */
    if (is_try_auto_update == 0)
    {
    #if (USING_AUTO_UPDATE_PROJECT && \
         (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
          USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY))
        /* �����Զ����� �� download �����й̼� */
        if ((fw_sta & 0x02) == 0x02)
        {
            if (_Firmware_AutoUpdate(DOWNLOAD_PART_NAME))
                goto __check_factory;
            else
                goto __jump_to_app;
        }
        #if (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
        /* �����Զ����� �� download ����û�й̼� */
        else
            goto __check_factory;
        #endif
    #else
        /* δ�����Զ����� */
        /* ����Ϊȱʧ APP �̼���״̬ */
        _fw_update_info.status = BOOT_STATUS_NO_APP;
        _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    #endif
    }
    
__check_factory:
    /* ִ�е��˴�����ζ���Ѿ��������������
     * 1. APP û�й̼� �� У�鲻ͨ�� �� APP ���Զ�����ʧ��
     * 2. download ����û�п��õĹ̼� �� download �����̼��������� APP ʧ��
     */
    #if (ENABLE_USE_FACTORY_FIRWARE)
    /* ����ʹ�� factory �Ĺ̼� */
    /* factory �����й̼� */
    if ((fw_sta & 0x01) == 0x01)
    {
        if (_Firmware_AutoUpdate(FACTORY_PART_NAME) == FM_ERR_OK)
            goto __jump_to_app;
    }
    #endif

    #if (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
    /* ���е����˵�� download ������ factory �����޿��õĹ̼� */
    /* �� APP �����й̼�������ת */
    if ((fw_sta & 0x04) == 0x04)
        goto __jump_to_app; 
    else
    {
        /* ����Ϊ���������͹̼���״̬ */
        _fw_update_info.status = BOOT_STATUS_AUTO_UPDATE_FAILED;
        _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    }
    #else
    /* ����Ϊ���������͹̼���״̬ */
    _fw_update_info.status = BOOT_STATUS_AUTO_UPDATE_FAILED;
    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
    return;
    #endif
    
__jump_to_app:
    _Bootloader_JumpToAPP(); 
}


/**
 * @brief  ���������Ƿ��п��õĹ̼�
 * @note   bit2: APP ������bit1: download ������bit0: factory ����
 * @retval 0: �޹̼���1: �й̼�
 */
static uint8_t _Firmware_Check(void)
{
    uint8_t fw_sta = 0;

    fw_sta  = (!FM_CheckFirmwareIntegrity(APP_ADDRESS) << 2);
    fw_sta |= (!FM_CheckFirmwareIntegrity(DOWNLOAD_ADDRESS) << 1);
    if (FACTORY_ADDRESS)
        fw_sta |= !FM_CheckFirmwareIntegrity(FACTORY_ADDRESS);
    
    BSP_Printf("%s: %d\r\n", __func__, fw_sta);

    return fw_sta;
}
#endif  /* #if (USING_PART_PROJECT > ONE_PART_PROJECT) */


/**
 * @brief  ��ת��APP�Ĵ�����
 * @note   ��ѡ��ֱ����ת�� APP ������Ҫע��ʹ�õ�������Ҫ���� deinit �������� APP ��������Ӱ��
 * @retval None
 */
static void _Bootloader_JumpToAPP(void)
{
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE)
    /* �ڶ��ν��룬��ת�� APP */
    if (update_flag == BOOTLOADER_RESET_MAGIC_WORD)
    {
        typedef void(*APP_MAIN_FUNC)(void);
        APP_MAIN_FUNC  APP_Main; 
        
        _stack_addr    = *(volatile uint32_t *)APP_ADDRESS;
        _reset_handler = *(volatile uint32_t *)(APP_ADDRESS + 4);
        
        /* �ر�ȫ���ж� */
        BSP_INT_DIS();
        
        /* ����̼����±�־λ */
        update_flag = 0;
        
        /* ��������ջָ�� */
        __set_MSP(_stack_addr);
        
        /* �� RTOS ���̣�����������Ҫ������Ϊ��Ȩ��ģʽ��ʹ�� MSP ָ�� */
        __set_CONTROL(0);
        
        /* �����ж������� */
        SCB->VTOR = APP_ADDRESS;

        /* ��ת�� APP ���׵�ַ�� MSP ����ַ +4 �Ǹ�λ�жϷ�������ַ */
        APP_Main = (APP_MAIN_FUNC)_reset_handler;
        APP_Main();
    }
    /* �״ν������ñ�־λ����λ */
    else
    {
        /* ���ñ�־λ */
        update_flag = BOOTLOADER_RESET_MAGIC_WORD;
        
        /* ��λ���½��� bootloader */
        HAL_NVIC_SystemReset();
    }

#else
    typedef void(*APP_MAIN_FUNC)(void);
    APP_MAIN_FUNC  APP_Main; 
    
    _stack_addr    = *(volatile uint32_t *)APP_ADDRESS;
    _reset_handler = *(volatile uint32_t *)(APP_ADDRESS + 4);

    /* ��������ʱ�ӵ�Ĭ��״̬��ʹ��HSIʱ�� */
    HAL_RCC_DeInit();
    
    /* �ر�ȫ���ж� */
    BSP_INT_DIS();

    /* �رյδ�ʱ������λ��Ĭ��ֵ */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    /* �ر������жϣ���������жϹ����־ */
    for (uint8_t i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    /* ���� deinit */
    HAL_DeInit();
    HAL_UART_DeInit(&huart1);
    HAL_DMA_DeInit(&hdma_usart1_rx);
    
    /* ��������ջָ�� */
    __set_MSP(_stack_addr);

    /* �� RTOS ���̣�����������Ҫ������Ϊ��Ȩ��ģʽ��ʹ�� MSP ָ�� */
    __set_CONTROL(0);
    
    /* �����ж������� */
    SCB->VTOR = APP_ADDRESS;

    /* ��ת�� APP ���׵�ַ�� MSP ����ַ +4 �Ǹ�λ�жϷ�������ַ */
    APP_Main = (APP_MAIN_FUNC)_reset_handler;
    APP_Main();

#endif

    /* ��λ����ת�ɹ��Ļ�������ִ�е����� */
    while (1)
    {
        
    }
}


#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
/**
 * @brief  �������¼�����
 * @note   
 * @param[in]  id: ������ ID
 * @param[in]  event: �������¼�
 * @retval None
 */
static void _Key_EventCallback(uint8_t id, KEY_EVENT  event)
{
    BSP_Printf("[ key ] You just press the button[%d], event: %d\r\n\r\n", id, event);
    
    if (event == KEY_LONG_PRESS)
    {
        if (_fw_update_info.exe_flow == EXE_FLOW_NOTHING
        ||  _fw_update_info.exe_flow == EXE_FLOW_NEED_HOST_SEND_FIRMWARE
        ||  _fw_update_info.exe_flow == EXE_FLOW_FIND_RUNNING_FIRMWARE
        ||  _fw_update_info.exe_flow == EXE_FLOW_JUMP_TO_APP
        ||  _fw_update_info.exe_flow == EXE_FLOW_FAILED)
        {
            _Bootloader_SetExeFlow(EXE_FLOW_RECOVERY);
        }
    }
}


/**
 * @brief  ��ȡ�����ĵ�ƽֵ
 * @note   
 * @retval ��ƽֵ
 */
static uint8_t _Key_GetLevel(void)
{
    return HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin);
}


/**
 * @brief  ɨ�谴��������
 * @note   
 * @param[in]  user_data: �û�����
 * @retval None
 */
static void _Timer_ScanKeyCallback(void *user_data)
{
    BSP_Key_Handler(2);
}
#endif  /* #if (ENABLE_FACTORY_FIRMWARE_BUTTON) */


/**
 * @brief  LED ��˸��������
 * @note   
 * @param[in]  user_data: �û�����
 * @retval None
 */
static void _Timer_LedFlashCallback(void *user_data)
{
    HAL_GPIO_TogglePin(SYS_LED_GPIO_Port, SYS_LED_Pin);
}


/**
 * @brief  �ȴ��������ݳ�ʱ�ص�����
 * @note   
 * @param[in]  user_data: �û�����
 * @retval None
 */
static void _Timer_HostDataTimeoutCallback(void *user_data)
{
    _Bootloader_SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
    BSP_Printf("%s: timeout!!!\r\n", __func__);
}


/**
 * @brief  ���ó����ִ������
 * @note   
 * @param[in]  flow: ִ������
 * @retval None
 */
static inline void _Bootloader_SetExeFlow(BOOT_EXE_FLOW flow)
{
    _fw_update_info.exe_flow = flow;
}


/**
 * @brief  ���ڷ������ݵĽӿ�
 * @note   
 * @param[in]  data: Ҫ���͵�����
 * @param[in]  len: Ҫ���͵����ݳ���
 * @param[in]  timeout: ��ʱʱ�䣬��λ ms
 * @retval None
 */
static void _UART_SendData(uint8_t *data, uint16_t len, uint32_t timeout)
{
    DT_Send(&_data_if, data, len);
}


/**
 * @brief  ����������ʱ�Ĵ�����
 * @note   
 * @param[in]  func: �����������ں���
 * @param[in]  line: ������������ c �ļ�������
 * @retval None
 */
void Assert_Failed(uint8_t *func, uint32_t line)
{
    BSP_Printf("\r\n[ error ]: %s(%d)\r\n\r\n", func, line);
    while (1);
}





