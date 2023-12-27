/**
 * \file            bsp_uart_stm32.c
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
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2022-11-23     Dino         the first version
 * v1.1     2022-12-04     Dino         增加 __HAL_UART_FLUSH_DRREGISTER
 * v1.2     2023-12-15     Dino         优化 UART
 */


/* Includes ------------------------------------------------------------------*/
#include "bsp_uart.h"


/* Private variables ---------------------------------------------------------*/
#if (BSP_USING_UART2_RE)
static volatile BSP_UART_ID _uart2_alter;
#endif
#if (BSP_USING_UART3_RE)
static volatile BSP_UART_ID _uart3_alter;
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

#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
static struct rt_event _rx_event;
static struct rt_event _tx_event;
#endif


/* Function prototypes -------------------------------------------------------*/
BSP_UART_ERR  BSP_UART_Port_LockInit(struct UART_STRUCT *uart);


/* Private function prototypes -----------------------------------------------*/
static uint32_t _UART_GetMask                   (uint32_t word_length, uint32_t parity);
static void     _UART_IntHandler                (struct UART_STRUCT *uart);
static void     _UART_Alternate                 (struct UART_STRUCT *uart);
static void     _UART_AlternateEnableDMAReceive (struct UART_STRUCT *uart);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  UART 接口初始化
 * @note   需在 UART 外设初始化后或本函数处进行 UART 外设初始化后才可调用
 * @param[in]  uart: UART 对象
 * @param[in]  rx_callback: 接收单字节数据中断的回调函数
 * @param[in]  rx_idle_callback: 空闲中断的回调函数
 * @param[in]  dma_rx_callback: DMA 接收中断的回调函数
 * @param[in]  dma_tx_callback: DMA 发送完成的回调函数
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
     * 当 uart->handle 的外设由系统初始化时，例如 CubeMX 生成的初始化代码，以下赋值操作是必须的。
     * 否则，需要自己将 uart->handle 初始化。
     * 由此可见， BSP_UART_Port_Init 需在 UART 外设初始化后或本函数处进行 UART 外设初始化后才可调用。
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

    if (uart->handle.hdmatx) {
        uart->handle.hdmatx->Parent = &uart->handle;
    }
    
    /* Clean the input path */
    __HAL_UART_FLUSH_DRREGISTER(&uart->handle);

    _UART_Alternate(uart);
    
    BSP_Printf("-----------------------------\r\n");
    BSP_Printf("[ %s ]\r\n", __func__);
    BSP_Printf("id: %d\r\n", uart->id);
//    BSP_Printf("name: %s\r\n", uart->name);
    BSP_Printf("rx_buff: 0x%.8p\r\n", &uart->rx_buff[0]);
    BSP_Printf("uart size: %d byte\r\n", sizeof(BSP_PRINTF_HANDLE));
    BSP_Printf("-----------------------------\r\n\r\n");
}


