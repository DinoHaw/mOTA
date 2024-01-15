/*!
    \file    gd32l23x.h
    \brief   general definitions for GD32L23x

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

#ifndef GD32L23X_H
#define GD32L23X_H

#ifdef __cplusplus
extern "C" {
#endif

/* define GD32L23x */
#if !defined (GD32L23x)
#define GD32L23x
#endif /* define GD32L23x */
#if !defined (GD32L23x)
#error "Please select the target GD32L23x device used in your application (in gd32l23x.h file)"
#endif /* undefine GD32L23x tip */

/* define value of high speed crystal oscillator (HXTAL) in Hz */
#if !defined  (HXTAL_VALUE)
#define HXTAL_VALUE    ((uint32_t)8000000)
#endif /* high speed crystal oscillator value */

/* define startup timeout value of high speed crystal oscillator (HXTAL) */
#if !defined  (HXTAL_STARTUP_TIMEOUT)
#define HXTAL_STARTUP_TIMEOUT   ((uint16_t)0x0FFFF)
#endif /* high speed crystal oscillator startup timeout */

/* define value of internal 16MHz RC oscillator (IRC16M) in Hz */
#if !defined  (IRC16M_VALUE)
#define IRC16M_VALUE  ((uint32_t)16000000)
#endif /* internal 16MHz RC oscillator value */

/* define startup timeout value of internal 16MHz RC oscillator (IRC16M) */
#if !defined  (IRC16M_STARTUP_TIMEOUT)
#define IRC16M_STARTUP_TIMEOUT   ((uint16_t)0x0500)
#endif /* internal 16MHz RC oscillator startup timeout */

#if !defined  (IRC48M_VALUE)
#define IRC48M_VALUE ((uint32_t)48000000)
#endif /* IRC48M_VALUE */

/* define value of internal 32KHz RC oscillator(IRC32K) in Hz */
#if !defined  (IRC32K_VALUE)
#define IRC32K_VALUE  ((uint32_t)32000)
#endif /* internal 32KHz RC oscillator value */

/* define value of low speed crystal oscillator (LXTAL)in Hz */
#if !defined  (LXTAL_VALUE)
#define LXTAL_VALUE  ((uint32_t)32768)
#endif /* low speed crystal oscillator value */

/* GD32L23x firmware library version number V1.0 */
#define __GD32L23X_STDPERIPH_VERSION_MAIN   (0x01) /*!< [31:24] main version     */
#define __GD32L23X_STDPERIPH_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version     */
#define __GD32L23X_STDPERIPH_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version     */
#define __GD32L23X_STDPERIPH_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __GD32L23X_STDPERIPH_VERSION        ((__GD32L23X_STDPERIPH_VERSION_MAIN << 24)\
        |(__GD32L23X_STDPERIPH_VERSION_SUB1 << 16)\
        |(__GD32L23X_STDPERIPH_VERSION_SUB2 << 8)\
        |(__GD32L23X_STDPERIPH_VERSION_RC))

/* configuration of the Cortex-M23 processor and core peripherals                                        */
#define __CM23_REV                0x0100U   /*!< Core revision r1p0                                      */
#define __SAUREGION_PRESENT       0U        /*!< SAU regions are not present                             */
#define __MPU_PRESENT             0U        /*!< MPU is present                                          */
#define __VTOR_PRESENT            1U        /*!< VTOR is present                                         */
#define __NVIC_PRIO_BITS          2U        /*!< Number of Bits used for Priority Levels                 */
#define __Vendor_SysTickConfig    0U        /*!< Set to 1 if different SysTick Config is used            */

