/**
 * \file            bsp_uart_port.c
 * \brief           portable file of the UART driver
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


/* Includes ------------------------------------------------------------------*/
#include "bsp_uart.h"


/* Private variables ---------------------------------------------------------*/
#if (BSP_USING_UART2_RE || BSP_USING_UART3_RE)
static volatile uint8_t _uart2_alter;
static volatile uint8_t _uart3_alter;
#endif

static UART_Callback_t  _UART_RxCallback;
static UART_Callback_t  _UART_RxIdleCallback;
static UART_Callback_t  _UART_DMA_RxCallback;
static UART_Callback_t  _UART_DMA_TxCallback;

#if (BSP_USING_UART1)
UART_CREATE(1);
#endif

#if (BSP_USING_UART2)
UART_CREATE(2);
#endif

#if (BSP_USING_UART2_RE)
UART_CREATE(2_RE);
#endif

#if (BSP_USING_UART3)
UART_CREATE(3);
#endif

#if (BSP_USING_UART3_RE)
UART_CREATE(3_RE);
#endif

#if (BSP_USING_UART4)
UART_CREATE(4);
#endif

#if (BSP_USING_UART5)
UART_CREATE(5);
#endif

#if (BSP_USING_UART6)
UART_CREATE(6);
#endif

struct UART_STRUCT *_uart_group[] = 
{
#if (BSP_USING_UART1)
    &UART(1),
#endif   
#if (BSP_USING_UART2)
    &UART(2),
#endif
#if (BSP_USING_UART2_RE)
    &UART(2_RE),
#endif
#if (BSP_USING_UART3)
    &UART(3),
#endif
#if (BSP_USING_UART3_RE)
    &UART(3_RE),
#endif
#if (BSP_USING_UART4)
    &UART(4),
#endif
#if (BSP_USING_UART5)
    &UART(5),
#endif
#if (BSP_USING_UART6)
    &UART(6),
#endif
};

static const uint8_t _uart_qty = sizeof(_uart_group) / sizeof(struct UART_STRUCT *); 


/* Private function prototypes -----------------------------------------------*/
static inline void _UART_IntHandler (struct UART_STRUCT *uart);
static inline void _UART_Alternate  (struct UART_STRUCT *uart);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  UART �ӿڳ�ʼ��
 * @note   ���� UART �����ʼ����򱾺��������� UART �����ʼ����ſɵ���
 * @param[in]  uart: UART ����
 * @param[in]  rx_callback: ���յ��ֽ������жϵĻص�����
 * @param[in]  rx_idle_callback: �����жϵĻص�����
 * @param[in]  dma_rx_callback: DMA �����жϵĻص�����
 * @param[in]  dma_tx_callback: DMA ������ɵĻص�����
 * @retval None
 */
void BSP_UART_Port_Init( struct UART_STRUCT *uart, 
                         UART_Callback_t rx_callback, 
                         UART_Callback_t rx_idle_callback, 
                         UART_Callback_t dma_rx_callback, 
                         UART_Callback_t dma_tx_callback)
{
    BSP_UART_Port_LockInit(uart);

    _UART_RxCallback     = rx_callback;
    _UART_RxIdleCallback = rx_idle_callback;
    _UART_DMA_RxCallback = dma_rx_callback;
    _UART_DMA_TxCallback = dma_tx_callback;
    
    /**
     * �� uart->handle ��������ϵͳ��ʼ��ʱ������ CubeMX ���ɵĳ�ʼ�����룬���¸�ֵ�����Ǳ���ġ�
     * ������Ҫ�Լ��� uart->handle ��ʼ����
     * �ɴ˿ɼ��� BSP_UART_Port_Init ���� UART �����ʼ����򱾺��������� UART �����ʼ����ſɵ��á�
     */
    switch (uart->id)
    {
    #if (BSP_USING_UART1)
        case BSP_UART1:     uart->handle = UART1_HANDLE; break;
    #endif
    #if (BSP_USING_UART2)
        case BSP_UART2:     uart->handle = UART2_HANDLE; break;
    #endif
    #if (BSP_USING_UART2_RE)
        case BSP_UART2_RE:  uart->handle = UART2_HANDLE; break;
    #endif
    #if (BSP_USING_UART3)
        case BSP_UART3:     uart->handle = UART3_HANDLE; break;
    #endif
    #if (BSP_USING_UART3_RE)
        case BSP_UART3_RE:  uart->handle = UART3_HANDLE; break;
    #endif
    #if (BSP_USING_UART4)
        case BSP_UART4:     uart->handle = UART4_HANDLE; break;
    #endif
    #if (BSP_USING_UART5)
        case BSP_UART5:     uart->handle = UART5_HANDLE; break;
    #endif
    #if (BSP_USING_UART6)
        case BSP_UART6:     uart->handle = UART6_HANDLE; break;
    #endif
        default: break;
    }
    
    if (uart->handle.hdmatx)
        uart->handle.hdmatx->Parent = &uart->handle;
    
    BSP_Printf("-----------------------------\r\n");
    BSP_Printf("[ %s ]\r\n", __func__);
    BSP_Printf("id: %d\r\n", uart->id);
//    BSP_Printf("name: %s\r\n", uart->name);
    BSP_Printf("rx_buff: 0x%.8p\r\n", &uart->rx_buff[0]);
    BSP_Printf("huart size: %d byte\r\n", sizeof(UART1_HANDLE));
    BSP_Printf("uart size: %d byte\r\n", sizeof(_uart1));
    BSP_Printf("-----------------------------\r\n\r\n");
}


