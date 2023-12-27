#ifndef __BSP_GPIO_STM32_H__
#define __BSP_GPIO_STM32_H__

#include "bsp_common.h"

#define STM32_PORT(port)            GPIO##port##_BASE

#define GET_PIN(port, pin)          (uint32_t)((16 * ( ((uint32_t)STM32_PORT(port) - (uint32_t)GPIOA_BASE)/(0x0400UL) )) + pin)

#define STM32_PIN(index, gpio, gpio_index)          \
{                                                   \
    index, GPIO##gpio, GPIO_PIN_##gpio_index        \
}

typedef enum
{
    GPIO_LOW  = 0x00,
    GPIO_HIGH = !GPIO_LOW,

} BSP_GPIO_LEVEL;


/* STM32 GPIO driver */
struct GPIO_INDEX
{
    int index;
    GPIO_TypeDef *gpio;
    uint16_t pin;
};


void            BSP_GPIO_Write      (uint8_t io, BSP_GPIO_LEVEL level);
BSP_GPIO_LEVEL  BSP_GPIO_Read       (uint8_t io);
void            BSP_GPIO_Toggle     (uint8_t io);
void            BSP_GPIO_SetMode    (uint8_t io, uint32_t mode, uint32_t pull);


#endif

