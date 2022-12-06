/**
 * \file            protocol_parser.c
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


/* TODO: 未实现的功能：回复主机超时检测，超时则重发响应 */


/* Includes ------------------------------------------------------------------*/
#include "protocol_parser.h"


/* Exported variables ---------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* 根据协议自定义的变量和数据 */
static uint8_t                  _is_exe_cmd;            /* 正在处理主机数据的标志位 */
static uint8_t                  _enable_recv_cmd;       /* 使能是否接收主机的指令包 */
static uint8_t                  *_dev_rx_data;          /* 协议解析的数据来源 */
static uint16_t                 _dev_rx_len;            /* 接收到的数据长度 */
static uint8_t                  _ymodem_pkt_num;        /* 记录 YModem 协议的 packet number */
static YMODEM_EXE_FLOW          _exe_flow;              /* 记录协议的执行流程 */
static union HOST_MESSAGE       *_host_msg;             /* 接收主机数据包的缓存池，称为主机消息 */
static struct PP_DEV_TX_PKG     _dev_tx_pkg;            /* 用于存放设备上发数据组包的部分参数 */
static struct BSP_TIMER         _timer_send_c;          /* 用于定时向主机发送数据的定时器 */

/* 协议析构层的回调函数，不建议修改 */
static PP_Send_t                _PP_Send;               /* 数据发送接口 */
static PP_PrepareCallback_t     _PP_Prepare;            /* 收到主机指令时的预备处理接口 */
static PP_ReplyCallback_t       _PP_GetReplyInfo;       /* 正在执行指令时响应主机查询执行过程和结果的接口 */


/* Private function prototypes -----------------------------------------------*/
/* 根据协议自定义的函数 */
static void                 _Host_HeartBeatProcess   (void);
static void                 _Host_CommandProcess     (void);
static void                 _Timeout_Handler         (void *user_data);
static PP_CMD_ERR_CODE      _Set_ExeFlow             (PP_CMD  cmd);
static void                 _YModem_Reset            (void);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  协议析构层的初始化
 * @note   
 * @param[in]  Send: 底层数据发送接口
 * @param[in]  HeartbeatCallback: 心跳包的响应接口
 * @param[in]  PrepareCallback: 指令包的处理接口
 * @param[in]  Set_ReplyInfo: 查询指令执行结果的处理接口
 * @retval None
 */
void PP_Init(PP_Send_t               Send, 
             PP_HeartbeatCallback_t  HeartbeatCallback,
             PP_PrepareCallback_t    PrepareCallback, 
             PP_ReplyCallback_t      Set_ReplyInfo)
{
    _PP_Send         = Send;
    _PP_Prepare      = PrepareCallback;
    _PP_GetReplyInfo = Set_ReplyInfo;
    
    _enable_recv_cmd = 1;
    BSP_Timer_Init( &_timer_send_c, 
                    _Timeout_Handler, 
                    1000, 
                    TIMER_RUN_FOREVER, 
                    TIMER_TYPE_HARDWARE);
    BSP_Timer_Start(&_timer_send_c);
}


/**
 * @brief  协议解析处理函数
 * @note   需要循环调用
 * @retval PP_CMD_ERR_CODE
 */
