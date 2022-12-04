/**
 * @file        bsp_gpio.c
 * @brief       STM32 GPIO通用抽象接口
 * @date        2022-10-23
 * @version     V1.0
 * @copyright   Copyright (c) 2022-2032  佳都科技
 **********************************************************************************
 * @attention 
 * 
 * @par 修改记录:
 * <table>
 * <tr><th>日期         <th>版本        <th>作者        <th>描述
 * <tr><td>2022/10/23   <td>1.0         <td>Dino       <td>创建初始版本
 * </table>
 *
 **********************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "bsp_gpio.h"


/* Private variables ---------------------------------------------------------*/
static const struct GPIO_INDEX _gpios[] = 
{
#if defined(GPIOA)
    STM32_PIN(0 ,  A, 0 ),
    STM32_PIN(1 ,  A, 1 ),
    STM32_PIN(2 ,  A, 2 ),
    STM32_PIN(3 ,  A, 3 ),
    STM32_PIN(4 ,  A, 4 ),
    STM32_PIN(5 ,  A, 5 ),
    STM32_PIN(6 ,  A, 6 ),
    STM32_PIN(7 ,  A, 7 ),
    STM32_PIN(8 ,  A, 8 ),
    STM32_PIN(9 ,  A, 9 ),
    STM32_PIN(10,  A, 10),
    STM32_PIN(11,  A, 11),
    STM32_PIN(12,  A, 12),
    STM32_PIN(13,  A, 13),
    STM32_PIN(14,  A, 14),
    STM32_PIN(15,  A, 15),
#if defined(GPIOB)
    STM32_PIN(16,  B, 0),
    STM32_PIN(17,  B, 1),
    STM32_PIN(18,  B, 2),
    STM32_PIN(19,  B, 3),
    STM32_PIN(20,  B, 4),
    STM32_PIN(21,  B, 5),
    STM32_PIN(22,  B, 6),
    STM32_PIN(23,  B, 7),
    STM32_PIN(24,  B, 8),
    STM32_PIN(25,  B, 9),
    STM32_PIN(26,  B, 10),
    STM32_PIN(27,  B, 11),
    STM32_PIN(28,  B, 12),
    STM32_PIN(29,  B, 13),
    STM32_PIN(30,  B, 14),
    STM32_PIN(31,  B, 15),
#if defined(GPIOC)
    STM32_PIN(32,  C, 0),
    STM32_PIN(33,  C, 1),
    STM32_PIN(34,  C, 2),
    STM32_PIN(35,  C, 3),
    STM32_PIN(36,  C, 4),
    STM32_PIN(37,  C, 5),
    STM32_PIN(38,  C, 6),
    STM32_PIN(39,  C, 7),
    STM32_PIN(40,  C, 8),
    STM32_PIN(41,  C, 9),
    STM32_PIN(42,  C, 10),
    STM32_PIN(43,  C, 11),
    STM32_PIN(44,  C, 12),
    STM32_PIN(45,  C, 13),
    STM32_PIN(46,  C, 14),
    STM32_PIN(47,  C, 15),
#if defined(GPIOD)
    STM32_PIN(48,  D, 0),
    STM32_PIN(49,  D, 1),
    STM32_PIN(50,  D, 2),
    STM32_PIN(51,  D, 3),
    STM32_PIN(52,  D, 4),
    STM32_PIN(53,  D, 5),
    STM32_PIN(54,  D, 6),
    STM32_PIN(55,  D, 7),
    STM32_PIN(56,  D, 8),
    STM32_PIN(57,  D, 9),
    STM32_PIN(58,  D, 10),
    STM32_PIN(59,  D, 11),
    STM32_PIN(60,  D, 12),
    STM32_PIN(61,  D, 13),
    STM32_PIN(62,  D, 14),
    STM32_PIN(63,  D, 15),
#if defined(GPIOE)
    STM32_PIN(64,  E, 0),
    STM32_PIN(65,  E, 1),
    STM32_PIN(66,  E, 2),
    STM32_PIN(67,  E, 3),
    STM32_PIN(68,  E, 4),
    STM32_PIN(69,  E, 5),
    STM32_PIN(70,  E, 6),
    STM32_PIN(71,  E, 7),
    STM32_PIN(72,  E, 8),
    STM32_PIN(73,  E, 9),
    STM32_PIN(74,  E, 10),
    STM32_PIN(75,  E, 11),
    STM32_PIN(76,  E, 12),
    STM32_PIN(77,  E, 13),
    STM32_PIN(78,  E, 14),
    STM32_PIN(79,  E, 15),
#if defined(GPIOF)
    STM32_PIN(80,  F, 0),
    STM32_PIN(81,  F, 1),
    STM32_PIN(82,  F, 2),
    STM32_PIN(83,  F, 3),
    STM32_PIN(84,  F, 4),
    STM32_PIN(85,  F, 5),
    STM32_PIN(86,  F, 6),
    STM32_PIN(87,  F, 7),
    STM32_PIN(88,  F, 8),
    STM32_PIN(89,  F, 9),
    STM32_PIN(90,  F, 10),
    STM32_PIN(91,  F, 11),
    STM32_PIN(92,  F, 12),
    STM32_PIN(93,  F, 13),
    STM32_PIN(94,  F, 14),
    STM32_PIN(95,  F, 15),
#if defined(GPIOG)
    STM32_PIN(96,  G, 0),
    STM32_PIN(97,  G, 1),
    STM32_PIN(98,  G, 2),
    STM32_PIN(99,  G, 3),
    STM32_PIN(100, G, 4),
    STM32_PIN(101, G, 5),
    STM32_PIN(102, G, 6),
    STM32_PIN(103, G, 7),
    STM32_PIN(104, G, 8),
    STM32_PIN(105, G, 9),
    STM32_PIN(106, G, 10),
    STM32_PIN(107, G, 11),
    STM32_PIN(108, G, 12),
    STM32_PIN(109, G, 13),
    STM32_PIN(110, G, 14),
    STM32_PIN(111, G, 15),
#if defined(GPIOH)
    STM32_PIN(112, H, 0),
    STM32_PIN(113, H, 1),
    STM32_PIN(114, H, 2),
    STM32_PIN(115, H, 3),
    STM32_PIN(116, H, 4),
    STM32_PIN(117, H, 5),
    STM32_PIN(118, H, 6),
    STM32_PIN(119, H, 7),
    STM32_PIN(120, H, 8),
    STM32_PIN(121, H, 9),
    STM32_PIN(122, H, 10),
    STM32_PIN(123, H, 11),
    STM32_PIN(124, H, 12),
    STM32_PIN(125, H, 13),
    STM32_PIN(126, H, 14),
    STM32_PIN(127, H, 15),
#if defined(GPIOI)
    STM32_PIN(128, I, 0),
    STM32_PIN(129, I, 1),
    STM32_PIN(130, I, 2),
    STM32_PIN(131, I, 3),
    STM32_PIN(132, I, 4),
    STM32_PIN(133, I, 5),
    STM32_PIN(134, I, 6),
    STM32_PIN(135, I, 7),
    STM32_PIN(136, I, 8),
    STM32_PIN(137, I, 9),
    STM32_PIN(138, I, 10),
    STM32_PIN(139, I, 11),
    STM32_PIN(140, I, 12),
    STM32_PIN(141, I, 13),
    STM32_PIN(142, I, 14),
    STM32_PIN(143, I, 15),
#if defined(GPIOJ)
    STM32_PIN(144, J, 0),
    STM32_PIN(145, J, 1),
    STM32_PIN(146, J, 2),
    STM32_PIN(147, J, 3),
    STM32_PIN(148, J, 4),
    STM32_PIN(149, J, 5),
    STM32_PIN(150, J, 6),
    STM32_PIN(151, J, 7),
    STM32_PIN(152, J, 8),
    STM32_PIN(153, J, 9),
    STM32_PIN(154, J, 10),
    STM32_PIN(155, J, 11),
    STM32_PIN(156, J, 12),
    STM32_PIN(157, J, 13),
    STM32_PIN(158, J, 14),
    STM32_PIN(159, J, 15),
#if defined(GPIOK)
    STM32_PIN(160, K, 0),
    STM32_PIN(161, K, 1),
    STM32_PIN(162, K, 2),
    STM32_PIN(163, K, 3),
    STM32_PIN(164, K, 4),
    STM32_PIN(165, K, 5),
    STM32_PIN(166, K, 6),
    STM32_PIN(167, K, 7),
    STM32_PIN(168, K, 8),
    STM32_PIN(169, K, 9),
    STM32_PIN(170, K, 10),
    STM32_PIN(171, K, 11),
    STM32_PIN(172, K, 12),
    STM32_PIN(173, K, 13),
    STM32_PIN(174, K, 14),
    STM32_PIN(175, K, 15),
#endif /* defined(GPIOK) */
#endif /* defined(GPIOJ) */
#endif /* defined(GPIOI) */
#endif /* defined(GPIOH) */
#endif /* defined(GPIOG) */
#endif /* defined(GPIOF) */
#endif /* defined(GPIOE) */
#endif /* defined(GPIOD) */
#endif /* defined(GPIOC) */
#endif /* defined(GPIOB) */
#endif /* defined(GPIOA) */
};

