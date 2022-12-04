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


/* TODO: δʵ�ֵĹ��ܣ��ظ�������ʱ��⣬��ʱ���ط���Ӧ */


/* Includes ------------------------------------------------------------------*/
#include "protocol_parser.h"


/* Exported variables ---------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* ����Э���Զ���ı��������� */
static uint8_t                  _is_exe_cmd;            /* ���ڴ����������ݵı�־λ */
static uint8_t                  _enable_recv_cmd;       /* ʹ���Ƿ����������ָ��� */
static uint8_t                  *_dev_rx_data;          /* Э�������������Դ */
static uint16_t                 _dev_rx_len;            /* ���յ������ݳ��� */
static uint8_t                  _ymodem_pkt_num;        /* ��¼ YModem Э��� packet number */
static YMODEM_EXE_FLOW          _exe_flow;              /* ��¼Э���ִ������ */
static union HOST_MESSAGE       *_host_msg;             /* �����������ݰ��Ļ���أ���Ϊ������Ϣ */
static struct PP_DEV_TX_PKG     _dev_tx_pkg;            /* ���ڴ���豸�Ϸ���������Ĳ��ֲ��� */
static struct BSP_TIMER         _timer_send_c;          /* ���ڶ�ʱ�������������ݵĶ�ʱ�� */

/* Э��������Ļص��������������޸� */
static PP_Send_t                _PP_Send;               /* ���ݷ��ͽӿ� */
static PP_PrepareCallback_t     _PP_Prepare;            /* �յ�����ָ��ʱ��Ԥ������ӿ� */
static PP_ReplyCallback_t       _PP_GetReplyInfo;       /* ����ִ��ָ��ʱ��Ӧ������ѯִ�й��̺ͽ���Ľӿ� */


/* Private function prototypes -----------------------------------------------*/
/* ����Э���Զ���ĺ��� */
static void                 _Host_HeartBeatProcess   (void);
static void                 _Host_CommandProcess     (void);
static void                 _Timeout_Handler         (void *user_data);
static PP_CMD_ERR_CODE      _Set_ExeFlow             (PP_CMD  cmd);
static void                 _YModem_Reset            (void);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  Э��������ĳ�ʼ��
 * @note   
 * @param[in]  Send: �ײ����ݷ��ͽӿ�
 * @param[in]  HeartbeatCallback: ����������Ӧ�ӿ�
 * @param[in]  PrepareCallback: ָ����Ĵ���ӿ�
 * @param[in]  Set_ReplyInfo: ��ѯָ��ִ�н���Ĵ���ӿ�
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
 * @brief  Э�����������
 * @note   ��Ҫѭ������
 * @retval PP_CMD_ERR_CODE
 */
PP_CMD_ERR_CODE  PP_Handler(uint8_t *data, uint16_t len)
{
    PP_CMD_ERR_CODE  err_code;

    /* ������ */
    if (data && len && _enable_recv_cmd)
    {
        /* �ݴ�͸�ʽ�� */
        _dev_rx_data = data;
        _dev_rx_len  = len;
        _host_msg    = (union HOST_MESSAGE *)_dev_rx_data;

        /* ֻ������֡�������´����� */
        if (_host_msg->pkg.header == YMODEM_SOH
        ||  _host_msg->pkg.header == YMODEM_STX)
        {
            /* �ж����к��Ƿ����˳�� */
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

            /* �ж��������к��Ƿ���ȷ */
            uint8_t pkt_num = ~(_host_msg->pkg.pkt_num);
            if (_host_msg->pkg.not_pkt_num != pkt_num)
            {
                BSP_Printf("error: packet number: %.2X %.2X\r\n", _host_msg->pkg.pkt_num, _host_msg->pkg.not_pkt_num);
                err_code = PP_ERR_PKT_NUM_ERR;
                goto __error_exit;
            }

            /* ��ȡЭ��֡�е������ֶγ��� */
            uint16_t data_len;
            if (_host_msg->pkg.header == YMODEM_SOH)
                data_len = YMODEM_SOH_DATA_LEN;

            else if (_host_msg->pkg.header == YMODEM_STX)
                data_len = YMODEM_STX_DATA_LEN;

            /* ֡�����ж� */
            /* ��ֵ��ǣ� Xshell �ڷ������һ���� SOH ����֡ʱ�ḽ�������ֽڵ� 0x4F ��ԭ��δ֪��
             * Ϊ�˴���������⣬��������Ϊ����֡�������󣬴˴�Ƕ���ˡ� ymodem_pkt_num != 0 �����ж� */
            if (_ymodem_pkt_num != 0)
            {
                if (_dev_rx_len != (data_len + YMODEM_FRAME_FIXED_LEN))
                {
                    BSP_Printf("error: _dev_rx_len: %d\r\n", _dev_rx_len);
                    err_code = PP_ERR_FRAME_LENGTH_ERR;
                    goto __error_exit;
                }
            }

            /* У�������Ƿ���ȷ */
            uint16_t crc16 = crc16_xmodem(_host_msg->pkg.data, data_len);
            uint16_t raw_crc16 = (_host_msg->pkg.data[data_len] << 8) | _host_msg->pkg.data[data_len + 1];
            if (crc16 != raw_crc16)
            {
                BSP_Printf("error: crc16: %.4X\r\n", crc16);
                BSP_Printf("error: raw crc16: %.4X\r\n", raw_crc16);
                err_code = PP_ERR_FRAME_VERIFY_ERR;
                goto __error_exit;
            }

            /* ִ�е��˴���¼���кż�1 */
            _ymodem_pkt_num++;
        }
        BSP_Printf("Ymodem recv len: %d (%.2X)\r\n", _dev_rx_len, _host_msg->pkg.header);

        /* �������� */
        if (_Set_ExeFlow((PP_CMD)_host_msg->pkg.header))
        {
            BSP_Printf("error: flow illegal\r\n");
            err_code = PP_ERR_EXE_FLOW_ERR;
            goto __error_exit;
        }

        /* ���� Host_CommandProcess �����յ������� */
        /* �������һ���յ�SOH����֡����������ִ�� Host_CommandProcess ������ EOT CAN */
        /* ��Ҫע���ǣ� Host_CommandProcess ִ�������δ�ظ��������ظ������� Host_HeartBeatProcess ���� */
        _Host_CommandProcess();
        return PP_ERR_OK;

    __error_exit:
        _dev_tx_pkg.response = YMODEM_NAK;
        _PP_Send(&_dev_tx_pkg.response, 1, HAL_MAX_DELAY);
        return err_code;
    }

    if (_is_exe_cmd)
    {
        /* ģ������������������ѯ�Ƿ��Э���������ϣ����������ظ� */
        _Host_HeartBeatProcess();
    }
    return PP_ERR_OK;
}


