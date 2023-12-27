#ifndef __BSP_UART_STM32_H__
#define __BSP_UART_STM32_H__

#include "bsp_common.h"

#if (ENABLE_DEBUG_PRINT)
    #if (EANBLE_PRINTF_USING_RTT)
    #define BSP_Printf(...)             SEGGER_RTT_printf(SEGGER_RTT_PRINTF_TERMINAL, __VA_ARGS__)
    #endif
#else
    #define BSP_Printf(...)
#endif


#define BSP_UART_ENABLE_RX(UARTx)       UARTx.Instance->CR1 |= (uint32_t)0x0004
#define BSP_UART_DISABLE_RX(UARTx)      UARTx.Instance->CR1 &= (~(uint32_t)0x0004)


#if (BSP_USING_UART1)
extern UART_HandleTypeDef huart1;
#endif
#if (BSP_USING_UART2) || (BSP_USING_UART2_RE)
extern UART_HandleTypeDef huart2;
#endif
#if (BSP_USING_UART3) || (BSP_USING_UART3_RE)
extern UART_HandleTypeDef huart3;
#endif
#if (BSP_USING_UART4)
extern UART_HandleTypeDef huart4;
#endif
#if (BSP_USING_UART5)
extern UART_HandleTypeDef huart5;
#endif
#if (BSP_USING_UART6)
extern UART_HandleTypeDef huart6;
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
#elif defined(STM32L475xx)
#define UART1_DMA_RX_IRQHandler         DMA1_Channel5_IRQHandler
#define UART1_DMA_TX_IRQHandler         DMA1_Channel4_IRQHandler
#endif

#endif
