/**
 * \file            bootloader_port.c
 * \brief           communication and protocol part of the bootloader
 */

/*
 * Copyright (c) 2023 Dino Haw
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
 * v1.0     2023-12-10     Dino         the first version
 */

/* Includes ------------------------------------------------------------------*/
#include "bootloader.h"


/* Private variables ---------------------------------------------------------*/
static bool     _is_first_pkg;                              /* 是否为第一个收到的数据包 */
static bool     _is_firmware_head;                          /* 是否为固件包头的标志位 */
static uint16_t _dev_rx_len;                                /* 指示缓存区接收到的数据量，单位 byte */
static uint8_t  _dev_rx_buff[PP_MSG_BUFF_SIZE + 16];        /* 设备底层数据接收缓存区 */
static uint8_t  _fw_sub_pkg_data[PP_FIRMWARE_PKG_SIZE];     /* 暂存固件包体 */
static uint16_t _fw_sub_pkg_len;                            /* 记录固件包体大小，包含两个字节的数据长度 */
static uint32_t _stack_addr;                                /* APP 栈顶地址 */
static uint32_t _reset_handler;                             /* APP reset handler 地址 */

static struct BSP_TIMER         _timer_wait_data;           /* 检测主机数据下发超时的 timer */
static struct DATA_TRANSFER     _data_if;                   /* 数据传输的接口 */
#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
static struct BSP_TIMER         _timer_key;                 /* 用于按键扫描的 timer */
static struct BSP_KEY           _recovery_key;              /* 用于恢复出厂固件的按键 */  
#endif


/* Extern function prototypes ------------------------------------------------*/
extern bool                 Bootloader_IntoRecovryMode  (void);
extern void                 Bootloader_SetCommStatus    (COMM_STATUS status, 
                                                         uint8_t *data, 
                                                         uint16_t data_len);
extern PP_CMD_EXE_RESULT    Bootloader_GetExeResult     (void);
extern PP_CMD_ERR_CODE      Bootloader_GetExeErrCode    (void);


/* Private function prototypes -----------------------------------------------*/
static void     _PP_DataPackageProcess          (PP_CMD cmd, uint8_t *data, uint16_t data_len);
static void     _PP_SetReplyData                (PP_CMD cmd, 
                                                 PP_CMD_EXE_RESULT *cmd_exe_result, 
                                                 uint8_t *data, 
                                                 uint16_t *data_len);
static void     _Timer_HostDataTimeoutCallback  (void *user_data);
static void     _UART_SendData                  (uint8_t *data, uint16_t len, uint32_t timeout);
#if (ENABLE_FACTORY_FIRMWARE_BUTTON)
static uint8_t  _Key_GetLevel                   (void);
static void     _Key_EventCallback              (uint8_t id, KEY_EVENT  event);
static void     _Timer_ScanKeyCallback          (void *user_data);
#endif


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  通讯与协议相关初始化
 * @note   
 * @retval None
 */
void Bootloader_Port_Init(void)
{
#if (WAIT_HOST_DATA_MAX_TIME)
    /* 初始状态为等待主机是否下发数据的定时器 */
    BSP_Timer_Init( &_timer_wait_data,
                    _Timer_HostDataTimeoutCallback,
                    WAIT_HOST_DATA_MAX_TIME,
                    TIMER_RUN_FOREVER,
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_wait_data);
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

    /* 软件初始化 */
    DT_Init(&_data_if, BSP_UART1, _dev_rx_buff, &_dev_rx_len, PP_MSG_BUFF_SIZE + 16);
    PP_Init(_UART_SendData, NULL, _PP_DataPackageProcess, _PP_SetReplyData);
    
    BSP_Printf("FLASH_PAGE_SIZE: 0x%.8X\r\n", FLASH_PAGE_SIZE);
    
    _is_first_pkg     = false;
    _is_firmware_head = false;
}


/**
 * @brief  主机数据接收处理函数
 * @note   
 * @retval None
 */
void Bootloader_Port_HostDataProcess(void)
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
            BSP_Printf("uart recv len: %d\r\n", _dev_rx_len);
        }
        _dev_rx_len = 0;
    }
    else
        PP_Handler(NULL, 0);
}


/**
 * @brief  复位函数
 * @note   
 * @retval None
 */
void Bootloader_Port_Reset(void)
{
    _is_first_pkg     = false;
    _is_firmware_head = false;
}


/**
 * @brief  跳转至APP的处理函数
 * @note   若选择直接跳转至 APP ，则需要注意使用到的外设要进行 deinit ，否则会对 APP 的运行有影响
 * @retval None
 */