/**
 * @brief  ����Э��������Ĳ���
 * @note   
 * @param[in]  para: ��Ҫ�������õ�ѡ�ͻ����
 * @param[in]  value: ��Ӧ�����ݻ�ֵ
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
 * @brief  �������·����������Ĵ���
 * @note   �������·�������֡���лظ��� ymodem û�����������˴�ֻ��ģ��
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
        /* ��ǰ���Ѿ��� 1 ���˴������ڶ����ݴ��������⣬��Э�鱾�����⣬�����Ҫ���� */
        _ymodem_pkt_num--;
        _dev_tx_pkg.response = YMODEM_NAK;
    }
    else if (result == PP_RESULT_CANCEL)
    {
        _dev_tx_pkg.response = YMODEM_CAN;
    }
    /* ҵ��㻹�ڴ������ݣ���ʱ���ظ����� */
    else
        return;

    _is_exe_cmd = 0;
    _PP_Send(&_dev_tx_pkg.response, 1, HAL_MAX_DELAY);
}


/**
 * @brief  �������·�ָ��Ĵ���
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
 * @brief  ����Э��ִ������
 * @note   
 * @param[in]  cmd: ������ָ��
 * @retval PP_CMD_ERR_CODE
 */
static PP_CMD_ERR_CODE  _Set_ExeFlow(PP_CMD  cmd)
{
    switch (cmd)
    {
        case PP_CMD_SOH:
        {
            /* ��һ�� SOH ����֡ */
            if (_exe_flow == YMODEM_FLOW_NONE)
            {
                _exe_flow = YMODEM_FLOW_START;
                _dev_tx_pkg.response = YMODEM_ACK;
            }
            /* ���һ���յ� SOH ����֡ */
            else if (_exe_flow == YMODEM_FLOW_SECOND_EOT)
            {
                BSP_Timer_Pause(&_timer_send_c);
                _exe_flow = YMODEM_FLOW_SUCCESS;
                _ymodem_pkt_num = 0;
                _dev_tx_pkg.response = YMODEM_ACK;
            }
            /* ���ڴ������ݵ� SOH ����֡ */
            else
            {
                /* ��ͣ�����ַ��� C ���Ķ�ʱ�� */
                BSP_Timer_Pause(&_timer_send_c);
                _dev_tx_pkg.response = YMODEM_ACK;
            }
            break;
        }
        case PP_CMD_STX:
        {
            /* ��Ϊ��һ�������ݵ�����֡������ STX ����˴˴��б�Ҫ��ͣ�����ַ��� C ���Ķ�ʱ�� */
            BSP_Timer_Pause(&_timer_send_c);
            _dev_tx_pkg.response = YMODEM_ACK;
            break;
        }
        case PP_CMD_EOT:
        {
            /* ��һ�� EOT */
            if (_exe_flow == YMODEM_FLOW_START)
            {
                _exe_flow = YMODEM_FLOW_FIRST_EOT;
                _dev_tx_pkg.response = YMODEM_NAK;
            }
            /* �ڶ��� EOT */
            else if (_exe_flow == YMODEM_FLOW_FIRST_EOT)
            {
                _exe_flow = YMODEM_FLOW_SECOND_EOT;
                _ymodem_pkt_num = 0;
                _dev_tx_pkg.response = YMODEM_ACK;
                /* ���� ACK ��Ҫ�������� C �� */
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
 * @brief  ��λһЩȫ�ֱ����ͱ�־��Ϣ
 * @note   
 * @retval None
 */
static void _YModem_Reset(void)
{
    _is_exe_cmd = 0;
    _enable_recv_cmd = 1;        /* ʹ���Ƿ����������ָ��� */
    _ymodem_pkt_num = 0;
    _exe_flow = YMODEM_FLOW_NONE;
}


/**
 * @brief  ��ʱ����ʱ�ص�����
 * @note   
 * @param[in]  user_data: �û�����
 * @retval None
 */
static void _Timeout_Handler(void *user_data)
{
    static uint8_t c[1] = {YMODEM_C};
    _PP_Send(c, 1, HAL_MAX_DELAY);
}