/**
 * @brief  UART �ӿ�ͨ�ſ������ĳ�ʼ��
 * @note   
 * @param[in]  uart: UART ����
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_LockInit(struct UART_STRUCT *uart)
{
#if (USING_RTOS_TYPE)
//    char name[ MAX_NAME_LEN ] = {0};
    char name[ MAX_NAME_LEN ];
    
    strncpy(name, uart->name, strlen(name));
    strncat(name, "_tsm", 4);
    
    #if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_sem_init(&uart->tx_lock, name, 1, RT_IPC_FLAG_PRIO) != RT_EOK)
    {    
        return BSP_UART_ERR_LOCK_INIT_ERR;
    }
    #elif (USING_RTOS_TYPE == RTOS_USING_UCOS)
    OS_ERR err;
    
    OSSemCreate((OS_SEM    *)&uart->tx_lock,
                (CPU_CHAR  *)name,
                (OS_SEM_CTR )1,
                (OS_ERR    *)&err);
    
    if (err != OS_ERR_NONE)
    {    
        return BSP_UART_ERR_LOCK_INIT_ERR;
    }
    #endif
    
    memset(name, 0, sizeof(name));
    strncpy(name, uart->name, strlen(name));
    strncat(name, "_rsm", 4);
    
    #if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_sem_init(&uart->rx_sem, name, 0, RT_IPC_FLAG_PRIO) != RT_EOK)
    {    
        return BSP_UART_ERR_LOCK_INIT_ERR;
    }
    #elif (USING_RTOS_TYPE == RTOS_USING_UCOS)
    OSSemCreate((OS_SEM    *)&uart->rx_sem,
                (CPU_CHAR  *)name,
                (OS_SEM_CTR )0,
                (OS_ERR    *)&err);
                
    if (err != OS_ERR_NONE)
    {    
        return BSP_UART_ERR_LOCK_INIT_ERR;
    }
    #endif
#endif
    
    return BSP_UART_ERR_OK;
}


/**
 * @brief  UART �ȴ��������ݵ��ź���
 * @note   
 * @param[in]  uart: UART ����
 * @retval BSP_UART_ERR
 */
inline BSP_UART_ERR  BSP_UART_Port_RxLock(struct UART_STRUCT *uart)
{
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_sem_take(&uart->rx_sem, RT_WAITING_FOREVER) != RT_EOK)
    {    
        return BSP_UART_ERR_LOCK_ERR;
    }
#elif (USING_RTOS_TYPE == RTOS_USING_UCOS)
    OS_ERR err;
    
    OSSemPend((OS_SEM *)&uart->rx_sem,
              (OS_TICK )0,
              (OS_OPT  )OS_OPT_PEND_BLOCKING,
              (CPU_TS *)NULL,
              (OS_ERR *)&err);
    
    if (err != OS_ERR_NONE)
    {    
        return BSP_UART_ERR_LOCK_ERR;
    }
#endif
    return BSP_UART_ERR_OK;
}


/**
 * @brief  UART ���յ����ݷ������ź���
 * @note   
 * @param[in]  uart: UART ����
 * @retval BSP_UART_ERR
 */
inline BSP_UART_ERR  BSP_UART_Port_RxUnlock(struct UART_STRUCT *uart)
{
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_sem_release(&uart->rx_sem) != RT_EOK)
    {    
        return BSP_UART_ERR_UNLOCK_ERR;
    }
