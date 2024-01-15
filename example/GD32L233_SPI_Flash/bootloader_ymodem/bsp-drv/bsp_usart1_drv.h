#ifndef BSP_USART1_DRV_H
#define BSP_USART1_DRV_H

#include "gd32l23x.h"

/* V0.1
 COM_USART1_TX        | PA2   | USART | 
 ---------------------|-------|-------|-----------------------
 COM_USART1_RX        | PA3   | USART | 
 ---------------------|-------|-------|-----------------------
 
 ---------------------|-------|-------|-----------------------
 COM_USART1           |       |       | USART1

*/

/************ USART1 连接引脚定义**********/

//USART
#define COM_USART1                 USART1
#define COM_USART1_CLK             RCU_USART1

#define COM_USART1_TX_PORT         GPIOA
#define COM_USART1_TX_PIN          GPIO_PIN_2  /* 连接 USART1 的 TX 信号 */
#define COM_USART1_TX_CLK          RCU_GPIOA

#define COM_USART1_RX_PORT         GPIOA
#define COM_USART1_RX_PIN          GPIO_PIN_3  /* 连接 USART1 的 RX 信号 */
#define COM_USART1_RX_CLK          RCU_GPIOA


/* USART1 连接硬件初始化 */
void bsp_usart1_hardware_init(void);
void bsp_usart1_hardware_deinit(void);
/* 向 USART1 发送一个字节 */
void bsp_usart1_send_byte(uint8_t ch);
/* 向 USART1 发送字符串 */
void bsp_usart1_send_string(char *str, uint32_t cnt);

#endif /* BSP_USART1_DRV_H */