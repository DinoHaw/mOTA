/**
 * \file            bootloader.c
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
 *                                      3. 删除 _fw_update_info.status
 */

/* Includes ------------------------------------------------------------------*/
#include "bootloader.h"


/* Private variables ---------------------------------------------------------*/
/* 固件更新的标志位，该标志位不能被清零 */
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE) \
||  defined(USING_CUSTOM_UPDATE_FLAG)

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

#if (USING_PART_PROJECT > ONE_PART_PROJECT)
static const char *_part_name;                          /* 记录当前操作的固件分区 */
#endif
static uint8_t *_firmware_data;
static uint32_t _firmware_data_len;
static struct FIRMWARE_UPDATE_INFO  _fw_update_info;    /* 固件更新的信息记录 */


/* Extern function prototypes ------------------------------------------------*/
extern void         Bootloader_Port_Init            (void);
extern void         Bootloader_Port_HostDataProcess (void);
extern void         Bootloader_Port_Reset           (void);
extern void         Bootloader_Port_SystemReset     (void);
extern void         Bootloader_Port_JumpToAPP       (void);


/* Private function prototypes -----------------------------------------------*/
static void         _SetExeFlow                 (BOOT_EXE_FLOW flow);
static void         _JumpToAPP                  (void);
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
static uint8_t      _Firmware_Check             (void);
static void         _Firmware_CheckAndHandle    (void);
static FM_ERR_CODE  _Firmware_AutoUpdate        (const char *part_name);
#endif
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE) \
||  defined(USING_CUSTOM_UPDATE_FLAG)
/* WEAK 函数 */
uint64_t            Bootloader_GetUpdateFlag    (void);
void                Bootloader_SetUpdateFlag    (uint64_t flag);
#endif


/* Constructor ---------------------------------------------------------------*/
/**
 * @brief  上电立即执行的代码
 * @note   
 * @retval None
 */
__attribute__((constructor)) 
static void _SystemStart(void)
{
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE) \
||  defined(USING_CUSTOM_UPDATE_FLAG)
    uint64_t flag = Bootloader_GetUpdateFlag();

    /* 检查固件更新标志位 */
    if (flag == BOOTLOADER_RESET_MAGIC_WORD)
        _JumpToAPP();

    if (flag != FIRMWARE_UPDATE_MAGIC_WORD)
    {
        if (flag == FIRMWARE_RECOVERY_MAGIC_WORD)
            _SetExeFlow(EXE_FLOW_RECOVERY);
        else
            _SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
    }
    Bootloader_SetUpdateFlag(0);
#endif
}


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  外设初始化前的一些处理
 * @note   执行到此处，内核时钟已初始化
 * @retval None
 */
void Bootloader_Init(void)
{
    /* 此处的 ASSERT 最佳实现方式应为编译阶段即可报错，但尚未找到好的实现方法 */
    ASSERT(sizeof(AES256_KEY) != 32);
    ASSERT(sizeof(AES256_IV) != 16);

#if (ENABLE_DEBUG_PRINT)
    uint32_t hal_version = HAL_GetHalVersion();

    BSP_Printf("[mOTA] DinoHaw\r\n");
    BSP_Printf("UID: %.8X %.8X %.8X\r\n", HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2());
    BSP_Printf("bootloader Version: V%d.%d\r\n", BOOT_VERSION_MAIN, BOOT_VERSION_SUB);
    BSP_Printf("HAL Version: V%d.%d.%d.%d\r\n", (hal_version >> 24), (uint8_t)(hal_version >> 16), (uint8_t)(hal_version >> 8), (uint8_t)hal_version);
    #if (IS_ENABLE_SPI_FLASH)
    BSP_Printf("FAL Version: V%s\r\n", FAL_SW_VERSION);
    BSP_Printf("SFUD Version: V%s\r\n", SFUD_SW_VERSION);
    #endif
    BSP_Printf("perf_counter version: V%d.%d.%d\r\n", __PERF_COUNTER_VER_MAJOR__, __PERF_COUNTER_VER_MINOR__, __PERF_COUNTER_VER_REVISE__);
#endif

    FM_Init();
    Bootloader_Port_Init();
}