#elif (USING_RTOS_TYPE == RTOS_USING_UCOS)
    OS_ERR err;

    OSSemPost((OS_SEM *)&uart->rx_sem,
              (OS_OPT  )OS_OPT_POST_FIFO,
              (OS_ERR *)&err);
    
    if (err != OS_ERR_NONE)
    {    
        return BSP_UART_ERR_UNLOCK_ERR;
    }
#endif
    return BSP_UART_ERR_OK;
}


/**
 * @brief  UART ������
 * @note   
 * @param[in]  uart: UART ����
 * @retval BSP_UART_ERR
 */
inline BSP_UART_ERR  BSP_UART_Port_TxLock(struct UART_STRUCT *uart)
{
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_sem_take(&uart->tx_lock, RT_WAITING_FOREVER) != RT_EOK)
    {
        return BSP_UART_ERR_LOCK_ERR;
    }
#elif (USING_RTOS_TYPE == RTOS_USING_UCOS)
    OS_ERR err;
    
    OSSemPend((OS_SEM *)&uart->tx_lock,
              (OS_TICK )0,
              (OS_OPT  )OS_OPT_PEND_BLOCKING,
              (CPU_TS *)NULL,
              (OS_ERR *)&err);
    
    if (err != OS_ERR_NONE)
    {    
        return BSP_UART_ERR_LOCK_ERR;
    }
#endif
    return BSP_UART_ERR_OK;
}


/**
 * @brief  UART ������Ͻ���
 * @note   
 * @param[in]  uart: UART ����
 * @retval BSP_UART_ERR
 */
inline BSP_UART_ERR  BSP_UART_Port_TxUnlock(struct UART_STRUCT *uart)
{  
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_sem_release(&uart->tx_lock) != RT_EOK)
    {    
        return BSP_UART_ERR_UNLOCK_ERR;
    }
#elif (USING_RTOS_TYPE == RTOS_USING_UCOS)
    OS_ERR err;

    OSSemPost((OS_SEM *)&uart->tx_lock,
              (OS_OPT  )OS_OPT_POST_FIFO,
              (OS_ERR *)&err);
    
    if (err != OS_ERR_NONE)
    {    
        return BSP_UART_ERR_UNLOCK_ERR;
    }
#endif
    return BSP_UART_ERR_OK;
}


/**
 * @brief  ʹ�� UART ��������
 * @note   
 * @param[in]  uart: UART ����
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_EnableReceive(struct UART_STRUCT *uart)
{
    BSP_UART_ERR  ret = BSP_UART_ERR_OK;
    
    if (uart->handle.hdmarx)
        uart->handle.hdmarx->Parent = &uart->handle;

    _UART_Alternate(uart);

    if (uart->handle.hdmarx)
        ret = (BSP_UART_ERR)HAL_UART_Receive_DMA(&uart->handle, (uint8_t *)uart->rx_buff, uart->rx_buff_max_len);
    else
        __HAL_UART_ENABLE_IT(&uart->handle, UART_IT_RXNE); 
    
    /* ���������ж� */
    __HAL_UART_ENABLE_IT(&uart->handle, UART_IT_IDLE);
    
    return ret;
}


/**
 * @brief  ��ֹ UART ��������
 * @note   
 * @param[in]  uart: UART ����
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_DisableReceive(struct UART_STRUCT *uart)
{
    BSP_UART_ERR  ret = BSP_UART_ERR_OK;
    
    if (uart->handle.hdmarx) 
        ret = (BSP_UART_ERR)HAL_UART_DMAPause(&uart->handle);
    else
        __HAL_UART_DISABLE_IT(&uart->handle, UART_IT_RXNE); 

    __HAL_UART_DISABLE_IT(&uart->handle, UART_IT_IDLE);
    
    return ret;
}


/**
 * @brief  ���� UART ����һ֡����
 * @note   
 * @param[in]  uart: UART ����
 * @param[in]  data: Ҫ���͵�����
 * @param[in]  len: Ҫ���͵����ݳ��ȣ���λ byte����󳤶�: 65535 byte
 * @param[in]  timeout: �����ʱʱ�䣬��λ ms�����ָ��ʱ��: 65535 ms
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_Send(struct UART_STRUCT *uart, const uint8_t *data, uint16_t len, uint16_t timeout)
{
    _UART_Alternate(uart);

    if (timeout == 0)
    {
        if (uart->handle.hdmatx)
            return (BSP_UART_ERR)HAL_UART_Transmit_DMA(&uart->handle, (uint8_t *)data, len);
        else
            return (BSP_UART_ERR)HAL_UART_Transmit_IT(&uart->handle, (uint8_t *)data, len);
    }
    else
        return (BSP_UART_ERR)HAL_UART_Transmit(&uart->handle, (uint8_t *)data, len, timeout);
}


#if (ENABLE_DEBUG_PRINT && EANBLE_PRINTF_USING_RTT == 0)
/**
 * @brief  ͬ printf
 * @note   ��ʹ��RTOS���� BSP_Printf �������ж���ʹ��
 * @param[in]  fmt: ��ʽ���ַ�
 * @param[in]  ...: ����������
 * @retval None
 */
