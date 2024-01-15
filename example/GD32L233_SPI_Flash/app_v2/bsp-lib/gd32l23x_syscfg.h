/*!
    \file    gd32l23x_syscfg.h
    \brief   definitions for the SYSCFG

    \version 2021-08-04, V1.0.0, firmware for GD32L23x
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef GD32L23X_SYSCFG_H
#define GD32L23X_SYSCFG_H

#include "gd32l23x.h"

/* SYSCFG definitions */
#define SYSCFG                              SYSCFG_BASE

/* registers definitions */
#define SYSCFG_CFG0                         REG32(SYSCFG + 0x00U)                       /*!< system configuration register 0 */
#define SYSCFG_EXTISS0                      REG32(SYSCFG + 0x08U)                       /*!< EXTI sources selection register 0 */
#define SYSCFG_EXTISS1                      REG32(SYSCFG + 0x0CU)                       /*!< EXTI sources selection register 1 */
#define SYSCFG_EXTISS2                      REG32(SYSCFG + 0x10U)                       /*!< EXTI sources selection register 2 */
#define SYSCFG_EXTISS3                      REG32(SYSCFG + 0x14U)                       /*!< EXTI sources selection register 3 */
#define SYSCFG_CPU_IRQ_LAT                  REG32(SYSCFG + 0x100U)                      /*!< IRQ latency register */

/* SYSCFG_CFG0 bits definitions */
#define SYSCFG_CFG0_BOOT_MODE               BITS(0,1)                                   /*!< SYSCFG memory remap config */
#define SYSCFG_CFG0_PA11_PA12_RMP           BIT(4)                                      /*!< PA11 and PA12 remapping bit for small packages (28 and 20 pins) */
#define SYSCFG_CFG0_BOOT0_PD3_RMP           BIT(6)                                      /*!< BOOT0 and PD3 remapping bit */
#define SYSCFG_CFG0_PB6_HCCE                BIT(16)                                     /*!< PB6 pin high current capability enable */
#define SYSCFG_CFG0_PB7_HCCE                BIT(17)                                     /*!< PB7 pin high current capability enable */
#define SYSCFG_CFG0_PB8_HCCE                BIT(18)                                     /*!< PB8 pin high current capability enable */
#define SYSCFG_CFG0_PB9_HCCE                BIT(19)                                     /*!< PB9 pin high current capability enable */

/* SYSCFG_EXTISS0 bits definitions */
#define SYSCFG_EXTISS0_EXTI0_SS             BITS(0,3)                                   /*!< EXTI 0 configuration */
#define SYSCFG_EXTISS0_EXTI1_SS             BITS(4,7)                                   /*!< EXTI 1 configuration */
#define SYSCFG_EXTISS0_EXTI2_SS             BITS(8,11)                                  /*!< EXTI 2 configuration */
#define SYSCFG_EXTISS0_EXTI3_SS             BITS(12,15)                                 /*!< EXTI 3 configuration */

/* SYSCFG_EXTISS1 bits definitions */
#define SYSCFG_EXTISS1_EXTI4_SS             BITS(0,3)                                   /*!< EXTI 4 configuration */
#define SYSCFG_EXTISS1_EXTI5_SS             BITS(4,7)                                   /*!< EXTI 5 configuration */
#define SYSCFG_EXTISS1_EXTI6_SS             BITS(8,11)                                  /*!< EXTI 6 configuration */
#define SYSCFG_EXTISS1_EXTI7_SS             BITS(12,15)                                 /*!< EXTI 7 configuration */

/* SYSCFG_EXTISS2 bits definitions */
#define SYSCFG_EXTISS2_EXTI8_SS             BITS(0,3)                                   /*!< EXTI 8 configuration */
#define SYSCFG_EXTISS2_EXTI9_SS             BITS(4,7)                                   /*!< EXTI 9 configuration */
#define SYSCFG_EXTISS2_EXTI10_SS            BITS(8,11)                                  /*!< EXTI 10 configuration */
#define SYSCFG_EXTISS2_EXTI11_SS            BITS(12,15)                                 /*!< EXTI 11 configuration */