/**
 * @brief  UART 接口通信控制锁的初始化
 * @note   
 * @param[in]  uart: UART 对象
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_LockInit(struct UART_STRUCT *uart)
{
    static bool is_init = false;

    if (is_init) {
        return BSP_UART_ERR_OK;
    }

#if (USING_RTOS_TYPE)
    char name[ MAX_NAME_LEN ] = {0};
    
    strncpy(name, uart->name, strlen(name));
    strncat(name, "_tx", 3);
    
    #if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_event_init(&_tx_event, name, RT_IPC_FLAG_PRIO) != RT_EOK)
    {
        return BSP_UART_ERR_LOCK_INIT_ERR;
    }
    _tx_event.set = 0xFFFFFFFF;

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
    strncat(name, "_rx", 3);
    
    #if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_event_init(&_rx_event, name, RT_IPC_FLAG_PRIO) != RT_EOK)
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

    is_init = true;
    
    return BSP_UART_ERR_OK;
}


/**
 * @brief  UART 等待接收数据的信号锁
 * @note   
 * @param[in]  uart: UART 对象
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_RxLock(struct UART_STRUCT *uart)
{
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    rt_uint32_t e;
    if (rt_event_recv(&_rx_event, uart->id,
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e) != RT_EOK)
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
 * @brief  UART 接收到数据发出的信号锁
 * @note   
 * @param[in]  uart: UART 对象
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_RxUnlock(struct UART_STRUCT *uart)
{
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_event_send(&_rx_event, uart->id) != RT_EOK)
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
 * @brief  UART 发送锁
 * @note   
 * @param[in]  uart: UART 对象
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_TxLock(struct UART_STRUCT *uart)
{
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    rt_uint32_t e;
    if (rt_event_recv(&_tx_event, uart->id,
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e) != RT_EOK)
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
 * @brief  UART 发送完毕解锁
 * @note   
 * @param[in]  uart: UART 对象
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_TxUnlock(struct UART_STRUCT *uart)
{  
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    if (rt_event_send(&_tx_event, uart->id) != RT_EOK)
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
 * @brief  使能 UART 接收数据
 * @note   
 * @param[in]  uart: UART 对象
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_EnableReceive(struct UART_STRUCT *uart)
{
    BSP_UART_ERR  ret = BSP_UART_ERR_OK;
    
    if (uart->handle.hdmarx) {
        uart->handle.hdmarx->Parent = &uart->handle;
    }

    _UART_Alternate(uart);

    if (uart->handle.hdmarx) {
        ret = (BSP_UART_ERR)HAL_UART_Receive_DMA(&uart->handle, (uint8_t *)uart->rx_buff, uart->rx_buff_max_len);
    } else {
        __HAL_UART_ENABLE_IT(&uart->handle, UART_IT_RXNE); 
    }
    
    /* 开启空闲中断 */
    __HAL_UART_ENABLE_IT(&uart->handle, UART_IT_IDLE);
    
    return ret;
}


/**
 * @brief  禁止 UART 接收数据
 * @note   
 * @param[in]  uart: UART 对象
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_DisableReceive(struct UART_STRUCT *uart)
{
    BSP_UART_ERR  ret = BSP_UART_ERR_OK;
    
    if (uart->handle.hdmarx) {
        ret = (BSP_UART_ERR)HAL_UART_DMAPause(&uart->handle);
    } else {
        __HAL_UART_DISABLE_IT(&uart->handle, UART_IT_RXNE); 
    }

    __HAL_UART_DISABLE_IT(&uart->handle, UART_IT_IDLE);
    
    return ret;
}


/**
 * @brief  控制 UART 发送一帧数据
 * @note   
 * @param[in]  uart: UART 对象
 * @param[in]  data: 要发送的数据
 * @param[in]  len: 要发送的数据长度，单位 byte。最大长度: 65535 byte
 * @param[in]  timeout: 最大处理超时时间，单位 ms。最大指定时间: 65535 ms
 * @retval BSP_UART_ERR
 */
BSP_UART_ERR  BSP_UART_Port_Send(struct UART_STRUCT *uart, const uint8_t *data, uint16_t len, uint16_t timeout)
{
    _UART_Alternate(uart);

    if (timeout == 0)
    {
        if (uart->handle.hdmatx) {
            return (BSP_UART_ERR)HAL_UART_Transmit_DMA(&uart->handle, (uint8_t *)data, len);
        } else {
            return (BSP_UART_ERR)HAL_UART_Transmit_IT(&uart->handle, (uint8_t *)data, len);
        }
    }
    else {
        return (BSP_UART_ERR)HAL_UART_Transmit(&uart->handle, (uint8_t *)data, len, timeout);
    }
}


#if (ENABLE_DEBUG_PRINT && EANBLE_PRINTF_USING_RTT == 0)
/**
 * @brief  同 printf
 * @note   若使用RTOS，则 BSP_Printf 不能在中断中使用
 * @param[in]  fmt: 格式化字符
 * @param[in]  ...: 不定长参数
 * @retval None
 */