static const uint8_t _gpio_qty = sizeof(_gpios) / sizeof(_gpios[0]);


/* Private function prototypes -----------------------------------------------*/
static const struct GPIO_INDEX *_GPIO_GetPin(uint8_t io);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  写GPIO
 * @note   
 * @param[in]  io: IO number
 * @param[in]  level: 0/1
 * @retval None
 */
void BSP_GPIO_Write(uint8_t io, uint8_t level)
{
    const struct GPIO_INDEX *index;

    index = _GPIO_GetPin(io);
    if (index == NULL)
        return;

    HAL_GPIO_WritePin(index->gpio, index->pin, (GPIO_PinState)level);
}


/**
 * @brief  读GPIO
 * @note   
 * @param[in]  io: IO number
 * @retval 0/1
 */
uint8_t BSP_GPIO_Read(uint8_t io)
{
    const struct GPIO_INDEX *index;

    index = _GPIO_GetPin(io);
    if (index == NULL)
        return GPIO_HIGH;

    return HAL_GPIO_ReadPin(index->gpio, index->pin);
}


/**
 * @brief  GPIO电平翻转
 * @note   
 * @param[in]  io: IO number
 * @retval None
 */
void BSP_GPIO_Toggle(uint8_t io)
{
    const struct GPIO_INDEX *index;

    index = _GPIO_GetPin(io);
    if (index == NULL)
        return;

    HAL_GPIO_TogglePin(index->gpio, index->pin);
}