/* SYSCFG_EXTISS3 bits definitions */
#define SYSCFG_EXTISS3_EXTI12_SS            BITS(0,3)                                   /*!< EXTI 12 configuration */
#define SYSCFG_EXTISS3_EXTI13_SS            BITS(4,7)                                   /*!< EXTI 13 configuration */
#define SYSCFG_EXTISS3_EXTI14_SS            BITS(8,11)                                  /*!< EXTI 14 configuration */
#define SYSCFG_EXTISS3_EXTI15_SS            BITS(12,15)                                 /*!< EXTI 15 configuration */

/* SYSCFG_VREF_CS bits definitions */
#define SYSCFG_VREFEN                       BIT(0)                                      /*!< VREF enable */
#define SYSCFG_HIPM                         BIT(1)                                      /*!< High impedance mode */
#define SYSCFG_VREFRDY                      BIT(3)                                      /*!< VREF ready */

/* SYSCFG_VREF_CALIB bits definitions */
#define SYSCFG_VREF_CALIB_VREFCAL           BITS(0,5)                                   /*!< VREF calibration value */

/* SYSCFG_CPU_IRQ_LAT bits definitions */
#define SYSCFG_CPU_IRQ_LAT_IRQ_LATENCY      BITS(0,7)                                   /*!< IRQ_LATENCY specifies the minimum number of cycles between an interrupt */

/* constants definitions */
/* boot mode definitions */
#define SYSCFG_BOOTMODE_FLASH               ((uint8_t)0x00U)                            /*!< boot from main flash */
#define SYSCFG_BOOTMODE_SYSTEM              ((uint8_t)0x01U)                            /*!< boot from system flash memory */
#define SYSCFG_BOOTMODE_SRAM                ((uint8_t)0x03U)                            /*!< boot from embedded SRAM */

/* DMA remap definitions */
#define SYSCFG_PA11_PA12_REMAP              SYSCFG_CFG0_PA11_PA12_RMP                   /*!< PA11 PA12 remap */
#define SYSCFG_BOOT0_PD3_REMAP              SYSCFG_CFG0_BOOT0_PD3_RMP                   /*!< BOOT0 PD3 remap */

/* pin high current capability definitions */
#define SYSCFG_PB6_HIGH_CURRENT             SYSCFG_CFG0_PB6_HCCE                        /*!< PB6 pin high current capability */
#define SYSCFG_PB7_HIGH_CURRENT             SYSCFG_CFG0_PB7_HCCE                        /*!< PB7 pin high current capability */
#define SYSCFG_PB8_HIGH_CURRENT             SYSCFG_CFG0_PB8_HCCE                        /*!< PB7 pin high current capability */
#define SYSCFG_PB9_HIGH_CURRENT             SYSCFG_CFG0_PB9_HCCE                        /*!< PB9 pin high current capability */

/* EXTI source select definition */
#define EXTISS0                             ((uint8_t)0x00U)                            /*!< EXTI source select register 0 */
#define EXTISS1                             ((uint8_t)0x01U)                            /*!< EXTI source select register 1 */
#define EXTISS2                             ((uint8_t)0x02U)                            /*!< EXTI source select register 2 */
#define EXTISS3                             ((uint8_t)0x03U)                            /*!< EXTI source select register 3 */

/* EXTI source select mask bits definition */
#define EXTI_SS_MASK                        BITS(0,3)                                   /*!< EXTI source select mask */

/* EXTI source select jumping step definition */
#define EXTI_SS_JSTEP                       ((uint8_t)(0x04U))                          /*!< EXTI source select jumping step */

/* EXTI source select moving step definition */
#define EXTI_SS_MSTEP(pin)                  (EXTI_SS_JSTEP * ((pin) % EXTI_SS_JSTEP))   /*!< EXTI source select moving step */