void BSP_Printf(const char *fmt, ...)
{
    if (BSP_PRINTF_HANDLE.init == 0)
        return;

    BSP_UART_Port_TxLock(&BSP_PRINTF_HANDLE);
    
    va_list args;
    uint16_t len;
    static char buff[BSP_PRINTF_BUFF_SIZE];
    
    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    memset(buff, 0, BSP_PRINTF_BUFF_SIZE);
    len = vsnprintf(buff, sizeof(buff) - 1, fmt, args);
    if (len > BSP_PRINTF_BUFF_SIZE - 1)
        len = BSP_PRINTF_BUFF_SIZE - 1;
    
    #if (USING_RTOS_TYPE)
        BSP_UART_Port_Send(&BSP_PRINTF_HANDLE, (uint8_t *)buff, len, 0);
    #else
        BSP_UART_Port_Send(&BSP_PRINTF_HANDLE, (uint8_t *)buff, len, 0xFFFF);
    #endif
    
    va_end(args);
}
#endif


/**
 * @brief  �� UART ��ȡһ���ֽڵ�����
 * @note   
 * @param[in]  uart: UART ����
 * @retval UART ����
 */
inline uint32_t BSP_UART_Port_GetOneByte(struct UART_STRUCT *uart)
{
    return uart->handle.Instance->DR;
}


/**
 * @brief  ��ȡ DMA ��ǰ�ļ���
 * @note   
 * @param[in]  uart: UART ����
 * @retval ����ֵ
 */
inline uint32_t BSP_UART_Port_GetDmaCounter(struct UART_STRUCT *uart)
{
    return __HAL_DMA_GET_COUNTER(uart->handle.hdmarx);
}


/**
 * @brief  ͨ�� UART ID ��ȡ UART ����
 * @note   
 * @param[in]  id: ���� ID
 * @retval NULL: ��Ч�� ID���� NULL: UART ����
 */
struct UART_STRUCT *BSP_UART_Port_GetHandle(BSP_UART_ID id)
{
    for (uint8_t i = 0; i < _uart_qty; i++)
    {
        if (_uart_group[i]->id == id)
        {
            return _uart_group[i];
        }
    }
    