PP_CMD_ERR_CODE  PP_Handler(uint8_t *data, uint16_t len)
{
    PP_CMD_ERR_CODE  err_code;

    /* 有数据 */
    if (data && len && _enable_recv_cmd)
    {
        /* 暂存和格式化 */
        _dev_rx_data = data;
        _dev_rx_len  = len;
        _host_msg    = (union HOST_MESSAGE *)_dev_rx_data;

        /* 只有数据帧才做以下错误检查 */
        if (_host_msg->pkg.header == YMODEM_SOH
        ||  _host_msg->pkg.header == YMODEM_STX)
        {
            /* 判断序列号是否符合顺序 */
            if (_host_msg->pkg.pkt_num != _ymodem_pkt_num)
            {
                if (_host_msg->pkg.pkt_num == (_ymodem_pkt_num - 1))
                {
                    BSP_Printf("error: duplicate frame\r\n");
                    err_code = PP_ERR_DUPLICATE_FRAME;
                }
                else
                {
                    BSP_Printf("error: omission frame\r\n");
                    err_code = PP_ERR_OMISSION_FRAME;
                }
                goto __error_exit;
            }

            /* 判断正反序列号是否正确 */
            uint8_t pkt_num = ~(_host_msg->pkg.pkt_num);
            if (_host_msg->pkg.not_pkt_num != pkt_num)
            {
                BSP_Printf("error: packet number: %.2X %.2X\r\n", _host_msg->pkg.pkt_num, _host_msg->pkg.not_pkt_num);
                err_code = PP_ERR_PKT_NUM_ERR;
                goto __error_exit;
            }

            /* 获取协议帧中的数据字段长度 */
            uint16_t data_len;
            if (_host_msg->pkg.header == YMODEM_SOH)
                data_len = YMODEM_SOH_DATA_LEN;

            else if (_host_msg->pkg.header == YMODEM_STX)
                data_len = YMODEM_STX_DATA_LEN;

            /* 帧长度判断 */
            /* 奇怪的是， Xshell 在发送最后一个空 SOH 数据帧时会附加两个字节的 0x4F ，原因未知。
             * 为了处理这个问题，避免误判为数据帧长度有误，此处嵌套了“ ymodem_pkt_num != 0 ”的判断 */
            if (_ymodem_pkt_num != 0)
            {
                if (_dev_rx_len != (data_len + YMODEM_FRAME_FIXED_LEN))
                {
                    BSP_Printf("error: _dev_rx_len: %d\r\n", _dev_rx_len);
                    err_code = PP_ERR_FRAME_LENGTH_ERR;
                    goto __error_exit;
                }
            }

            /* 校验数据是否正确 */
            uint16_t crc16 = crc16_xmodem(_host_msg->pkg.data, data_len);
            uint16_t raw_crc16 = (_host_msg->pkg.data[data_len] << 8) | _host_msg->pkg.data[data_len + 1];
            if (crc16 != raw_crc16)
            {
                BSP_Printf("error: crc16: %.4X\r\n", crc16);
                BSP_Printf("error: raw crc16: %.4X\r\n", raw_crc16);
                err_code = PP_ERR_FRAME_VERIFY_ERR;
                goto __error_exit;
            }

            /* 执行到此处记录序列号加1 */
            _ymodem_pkt_num++;
        }
        BSP_Printf("Ymodem recv len: %d (%.2X)\r\n", _dev_rx_len, _host_msg->pkg.header);

        /* 设置流程 */
        if (_Set_ExeFlow((PP_CMD)_host_msg->pkg.header))
        {
            BSP_Printf("error: flow illegal\r\n");
            err_code = PP_ERR_EXE_FLOW_ERR;
            goto __error_exit;
        }

        /* 调用 Host_CommandProcess 处理收到的数据 */
        /* 除了最后一个空的SOH数据帧，其他都会执行 Host_CommandProcess ，包括 EOT CAN */
        /* 需要注意是， Host_CommandProcess 执行完后仍未回复主机，回复部分由 Host_HeartBeatProcess 处理 */
        _Host_CommandProcess();
        return PP_ERR_OK;

    __error_exit:
        _dev_tx_pkg.response = YMODEM_NAK;
        _PP_Send(&_dev_tx_pkg.response, 1, HAL_MAX_DELAY);
        return err_code;
    }

    if (_is_exe_cmd)
    {
        /* 模拟主机的心跳处理，查询是否对协议包处理完毕，以向主机回复 */
        _Host_HeartBeatProcess();
    }
    return PP_ERR_OK;
}


/**
 * @brief  配置协议析构层的参数
 * @note   
 * @param[in]  para: 需要进行配置的选型或参数
 * @param[in]  value: 对应的数据或值
 * @retval None
 */
