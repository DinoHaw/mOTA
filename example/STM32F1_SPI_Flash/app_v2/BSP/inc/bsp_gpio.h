#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__


#include "bsp_common.h"


#define GPIO_LOW                    0x00
#define GPIO_HIGH                   0x01

#define STM32_PORT(port)            GPIO##port##_BASE

#define GET_PIN(port, pin)          (uint32_t)((16 * ( ((uint32_t)STM32_PORT(port) - (uint32_t)GPIOA_BASE)/(0x0400UL) )) + pin)

#define STM32_PIN(index, gpio, gpio_index)          \
{                                                   \
    index, GPIO##gpio, GPIO_PIN_##gpio_index        \
}

/* STM32 GPIO driver */
struct GPIO_INDEX
{
    int index;
    GPIO_TypeDef *gpio;
    uint16_t pin;
};

void            BSP_GPIO_Write      (uint8_t io, uint8_t level);
uint8_t         BSP_GPIO_Read       (uint8_t io);
void            BSP_GPIO_Toggle     (uint8_t io);
void            BSP_GPIO_SetMode    (uint8_t io, uint32_t mode, uint32_t pull);

GPIO_TypeDef *  BSP_GPIO_GetPort    (uint8_t io);
uint16_t        BSP_GPIO_GetPin     (uint8_t io);

#endif