    return NULL;
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  UART �жϵļ��д���
 * @note   
 * @param[in]  uart: UART ����
 * @retval None
 */
static inline void _UART_IntHandler(struct UART_STRUCT *uart)
{
    if (uart->handle.Instance == NULL)
        return;
    
    if (__HAL_UART_GET_FLAG(&uart->handle, UART_FLAG_RXNE) != RESET)
    {
        /* RXNE �жϱ�־λ���ڶ�ȡ DR �Ĵ�����ʱ�����㣬���޷�ȷ�� _UART_RxCallback �Ƿ���ȡDR�Ĵ�����
           �����ڴ˴�����һ������ */
        __HAL_UART_CLEAR_FLAG(&uart->handle, UART_FLAG_RXNE);
        
        if (_UART_RxCallback)
            _UART_RxCallback(uart);
    }
    else if ((__HAL_UART_GET_FLAG(&uart->handle, UART_FLAG_IDLE) != RESET)
    &&       (__HAL_UART_GET_IT_SOURCE(&uart->handle, UART_IT_IDLE) != RESET))
    {
        /* IDLE �жϱ�־λֻ��������� */
        __HAL_UART_CLEAR_IDLEFLAG(&uart->handle);

        if (_UART_RxIdleCallback)
            _UART_RxIdleCallback(uart);
    }
    else
    {
        HAL_UART_IRQHandler(&(uart->handle));
    }
}


/**
 * @brief  ���ڸ����л�����
 * @note   
 * @param[in]  uart: UART ����
 * @retval None
 */
static inline void _UART_Alternate(struct UART_STRUCT *uart)
{
#if (BSP_USING_UART2_RE || BSP_USING_UART3_RE)
    switch (uart->id)
    {
        case BSP_UART2:
        case BSP_UART2_RE:
        {
            if (_uart2_alter != uart->id)    /* �����л� */
            {
                if (uart->id == BSP_UART2)
                {
                    BSP_GPIO_SetMode(GET_PIN(A, 2), GPIO_MODE_AF_PP, GPIO_PULLUP);  /* TX */
                    BSP_GPIO_SetMode(GET_PIN(A, 3), GPIO_MODE_INPUT, GPIO_PULLUP);  /* RX */
                    __HAL_AFIO_REMAP_USART2_DISABLE();
                }
                else
                {
                    BSP_GPIO_SetMode(GET_PIN(D, 5), GPIO_MODE_AF_PP, GPIO_PULLUP);  /* TX */
                    BSP_GPIO_SetMode(GET_PIN(D, 6), GPIO_MODE_INPUT, GPIO_PULLUP);  /* RX */
                    __HAL_AFIO_REMAP_USART2_ENABLE();
                }
                _uart2_alter = uart->id;
            }
            break;
        }
        case BSP_UART3:
        case BSP_UART3_RE:
        {
            if (_uart3_alter != uart->id)    /* �����л� */
            {
                if (uart->id == BSP_UART3)
                {
                    BSP_GPIO_SetMode(GET_PIN(B, 10), GPIO_MODE_AF_PP, GPIO_PULLUP); /* TX */
                    BSP_GPIO_SetMode(GET_PIN(B, 11), GPIO_MODE_INPUT, GPIO_PULLUP); /* RX */
                    __HAL_AFIO_REMAP_USART3_DISABLE();
                }
                else
                {
                    BSP_GPIO_SetMode(GET_PIN(D, 8), GPIO_MODE_AF_PP, GPIO_PULLUP);  /* TX */
                    BSP_GPIO_SetMode(GET_PIN(D, 9), GPIO_MODE_INPUT, GPIO_PULLUP);  /* RX */
                    __HAL_AFIO_REMAP_USART3_ENABLE();
                }
                _uart3_alter = uart->id;
            }
            break;
        }
        default: break;
    }
#endif
}


/* Callback functions ---------------------------------------------------------*/
/* ���� 4 ���������� HAL �� UART ��ص��жϺ����Ļص����� */
/* UART DMA ���հ����ж� */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    struct UART_STRUCT *uart = (struct UART_STRUCT *)huart;
    
    if (_UART_DMA_RxCallback)
        _UART_DMA_RxCallback(uart);
}

/* UART DMA ����ȫ���ж� */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    struct UART_STRUCT *uart = (struct UART_STRUCT *)huart;
    
    if (_UART_DMA_RxCallback)
        _UART_DMA_RxCallback(uart);
}

/* UART DMA ��������ж� */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    struct UART_STRUCT *uart = (struct UART_STRUCT *)huart;
    
    if (_UART_DMA_TxCallback)
        _UART_DMA_TxCallback(uart); 
}

/* UART �жϼ�⵽����Ļص����� */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    struct UART_STRUCT *uart = (struct UART_STRUCT *)huart;

    BSP_Printf("%s: %d %d\r\n", __FUNCTION__, uart->id, huart->ErrorCode);
}


/* Interrupt request functions ---------------------------------------------------------*/
#if (BSP_USING_UART1)
void USART1_IRQHandler(void)
{
    BSP_INT_ENTER();
    
    _UART_IntHandler(&UART(1));
    
    BSP_INT_EXIT();
}

void UART1_DMA_RX_IRQHandler(void)
{
    BSP_INT_ENTER();

    if (UART(1).rx_init)
    {    
        HAL_DMA_IRQHandler(UART(1).handle.hdmarx);
    }

    BSP_INT_EXIT();
}

void UART1_DMA_TX_IRQHandler(void)
{
    BSP_INT_ENTER();

    HAL_DMA_IRQHandler(UART(1).handle.hdmatx);

    BSP_INT_EXIT();
}
#endif

#if (BSP_USING_UART2)
void USART2_IRQHandler(void)
{
    BSP_INT_ENTER();

#if (BSP_USING_UART2_RE)
    if (_uart2_alter == BSP_UART2)
        _UART_IntHandler(&UART(2));
    else
        _UART_IntHandler(&UART(2_RE));
#else
    _UART_IntHandler(&UART(2));
#endif
    
    BSP_INT_EXIT();
}