void PP_Config(PP_CONFIG_PARA  para, void *value)
{
    if (para == PP_CONFIG_RESET)
    {
        _YModem_Reset();
        BSP_Timer_Restart(&_timer_send_c);
    }
    else if (para == PP_CONFIG_ENABLE_RECV_CMD)
    {
        uint8_t *enable = (uint8_t *)value;
        _enable_recv_cmd = *enable;
        if (_enable_recv_cmd == 0)
            BSP_Timer_Pause(&_timer_send_c);
    }
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  对主机下发的心跳包的处理
 * @note   对主机下发的数据帧进行回复， ymodem 没有心跳包，此处只是模拟
 * @retval None
 */
static void _Host_HeartBeatProcess(void)
{
    static PP_CMD_EXE_RESULT  result;

    _PP_GetReplyInfo((PP_CMD)_host_msg->pkg.header, &result, NULL, NULL);

    if (result == PP_RESULT_OK)
    {
//        _dev_tx_pkg.response = YMODEM_ACK;
    }
    else if (result == PP_RESULT_FAILED)
    {
        /* 因前面已经加 1 ，此处是由于对数据处理有问题，非协议本身问题，因此需要减回 */
        _ymodem_pkt_num--;
        _dev_tx_pkg.response = YMODEM_NAK;
    }
    else if (result == PP_RESULT_CANCEL)
    {
        _dev_tx_pkg.response = YMODEM_CAN;
    }
    /* 业务层还在处理数据，暂时不回复主机 */
    else
        return;

    _is_exe_cmd = 0;
    _PP_Send(&_dev_tx_pkg.response, 1, HAL_MAX_DELAY);
}


/**
 * @brief  对主机下发指令的处理
 * @note   
 * @retval None
 */
static void _Host_CommandProcess(void)
{
    uint16_t data_len = 0;

    _is_exe_cmd = 1;

    if (_host_msg->pkg.header == YMODEM_SOH
    ||  _host_msg->pkg.header == YMODEM_STX)
        data_len = _dev_rx_len - YMODEM_FRAME_FIXED_LEN;

    _PP_Prepare((PP_CMD)_host_msg->pkg.header, _host_msg->pkg.data, data_len);
}


/**
 * @brief  设置协议执行流程
 * @note   
 * @param[in]  cmd: 主机的指令
 * @retval PP_CMD_ERR_CODE
 */
static PP_CMD_ERR_CODE  _Set_ExeFlow(PP_CMD  cmd)
{
    switch (cmd)
    {
        case PP_CMD_SOH:
        {
            /* 第一个 SOH 数据帧 */
            if (_exe_flow == YMODEM_FLOW_NONE)
            {
                _exe_flow = YMODEM_FLOW_START;
                _dev_tx_pkg.response = YMODEM_ACK;
            }
            /* 最后一个空的 SOH 数据帧 */
            else if (_exe_flow == YMODEM_FLOW_SECOND_EOT)
            {
                BSP_Timer_Pause(&_timer_send_c);
                _exe_flow = YMODEM_FLOW_SUCCESS;
                _ymodem_pkt_num = 0;
                _dev_tx_pkg.response = YMODEM_ACK;
            }
            /* 正在传输数据的 SOH 数据帧 */
            else
            {
                /* 暂停发送字符“ C ”的定时器 */
                BSP_Timer_Pause(&_timer_send_c);
                _dev_tx_pkg.response = YMODEM_ACK;
            }
            break;
        }
        case PP_CMD_STX:
        {
            /* 因为第一个有数据的数据帧可能是 STX ，因此此处有必要暂停发送字符“ C ”的定时器 */
            BSP_Timer_Pause(&_timer_send_c);
            _dev_tx_pkg.response = YMODEM_ACK;
            break;
        }
        case PP_CMD_EOT:
        {
            /* 第一个 EOT */
            if (_exe_flow == YMODEM_FLOW_START)
            {
                _exe_flow = YMODEM_FLOW_FIRST_EOT;
                _dev_tx_pkg.response = YMODEM_NAK;
            }
            /* 第二个 EOT */
            else if (_exe_flow == YMODEM_FLOW_FIRST_EOT)
            {
                _exe_flow = YMODEM_FLOW_SECOND_EOT;
                _ymodem_pkt_num = 0;
                _dev_tx_pkg.response = YMODEM_ACK;
                /* 发完 ACK 需要继续发“ C ” */
                BSP_Timer_Restart(&_timer_send_c);
            }
            else
                return PP_ERR_EXE_FLOW_ERR;
            break;
        }
        case PP_CMD_CAN:
        {
            _exe_flow = YMODEM_FLOW_CANCEL;
            _dev_tx_pkg.response = YMODEM_ACK;
            break;
        }
        default: break;
    }
    
    return PP_ERR_OK;
}


/**
 * @brief  复位一些全局变量和标志信息
 * @note   
 * @retval None
 */
static void _YModem_Reset(void)
{
    _is_exe_cmd = 0;
    _enable_recv_cmd = 1;        /* 使能是否接收主机的指令包 */
    _ymodem_pkt_num = 0;
    _exe_flow = YMODEM_FLOW_NONE;
}


/**
 * @brief  定时器超时回调函数
 * @note   
 * @param[in]  user_data: 用户数据
 * @retval None
 */
static void _Timeout_Handler(void *user_data)
{
    static uint8_t c[1] = {YMODEM_C};
    _PP_Send(c, 1, HAL_MAX_DELAY);
}





