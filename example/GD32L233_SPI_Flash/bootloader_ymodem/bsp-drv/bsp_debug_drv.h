#ifndef BSP_DEBUG_DRV_H
#define BSP_DEBUG_DRV_H

#include "bsp_common.h"

/* V0.1
 COM_DEBUG_TX      | PA9   | USART | 
 ------------------|-------|-------|-----------------------
 COM_DEBUG_RX      | PA10  | USART | 
 ------------------|-------|-------|-----------------------
 COM_DEBUG_CTS     |       | USART | 
 ------------------|-------|-------|-----------------------
 COM_DEBUG_RTS     | 	   | USART | 
 ------------------|-------|-------|-----------------------
 COM_DEBUG_PWR     | PA12  | GPIO  |
 ------------------|-------|-------|-----------------------
 
 ------------------|-------|-------|-----------------------
 COM_DEBUG_UARTx   |       |       | USART0
*/

/************ DEBUG 引脚定义**********/
#define COM_DEBUG_UARTx         USART0
#define COM_DEBUG_CLK           RCU_USART0

#define COM_DEBUG_TX_PORT       GPIOA
#define COM_DEBUG_TX_PIN        GPIO_PIN_9      /* DEBUG 的 TX 信号 */
#define COM_DEBUG_TX_CLK        RCU_GPIOA

#define COM_DEBUG_RX_PORT       GPIOA
#define COM_DEBUG_RX_PIN        GPIO_PIN_10     /* DEBUG 的 RX 信号 */
#define COM_DEBUG_RX_CLK        RCU_GPIOA

/* DEBUG 模块硬件初始化 */
void bsp_debug_hardware_init(void);

/* DEBUG 发送一个字节 */
void bsp_debug_send_byte(uint8_t ch);
/* DEBUG 发送字符串 */
void bsp_debug_send_string(char *str,uint32_t cnt);

#endif /* BSP_DEBUG_DRV_H */