#ifndef BSP_UART_DRV_PORT_H
#define BSP_UART_DRV_PORT_H

#include "bsp_common.h"

#define BSP_USING_UART1                     1

/* 定义项 */
typedef enum 
{
#if (BSP_USING_UART1)
    BSP_UART1    = 0x01,
#endif
#if (BSP_USING_UART2)
    BSP_UART2    = 0x02,
#endif
#if (BSP_USING_UART2_RE)
    BSP_UART2_RE = 0x82,
#endif
#if (BSP_USING_UART3)
    BSP_UART3    = 0x03,
#endif
#if (BSP_USING_UART3_RE)
    BSP_UART3_RE = 0x83,
#endif
#if (BSP_USING_UART4)
    BSP_UART4    = 0x04,
#endif
#if (BSP_USING_UART5)
    BSP_UART5    = 0x05,
#endif
#if (BSP_USING_UART6)
    BSP_UART6    = 0x06,
#endif
} BSP_UART_ID;

typedef enum 
{
    BSP_UART_ERR_OK                 = 0x00U,
    BSP_UART_ERR_COMM_ERR           = 0x01U,        /* 通讯错误（源自 HAL 库） */
    BSP_UART_ERR_BUSY               = 0x02U,        /* UART 外设忙碌（源自 HAL 库） */
    BSP_UART_ERR_TIMEOUT            = 0x03U,        /* 处理时间超时（源自 HAL 库） */
    BSP_UART_ERR_NOT_FOUND          = 0x04U,        /* 未找到对应的 UART 对象 */
    BSP_UART_ERR_LOCK_INIT_ERR      = 0x05U,        /* 通讯信号锁初始化失败 */
    BSP_UART_ERR_LOCK_ERR           = 0x06U,        /* 通讯信号上锁失败 */
    BSP_UART_ERR_UNLOCK_ERR         = 0x07U,        /* 通讯信号解锁失败 */
    BSP_UART_ERR_NO_RECV_FRAME      = 0x08U,        /* 还未收到一帧完整的数据 */
    BSP_UART_ERR_NO_INIT            = 0x09U,        /* 使用的 UART 对象还未初始化 */
    BSP_UART_ERR_NAME_DUPLICATE     = 0x0AU,        /* UART 对象命名重复 */

} BSP_UART_ERR;

struct UART_STRUCT
{
    /* 串口唯一标识信息 */
    const uint8_t id;

    /* 用户串口数据，需要用户传入（二级缓存），当一级缓存半满和全满时，将会写入二级缓存 */
    uint8_t  *rx_data;
    uint16_t * volatile rx_data_len;
    uint16_t rx_data_max_len;
    
    /* 一些标志位 */
    volatile bool is_init;         /* 串口组件初始化标志位 */
    volatile bool is_rx_init;      /* 串口组件的接收功能初始化标志位 */
    volatile bool is_idle_int;     /* 是否发生空闲中断的标志位 */
    
    /* 串口数据一级缓存（由 DMA 无条件写入） */
    const uint8_t  *rx_buff;
    const uint16_t rx_buff_max_len;

    /* 环形缓存的“写”位置 */
    uint16_t old_pos;
    
    /* 回调函数 */
    uint8_t (*RX_Indicate)(struct UART_STRUCT *uart);
    uint8_t (*TX_Complete)(struct UART_STRUCT *uart);

    void *user_data;
};

typedef void (*UART_Callback_t)(struct UART_STRUCT *uart);



BSP_UART_ERR  BSP_UART_Init(BSP_UART_ID  id);

BSP_UART_ERR  BSP_UART_EnableReceive(BSP_UART_ID  id, uint8_t *data, uint16_t *len, uint16_t max_len);
BSP_UART_ERR  BSP_UART_Send(BSP_UART_ID  id, const uint8_t *data, uint16_t len, uint16_t timeout);

BSP_UART_ERR  BSP_UART_IsFrameEnd(BSP_UART_ID  id);
BSP_UART_ERR  BSP_UART_ClearUserBuff(BSP_UART_ID  id);
uint32_t BSP_UART_Port_GetDmaCounter(struct UART_STRUCT *uart);

#endif /* BSP_UART_DRV_PORT_H */