/**
 * @brief  设置GPIO工作模式
 * @note   
 * @param[in]  io: IO number
 * @param[in]  mode: IO模式（This parameter can be a value of @ref GPIO_mode_define stm32f1xx_hal_gpio.h）
 * @param[in]  pull: 上下拉（GPIO_NOPULL、GPIO_PULLUP、GPIO_PULLDOWN）
 * @retval None
 */
void BSP_GPIO_SetMode(uint8_t io, uint32_t mode, uint32_t pull)
{
    const struct GPIO_INDEX *index;
    GPIO_InitTypeDef GPIO_InitStruct;

    index = _GPIO_GetPin(io);
    if (index == NULL)
        return;

    /* Configure GPIO_InitStructure */
    GPIO_InitStruct.Pin   = index->pin;
    GPIO_InitStruct.Mode  = mode;
    GPIO_InitStruct.Pull  = pull;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(index->gpio, &GPIO_InitStruct);
}


/**
 * @brief  通过IO number获取port
 * @note   
 * @param[in]  io: IO number
 * @retval GPIOx
 */
GPIO_TypeDef *BSP_GPIO_GetPort(uint8_t io)
{
    const struct GPIO_INDEX *index;

    index = _GPIO_GetPin(io);
    if (index == NULL)
        return NULL;
    
    return index->gpio;
}


/**
 * @brief  通过IO number获取pin
 * @note   
 * @param[in]  io: IO number
 * @retval GPIO_PIN_x
 */
uint16_t BSP_GPIO_GetPin(uint8_t io)
{
    const struct GPIO_INDEX *index;

    index = _GPIO_GetPin(io);
    if (index == NULL)
        return NULL;
    
    return index->pin;
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  通过IO number获取IO对象
 * @note   
 * @param[in]  io: IO number
 * @retval IO对象
 */
static inline const struct GPIO_INDEX *_GPIO_GetPin(uint8_t io)
{
    const struct GPIO_INDEX *index;

    if (io < _gpio_qty)
    {
        index = &_gpios[io];

        if (index->index == -1)
            index = NULL;
    }
    else
        index = NULL;

    return index;
};