void BSP_Printf(const char *fmt, ...)
{
    if (BSP_PRINTF_HANDLE.is_init == 0) {
        return;
    }

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
    if (len > BSP_PRINTF_BUFF_SIZE - 1) {
        len = BSP_PRINTF_BUFF_SIZE - 1;
    }
    
    #if (USING_RTOS_TYPE)
        BSP_UART_Port_Send(&BSP_PRINTF_HANDLE, (uint8_t *)buff, len, 0);
    #else
        BSP_UART_Port_Send(&BSP_PRINTF_HANDLE, (uint8_t *)buff, len, 0xFFFF);
    #endif
    
    va_end(args);
}
#endif


/**
 * @brief  从 UART 获取一个字节的数据
 * @note   
 * @param[in]  uart: UART 对象
 * @retval UART 数据
 */
uint32_t BSP_UART_Port_GetOneByte(struct UART_STRUCT *uart)
{
#if defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32F0) \
||  defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32H7) \
||  defined(SOC_SERIES_STM32G4)
    return (uart->handle.Instance->RDR & _UART_GetMask(uart->handle.Init.WordLength, uart->handle.Init.Parity));
#else
    return (uart->handle.Instance->DR & _UART_GetMask(uart->handle.Init.WordLength, uart->handle.Init.Parity));
#endif
}


/**
 * @brief  获取 DMA 当前的计数
 * @note   
 * @param[in]  uart: UART 对象
 * @retval 计数值
 */
uint32_t BSP_UART_Port_GetDmaCounter(struct UART_STRUCT *uart)
{
    return __HAL_DMA_GET_COUNTER(uart->handle.hdmarx);
}


/**
 * @brief  通过 UART ID 获取 UART 对象
 * @note   
 * @param[in]  id: 串口 ID
 * @retval NULL: 无效的 ID。非 NULL: UART 对象
 */
struct UART_STRUCT *BSP_UART_Port_GetHandle(BSP_UART_ID id)
{
    for (uint8_t i = 0; i < _uart_qty; i++)
    {
        if (_uart_group[i]->id == id) {
            return _uart_group[i];
        }
    }
    