/**
 * @brief  bootloader 核心逻辑
 * @note   应被不间断调用
 * @retval None
 */
void Bootloader_Loop(void)
{
    /* 主机数据处理函数 */
    Bootloader_Port_HostDataProcess();

    /* 应用执行流程状态机 */
    switch (_fw_update_info.exe_flow)
    {
        /* 需要主机下发固件包，对应 YModem 协议，需要 */
        case EXE_FLOW_NEED_HOST_SEND_FIRMWARE:
        {
            PP_Config(PP_CONFIG_RESET, NULL);
            _SetExeFlow(EXE_FLOW_WAIT_FIRMWARE);
            break;
        }
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
                _JumpToAPP();
            else
                _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE);
            break;
        }
    #endif
        /* 暂存和校验固件包头的流程，此时，已进入固件更新的开始阶段 */
        case EXE_FLOW_VERIFY_FIRMWARE_HEAD:
        {
            struct FPK_HEAD *p_fpk_head = (struct FPK_HEAD *)_firmware_data;
            
            _fw_update_info.cmd_exe_err_code = PP_ERR_OK;

        #if (USING_PART_PROJECT > ONE_PART_PROJECT)
            /* 取出固件包头中的分区名 */
            _part_name = p_fpk_head->part_name;
            
            #if (ENABLE_AUTO_CORRECT_PART)
            /* 自动将固件包指定的 APP 分区修正为 download 分区 */
            if (strncmp(_part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                _part_name = DOWNLOAD_PART_NAME;
            #else
            /* 固件包指定的存放分区合法性检查，多分区方案时，不能指定放在 APP 分区 */
            if (strncmp(_part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
            {
                _SetExeFlow(EXE_FLOW_FAILED);
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
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                _fw_update_info.cmd_exe_err_code = PP_ERR_FIRMWARE_OVERSIZE;
                break;
            }
            
            #if (USING_PART_PROJECT == DOUBLE_PART_PROJECT)
            if (p_fpk_head->pkg_size > DOWNLOAD_PART_SIZE)
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                _fw_update_info.cmd_exe_err_code = PP_ERR_FIRMWARE_OVERSIZE;
                break;
            }
            #elif (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
            if (strncmp(_part_name, DOWNLOAD_PART_NAME, MAX_NAME_LEN) == 0)
            {
                if (p_fpk_head->pkg_size > DOWNLOAD_PART_SIZE)
                {
                    _SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_FIRMWARE_OVERSIZE;
                    break;
                }
            }
            else if (strncmp(_part_name, FACTORY_PART_NAME, MAX_NAME_LEN) == 0)
            {
                #if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
                if (p_fpk_head->pkg_size > FACTORY_PART_SIZE)
                {
                    _SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_FIRMWARE_OVERSIZE;
                    break;
                }
                #else
                /* 双分区的方案不可能存在 factory 分区 */
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                _fw_update_info.cmd_exe_err_code = PP_ERR_DOES_NOT_EXIST_FACTORY;
                break;
                #endif
            }
            #endif
        #endif  /* #if (ENABLE_CHECK_FIRMWARE_SIZE) */
            
        #if (USING_PART_PROJECT == ONE_PART_PROJECT)
            /* 因单分区的方案无视固件包指定的分区，因此不需要判断分区存不存在 */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_StorageFirmwareHead(APP_PART_NAME, _firmware_data);
        #else  
            /* 暂存固件包头 */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_StorageFirmwareHead(_part_name, _firmware_data);
        #endif
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _SetExeFlow(EXE_FLOW_ERASE_OLD_FIRMWARE);
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
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
            _part_name = FM_GetPartName();
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_IsEmpty(_part_name);
        #endif
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                _SetExeFlow(EXE_FLOW_ERASE_OLD_FIRMWARE_DONE);
            #else
                _SetExeFlow(EXE_FLOW_WRITE_FIRMWARE_HEAD);
            #endif
                break;
            }
            else if (_fw_update_info.cmd_exe_err_code != FM_ERR_FLASH_NO_EMPTY)
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                break;
            }
            
            /* 分区不为空时才擦除 */
        #if (USING_PART_PROJECT == ONE_PART_PROJECT)
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(APP_PART_NAME);
        #else
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(_part_name);
        #endif
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                _SetExeFlow(EXE_FLOW_ERASE_OLD_FIRMWARE_DONE);
            #else
                _SetExeFlow(EXE_FLOW_WRITE_FIRMWARE_HEAD);
            #endif
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
            }
            break;
        }
        #if (USING_PART_PROJECT > ONE_PART_PROJECT)
        /* 将固件包头写入 flash */
        case EXE_FLOW_WRITE_FIRMWARE_HEAD:
        {
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareSubPackage(
                                                                    _part_name, 
                                                                    _firmware_data, 
                                                                    FPK_HEAD_SIZE);
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _SetExeFlow(EXE_FLOW_WRITE_FIRMWARE_HEAD_DONE);
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
            }
            break;
        }
        #endif
        /* 校验收到的固件分包的数据正确性。说明主机下发了固件分包 */
        case EXE_FLOW_VERIFY_FIRMWARE_PKG:
        {
            /* 因 YModem 协议已进行校验，因此这个流程跳过即可，直接进入下一步 */
            _SetExeFlow(EXE_FLOW_WRITE_NEW_FIRMWARE);
            break;
        }
        /* 将固件分包写入分区中 */
        case EXE_FLOW_WRITE_NEW_FIRMWARE:
        {
        #if (USING_PART_PROJECT == ONE_PART_PROJECT)
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareSubPackage(
                                                                    APP_PART_NAME, 
                                                                    _firmware_data, 
                                                                    _firmware_data_len);
        #else
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareSubPackage(
                                                                    _part_name, 
                                                                    _firmware_data, 
                                                                    _firmware_data_len);
        #endif
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _SetExeFlow(EXE_FLOW_WRITE_NEW_FIRMWARE_DONE);
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
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
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareDone(_part_name);
        #endif
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _fw_update_info.total_progress = 0;
                _SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE);
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
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
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), false);
        #else
            /* 取出固件包头中的分区名 */
            _part_name = FM_GetPartName();
            
            #if (ENABLE_AUTO_CORRECT_PART)
                /* 自动将固件包指定的 APP 分区修正为 download 分区 */
                if (strncmp(_part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                    _part_name = DOWNLOAD_PART_NAME;
            #else
                /* 固件包指定的存放分区合法性检查，多分区方案时，不能指定放在 APP 分区 */
                if (strncmp(_part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
                {
                    _SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result   = PP_RESULT_CANCEL;
                    _fw_update_info.cmd_exe_err_code = PP_ERR_CAN_NOT_PLACE_IN_APP;
                    break;
                }
            #endif
            /* 校验固件 */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(_part_name, FM_GetPackageCRC32(), false);
        #endif
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
            #if (USING_PART_PROJECT == ONE_PART_PROJECT)
                _fw_update_info.total_progress = 100;
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                _SetExeFlow(EXE_FLOW_JUMP_TO_APP);
            #else
                #if (ENABLE_FACTORY_UPDATE_TO_APP)
                _fw_update_info.total_progress = 20;
                _SetExeFlow(EXE_FLOW_ERASE_APP);
                #else
                /* 不允许固件写入 factory 分区后也将固件更新至 APP 分区 */
                /* 未在执行恢复出厂固件 且 操作的是 factory 分区，跳过写入 APP 分区的流程 */
                if (_fw_update_info.is_recovery == false
                &&  strncmp(_part_name, FACTORY_PART_NAME, MAX_NAME_LEN) == 0)
                {
                    _fw_update_info.total_progress = 100;
                    _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                    _SetExeFlow(EXE_FLOW_JUMP_TO_APP);
                }
                else
                {
                    _fw_update_info.total_progress = 20;
                    _SetExeFlow(EXE_FLOW_ERASE_APP);
                }
                #endif
            #endif
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
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
                _SetExeFlow(EXE_FLOW_UPDATE_TO_APP);
                break;
            }
            else if (_fw_update_info.cmd_exe_err_code != FM_ERR_FLASH_NO_EMPTY)
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                break;
            }
            
            /* 为空时才擦除分区 */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(APP_PART_NAME);
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _fw_update_info.total_progress = 40;
                _SetExeFlow(EXE_FLOW_UPDATE_TO_APP);
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
            }
            BSP_Printf("%s: step 2 !!!\r\n", __func__);
            break;
        }
        /* 将新的固件写入 APP 分区中，边解密边写入 */
        case EXE_FLOW_UPDATE_TO_APP:
        {
            _fw_update_info.step = STEP_UPDATE_TO_APP;
            
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_UpdateToAPP(_part_name);   
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _fw_update_info.total_progress = 60;
                _SetExeFlow(EXE_FLOW_VERIFY_APP);
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
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
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), true);
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                break;
            }

            /* 若正在执行恢复出厂固件，则擦除 download 分区固件 */
            if (_fw_update_info.is_recovery)
            {
                /* 判断 download 分区是否为空 */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_IsEmpty(DOWNLOAD_PART_NAME);
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _fw_update_info.total_progress = 80;
                    _SetExeFlow(EXE_FLOW_UPDATE_TO_APP_DONE);
                    break;
                }
                else if (_fw_update_info.cmd_exe_err_code != FM_ERR_FLASH_NO_EMPTY)
                {
                    _SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                    break;
                }
                
                /* 为空时才擦除分区 */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(DOWNLOAD_PART_NAME);
                if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
                {
                    _fw_update_info.total_progress = 80;
                    _SetExeFlow(EXE_FLOW_UPDATE_TO_APP_DONE);
                }
                else
                {
                    _SetExeFlow(EXE_FLOW_FAILED);
                    _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
                }
            }
            else
            {
                _fw_update_info.total_progress = 80;
                _SetExeFlow(EXE_FLOW_UPDATE_TO_APP_DONE);
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
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_UpdateFirmwareVersion(_part_name);
            if (_fw_update_info.cmd_exe_err_code != FM_ERR_OK)
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
            }
        #endif
            
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_WriteFirmwareDone(APP_PART_NAME);
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _SetExeFlow(EXE_FLOW_ERASE_DOWNLOAD);
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
            }
            break;
        }
        /* 擦除暂存固件的分区。如果是 factory 分区，则不需要擦除 */
        case EXE_FLOW_ERASE_DOWNLOAD:
        {
            _fw_update_info.step = STEP_ERASE_DOWNLOAD;

            if (strncmp(_part_name, FACTORY_PART_NAME, MAX_NAME_LEN) == 0)
                _fw_update_info.cmd_exe_err_code = PP_ERR_OK;
            else
            {
            #if (USING_AUTO_UPDATE_PROJECT == ERASE_DOWNLOAD_PART_PROJECT)
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_EraseFirmware(_part_name);
            #else
                _fw_update_info.cmd_exe_err_code = PP_ERR_OK;
            #endif
            }
                
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _fw_update_info.total_progress = 100;
                _fw_update_info.cmd_exe_result = PP_RESULT_OK;
                _SetExeFlow(EXE_FLOW_JUMP_TO_APP);
            }
            else
            {
                _SetExeFlow(EXE_FLOW_FAILED);
                _fw_update_info.cmd_exe_result = PP_RESULT_CANCEL;
            }
            BSP_Printf("%s: step 5 !!!\r\n", __func__);
            break;
        }
        /* 跳转至 APP */
        case EXE_FLOW_JUMP_TO_APP:
        {
            if (FM_CheckFirmwareIntegrity(APP_ADDRESS) == FM_ERR_OK)
                _JumpToAPP();
            else
                _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE);
            break;
        }
        #endif  /* #if (USING_PART_PROJECT > ONE_PART_PROJECT) */
        /* 主机下发的恢复出厂固件的指令 或 APP 传递了需要恢复出厂固件的信息 */
        case EXE_FLOW_RECOVERY:
        {
        #if (USING_PART_PROJECT != TRIPLE_PART_PROJECT)
            #if (FACTORY_NO_FIRMWARE_SOLUTION == JUMP_TO_APP)
                _SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
            #else
                _SetExeFlow(EXE_FLOW_NOTHING);
            #endif
            _fw_update_info.cmd_exe_result   = PP_RESULT_FAILED;
            _fw_update_info.cmd_exe_err_code = PP_ERR_DOES_NOT_EXIST_FACTORY;
        #else
            /* 检测 factory 是否有可用的固件 */
            if ((_Firmware_Check() & 0x01) == 0)
            {
            #if (FACTORY_NO_FIRMWARE_SOLUTION == JUMP_TO_APP)
                _SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
            #else
                _SetExeFlow(EXE_FLOW_NOTHING);
            #endif
                _fw_update_info.cmd_exe_result   = PP_RESULT_FAILED;
                _fw_update_info.cmd_exe_err_code = PP_ERR_NO_FACTORY_FIRMWARE;
                break;
            }

            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_ReadFirmwareHead(FACTORY_PART_NAME);
            if (_fw_update_info.cmd_exe_err_code == FM_ERR_OK)
            {
                _fw_update_info.is_recovery = true;
                _SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE);
            }
            else
            {
            #if (FACTORY_NO_FIRMWARE_SOLUTION == JUMP_TO_APP)
                _SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
            #else
                _SetExeFlow(EXE_FLOW_NOTHING);
            #endif
                _fw_update_info.cmd_exe_result = PP_RESULT_FAILED;
            }
        #endif
            break;
        }
        /* 固件更新失败的处理逻辑 */
        case EXE_FLOW_FAILED:
        {
            Bootloader_Port_Reset();
            _fw_update_info.is_recovery = false;
            _fw_update_info.step        = STEP_VERIFY_FIRMWARE;
            _SetExeFlow(EXE_FLOW_NOTHING);
            break;
        }
        default: break;
    }
}


