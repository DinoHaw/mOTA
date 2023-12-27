/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-5-10      ShiHao       first version
 * 2023-9-11      Dino         simplified version
 */

/* Includes ------------------------------------------------------------------*/
#include "bsp_gpio_stm32.h"


/* Definition ----------------------------------------------------------------*/
#define GPIO_PORT(port)             ((uint8_t)(((port) >> 4) & 0xFu))
#define GPIO_PIN(pin)               ((uint8_t)((pin) & 0xFu))

#if defined(SOC_SERIES_STM32MP1)
    #if defined(GPIOZ)
    #define GPIOZ_PORT_BASE         (175)   /* GPIO_STM32_PORT_MAX * 16 - 16 */
    #define GPIO_STM32_PORT(pin)    ((pin > GPIOZ_PORT_BASE) ? ((GPIO_TypeDef *)(GPIOZ_BASE )) : ((GPIO_TypeDef *)(GPIOA_BASE + (0x1000u * GPIO_PORT(pin)))))
    #else
    #define GPIO_STM32_PORT(pin)    ((GPIO_TypeDef *)(GPIOA_BASE + (0x1000u * GPIO_PORT(pin))))
    #endif /* GPIOZ */
#else
#define GPIO_STM32_PORT(pin)        ((GPIO_TypeDef *)(GPIOA_BASE + (0x400u * GPIO_PORT(pin))))
#endif /* SOC_SERIES_STM32MP1 */

#define GPIO_STM32_PIN(pin)         ((uint16_t)(1u << GPIO_PIN(pin)))

#if defined(GPIOZ)
#define __STM32_PORT_MAX    12u
#elif defined(GPIOK)
#define __STM32_PORT_MAX    11u
#elif defined(GPIOJ)
#define __STM32_PORT_MAX    10u
#elif defined(GPIOI)
#define __STM32_PORT_MAX    9u
#elif defined(GPIOH)
#define __STM32_PORT_MAX    8u
#elif defined(GPIOG)
#define __STM32_PORT_MAX    7u
#elif defined(GPIOF)
#define __STM32_PORT_MAX    6u
#elif defined(GPIOE)
#define __STM32_PORT_MAX    5u
#elif defined(GPIOD)
#define __STM32_PORT_MAX    4u
#elif defined(GPIOC)
#define __STM32_PORT_MAX    3u
#elif defined(GPIOB)
#define __STM32_PORT_MAX    2u
#elif defined(GPIOA)
#define __STM32_PORT_MAX    1u
#else
#define __STM32_PORT_MAX    0u
#error Unsupported STM32 GPIO peripheral.
#endif

#define GPIO_STM32_PORT_MAX     __STM32_PORT_MAX


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  写 GPIO
 * @note   
 * @param[in]  io: IO number
 * @param[in]  level: GPIO_LOW | GPIO_HIGH
 * @retval None
 */
void BSP_GPIO_Write(uint8_t io, BSP_GPIO_LEVEL level)
{
    GPIO_TypeDef *gpio_port;
    uint16_t gpio_pin;

    if (GPIO_PORT(io) < GPIO_STM32_PORT_MAX)
    {
        gpio_port = GPIO_STM32_PORT(io);
        gpio_pin  = GPIO_STM32_PIN(io);

        HAL_GPIO_WritePin(gpio_port, gpio_pin, (GPIO_PinState)level);
    }
}


/**
 * @brief  读 GPIO
 * @note   
 * @param[in]  io: IO number
 * @retval GPIO_LOW | GPIO_HIGH
 */
BSP_GPIO_LEVEL BSP_GPIO_Read(uint8_t io)
{
    GPIO_TypeDef *gpio_port;
    uint16_t gpio_pin;

    if (GPIO_PORT(io) < GPIO_STM32_PORT_MAX)
    {
        gpio_port = GPIO_STM32_PORT(io);
        gpio_pin  = GPIO_STM32_PIN(io);
        return (BSP_GPIO_LEVEL)HAL_GPIO_ReadPin(gpio_port, gpio_pin);
    }

    return GPIO_HIGH;
}


/**
 * @brief  GPIO电平翻转
 * @note   
 * @param[in]  io: IO number
 * @retval None
 */
void BSP_GPIO_Toggle(uint8_t io)
{
    GPIO_TypeDef *gpio_port;
    uint16_t gpio_pin;

    if (GPIO_PORT(io) < GPIO_STM32_PORT_MAX)
    {
        gpio_port = GPIO_STM32_PORT(io);
        gpio_pin  = GPIO_STM32_PIN(io);

        HAL_GPIO_TogglePin(gpio_port, gpio_pin);
    }
}


/**
 * @brief  设置 GPIO 工作模式
 * @note   
 * @param[in]  io: IO number
 * @param[in]  mode: IO 模式 @ref GPIO_mode_define stm32f1xx_hal_gpio.h
 * @param[in]  pull: 上下拉 (GPIO_NOPULL | GPIO_PULLUP | GPIO_PULLDOWN)
 * @retval None
 */
void BSP_GPIO_SetMode(uint8_t io, uint32_t mode, uint32_t pull)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (GPIO_PORT(io) < GPIO_STM32_PORT_MAX) 
    {
        /* Configure GPIO_InitStructure */
        GPIO_InitStruct.Pin   = GPIO_STM32_PIN(io);
        GPIO_InitStruct.Mode  = mode;
        GPIO_InitStruct.Pull  = pull;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIO_STM32_PORT(io), &GPIO_InitStruct);
    }
}
