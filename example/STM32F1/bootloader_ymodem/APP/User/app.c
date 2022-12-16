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
 * 2022-12-04     Dino         1. 增加一个记录版本的机制，可选写在 APP 分区
 *                             2. 修复 AC6 -O0 优化下，无法正常运行 APP 的问题
 *                             3. 增加长按按键恢复出厂固件的选项
 *                             4. 将 flash 的擦除粒度配置移至 user_config.h 
 *                             5. 增加 Main_Satrt() 函数
 *                             6. 增加是否判断固件包超过分区大小的选项
 * 2022-12-07     Dino         增加对 STM32L4 的支持
 */


/* Includes ------------------------------------------------------------------*/
#include "app.h"


/* Private variables ---------------------------------------------------------*/
/* 固件更新的标志位，该标志位不能被清零 */
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

static uint8_t  _is_firmware_head;                          /* 是否为固件包头的标志位 */
static uint16_t _dev_rx_len;                                /* 指示缓存区接收到的数据量，单位 byte */
static uint8_t  _dev_rx_buff[PP_MSG_BUFF_SIZE + 16];        /* 设备底层数据接收缓存区 */
static uint8_t  _fw_head_data[FPK_HEAD_SIZE];               /* 用于暂存固件包头 */
static uint8_t  _fw_sub_pkg_data[PP_FIRMWARE_PKG_SIZE];     /* 暂存固件包体 */
static uint16_t _fw_sub_pkg_len;                            /* 记录固件包体大小，包含两个字节的数据长度 */
static uint32_t _stack_addr;                                /* APP 栈顶地址 */
static uint32_t _reset_handler;                             /* APP reset handler 地址 */

static struct BSP_TIMER _timer_led;                         /* LED 的闪烁 timer */
static struct BSP_TIMER _timer_wait_data;                   /* 检测主机数据发送超时的 timer */
#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
static struct BSP_TIMER _timer_key;                         /* 用于按键扫描的 timer */
static struct BSP_KEY   _recovery_key;                      /* 用于恢复出厂固件的按键 */  
#endif

static struct DATA_TRANSFER         _data_if;               /* 数据传输的接口 */
static struct FIRMWARE_UPDATE_INFO  _fw_update_info;        /* 固件更新的信息记录 */


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
 * @brief  进入 main 函数后需要立即执行的代码
 * @note   为了最大程度的减少对 APP 的影响，更建议放在 SystemInit() 函数第一行执行
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
 * @brief  外设初始化前的一些处理
 * @note   执行到此处，内核时钟已初始化
 * @retval None
 */