/**
 * @brief  通知 bootloader 当前设备与上位机所处的通讯阶段
 * @note   1. 本函数不保存数据，传入的 data 在 bootloader 使用前不能被修改 
 *         2. 没有 data 传入 NULL 和 0
 * @param[in]  status: 与上位机所处的通讯阶段
 * @param[in]  data: 数据
 * @param[in]  data_len: 数据长度 byte
 * @retval None
 */
void Bootloader_SetCommStatus(COMM_STATUS status, uint8_t *data, uint16_t data_len)
{
    if (data)
    {
        _firmware_data     = data;
        _firmware_data_len = data_len;
    }

    switch (status)
    {
        case COMM_STATUS_RECV_DATA:
        {
            _fw_update_info.cmd_exe_result = PP_RESULT_PROCESS;
            break;
        }
        case COMM_STATUS_FIRMWARE_HEAD:
        {
            _SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE_HEAD);
            break;
        }
        case COMM_STATUS_FIRMWARE_PKG:
        {
            _SetExeFlow(EXE_FLOW_VERIFY_FIRMWARE_PKG);
            break;
        }
        case COMM_STATUS_FILE_DONE:
        {
            _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            _SetExeFlow(EXE_FLOW_UPDATE_FIRMWARE);
            break;
        }
        case COMM_STATUS_CANCEL:
        {
            _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            _SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
            break;
        }
        case COMM_STATUS_RECV_TIMEOUT:
        {
            _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            _SetExeFlow(EXE_FLOW_FIND_RUNNING_FIRMWARE);
            BSP_Printf("%s: timeout!!!\r\n", __func__);
            break;
        }
        case COMM_STATUS_FILE_INFO:
        case COMM_STATUS_START_UPDATE:
        case COMM_STATUS_UNKNOWN:
        {
            _fw_update_info.cmd_exe_result = PP_RESULT_OK;
            break;
        }
        default: break;
    }
}