/* EXTI source port definitions */
#define EXTI_SOURCE_GPIOA                   ((uint8_t)0x00U)                            /*!< EXTI GPIOA configuration */
#define EXTI_SOURCE_GPIOB                   ((uint8_t)0x01U)                            /*!< EXTI GPIOB configuration */
#define EXTI_SOURCE_GPIOC                   ((uint8_t)0x02U)                            /*!< EXTI GPIOC configuration */
#define EXTI_SOURCE_GPIOD                   ((uint8_t)0x03U)                            /*!< EXTI GPIOD configuration */
#define EXTI_SOURCE_GPIOF                   ((uint8_t)0x05U)                            /*!< EXTI GPIOF configuration */

/* EXTI source pin definitions */
#define EXTI_SOURCE_PIN0                    ((uint8_t)0x00U)                            /*!< EXTI GPIO pin0 configuration */
#define EXTI_SOURCE_PIN1                    ((uint8_t)0x01U)                            /*!< EXTI GPIO pin1 configuration */
#define EXTI_SOURCE_PIN2                    ((uint8_t)0x02U)                            /*!< EXTI GPIO pin2 configuration */
#define EXTI_SOURCE_PIN3                    ((uint8_t)0x03U)                            /*!< EXTI GPIO pin3 configuration */
#define EXTI_SOURCE_PIN4                    ((uint8_t)0x04U)                            /*!< EXTI GPIO pin4 configuration */
#define EXTI_SOURCE_PIN5                    ((uint8_t)0x05U)                            /*!< EXTI GPIO pin5 configuration */
#define EXTI_SOURCE_PIN6                    ((uint8_t)0x06U)                            /*!< EXTI GPIO pin6 configuration */
#define EXTI_SOURCE_PIN7                    ((uint8_t)0x07U)                            /*!< EXTI GPIO pin7 configuration */
#define EXTI_SOURCE_PIN8                    ((uint8_t)0x08U)                            /*!< EXTI GPIO pin8 configuration */
#define EXTI_SOURCE_PIN9                    ((uint8_t)0x09U)                            /*!< EXTI GPIO pin9 configuration */
#define EXTI_SOURCE_PIN10                   ((uint8_t)0x0AU)                            /*!< EXTI GPIO pin10 configuration */
#define EXTI_SOURCE_PIN11                   ((uint8_t)0x0BU)                            /*!< EXTI GPIO pin11 configuration */
#define EXTI_SOURCE_PIN12                   ((uint8_t)0x0CU)                            /*!< EXTI GPIO pin12 configuration */
#define EXTI_SOURCE_PIN13                   ((uint8_t)0x0DU)                            /*!< EXTI GPIO pin13 configuration */
#define EXTI_SOURCE_PIN14                   ((uint8_t)0x0EU)                            /*!< EXTI GPIO pin14 configuration */
#define EXTI_SOURCE_PIN15                   ((uint8_t)0x0FU)                            /*!< EXTI GPIO pin15 configuration */

/* SYSCFG_CPU_IRQ_LAT register IRQ_LATENCY value */
#define IRQ_LATENCY(regval)                (BITS(0,7) & ((uint32_t)(regval) << 0U))     /*!< write value to IRQ_LATENCY bits field */

/* function declarations */
/* initialization functions */
/* reset the SYSCFG registers */
void syscfg_deinit(void);

/* configure the GPIO pin as EXTI Line */
void syscfg_exti_line_config(uint8_t exti_port, uint8_t exti_pin);

/* enable remap pin function for small packages */
void syscfg_pin_remap_enable(uint32_t remap_pin);
/* disable remap pin function for small packages */
void syscfg_pin_remap_disable(uint32_t remap_pin);

/* enable PBx(x=6,7,8,9) high current capability */
void syscfg_high_current_enable(uint32_t syscfg_gpio);
/* disable PBx(x=6,7,8,9) high current capability */
void syscfg_high_current_disable(uint32_t syscfg_gpio);

/* set the IRQ_LATENCY value */
void irq_latency_set(uint8_t irq_latency);
/* get the boot mode */
uint8_t syscfg_bootmode_get(void);

#endif /* gd32l23x_SYSCFG_H */