/* define interrupt number */
typedef enum IRQn {
    /* Cortex-M23 processor exceptions numbers */
    NonMaskableInt_IRQn          = -14,    /*!< non maskable interrupt                                   */
    HardFault_IRQn               = -13,    /*!< hardfault interrupt                                      */

    SVCall_IRQn                  = -5,     /*!< sv call interrupt                                        */

    PendSV_IRQn                  = -2,     /*!< pend sv interrupt                                        */
    SysTick_IRQn                 = -1,     /*!< system tick interrupt                                    */
    /* interruput numbers */
    WWDGT_IRQn                   = 0,      /*!< window watchdog timer interrupt                          */
    LVD_IRQn                     = 1,      /*!< LVD through EXTI line detect interrupt                   */
    TAMPER_STAMP_IRQn            = 2,      /*!< RTC Tamper and TimeStamp interrupt                       */
    RTC_WKUP_IRQn                = 3,      /*!< RTC Wakeup interrupt                                     */
    FMC_IRQn                     = 4,      /*!< FMC interrupt                                            */
    RCU_CTC_IRQn                 = 5,      /*!< RCU and CTC interrupt                                    */
    EXTI0_IRQn                   = 6,      /*!< EXTI line 0 interrupts                                   */
    EXTI1_IRQn                   = 7,      /*!< EXTI line 1 interrupts                                   */
    EXTI2_IRQn                   = 8,      /*!< EXTI line 2 interrupts                                   */
    EXTI3_IRQn                   = 9,      /*!< EXTI line 3 interrupts                                   */
    EXTI4_IRQn                   = 10,     /*!< EXTI line 4 interrupts                                   */
    DMA_Channel0_IRQn            = 11,     /*!< DMA channel 0 interrupt                                  */
    DMA_Channel1_IRQn            = 12,     /*!< DMA channel 1 interrupt                                  */
    DMA_Channel2_IRQn            = 13,     /*!< DMA channel 2 interrupt                                  */
    DMA_Channel3_IRQn            = 14,     /*!< DMA channel 3 interrupt                                  */
    DMA_Channel4_IRQn            = 15,     /*!< DMA channel 4 interrupt                                  */
    DMA_Channel5_IRQn            = 16,     /*!< DMA channel 5 interrupt                                  */
    DMA_Channel6_IRQn            = 17,     /*!< DMA channel 6 interrupt                                  */
    ADC_IRQn                     = 18,     /*!< ADC interrupts                                           */
    USBD_HP_IRQn                 = 19,     /*!< USBD High Priority                                       */
    USBD_LP_IRQn                 = 20,     /*!< USBD Low Priority                                        */
    TIMER1_IRQn                  = 21,     /*!< TIMER1 interrupt                                         */
    TIMER2_IRQn                  = 22,     /*!< TIMER2 interrupt                                         */
    TIMER8_IRQn                  = 23,     /*!< TIMER8 interrupt                                         */
    TIMER11_IRQn                 = 24,     /*!< TIMER11 interrupt                                        */
    TIMER5_IRQn                  = 25,     /*!< TIMER5 interrupt                                         */
    TIMER6_IRQn                  = 26,     /*!< TIMER6 interrupt                                         */
    USART0_IRQn                  = 27,     /*!< USART0 interrupt                                         */
    USART1_IRQn                  = 28,     /*!< USART1 interrupt                                         */
    UART3_IRQn                   = 29,     /*!< UART3 interrupt                                          */
    UART4_IRQn                   = 30,     /*!< UART4 interrupt                                          */
    I2C0_EV_IRQn                 = 31,     /*!< I2C0 event interrupt                                     */
    I2C0_ER_IRQn                 = 32,     /*!< I2C0 error interrupt                                     */
    I2C1_EV_IRQn                 = 33,     /*!< I2C1 event interrupt                                     */
    I2C1_ER_IRQn                 = 34,     /*!< I2C1 error interrupt                                     */
    SPI0_IRQn                    = 35,     /*!< SPI0 interrupt                                           */
    SPI1_IRQn                    = 36,     /*!< SPI1 interrupt                                           */
    DAC_IRQn                     = 37,     /*!< DAC interrupt                                            */
    I2C2_EV_IRQn                 = 39,     /*!< I2C2 event interrupt                                     */
    I2C2_ER_IRQn                 = 40,     /*!< I2C2 error interrupt                                     */
    RTC_Alarm_IRQn               = 41,     /*!< RTC Alarm interrupt                                      */
    USBD_WKUP_IRQn               = 42,     /*!< USBD Wakeup interrupt                                    */
    EXTI5_9_IRQn                 = 43,     /*!< EXTI line 5 to 9 interrupts                              */
    EXTI10_15_IRQn               = 47,     /*!< EXTI line 10 to 15 interrupts                            */
    DMAMUX_IRQn                  = 55,     /*!< DMAMUX interrupt                                         */
    CMP0_IRQn                    = 56,     /*!< Comparator 0 interrupt                                   */
    CMP1_IRQn                    = 57,     /*!< Comparator 1 interrupt                                   */
    I2C0_WKUP_IRQn               = 58,     /*!< I2C0 Wakeup interrupt                                    */
    I2C2_WKUP_IRQn               = 59,     /*!< I2C2 Wakeup interrupt                                    */
    USART0_WKUP_IRQn             = 60,     /*!< USART0 Wakeup interrupt                                  */
    LPUART_IRQn                  = 61,     /*!< LPUART global interrupt                                  */
    CAU_IRQn                     = 62,     /*!< CAU interrupt                                            */
    TRNG_IRQn                    = 63,     /*!< TRNG interrupt                                           */
    SLCD_IRQn                    = 64,     /*!< SLCD interrupt                                           */
    USART1_WKUP_IRQn             = 65,     /*!< USART1 Wakeup interrupt                                  */
    I2C1_WKUP_IRQn               = 66,     /*!< I2C1 Wakeup interrupt                                    */
    LPUART_WKUP_IRQn             = 67,     /*!< LPUART Wakeup interrupt                                  */
    LPTIMER_IRQn                 = 68,     /*!< LPTIMER interrupt                                        */
} IRQn_Type;

/* includes */
#include "core_cm23.h"
#include "system_gd32l23x.h"
#include <stdint.h>

/* enum definitions */
typedef enum {DISABLE = 0, ENABLE = !DISABLE} EventStatus, ControlStatus;
typedef enum {RESET = 0, SET = !RESET} FlagStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrStatus;