void System_Init(void)
{
    BSP_Board_Init();

#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE)
    /* 检查固件更新标志位 */
    if (update_flag != FIRMWARE_UPDATE_MAGIC_WORD)
    {
        if (update_flag == FIRMWARE_RECOVERY_MAGIC_WORD)
            _Bootloader_SetExeFlow(EXE_FLOW_RECOVERY);
        else
            _Bootloader_SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
    }
#endif

    /* TODO: 此处的 ASSERT 最佳实现方式应为编译阶段即可报错，但尚未找到方法 */
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
 * @brief  应用初始化
 * @note   此时外设已经初始化完毕
 * @retval None
 */
void APP_Init(void)
{
#if (ENABLE_DEBUG_PRINT)
    #if (EANBLE_PRINTF_USING_RTT)
    /* 配置通道 0 ，下行配置 */
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
    
    /* BSP 初始化 */
    BSP_Timer_Init( &_timer_led, 
                    _Timer_LedFlashCallback, 
                    100,
                    TIMER_RUN_FOREVER, 
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_led);

#if (WAIT_HOST_DATA_MAX_TIME)
    /* 初始状态为等待主机是否下发数据的定时器 */
    BSP_Timer_Init( &_timer_wait_data,
                    _Timer_HostDataTimeoutCallback,
                    WAIT_HOST_DATA_MAX_TIME,
                    TIMER_RUN_FOREVER,
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_wait_data);
#endif

    /* 软件初始化 */
    DT_Init(&_data_if, BSP_UART1, _dev_rx_buff, &_dev_rx_len, PP_MSG_BUFF_SIZE + 16);
    PP_Init(_UART_SendData, NULL, _Bootloader_ProtocolDataHandle, _Bootloader_SetReplyInfo);
    FM_Init();
}


/**
 * @brief  应用开始运行
 * @note   APP_Running 本身就在一个 while 循环内
 * @retval None
 */
void APP_Running(void)
{
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
    /* 记录当前操作的固件分区 */
    char *part_name = NULL;
#endif

    while (1)
    {
        /* 主机数据处理函数 */
        _Host_Data_Handler();

        /* 应用执行流程状态机 */
        switch (_fw_update_info.exe_flow)
        {
        #if (USING_PART_PROJECT > ONE_PART_PROJECT)
            /* 无须更新固件 或 更新固件过程中等待主机指令超时 或 更新失败时 的处理逻辑 */
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
            /* 暂存和校验固件包头的流程，此时，已进入固件更新的开始阶段 */
            case EXE_FLOW_VERIFY_FIRMWARE_HEAD:
            {
                _fw_update_info.cmd_exe_err_code = PP_ERR_OK;
                
                /* 取出固件包头中的分区名 */
                struct FPK_HEAD *p_fpk_head = (struct FPK_HEAD *)_fw_head_data;
                part_name = p_fpk_head->part_name;

            #if (USING_PART_PROJECT > ONE_PART_PROJECT)
                #if (ENABLE_AUTO_CORRECT_PART)
                /* 自动将固件包指定的 APP 分区修正为 download 分区 */
                if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                    part_name = DOWNLOAD_PART_NAME;
                #else
                /* 固件包指定的存放分区合法性检查，多分区方案时，不能指定放在 APP 分区 */
                if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                {
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_CAN_NOT_PLACE_IN_APP;
                    break;
                }
                #endif
            #endif
                
                /* 检测固件包是否超过了分区大小 */
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
                    /* 双分区的方案不可能存在 factory 分区 */
                    _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_DOES_NOT_EXIST_FACTORY;
                    break;
                    #endif
                }
                #endif
            #endif  /* #if (ENABLE_CHECK_FIRMWARE_SIZE) */
                
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                /* 因单分区的方案无视固件包指定的分区，因此不需要判断分区存不存在 */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_StorageFirmwareHead(APP_PART_NAME, _fw_head_data);
            #else  
                /* 暂存固件包头 */
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
            /* 单分区: 擦除 APP 固件。多分区: 将需要放入固件的分区擦除 */
            case EXE_FLOW_ERASE_OLD_FIRMWARE:
            {
                /* 判断分区是否为空 */
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
                
                /* 分区不为空时才擦除 */
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
            /* 将固件包头写入 flash */
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
            /* 校验收到的固件分包的数据正确性。说明主机下发了固件分包 */
            case EXE_FLOW_VERIFY_FIRMWARE_PKG:
            {
                /* 因 YModem 协议已进行校验，因此这个流程跳过即可，直接进入下一步 */
                _Bootloader_SetExeFlow(EXE_FLOW_WRITE_NEW_FIRMWARE);
                break;
            }
            /* 将固件分包写入分区中 */
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
            /* 固件已接收完毕。此时将分区首地址的 4 个字节写入 */
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
            /* 校验下载好的固件包体数据的正确性。单分区: 执行成功则 100% 完成，多分区: 执行成功则完成 25% */
            /* 意外更新时，也从这个流程开始 */
            case EXE_FLOW_ACCIDENT_UPDATE:
            case EXE_FLOW_VERIFY_FIRMWARE:
            {
                _fw_update_info.step = STEP_VERIFY_FIRMWARE;
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.step = STEP_ERASE_DOWNLOAD;
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 0);
            #else
                /* 取出固件包头中的分区名 */
                part_name = FM_GetPartName();
                
                #if (ENABLE_AUTO_CORRECT_PART)
                    /* 自动将固件包指定的 APP 分区修正为 download 分区 */
                    if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                        part_name = DOWNLOAD_PART_NAME;
                #else
                    /* 固件包指定的存放分区合法性检查，多分区方案时，不能指定放在 APP 分区 */
                    if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                    {
                        _Bootloader_SetExeFlow(EXE_FLOW_FAILED);
                        _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                        _fw_update_info.cmd_exe_err_code = PP_ERR_CAN_NOT_PLACE_IN_APP;
                        break;
                    }
                #endif
                /* 校验固件 */
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
            /* 擦除 APP 分区的固件，准备写入新的固件 */
            case EXE_FLOW_ERASE_APP:
            {
                _fw_update_info.step = STEP_ERASE_APP;
                /* 判断 APP 分区是否为空 */
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
                
                /* 为空时才擦除分区 */
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
            /* 将新的固件写入 APP 分区中，边解密边写入 */
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
            /* 校验 APP 分区的固件数据正确性 */
            case EXE_FLOW_VERIFY_APP:
            {
                _fw_update_info.step = STEP_VERIFY_APP;
                /* 因此时 APP 分区的首地址数据仍未写入，因此需要让 FM_VerifyFirmware 自动填充以进行校验 */
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
            /* 校验 APP 分区固件通过后，将剩余数据写入 */
            case EXE_FLOW_UPDATE_TO_APP_DONE:
            {
            #if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT ||   \
                 USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
                /* 更新记录的固件版本 */
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
            /* 擦除暂存固件的分区。如果是 factory 分区，则不需要擦除 */
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
            /* 跳转至 APP */
            case EXE_FLOW_JUMP_TO_APP:
            {
                if (FM_CheckFirmwareIntegrity(APP_ADDRESS) == FM_ERR_OK)
                    _Bootloader_JumpToAPP();
                else
                    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE);
                break;
            }
            #endif  /* #if (USING_PART_PROJECT > ONE_PART_PROJECT) */
            /* 主机下发的恢复出厂固件的指令 或 APP 传递了需要恢复出厂固件的信息 */
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
                /* 检测 factory 是否有可用的固件 */
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
            /* 固件更新失败的处理逻辑 */
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
 * @brief  固件写入时的回调函数
 * @note   
 * @param[in]  progress: 进度（0-10000）
 * @retval None
 */
void Firmware_OperateCallback(uint16_t progress)
{
    /* 更新固件更新进度百分比 */
    _fw_update_info.total_progress = (_fw_update_info.step * 20) + (progress / 500);

    /* 处理主机的数据，防止阻塞 */
    _Host_Data_Handler();
    BSP_Printf("%s: progress: %d\r\n", __func__, _fw_update_info.total_progress);
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  数据接收处理函数
 * @note   
 * @retval None
 */
static void _Host_Data_Handler(void)
{
    /* 轮询方式，防止应用阻塞 */
    if (DT_PollingReceive(&_data_if) == DT_RESULT_RECV_FRAME_DATA)
    {
    #if (WAIT_HOST_DATA_MAX_TIME)
        BSP_Timer_Restart(&_timer_wait_data);
    #endif

        /* 调用协议析构层的处理函数并将接收到的一帧数据导入 */
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
 * @brief  负责接收协议解析出来的数据
 * @note   通过协议的指令包，设置程序的执行流程
 * @param[in]  cmd: 接收到的指令
 * @param[in]  data: 接收到的参数
 * @param[in]  data_len: 接收到的参数长度，单位 byte
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
            /* 重置 timer 为等待主机下发固件包的定时器 */
            BSP_Timer_Pause(&_timer_wait_data);
            BSP_Timer_Init( &_timer_wait_data, 
                            _Timer_HostDataTimeoutCallback, 
                            (60 * 1000), 
                            TIMER_RUN_FOREVER, 
                            TIMER_TYPE_HARDWARE);
            BSP_Timer_Start(&_timer_wait_data);
        #endif
            
            /* 最后一个 SOH 空数据帧 */
            if (eot_flag && cmd == PP_CMD_SOH)
            {
                eot_flag = 0;
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                _Bootloader_SetExeFlow(EXE_FLOW_UPDATE_FIRMWARE);
                break;
            }

            /* 正常数据帧的处理 */
            _fw_update_info.status = BOOT_STATUS_ENTER_UPDATE_MODE;

            /* 拷贝数据 */
            _fw_sub_pkg_len = data_len;
            memcpy(&_fw_sub_pkg_data[0], &data[0], _fw_sub_pkg_len);
            BSP_Printf("_fw_sub_pkg_len: %d\r\n", _fw_sub_pkg_len);

            /* 第一个 SOH 数据帧，包含文件名和文件大小等信息 */
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
            
            /* 固件包头，单独处理 */
            if (_is_firmware_head == 0)
            {
                _is_firmware_head = 1;
                memcpy(&_fw_head_data[0], &data[0], FPK_HEAD_SIZE);
                _Bootloader_SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE_HEAD);
            }
            /* 固件包体 */
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
 * @brief  回复指令的执行情况，协议会自动组包
 * @note   根据当前执行的指令和执行情况，发送对应数据
 * @param[in]   cmd: 正在执行的指令
 * @param[out]  cmd_exe_result: 指令执行结果
 * @param[out]  data: 需要响应的数据
 * @param[out]  data_len: 需要响应的数据长度，单位 byte
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
 * @brief  自动更新固件的处理函数
 * @note   
 * @param[in]  part_name: 固件所在的分区名
 * @retval FM_ERR_CODE
 */
static FM_ERR_CODE  _Firmware_AutoUpdate(const char *part_name)
{
    static uint8_t value = 0;
    FM_ERR_CODE  result = FM_ERR_OK;
    
    /* 设置为正在意外更新固件的状态 */
    _fw_update_info.status = BOOT_STATUS_ACCIDENT_UPDATE;
    /* 禁止接收主机的指令 */
    PP_Config(PP_CONFIG_ENABLE_RECV_CMD, &value);
    
    /* 读取固件包头 */
    result = FM_ReadFirmwareHead(part_name);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
        return result;
    }
    
    /* 校验固件正确性 */
    result = FM_VerifyFirmware(part_name, FM_GetPackageCRC32(), 0);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
        return result;
    }
    
    /* 检测 APP 分区是否为空，不为空时擦除分区 */
    result = FM_IsEmpty(APP_PART_NAME);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: app is not empty\r\n", __func__);
        result = FM_EraseFirmware(APP_PART_NAME);
    }
    
    /* 将固件更新至 APP 分区 */
    result = FM_UpdateToAPP(part_name);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
        return result;
    }
    
    /* 校验 APP 分区的固件 */
    result = FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 1);
    if (result != FM_ERR_OK)
    {
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
        return result;
    }
    
#if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT ||   \
     USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
    /* 记录固件版本在 flash 的方案，需更新记录的固件版本 */
    result = FM_UpdateFirmwareVersion(part_name);
#endif

    /* 将剩余的数据写入 APP 分区 */
    result = FM_WriteFirmwareDone(APP_PART_NAME);
    
#if (USING_AUTO_UPDATE_PROJECT == ERASE_DOWNLOAD_PART_PROJECT)
    /* 擦除 download 分区的方案，需直接擦除 */
    result = FM_EraseFirmware(part_name);
#endif
    
    if (result != FM_ERR_OK)
        BSP_Printf("%s: err: %d\r\n", __func__, __LINE__);
    
    return result;
}


/**
 * @brief  固件的检查和处理函数
 * @note   上电时检查 或 更新失败时检查，执行的优先级低于主机指令，因此需确认主机不需要更新固件时才可执行
 * @retval None
 */
static void _Firmware_CheckAndHandle(void)
{
    uint8_t is_try_auto_update = 0;
    uint8_t fw_sta = _Firmware_Check();
    
    /* 本函数的处理逻辑较复杂。为了提高执行效率，减少资源占用，使用了 goto 。阅读时建议结合《固件检测和处理机制》文档查看 */

    /* 没有任何可用的固件 */
    if (fw_sta == 0)
    {
        _fw_update_info.status = BOOT_STATUS_NO_FIRMWARE;
        _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    }
    
#if (USING_AUTO_UPDATE_PROJECT)
    /* download 分区有固件 */
    if ((fw_sta & 0x02) == 0x02)
    {
    #if (USING_AUTO_UPDATE_PROJECT == ERASE_DOWNLOAD_PART_PROJECT)
        /* 本方案是检测到 download 分区有可用固件则开始自动更新 */
        if (_Firmware_AutoUpdate(DOWNLOAD_PART_NAME))
        {
            is_try_auto_update = 1;
            goto __check_app;
        }
        else
            goto __jump_to_app;
    #elif (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT || \
           USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
        /* 本方案是检测 download 分区固件包头的版本信息，以判断是否需要自动更新 */
        /* 读固件包头 */
        _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_ReadFirmwareHead(DOWNLOAD_PART_NAME);
        if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
            goto __check_app;
        
        /* 需要自动更新 */
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
    /* APP 分区有固件 */
    if ((fw_sta & 0x04) == 0x04)
    {
    #if (USING_APP_SAFETY_CHECK_PROJECT)
        /* 启用 APP 固件检查 且 download 分区有固件 */
        if ((fw_sta & 0x02) == 0x02)
        {
            /* 读固件包头 */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_ReadFirmwareHead(DOWNLOAD_PART_NAME);

        #if (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
             USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
            /* 启用 APP 固件检查的自动更新方案，则跳转至检查 download 分区固件 */
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                goto __check_download;
        #else
            /* 未启用 APP 固件检查的自动更新方案，则不继续执行，报错即可 */
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
            {
                /* 设置为读取 download 分区失败的状态 */
                _fw_update_info.status = BOOT_STATUS_READ_PART_ERR;
                _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                return;
            }
        #endif
            
            /* 校验 APP 固件 */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 0);
        #if (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
             USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
            /* 启用 APP 固件检查的自动更新方案，则跳转至检查 download 分区固件 */
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                goto __check_download;
        #else
            /* 未启用 APP 固件检查的自动更新方案，则不继续执行，报错即可 */
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
            {
                /* 设置为 APP 固件校验失败的状态 */
                _fw_update_info.status = BOOT_STATUS_APP_VERIFY_ERR;
                _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                return;
            }
        #endif
            
            /* 到这一步，说明 APP 校验成功 */
            goto __jump_to_app;
        }
        #if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
        /* 启用 APP 固件检查的自动更新方案 且 download 分区无固件 */
        else
        {
            /* factory 分区有固件 */
            if ((fw_sta & 0x01) == 0x01)
            {
                /* 读固件包头 */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_ReadFirmwareHead(FACTORY_PART_NAME);

            #if (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
                 USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
                /* 启用 APP 固件检查的自动更新方案，则跳转至检查 factory 分区固件 */
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                    goto __check_factory;
            #else
                /* 未启用 APP 固件检查的自动更新方案，则不继续执行，报错即可 */
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                {
                    /* 设置为读取 factory 分区失败的状态 */
                    _fw_update_info.status = BOOT_STATUS_READ_PART_ERR;
                    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                    return;
                }
            #endif
                
                /* 校验 APP 固件 */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), 0);

            #if (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
                 USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
                /* 启用 APP 固件检查的自动更新方案，则跳转至检查 factory 分区固件 */
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                    goto __check_factory;
            #else
                /* 未启用 APP 固件检查的自动更新方案，则不继续执行，报错即可 */
                if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
                {
                    /* 设置为 APP 固件校验失败的状态 */
                    _fw_update_info.status = BOOT_STATUS_APP_VERIFY_ERR;
                    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                    return;
                }
            #endif
                
                /* 到这一步，说明 APP 校验成功 */
                goto __jump_to_app;
            }
            /* 启用 APP 固件检查 且 download 分区无固件 且 factory 分区无固件 */
            else
            {
            #if (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
                goto __jump_to_app;
            #else
                /* 设置为 APP 固件为无法检查的状态 */
                _fw_update_info.status = BOOT_STATUS_APP_CAN_NOT_VERIFY;
                _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                return;
            #endif
            }
        }
        #else
            /* 不是三分区方案 */
            /* 启用 APP 固件检查 且 download 分区无固件 */
            #if (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
            goto __jump_to_app;
            #else
            /* 设置为 APP 固件为无法检查的状态 */
            _fw_update_info.status = BOOT_STATUS_APP_CAN_NOT_VERIFY;
            _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
            return;
            #endif
        #endif  /* #if (USING_PART_PROJECT == TRIPLE_PART_PROJECT) */
    #else
        /* APP 分区有固件 且 未启用 APP 固件检查 */
        goto __jump_to_app;
    #endif  /* #if (USING_APP_SAFETY_CHECK_PROJECT) */
    }
    #if (USING_AUTO_UPDATE_PROJECT == DO_NOT_AUTO_UPDATE)
    /* APP 分区无固件 且 未启用自动更新 */
    else
    {
        /* 设置为 APP 固件异常的状态 */
        _fw_update_info.status = BOOT_STATUS_NO_APP;
        _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    }
    #endif
    
__check_download:
    /* 执行到此处，意味着已经发生以下情况：
     * APP 没有固件 或 校验不通过 或 APP 的自动更新失败
     */
    /* 还未执行过自动更新 */
    if (is_try_auto_update == 0)
    {
    #if (USING_AUTO_UPDATE_PROJECT && \
         (USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP || \
          USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY))
        /* 启用自动更新 且 download 分区有固件 */
        if ((fw_sta & 0x02) == 0x02)
        {
            if (_Firmware_AutoUpdate(DOWNLOAD_PART_NAME))
                goto __check_factory;
            else
                goto __jump_to_app;
        }
        #if (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
        /* 启用自动更新 但 download 分区没有固件 */
        else
            goto __check_factory;
        #endif
    #else
        /* 未启用自动更新 */
        /* 设置为缺失 APP 固件的状态 */
        _fw_update_info.status = BOOT_STATUS_NO_APP;
        _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    #endif
    }
    
__check_factory:
    /* 执行到此处，意味着已经发生以下情况：
     * 1. APP 没有固件 或 校验不通过 或 APP 的自动更新失败
     * 2. download 分区没有可用的固件 或 download 分区固件包更新至 APP 失败
     */
    #if (ENABLE_USE_FACTORY_FIRWARE)
    /* 可以使用 factory 的固件 */
    /* factory 分区有固件 */
    if ((fw_sta & 0x01) == 0x01)
    {
        if (_Firmware_AutoUpdate(FACTORY_PART_NAME) == FM_ERR_OK)
            goto __jump_to_app;
    }
    #endif

    #if (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY)
    /* 运行到这里，说明 download 分区和 factory 分区无可用的固件 */
    /* 若 APP 分区有固件，则跳转 */
    if ((fw_sta & 0x04) == 0x04)
        goto __jump_to_app; 
    else
    {
        /* 设置为需主机发送固件的状态 */
        _fw_update_info.status = BOOT_STATUS_AUTO_UPDATE_FAILED;
        _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    }
    #else
    /* 设置为需主机发送固件的状态 */
    _fw_update_info.status = BOOT_STATUS_AUTO_UPDATE_FAILED;
    _Bootloader_SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
    return;
    #endif
    
__jump_to_app:
    _Bootloader_JumpToAPP(); 
}


/**
 * @brief  检查各分区是否有可用的固件
 * @note   bit2: APP 分区。bit1: download 分区。bit0: factory 分区
 * @retval 0: 无固件。1: 有固件
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
 * @brief  跳转至APP的处理函数
 * @note   若选择直接跳转至 APP ，则需要注意使用到的外设要进行 deinit ，否则会对 APP 的运行有影响
 * @retval None
 */
static void _Bootloader_JumpToAPP(void)
{
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE)
    /* 第二次进入，跳转至 APP */
    if (update_flag == BOOTLOADER_RESET_MAGIC_WORD)
    {
        typedef void(*APP_MAIN_FUNC)(void);
        APP_MAIN_FUNC  APP_Main; 
        
        _stack_addr    = *(volatile uint32_t *)APP_ADDRESS;
        _reset_handler = *(volatile uint32_t *)(APP_ADDRESS + 4);
        
        /* 关闭全局中断 */
        BSP_INT_DIS();
        
        /* 清除固件更新标志位 */
        update_flag = 0;
        
        /* 设置主堆栈指针 */
        __set_MSP(_stack_addr);
        
        /* 在 RTOS 工程，这条语句很重要，设置为特权级模式，使用 MSP 指针 */
        __set_CONTROL(0);
        
        /* 设置中断向量表 */
        SCB->VTOR = APP_ADDRESS;

        /* 跳转到 APP ，首地址是 MSP ，地址 +4 是复位中断服务程序地址 */
        APP_Main = (APP_MAIN_FUNC)_reset_handler;
        APP_Main();
    }
    /* 首次进入设置标志位并复位 */
    else
    {
        /* 设置标志位 */
        update_flag = BOOTLOADER_RESET_MAGIC_WORD;
        
        /* 复位重新进入 bootloader */
        HAL_NVIC_SystemReset();
    }

#else
    typedef void(*APP_MAIN_FUNC)(void);
    APP_MAIN_FUNC  APP_Main; 
    
    _stack_addr    = *(volatile uint32_t *)APP_ADDRESS;
    _reset_handler = *(volatile uint32_t *)(APP_ADDRESS + 4);

    /* 设置所有时钟到默认状态，使用HSI时钟 */
    HAL_RCC_DeInit();
    
    /* 关闭全局中断 */
    BSP_INT_DIS();

    /* 关闭滴答定时器，复位到默认值 */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    /* 关闭所有中断，清除所有中断挂起标志 */
    for (uint8_t i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    /* 外设 deinit */
    HAL_DeInit();
    HAL_UART_DeInit(&huart1);
    HAL_DMA_DeInit(&hdma_usart1_rx);
    
    /* 设置主堆栈指针 */
    __set_MSP(_stack_addr);

    /* 在 RTOS 工程，这条语句很重要，设置为特权级模式，使用 MSP 指针 */
    __set_CONTROL(0);
    
    /* 设置中断向量表 */
    SCB->VTOR = APP_ADDRESS;

    /* 跳转到 APP ，首地址是 MSP ，地址 +4 是复位中断服务程序地址 */
    APP_Main = (APP_MAIN_FUNC)_reset_handler;
    APP_Main();

#endif

    /* 复位或跳转成功的话，不会执行到这里 */
    while (1)
    {
        
    }
}


#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
/**
 * @brief  按键的事件处理
 * @note   
 * @param[in]  id: 按键的 ID
 * @param[in]  event: 按键的事件
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
 * @brief  读取按键的电平值
 * @note   
 * @retval 电平值
 */
static uint8_t _Key_GetLevel(void)
{
    return HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin);
}


