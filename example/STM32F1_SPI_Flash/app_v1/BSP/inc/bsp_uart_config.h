#ifndef __BSP_UART_CONFIG_H__
#define __BSP_UART_CONFIG_H__

#include "bsp_config.h"

#if (ENABLE_DEBUG_PRINT)
    #if (EANBLE_PRINTF_USING_RTT)
    #define BSP_Printf(...)                 SEGGER_RTT_printf(SEGGER_RTT_PRINTF_TERMINAL, __VA_ARGS__)
    #endif
#else
    #define BSP_Printf(...)
#endif

/* 配置项 */
#define UART1_HANDLE                    huart1
#define UART2_HANDLE                    huart2
#define UART3_HANDLE                    huart3
#define UART4_HANDLE                    huart4
#define UART5_HANDLE                    huart5
#define UART6_HANDLE                    huart6

/* 移植时到不同型号的单片机时需要修改 */
#if defined(STM32F411xE)
#define UART1_DMA_RX_IRQHandler         DMA2_Stream2_IRQHandler
#define UART1_DMA_TX_IRQHandler         DMA2_Stream7_IRQHandler 
#define UART2_DMA_RX_IRQHandler         DMA1_Stream5_IRQHandler
#define UART2_DMA_TX_IRQHandler         DMA1_Stream6_IRQHandler
#define UART6_DMA_RX_IRQHandler         DMA2_Stream1_IRQHandler
#define UART6_DMA_TX_IRQHandler         DMA2_Stream6_IRQHandler    
#elif defined(STM32F407xx)
#define UART1_DMA_RX_IRQHandler         DMA2_Stream2_IRQHandler
#define UART1_DMA_TX_IRQHandler         DMA2_Stream7_IRQHandler
#define UART2_DMA_RX_IRQHandler         DMA1_Stream5_IRQHandler
#define UART2_DMA_TX_IRQHandler         DMA1_Stream6_IRQHandler
#define UART3_DMA_RX_IRQHandler         DMA1_Stream1_IRQHandler
#define UART3_DMA_TX_IRQHandler         DMA1_Stream3_IRQHandler
#define UART4_DMA_RX_IRQHandler         DMA1_Stream2_IRQHandler
#define UART4_DMA_TX_IRQHandler         DMA1_Stream4_IRQHandler
#define UART5_DMA_RX_IRQHandler         DMA1_Stream0_IRQHandler
#define UART5_DMA_TX_IRQHandler         DMA1_Stream7_IRQHandler
#define UART6_DMA_RX_IRQHandler         DMA2_Stream1_IRQHandler
#define UART6_DMA_TX_IRQHandler         DMA2_Stream6_IRQHandler
#elif defined(STM32F103xE) || defined(STM32F103xB)
#define UART1_DMA_RX_IRQHandler         DMA1_Channel5_IRQHandler
#define UART1_DMA_TX_IRQHandler         DMA1_Channel4_IRQHandler
#define UART2_DMA_RX_IRQHandler         DMA1_Channel6_IRQHandler
#define UART3_DMA_RX_IRQHandler         DMA1_Channel3_IRQHandler
#define UART4_DMA_RX_IRQHandler         DMA2_Channel3_IRQHandler
#endif

#endif