    return NULL;
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  UART 中断的集中处理
 * @note   
 * @param[in]  uart: UART 对象
 * @retval None
 */
static inline void _UART_IntHandler(struct UART_STRUCT *uart)
{
    if (uart->handle.Instance == NULL) {
        return;
    }
    
    if (__HAL_UART_GET_FLAG(&uart->handle, UART_FLAG_RXNE) != RESET)
    {
        /* RXNE 中断标志位会在读取 DR 寄存器的时候清零，但无法确定 _UART_RxCallback 是否会读取DR寄存器，
           所以在此处进行一次清零 */
        __HAL_UART_CLEAR_FLAG(&uart->handle, UART_FLAG_RXNE);
        
        if (_UART_RxCallback) {
            _UART_RxCallback(uart);
        }
    }
    else if ((__HAL_UART_GET_FLAG(&uart->handle, UART_FLAG_IDLE) != RESET)
    &&       (__HAL_UART_GET_IT_SOURCE(&uart->handle, UART_IT_IDLE) != RESET))
    {
        /* IDLE 中断标志位只能软件清零 */
        __HAL_UART_CLEAR_IDLEFLAG(&uart->handle);

        if (_UART_RxIdleCallback) {
            _UART_RxIdleCallback(uart);
        }
    }
    else
    {
        HAL_UART_IRQHandler(&(uart->handle));
    }
}


/**
 * @brief  串口复用重新配置 DMA 接收
 * @note   
 * @param[in]  uart: UART 对象
 * @retval None
 */
static inline void _UART_AlternateEnableDMAReceive(struct UART_STRUCT *uart)
{
    uart->old_pos = 0;

    uart->handle.RxState       = HAL_UART_STATE_READY;
    uart->handle.hdmarx->State = HAL_DMA_STATE_READY;
    __HAL_UNLOCK(&uart->handle);
    __HAL_UNLOCK(uart->handle.hdmarx);
    HAL_UART_Receive_DMA(&uart->handle, (uint8_t *)uart->rx_buff, uart->rx_buff_max_len);
}


/**
 * @brief  串口复用切换函数
 * @note   
 * @param[in]  uart: UART 对象
 * @retval None
 */
static inline void _UART_Alternate(struct UART_STRUCT *uart)
{
    #include "bsp_gpio_stm32.h"
    
    switch (uart->id)
    {
#if (BSP_USING_UART2_RE)
        case BSP_UART2:
        case BSP_UART2_RE:
        {
            if (_uart2_alter != uart->id)    /* 复用切换 */
            {
                if (uart->id == BSP_UART2)
                {
                    BSP_GPIO_SetMode(GET_PIN(A, 2), GPIO_MODE_AF_PP, GPIO_PULLUP);  /* TX */
                    BSP_GPIO_SetMode(GET_PIN(A, 3), GPIO_MODE_INPUT, GPIO_PULLUP);  /* RX */
                
                    __HAL_AFIO_REMAP_USART2_DISABLE();

                    BSP_GPIO_Write(GET_PIN(D, 5), GPIO_HIGH);
                    BSP_GPIO_Write(GET_PIN(D, 6), GPIO_HIGH);
                    BSP_GPIO_SetMode(GET_PIN(D, 5), GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
                    BSP_GPIO_SetMode(GET_PIN(D, 6), GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
                }
                else
                {
                    BSP_GPIO_SetMode(GET_PIN(D, 5), GPIO_MODE_AF_PP, GPIO_PULLUP);  /* TX */
                    BSP_GPIO_SetMode(GET_PIN(D, 6), GPIO_MODE_INPUT, GPIO_PULLUP);  /* RX */

                    __HAL_AFIO_REMAP_USART2_ENABLE();

                    BSP_GPIO_Write(GET_PIN(A, 2), GPIO_HIGH);
                    BSP_GPIO_Write(GET_PIN(A, 3), GPIO_HIGH);
                    BSP_GPIO_SetMode(GET_PIN(A, 2), GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
                    BSP_GPIO_SetMode(GET_PIN(A, 3), GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
                }
                if (uart->handle.hdmarx) {
                    _UART_AlternateEnableDMAReceive(uart);
                }
                _uart2_alter = uart->id;
            }
            break;
        }
#endif
#if (BSP_USING_UART3_RE)
        case BSP_UART3:
        case BSP_UART3_RE:
        {
            if (_uart3_alter != uart->id)    /* 复用切换 */
            {
                if (uart->id == BSP_UART3)
                {
                    BSP_GPIO_SetMode(GET_PIN(B, 10), GPIO_MODE_AF_PP, GPIO_PULLUP); /* TX */
                    BSP_GPIO_SetMode(GET_PIN(B, 11), GPIO_MODE_INPUT, GPIO_PULLUP); /* RX */

                    __HAL_AFIO_REMAP_USART3_DISABLE();

                    BSP_GPIO_Write(GET_PIN(D, 8), GPIO_HIGH);
                    BSP_GPIO_Write(GET_PIN(D, 9), GPIO_HIGH);
                    BSP_GPIO_SetMode(GET_PIN(D, 8), GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
                    BSP_GPIO_SetMode(GET_PIN(D, 9), GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
                }
                else
                {
                    BSP_GPIO_SetMode(GET_PIN(D, 8), GPIO_MODE_AF_PP, GPIO_PULLUP);  /* TX */
                    BSP_GPIO_SetMode(GET_PIN(D, 9), GPIO_MODE_INPUT, GPIO_PULLUP);  /* RX */

                    __HAL_AFIO_REMAP_USART3_ENABLE();

                    BSP_GPIO_SetMode(GET_PIN(B, 10), GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
                    BSP_GPIO_SetMode(GET_PIN(B, 11), GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
                    BSP_GPIO_Write(GET_PIN(B, 10), GPIO_HIGH);
                    BSP_GPIO_Write(GET_PIN(B, 11), GPIO_HIGH);
                }
                if (uart->handle.hdmarx) {
                    _UART_AlternateEnableDMAReceive(uart);
                }
                _uart3_alter = uart->id;
            }
            break;
        }
#endif
        default: break;
    }
}


/**
 * @brief  获取 UART 掩码
 * @note   
 * @param[in]  word_length: UART 数据长度
 * @param[in]  parity: UART 奇偶校验位
 * @retval 掩码
 */
static uint32_t _UART_GetMask(uint32_t word_length, uint32_t parity)
{
    uint32_t mask;

    if (word_length == UART_WORDLENGTH_8B)
    {
        if (parity == UART_PARITY_NONE) {
            mask = 0x00FFU;
        } else {
            mask = 0x007FU;
        }
    }
#ifdef UART_WORDLENGTH_9B
    else if (word_length == UART_WORDLENGTH_9B)
    {
        if (parity == UART_PARITY_NONE) {
            mask = 0x01FFU;
        } else {
            mask = 0x00FFU;
        }
    }
#endif
#ifdef UART_WORDLENGTH_7B
    else if (word_length == UART_WORDLENGTH_7B)
    {
        if (parity == UART_PARITY_NONE) {
            mask = 0x007FU;
        } else {
            mask = 0x003FU;
        }
    }
    else {
        mask = 0x0000U;
    }
#endif
    return mask;
}


/* Callback functions ---------------------------------------------------------*/
/* 以下 4 个函数均是 HAL 库 UART 相关的中断函数的回调函数 */
/* UART DMA 接收半满中断 */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    struct UART_STRUCT *uart = (struct UART_STRUCT *)huart;
    
    if (_UART_DMA_RxCallback) {
        _UART_DMA_RxCallback(uart);
    }
}

/* UART DMA 接收全满中断 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    struct UART_STRUCT *uart = (struct UART_STRUCT *)huart;
    
    if (_UART_DMA_RxCallback) {
        _UART_DMA_RxCallback(uart);
    }
}

/* UART DMA 发送完成中断 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    struct UART_STRUCT *uart = (struct UART_STRUCT *)huart;
    
    if (_UART_DMA_TxCallback) {
        _UART_DMA_TxCallback(uart); 
    }
}

/* UART 中断检测到错误的回调函数 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    struct UART_STRUCT *uart = (struct UART_STRUCT *)huart;

    BSP_Printf("[%s] UART%d error: %d\r\n", __FUNCTION__, uart->id, huart->ErrorCode);
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

    if (UART(1).is_rx_init)
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
    if (_uart2_alter == BSP_UART2) {
        _UART_IntHandler(&UART(2));
    } else {
        _UART_IntHandler(&UART(2_RE));
    }
#else
    _UART_IntHandler(&UART(2));
#endif
    
    BSP_INT_EXIT();
}

void UART2_DMA_RX_IRQHandler(void)
{
    BSP_INT_ENTER();
    
    if (UART(2).is_rx_init)
    {
    #if (BSP_USING_UART2_RE)
        if (_uart2_alter == BSP_UART2) {
            HAL_DMA_IRQHandler(UART(2).handle.hdmarx);
        } else {
            HAL_DMA_IRQHandler(UART(2_RE).handle.hdmarx);
        }
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
    if (_uart2_alter == BSP_UART2) {
        HAL_DMA_IRQHandler(UART(2).handle.hdmatx);
    } else {
        HAL_DMA_IRQHandler(UART(2_RE).handle.hdmarx);
    }
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
    if (_uart3_alter == BSP_UART3) {
        _UART_IntHandler(&UART(3));
    } else {
        _UART_IntHandler(&UART(3_RE));
    }
#else
    _UART_IntHandler(&UART(3));
#endif

    BSP_INT_EXIT();
}

void UART3_DMA_RX_IRQHandler(void)
{
    BSP_INT_ENTER();
    
    if (UART(3).is_rx_init)
    {    
    #if (BSP_USING_UART3_RE)
        if (_uart3_alter == BSP_UART3) {
            HAL_DMA_IRQHandler(UART(3).handle.hdmarx);
        } else {
            HAL_DMA_IRQHandler(UART(3_RE).handle.hdmarx);
        }
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
    if (_uart3_alter == BSP_UART3) {
        HAL_DMA_IRQHandler(UART(3).handle.hdmarx);
    } else {
        HAL_DMA_IRQHandler(UART(3_RE).handle.hdmarx);
    }
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

    if (UART(4).is_rx_init)
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

    if (UART(5).is_rx_init)
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

    if (UART(6).is_rx_init)
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