/**
 * @brief  进入恢复出厂固件模式
 * @note   会视实际情况判断是否具备恢复出厂固件的条件
 * @retval true: 成功 | false: 失败
 */
bool Bootloader_IntoRecovryMode(void)
{
    if (_fw_update_info.exe_flow == EXE_FLOW_NOTHING
    ||  _fw_update_info.exe_flow == EXE_FLOW_NEED_HOST_SEND_FIRMWARE
    ||  _fw_update_info.exe_flow == EXE_FLOW_FIND_RUNNING_FIRMWARE
    ||  _fw_update_info.exe_flow == EXE_FLOW_JUMP_TO_APP
    ||  _fw_update_info.exe_flow == EXE_FLOW_FAILED)
    {
        _SetExeFlow(EXE_FLOW_RECOVERY);
        return true;
    }
    return false;
}


#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE) \
||  defined(USING_CUSTOM_UPDATE_FLAG)
/**
 * @brief  获取固件更新标志位
 * @note   
 * @retval 固件更新标志位
 */
__WEAK
uint64_t Bootloader_GetUpdateFlag(void)
{
    return update_flag;
}

/**
 * @brief  设置固件更新标志位
 * @note   
 * @param[in]  flag: 固件更新标志位
 * @retval None
 */
__WEAK
void Bootloader_SetUpdateFlag(uint64_t flag)
{
    update_flag = flag;
}
#endif