/**
 * @brief  扫描按键的任务
 * @note   
 * @param[in]  user_data: 用户数据
 * @retval None
 */
static void _Timer_ScanKeyCallback(void *user_data)
{
    BSP_Key_Handler(2);
}
#endif  /* #if (ENABLE_FACTORY_FIRMWARE_BUTTON) */


/**
 * @brief  LED 闪烁周期任务
 * @note   
 * @param[in]  user_data: 用户数据
 * @retval None
 */
static void _Timer_LedFlashCallback(void *user_data)
{
    HAL_GPIO_TogglePin(SYS_LED_GPIO_Port, SYS_LED_Pin);
}


/**
 * @brief  等待主机数据超时回调函数
 * @note   
 * @param[in]  user_data: 用户数据
 * @retval None
 */
static void _Timer_HostDataTimeoutCallback(void *user_data)
{
    _Bootloader_SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
    BSP_Printf("%s: timeout!!!\r\n", __func__);
}


/**
 * @brief  设置程序的执行流程
 * @note   
 * @param[in]  flow: 执行流程
 * @retval None
 */
static inline void _Bootloader_SetExeFlow(BOOT_EXE_FLOW flow)
{
    _fw_update_info.exe_flow = flow;
}


/**
 * @brief  用于发送数据的接口
 * @note   
 * @param[in]  data: 要发送的数据
 * @param[in]  len: 要发送的数据长度
 * @param[in]  timeout: 超时时间，单位 ms
 * @retval None
 */
static void _UART_SendData(uint8_t *data, uint16_t len, uint32_t timeout)
{
    DT_Send(&_data_if, data, len);
}


/**
 * @brief  参数检查错误时的处理函数
 * @note   
 * @param[in]  func: 错误发生的所在函数
 * @param[in]  line: 错误发生的所在 c 文件的行数
 * @retval None
 */
void Assert_Failed(uint8_t *func, uint32_t line)
{
    BSP_Printf("\r\n[ error ]: %s(%d)\r\n\r\n", func, line);
    while (1);
}