__NO_RETURN
void Bootloader_Port_JumpToAPP(void)
{
    typedef void(*APP_MAIN_FUNC)(void);
    APP_MAIN_FUNC  APP_Main; 

    /* 关闭全局中断 */
    __disable_irq();

#if (USING_IS_NEED_UPDATE_PROJECT != USING_APP_SET_FLAG_UPDATE)
    /* 设置所有时钟到默认状态，使用HSI时钟 */
    HAL_RCC_DeInit();

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
    
    extern DMA_HandleTypeDef hdma_usart1_rx;

    /* 外设 deinit ，需要自己根据实际使用的外设添加，建议使用 USING_APP_SET_FLAG_UPDATE 方式 */
    HAL_DeInit();
    HAL_UART_DeInit(&huart1);
    HAL_DMA_DeInit(&hdma_usart1_rx);
#endif

    _stack_addr    = *(volatile uint32_t *)APP_ADDRESS;
    _reset_handler = *(volatile uint32_t *)(APP_ADDRESS + 4);

    /* 设置主堆栈指针 */
    __set_MSP(_stack_addr);

    /* 在 RTOS 工程，这条语句很重要，设置为特权级模式，使用 MSP 指针 */
    __set_CONTROL(0);

    /* 设置中断向量表 */
    SCB->VTOR = APP_ADDRESS;

    /* 跳转到 APP ，首地址是 MSP ，地址 +4 是复位中断服务程序地址 */
    APP_Main = (APP_MAIN_FUNC)_reset_handler;
    APP_Main();

    /* 复位或跳转成功的话，不会执行到这里 */
    while (1) {}
}


/**
 * @brief  系统复位重启
 * @note   
 * @retval None
 */
void Bootloader_Port_SystemReset(void)
{
    HAL_NVIC_SystemReset();
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  负责接收协议解析出来的数据
 * @note   通过协议的指令包，设置程序的执行流程
 * @param[in]  cmd: 接收到的指令
 * @param[in]  data: 接收到的参数
 * @param[in]  data_len: 接收到的参数长度，单位 byte
 * @retval None
 */
static void _PP_DataPackageProcess(PP_CMD cmd, uint8_t *data, uint16_t data_len)
{
    static bool is_eot = false;

    Bootloader_SetCommStatus(COMM_STATUS_RECV_DATA, NULL, 0);

    switch (cmd)
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
            if (is_eot && cmd == PP_CMD_SOH)
            {
                is_eot = false;
                Bootloader_SetCommStatus(COMM_STATUS_FILE_DONE, NULL, 0);
                return;
            }

            /* data_len 不能大于 sizeof(_fw_sub_pkg_data) */
            /* 为了减少判断，这里假定 data_len 不大于 sizeof(_fw_sub_pkg_data) */
            _fw_sub_pkg_len = data_len;
            memcpy(&_fw_sub_pkg_data[0], &data[0], _fw_sub_pkg_len);
            BSP_Printf("_fw_sub_pkg_len: %d\r\n", _fw_sub_pkg_len);

            /* 第一个数据帧，包含文件名和文件大小等信息 */
            if (_is_first_pkg == false)
            {
                _is_first_pkg   = true;
                char *file_name = (char *)&_fw_sub_pkg_data[0];
                char *file_size = (char *)&_fw_sub_pkg_data[strlen(file_name) + 1];
                BSP_Printf("file name: %s\r\n", file_name);
                BSP_Printf("file size: %d\r\n", atol(file_size));
                Bootloader_SetCommStatus(COMM_STATUS_FILE_INFO, &_fw_sub_pkg_data[0], _fw_sub_pkg_len);
                return;
            }
            
            /* 固件包头，单独处理 */
            if (_is_firmware_head == false)
            {
                _is_firmware_head = true;
                Bootloader_SetCommStatus(COMM_STATUS_FIRMWARE_HEAD, &_fw_sub_pkg_data[0], FPK_HEAD_SIZE);
            }
            /* 固件包体 */
            else
                Bootloader_SetCommStatus(COMM_STATUS_FIRMWARE_PKG, &_fw_sub_pkg_data[0], _fw_sub_pkg_len);
            break;
        }
        case PP_CMD_EOT:
        {
            is_eot = true;
            Bootloader_SetCommStatus(COMM_STATUS_START_UPDATE, NULL, 0);
            break;
        }
        case PP_CMD_CAN:
        {
            is_eot            = false;
            _is_first_pkg     = false;
            _is_firmware_head = false;
            Bootloader_SetCommStatus(COMM_STATUS_CANCEL, NULL, 0);
            break;
        }
        default:
        {
            Bootloader_SetCommStatus(COMM_STATUS_UNKNOWN, NULL, 0);
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
static void _PP_SetReplyData(PP_CMD cmd, 
                             PP_CMD_EXE_RESULT *cmd_exe_result, 
                             uint8_t *data, 
                             uint16_t *data_len)
{
    *cmd_exe_result = Bootloader_GetExeResult();
    
    if (*cmd_exe_result == PP_RESULT_CANCEL
    ||  *cmd_exe_result == PP_RESULT_FAILED)
        BSP_Printf("cmd_exe_err_code: %.2X\r\n", Bootloader_GetExeErrCode());
}


/**
 * @brief  等待主机数据超时回调函数
 * @note   
 * @param[in]  user_data: 用户数据
 * @retval None
 */
static void _Timer_HostDataTimeoutCallback(void *user_data)
{
    Bootloader_SetCommStatus(COMM_STATUS_RECV_TIMEOUT, NULL, 0);
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
        Bootloader_IntoRecovryMode();
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