/**
 * @brief  获取与主机数据传输的执行结果
 * @note   
 * @retval 与主机数据传输的执行结果
 */
PP_CMD_EXE_RESULT Bootloader_GetExeResult(void)
{
    return _fw_update_info.cmd_exe_result;
}

/**
 * @brief  获取与主机数据传输的错误代码
 * @note   
 * @retval 与主机数据传输的错误代码
 */
PP_CMD_ERR_CODE Bootloader_GetExeErrCode(void)
{
    return _fw_update_info.cmd_exe_err_code;
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
    Bootloader_Port_HostDataProcess();
    BSP_Printf("%s: progress: %d\r\n", __func__, _fw_update_info.total_progress);
}


/* Private functions ---------------------------------------------------------*/
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
    result = FM_VerifyFirmware(part_name, FM_GetPackageCRC32(), false);
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
    result = FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), true);
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
    bool is_try_auto_update = false;
    uint8_t fw_sta = _Firmware_Check();
    
    /* 本函数的处理逻辑较复杂。为了提高执行效率，减少资源占用，使用了 goto 。阅读时建议结合《固件检测和处理机制》文档查看 */

    /* 没有任何可用的固件 */
    if (fw_sta == 0)
    {
        _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
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
            is_try_auto_update = true;
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
                is_try_auto_update = true;
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
                _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                return;
            }
        #endif
            
            /* 校验 APP 固件 */
            _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), false);
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
                _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
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
                    _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
                    return;
                }
            #endif
                
                /* 校验 APP 固件 */
                _fw_update_info.cmd_exe_err_code = (PP_CMD_ERR_CODE)FM_VerifyFirmware(APP_PART_NAME, FM_GetRawCRC32(), false);

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
                    _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
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
                _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
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
            _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
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
        _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    }
    #endif
    
