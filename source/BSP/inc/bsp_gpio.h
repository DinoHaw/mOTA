/**
 * \file            bsp_gpio.h
 * \brief           gpio driver
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