void UART2_DMA_RX_IRQHandler(void)
{
    BSP_INT_ENTER();
    
    if (UART(2).rx_init)
    {
    #if (BSP_USING_UART2_RE)
        if (_uart2_alter == BSP_UART2)
            HAL_DMA_IRQHandler(UART(2).handle.hdmarx);
        else
            HAL_DMA_IRQHandler(UART(2_RE).handle.hdmarx);
    #else
        HAL_DMA_IRQHandler(UART(2).handle.hdmarx);
    #endif
    }

    BSP_INT_EXIT();
}

void UART2_DMA_TX_IRQHandler(void)
{
    BSP_INT_ENTER();

#if (BSP_USING_UART2_RE)
    if (_uart2_alter == BSP_UART2)
        HAL_DMA_IRQHandler(UART(2).handle.hdmatx);
    else
        HAL_DMA_IRQHandler(UART(2_RE).handle.hdmarx);
#else
    HAL_DMA_IRQHandler(UART(2).handle.hdmarx);
#endif

    BSP_INT_EXIT();
}
#endif

#if (BSP_USING_UART3)
void USART3_IRQHandler(void)
{
    BSP_INT_ENTER();

#if (BSP_USING_UART3_RE)
    if (_uart3_alter == BSP_UART3)
        _UART_IntHandler(&UART(3));
    else
        _UART_IntHandler(&UART(3_RE));
#else
    _UART_IntHandler(&UART(3));
#endif

    BSP_INT_EXIT();
}

void UART3_DMA_RX_IRQHandler(void)
{
    BSP_INT_ENTER();
    
    if (UART(3).rx_init)
    {    
    #if (BSP_USING_UART3_RE)
        if (_uart3_alter == BSP_UART3)
            HAL_DMA_IRQHandler(UART(3).handle.hdmarx);
        else
            HAL_DMA_IRQHandler(UART(3_RE).handle.hdmarx);
    #else
        HAL_DMA_IRQHandler(UART(3).handle.hdmarx);
    #endif
    }

    BSP_INT_EXIT();
}

void UART3_DMA_TX_IRQHandler(void)
{
    BSP_INT_ENTER();

#if (BSP_USING_UART3_RE)
    if (_uart3_alter == BSP_UART3)
        HAL_DMA_IRQHandler(UART(3).handle.hdmarx);
    else
        HAL_DMA_IRQHandler(UART(3_RE).handle.hdmarx);
#else
    HAL_DMA_IRQHandler(UART(3).handle.hdmarx);
#endif

    BSP_INT_EXIT();
}
#endif

#if (BSP_USING_UART4)
void UART4_IRQHandler(void)
{
    BSP_INT_ENTER();

    _UART_IntHandler(&UART(4));

    BSP_INT_EXIT();
}

void UART4_DMA_RX_IRQHandler(void)
{
    BSP_INT_ENTER();

    if (UART(4).rx_init)
    {    
        HAL_DMA_IRQHandler(UART(4).handle.hdmarx);
    }

    BSP_INT_EXIT();
}

void UART4_DMA_TX_IRQHandler(void)
{
    BSP_INT_ENTER();

    HAL_DMA_IRQHandler(UART(4).handle.hdmatx);

    BSP_INT_EXIT();
}
#endif

#if (BSP_USING_UART5)
void UART5_IRQHandler(void)
{
    BSP_INT_ENTER();

    _UART_IntHandler(&UART(5));

    BSP_INT_EXIT();
}

void UART5_DMA_RX_IRQHandler(void)
{
    BSP_INT_ENTER();

    if (UART(5).rx_init)
    {    
        HAL_DMA_IRQHandler(UART(5).handle.hdmarx);
    }

    BSP_INT_EXIT();
}

void UART5_DMA_TX_IRQHandler(void)
{
    BSP_INT_ENTER();

    HAL_DMA_IRQHandler(UART(5).handle.hdmatx);

    BSP_INT_EXIT();
}
#endif

#if (BSP_USING_UART6)
void USART6_IRQHandler(void)
{
    BSP_INT_ENTER();

    _UART_IntHandler(&UART(6));

    BSP_INT_EXIT();
}

void UART6_DMA_RX_IRQHandler(void)
{
    BSP_INT_ENTER();

    if (UART(6).rx_init)
    {    
        HAL_DMA_IRQHandler(UART(6).handle.hdmarx);
    }

    BSP_INT_EXIT();
}

void UART6_DMA_TX_IRQHandler(void)
{
    BSP_INT_ENTER();

    HAL_DMA_IRQHandler(UART(6).handle.hdmatx);

    BSP_INT_EXIT();
}
#endif