__check_download:
    /* 执行到此处，意味着已经发生以下情况：
     * APP 没有固件 或 校验不通过 或 APP 的自动更新失败
     */
    /* 还未执行过自动更新 */
    if (is_try_auto_update == false)
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
        _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
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
        _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
        return;
    }
    #else
    /* 设置为需主机发送固件的状态 */
    _SetExeFlow(EXE_FLOW_NEED_HOST_SEND_FIRMWARE); 
    return;
    #endif
    
__jump_to_app:
    _JumpToAPP(); 
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
static void _JumpToAPP(void)
{
#if (USING_IS_NEED_UPDATE_PROJECT == USING_APP_SET_FLAG_UPDATE) \
||  defined(USING_CUSTOM_UPDATE_FLAG)
    /* 第二次进入，跳转至 APP */
    if (Bootloader_GetUpdateFlag() == BOOTLOADER_RESET_MAGIC_WORD)
    {
        /* 清除固件更新标志位 */
        Bootloader_SetUpdateFlag(0);

        Bootloader_Port_JumpToAPP();
    }
    /* 首次进入设置标志位并复位 */
    else
    {
        /* 设置标志位 */
        Bootloader_SetUpdateFlag(BOOTLOADER_RESET_MAGIC_WORD);
        
        /* 复位重新进入 bootloader */
        Bootloader_Port_SystemReset();
    }
#else
    Bootloader_Port_JumpToAPP();
#endif
}


/**
 * @brief  设置程序的执行流程
 * @note   
 * @param[in]  flow: 执行流程
 * @retval None
 */
static void _SetExeFlow(BOOT_EXE_FLOW flow)
{
    _fw_update_info.exe_flow = flow;
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