/* bit operations */
#define REG64(addr)                  (*(volatile uint64_t *)(uint32_t)(addr))
#define REG32(addr)                  (*(volatile uint32_t *)(uint32_t)(addr))
#define REG16(addr)                  (*(volatile uint16_t *)(uint32_t)(addr))
#define REG8(addr)                   (*(volatile uint8_t *)(uint32_t)(addr))
#define BIT(x)                       ((uint32_t)((uint32_t)0x01U<<(x)))
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))
#define GET_BITS(regval, start, end) (((regval) & BITS((start),(end))) >> (start))

/* main flash and SRAM memory map */
#define FLASH_BASE            ((uint32_t)0x08000000U)       /*!< main FLASH base address          */
#define SRAM_BASE             ((uint32_t)0x20000000U)       /*!< SRAM  base address               */
/* peripheral memory map */
#define APB1_BUS_BASE         ((uint32_t)0x40000000U)       /*!< apb1 base address                */
#define APB2_BUS_BASE         ((uint32_t)0x40010000U)       /*!< apb2 base address                */
#define AHB1_BUS_BASE         ((uint32_t)0x40020000U)       /*!< ahb1 base address                */
#define AHB2_BUS_BASE         ((uint32_t)0x48000000U)       /*!< ahb2 base address                */
/* advanced peripheral bus 1 memory map */
#define TIMER_BASE            (APB1_BUS_BASE + 0x00000000U) /*!< TIMER base address               */
#define SLCD_BASE             (APB1_BUS_BASE + 0x00002400U) /*!< LCD base address                 */
#define RTC_BASE              (APB1_BUS_BASE + 0x00002800U) /*!< RTC base address                 */
#define WWDGT_BASE            (APB1_BUS_BASE + 0x00002C00U) /*!< WWDGT base address               */
#define FWDGT_BASE            (APB1_BUS_BASE + 0x00003000U) /*!< FWDGT base address               */
#define SPI_BASE              (APB1_BUS_BASE + 0x00003800U) /*!< SPI base address                 */
#define USART_BASE            (APB1_BUS_BASE + 0x00004400U) /*!< USART base address               */
#define I2C_BASE              (APB1_BUS_BASE + 0x00005400U) /*!< I2C base address                 */
#define USBD_BASE             (APB1_BUS_BASE + 0x00005C00U) /*!< USBD base address                */
#define USBD_RAM_BASE         (APB1_BUS_BASE + 0x00006000U) /*!< USBD RAM base address            */
#define PMU_BASE              (APB1_BUS_BASE + 0x00007000U) /*!< PMU base address                 */
#define DAC_BASE              (APB1_BUS_BASE + 0x00007400U) /*!< DAC base address                 */
#define LPUART_BASE           (APB1_BUS_BASE + 0x00008000U) /*!< LPUART base address              */
#define LPTIMER_BASE          (APB1_BUS_BASE + 0x00009400U) /*!< LPUART base address              */
#define CTC_BASE              (APB1_BUS_BASE + 0x0000C800U) /*!< LPUART base address              */
/* advanced peripheral bus 2 memory map */
#define SYSCFG_BASE           (APB2_BUS_BASE + 0x00000000U) /*!< SYSCFG base address              */
#define EXTI_BASE             (APB2_BUS_BASE + 0x00000400U) /*!< EXTI base address                */
#define ADC_BASE              (APB2_BUS_BASE + 0x00002400U) /*!< ADC base address                 */
#define CMP_BASE              (APB2_BUS_BASE + 0x00007C00U) /*!< CMP base address                 */
/* advanced high performance bus 1 memory map */
#define DMA_BASE              (AHB1_BUS_BASE + 0x00000000U) /*!< DMA base address                 */
#define DMA_CHANNEL_BASE      (DMA_BASE + 0x00000008U)      /*!< DMA channel base address         */
#define DMAMUX_BASE           (AHB1_BUS_BASE + 0x00000800U) /*!< DMA base address                 */
#define RCU_BASE              (AHB1_BUS_BASE + 0x00001000U) /*!< RCU base address                 */
#define FMC_BASE              (AHB1_BUS_BASE + 0x00002000U) /*!< FMC base address                 */
#define CRC_BASE              (AHB1_BUS_BASE + 0x00003000U) /*!< CRC base address                 */
/* advanced high performance bus 2 memory map */
#define GPIO_BASE             (AHB2_BUS_BASE + 0x00000000U) /*!< GPIO base address                */
#define CAU_BASE              (AHB2_BUS_BASE + 0x08060000U) /*!< CAU base address                 */
#define TRNG_BASE             (AHB2_BUS_BASE + 0x08060800U) /*!< TRNG base address                */
/* option byte and debug memory map */
#define OB_BASE               ((uint32_t)0x1FFFF800U)       /*!< OB base address                  */
#define DBG_BASE              ((uint32_t)0x40015800U)       /*!< DBG base address                 */

#define VREF_BASE             ((uint32_t)0x40010030U)       /*!< VREF base address                */

#include "gd32l23x_libopt.h"

#ifdef __cplusplus
}
#endif

#endif /* GD32L23X_H */
