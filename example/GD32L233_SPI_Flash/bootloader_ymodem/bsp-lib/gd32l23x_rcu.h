/*!
    \file    gd32l23x_rcu.h
    \brief   definitions for the RCU

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

#ifndef GD32L23X_RCU_H
#define GD32L23X_RCU_H

#include "gd32l23x.h"

/* RCU definitions */
#define RCU                         RCU_BASE

/* registers definitions */
#define RCU_CTL                     REG32(RCU + 0x00000000U)        /*!< control register 0 */
#define RCU_CFG0                    REG32(RCU + 0x00000004U)        /*!< configuration register 0 */
#define RCU_INT                     REG32(RCU + 0x00000008U)        /*!< interrupt register */
#define RCU_APB2RST                 REG32(RCU + 0x0000000CU)        /*!< APB2 reset register */
#define RCU_APB1RST                 REG32(RCU + 0x00000010U)        /*!< APB1 reset register */
#define RCU_AHBEN                   REG32(RCU + 0x00000014U)        /*!< AHB enable register */
#define RCU_APB2EN                  REG32(RCU + 0x00000018U)        /*!< APB2 enable register */
#define RCU_APB1EN                  REG32(RCU + 0x0000001CU)        /*!< APB1 enable register  */
#define RCU_BDCTL                   REG32(RCU + 0x00000020U)        /*!< backup domain control register */
#define RCU_RSTSCK                  REG32(RCU + 0x00000024U)        /*!< reset source /clock register */
#define RCU_AHBRST                  REG32(RCU + 0x00000028U)        /*!< AHB reset register */
#define RCU_CFG1                    REG32(RCU + 0x0000002CU)        /*!< configuration register 1 */
#define RCU_CFG2                    REG32(RCU + 0x00000030U)        /*!< configuration register 2 */
#define RCU_AHB2EN                  REG32(RCU + 0x00000034U)        /*!< AHB2 enable register */
#define RCU_AHB2RST                 REG32(RCU + 0x00000038U)        /*!< AHB2 reset register */
#define RCU_VKEY                    REG32(RCU + 0x00000100U)       /*!< voltage key register */
#define RCU_LPLDO                   REG32(RCU + 0x00000128U)       /*!< Low-Power mode LDO voltage register */
#define RCU_LPB                     REG32(RCU + 0x0000012CU)       /*!< Low-Power bandgap mode register */

/* bits definitions */
/* RCU_CTL */
#define RCU_CTL_IRC16MEN            BIT(0)                    /*!< internal high speed oscillator enable */
#define RCU_CTL_IRC16MSTB           BIT(1)                    /*!< IRC16M high speed internal oscillator stabilization flag */
#define RCU_CTL_IRC16MADJ           BITS(3,7)                 /*!< high speed internal oscillator clock trim adjust value */
#define RCU_CTL_IRC16MCALIB         BITS(8,15)                /*!< high speed internal oscillator calibration value register */
#define RCU_CTL_HXTALEN             BIT(16)                   /*!< external high speed oscillator enable */
#define RCU_CTL_HXTALSTB            BIT(17)                   /*!< external crystal oscillator clock stabilization flag */
#define RCU_CTL_HXTALBPS            BIT(18)                   /*!< external crystal oscillator clock bypass mode enable */
#define RCU_CTL_CKMEN               BIT(19)                   /*!< HXTAL clock monitor enable */
#define RCU_CTL_IRC48MEN            BIT(20)                   /*!< internal high speed oscillator enable */
#define RCU_CTL_IRC48MSTB           BIT(21)                   /*!< IRC48M high speed internal oscillator stabilization flag */
#define RCU_CTL_LXTALCKMEN          BIT(22)                   /*!< LXTAL clock monitor enable */
#define RCU_CTL_LXTALCKMD           BIT(23)                   /*!< LXTAL clock failure detection */
#define RCU_CTL_PLLEN               BIT(24)                   /*!< PLL enable */
#define RCU_CTL_PLLSTB              BIT(25)                   /*!< PLL clock stabilization flag */

/* RCU_CFG0 */
#define RCU_CFG0_SCS                BITS(0,1)                 /*!< system clock switch */
#define RCU_CFG0_SCSS               BITS(2,3)                 /*!< system clock switch status */
#define RCU_CFG0_AHBPSC             BITS(4,7)                 /*!< AHB prescaler selection */
#define RCU_CFG0_APB1PSC            BITS(8,10)                /*!< APB1 prescaler selection */
#define RCU_CFG0_APB2PSC            BITS(11,13)               /*!< APB2 prescaler selection */
#define RCU_CFG0_ADCPSC             BITS(14,15)               /*!< ADC clock prescaler selection */
#define RCU_CFG0_PLLSEL             BITS(16,17)               /*!< PLL clock source selection */
#define RCU_CFG0_PLLMF              BITS(18,23)               /*!< PLL multiply factor */
#define RCU_CFG0_CKOUTSEL           BITS(24,26)               /*!< CK_OUT clock source selection */
#define RCU_CFG0_PLLMF_6            BIT(27)                   /*!< bit6 of PLL multiply factor */
#define RCU_CFG0_CKOUTDIV           BITS(28,30)               /*!< CK_OUT divider which the CK_OUT frequency can be reduced */
#define RCU_CFG0_PLLDV              BIT(31)                   /*!< CK_PLL divide by 1 or 2 */

/* RCU_INT */
#define RCU_INT_IRC32KSTBIF         BIT(0)                    /*!< IRC32K stabilization interrupt flag */
#define RCU_INT_LXTALSTBIF          BIT(1)                    /*!< LXTAL stabilization interrupt flag */
#define RCU_INT_IRC16MSTBIF         BIT(2)                    /*!< IRC16M stabilization interrupt flag */
#define RCU_INT_HXTALSTBIF          BIT(3)                    /*!< HXTAL stabilization interrupt flag */
#define RCU_INT_PLLSTBIF            BIT(4)                    /*!< PLL stabilization interrupt flag */
#define RCU_INT_IRC48MSTBIF         BIT(5)                    /*!< IRC48M stabilization interrupt flag */
#define RCU_INT_LXTALCKMIF          BIT(6)                    /*!< LXTAL clock stuck interrupt flag */
#define RCU_INT_CKMIF               BIT(7)                    /*!< HXTAL clock stuck interrupt flag */
#define RCU_INT_IRC32KSTBIE         BIT(8)                    /*!< IRC32K stabilization interrupt enable */
#define RCU_INT_LXTALSTBIE          BIT(9)                    /*!< LXTAL stabilization interrupt enable */
#define RCU_INT_IRC16MSTBIE         BIT(10)                   /*!< IRC16M stabilization interrupt enable */
#define RCU_INT_HXTALSTBIE          BIT(11)                   /*!< HXTAL stabilization interrupt enable */
#define RCU_INT_PLLSTBIE            BIT(12)                   /*!< PLL stabilization interrupt enable */
#define RCU_INT_IRC48MSTBIE         BIT(13)                   /*!< IRC48M stabilization interrupt enable */
#define RCU_INT_LXTALCKMIE          BIT(14)                   /*!< LXTAL clock stuck interrupt enable */
#define RCU_INT_IRC32KSTBIC         BIT(16)                   /*!< IRC32K stabilization interrupt clear */
#define RCU_INT_LXTALSTBIC          BIT(17)                   /*!< LXTAL stabilization interrupt clear */
#define RCU_INT_IRC16MSTBIC         BIT(18)                   /*!< IRC16M stabilization interrupt clear */
#define RCU_INT_HXTALSTBIC          BIT(19)                   /*!< HXTAL stabilization interrupt clear */
#define RCU_INT_PLLSTBIC            BIT(20)                   /*!< PLL stabilization interrupt clear */
#define RCU_INT_IRC48MSTBIC         BIT(21)                   /*!< IRC48M stabilization interrupt clear */
#define RCU_INT_LXTALCKMIC          BIT(22)                   /*!< LXTAL clock stuck interrupt clear */
#define RCU_INT_CKMIC               BIT(23)                   /*!< HXTAL clock stuck interrupt clear */

/* RCU_APB2RST */
#define RCU_APB2RST_SYSCFGRST       BIT(0)                    /*!< system configuration reset */
#define RCU_APB2RST_CMPRST          BIT(1)                    /*!< comparator reset */
#define RCU_APB2RST_ADCRST          BIT(9)                    /*!< ADC reset */
#define RCU_APB2RST_TIMER8RST       BIT(11)                   /*!< TIMER0 reset */
#define RCU_APB2RST_SPI0RST         BIT(12)                   /*!< SPI0 reset */
#define RCU_APB2RST_USART0RST       BIT(14)                   /*!< USART0 reset */

/* RCU_APB1RST */
#define RCU_APB1RST_TIMER1RST       BIT(0)                    /*!< TIMER1 timer reset */
#define RCU_APB1RST_TIMER2RST       BIT(1)                    /*!< TIMER2 timer reset */
#define RCU_APB1RST_TIMER5RST       BIT(4)                    /*!< TIMER5 timer reset */
#define RCU_APB1RST_TIMER6RST       BIT(5)                    /*!< TIMER6 timer reset */
#define RCU_APB1RST_TIMER11RST      BIT(8)                    /*!< TIMER11 timer reset */
#define RCU_APB1RST_LPTIMERRST      BIT(9)                    /*!< LPTIMER timer reset */
#define RCU_APB1RST_SLCDRST         BIT(10)                   /*!< SLCD reset */
#define RCU_APB1RST_WWDGTRST        BIT(11)                   /*!< WWDGT(window watchdog timer) reset */
#define RCU_APB1RST_SPI1RST         BIT(14)                   /*!< SPI1 reset */
#define RCU_APB1RST_USART1RST       BIT(17)                   /*!< USART1 reset */
#define RCU_APB1RST_LPUARTRST       BIT(18)                   /*!< LPUART reset */
#define RCU_APB1RST_UART3RST        BIT(19)                   /*!< UART3 reset */
#define RCU_APB1RST_UART4RST        BIT(20)                   /*!< UART4 reset */
#define RCU_APB1RST_I2C0RST         BIT(21)                   /*!< I2C0 reset */
#define RCU_APB1RST_I2C1RST         BIT(22)                   /*!< I2C1 reset */
#define RCU_APB1RST_USBDRST         BIT(23)                   /*!< USBD reset */
#define RCU_APB1RST_I2C2RST         BIT(24)                   /*!< I2C2 reset */
#define RCU_APB1RST_PMURST          BIT(28)                   /*!< PMU(power management unit) reset */
#define RCU_APB1RST_DACRST          BIT(29)                   /*!< DAC reset */
#define RCU_APB1RST_CTCRST          BIT(30)                   /*!< CTC reset */

/* RCU_AHBEN */
#define RCU_AHBEN_DMAEN             BIT(0)                    /*!< DMA clock enable */
#define RCU_AHBEN_SRAM0SPEN         BIT(2)                    /*!< SRAM0 interface clock enable */
#define RCU_AHBEN_FMCSPEN           BIT(4)                    /*!< FMC clock enable */
#define RCU_AHBEN_CRCEN             BIT(6)                    /*!< CRC clock enable */
#define RCU_AHBEN_SRAM1SPEN         BIT(7)                    /*!< SRAM1 interface clock enable */
#define RCU_AHBEN_PAEN              BIT(17)                   /*!< GPIO port A clock enable */
#define RCU_AHBEN_PBEN              BIT(18)                   /*!< GPIO port B clock enable */
#define RCU_AHBEN_PCEN              BIT(19)                   /*!< GPIO port C clock enable */
#define RCU_AHBEN_PDEN              BIT(20)                   /*!< GPIO port D clock enable */
#define RCU_AHBEN_PFEN              BIT(22)                   /*!< GPIO port F clock enable */

/* RCU_APB2EN */
#define RCU_APB2EN_SYSCFGEN         BIT(0)                    /*!< system configuration clock enable */
#define RCU_APB2EN_CMPEN            BIT(1)                    /*!< comparator clock enable */
#define RCU_APB2EN_ADCEN            BIT(9)                    /*!< ADC interface clock enable */
#define RCU_APB2EN_TIMER8EN         BIT(11)                   /*!< TIMER8 timer clock enable */
#define RCU_APB2EN_SPI0EN           BIT(12)                   /*!< SPI0 clock enable */
#define RCU_APB2EN_USART0EN         BIT(14)                   /*!< USART0 clock enable */
#define RCU_APB2EN_DBGMCUEN         BIT(22)                   /*!< DBGMCU clock enable */

/* RCU_APB1EN */
#define RCU_APB1EN_TIMER1EN         BIT(0)                    /*!< TIMER1 timer clock enable */
#define RCU_APB1EN_TIMER2EN         BIT(1)                    /*!< TIMER2 timer clock enable */
#define RCU_APB1EN_TIMER5EN         BIT(4)                    /*!< TIMER5 timer clock enable */
#define RCU_APB1EN_TIMER6EN         BIT(5)                    /*!< TIMER6 timer clock enable */
#define RCU_APB1EN_TIMER11EN        BIT(8)                    /*!< TIMER11 timer clock enable */
#define RCU_APB1EN_LPTIMEREN        BIT(9)                    /*!< LPTIMER timer clock enable */
#define RCU_APB1EN_SLCDEN           BIT(10)                   /*!< SLCD clock enable */
#define RCU_APB1EN_WWDGTEN          BIT(11)                   /*!< WWDGT(window watchdog timer) clock enable */
#define RCU_APB1EN_SPI1EN           BIT(14)                   /*!< SPI1 clock enable */
#define RCU_APB1EN_USART1EN         BIT(17)                   /*!< USART1 clock enable */
#define RCU_APB1EN_LPUARTEN         BIT(18)                   /*!< LPUART clock enable */
#define RCU_APB1EN_UART3EN          BIT(19)                   /*!< UART3 clock enable */
#define RCU_APB1EN_UART4EN          BIT(20)                   /*!< UART4 clock enable */
#define RCU_APB1EN_I2C0EN           BIT(21)                   /*!< I2C0 clock enable */
#define RCU_APB1EN_I2C1EN           BIT(22)                   /*!< I2C1 clock enable */
#define RCU_APB1EN_USBDEN           BIT(23)                   /*!< USBD clock enable */
#define RCU_APB1EN_I2C2EN           BIT(24)                   /*!< I2C2 clock enable */
#define RCU_APB1EN_PMUEN            BIT(28)                   /*!< PMU(power management unit) clock enable */
#define RCU_APB1EN_DACEN            BIT(29)                   /*!< DAC clock enable */
#define RCU_APB1EN_CTCEN            BIT(30)                   /*!< CTC clock enable */
#define RCU_APB1EN_BKPEN            BIT(31)                   /*!< BKP clock enable */

/* RCU_BDCTL */
#define RCU_BDCTL_LXTALEN           BIT(0)                    /*!< LXTAL enable */
#define RCU_BDCTL_LXTALSTB          BIT(1)                    /*!< external low-speed oscillator stabilization */
#define RCU_BDCTL_LXTALBPS          BIT(2)                    /*!< LXTAL bypass mode enable */
#define RCU_BDCTL_LXTALDRI          BITS(3,4)                 /*!< LXTAL drive capability */
#define RCU_BDCTL_RTCSRC            BITS(8,9)                 /*!< RTC clock entry selection */
#define RCU_BDCTL_RTCEN             BIT(15)                   /*!< RTC clock enable */
#define RCU_BDCTL_BKPRST            BIT(16)                   /*!< backup domain reset */

/* RCU_RSTSCK */
#define RCU_RSTSCK_IRC32KEN         BIT(0)                    /*!< IRC32K enable */
#define RCU_RSTSCK_IRC32KSTB        BIT(1)                    /*!< IRC32K stabilization */
#define RCU_RSTSCK_V12RSTF          BIT(23)                   /*!< V12 domain power reset flag */
#define RCU_RSTSCK_RSTFC            BIT(24)                   /*!< reset flag clear */
#define RCU_RSTSCK_EPRSTF           BIT(26)                   /*!< external pin reset flag */
#define RCU_RSTSCK_PORRSTF          BIT(27)                   /*!< power reset flag */
#define RCU_RSTSCK_SWRSTF           BIT(28)                   /*!< software reset flag */
#define RCU_RSTSCK_FWDGTRSTF        BIT(29)                   /*!< free watchdog timer reset flag */
#define RCU_RSTSCK_WWDGTRSTF        BIT(30)                   /*!< window watchdog timer reset flag */
#define RCU_RSTSCK_LPRSTF           BIT(31)                   /*!< low-power reset flag */

/* RCU_AHBRST */
#define RCU_AHBRST_CRCRST           BIT(6)                    /*!< CRC reset */
#define RCU_AHBRST_PARST            BIT(17)                   /*!< GPIO port A reset */
#define RCU_AHBRST_PBRST            BIT(18)                   /*!< GPIO port B reset */
#define RCU_AHBRST_PCRST            BIT(19)                   /*!< GPIO port C reset */
#define RCU_AHBRST_PDRST            BIT(20)                   /*!< GPIO port D reset */
#define RCU_AHBRST_PFRST            BIT(22)                   /*!< GPIO port F reset */

/* RCU_CFG1 */
#define RCU_CFG1_PREDV              BITS(0,3)                 /*!< CK_HXTAL divider previous PLL */

/* RCU_CFG2 */
#define RCU_CFG2_USART0SEL          BITS(0,1)                 /*!< CK_USART0 clock source selection */
#define RCU_CFG2_I2C0SEL            BITS(2,3)                 /*!< CK_I2C0 clock source selection */
#define RCU_CFG2_I2C1SEL            BITS(4,5)                 /*!< CK_I2C1 clock source selection */
#define RCU_CFG2_I2C2SEL            BITS(6,7)                 /*!< CK_I2C2 clock source selection */
#define RCU_CFG2_ADCSEL             BIT(8)                    /*!< CK_ADC clock source selection */
#define RCU_CFG2_LPTIMERSEL         BITS(9,10)                /*!< CK_LPTIMER clock source selection */
#define RCU_CFG2_LPUARTSEL          BITS(11,12)               /*!< CK_LPUART clock source selection */
#define RCU_CFG2_USBDSEL            BIT(13)                   /*!< CK_USBD clock source selection */
#define RCU_CFG2_USART1SEL          BITS(16,17)               /*!< CK_USART0 clock source selection */
#define RCU_CFG2_IRC16MDIVSEL       BITS(18,20)               /*!< CK_IRC16M divided clock selection */
#define RCU_CFG2_ADCPSC2            BITS(30,31)               /*!< bits of ADCPSC */

/* RCU_AHB2EN */
#define RCU_AHB2EN_CAUEN            BIT(1)                    /*!< CAU clock enable */
#define RCU_AHB2EN_TRNGEN           BIT(3)                    /*!< TRNG clock enable */

/* RCU_AHB2RST */
#define RCU_AHB2RST_CAURST          BIT(1)                    /*!< CRU reset */
#define RCU_AHBR2ST_TRNGRST         BIT(3)                    /*!< TRNG reset */
/* RCU_VKEY */
#define RCU_VKEY_KEY                BITS(0,31)                /*!< key of RCU_DSV register */

/* RCU_LPLDO */
#define RCU_LPLDO_LPLDOVOS          BIT(0)                    /*!< Deep-sleep 1/2 mode voltage select */

/* RCU_LPB */
#define RCU_LPB_LPBMSEL             BITS(0,1)                 /*!< Low power mode selection signal */

/* constants definitions */
/* define the peripheral clock enable bit position and its register index offset */
#define RCU_REGIDX_BIT(regidx, bitpos)      (((uint32_t)(regidx)<<6) | (uint32_t)(bitpos))
#define RCU_REG_VAL(periph)                 (REG32(RCU + ((uint32_t)(periph)>>6)))
#define RCU_BIT_POS(val)                    ((uint32_t)(val) & 0x1FU)
/* define the voltage key unlock value */
#define RCU_VKEY_UNLOCK                     ((uint32_t)0x1A2B3C4D)

/* register index */
/* peripherals enable */
#define AHBEN_REG_OFFSET                0x14U                     /*!< AHB enable register offset */
#define APB1EN_REG_OFFSET               0x1CU                     /*!< APB1 enable register offset */
#define APB2EN_REG_OFFSET               0x18U                     /*!< APB2 enable register offset */
#define AHB2_REG_OFFSET                 0x34U                     /*!< AHB2 enable register offset */

/* peripherals reset */
#define AHBRST_REG_OFFSET               0x28U                     /*!< AHB reset register offset */
#define APB1RST_REG_OFFSET              0x10U                     /*!< APB1 reset register offset */
#define APB2RST_REG_OFFSET              0x0CU                     /*!< APB2 reset register offset */
#define AHB2RST_REG_OFFSET              0x38U                     /*!< AHB2 reset register offset */
#define RSTSCK_REG_OFFSET               0x24U                     /*!< reset source/clock register offset */

/* clock control */
#define CTL_REG_OFFSET                  0x00U                     /*!< control register offset */
#define BDCTL_REG_OFFSET                0x20U                     /*!< backup domain control register offset */

/* clock stabilization and stuck interrupt */
#define INT_REG_OFFSET                  0x08U                     /*!< clock interrupt register offset */

/* configuration register */
#define CFG0_REG_OFFSET                 0x04U                     /*!< clock configuration register 0 offset */
#define CFG1_REG_OFFSET                 0x2CU                     /*!< clock configuration register 1 offset */
#define CFG2_REG_OFFSET                 0x30U                     /*!< clock configuration register 2 offset */

/* peripheral clock enable */
typedef enum {
    /* AHB peripherals */
    RCU_DMA     = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 0U),                /*!< DMA clock */
    RCU_CAU     = RCU_REGIDX_BIT(AHB2_REG_OFFSET, 1U),                 /*!< CAU clock */
    RCU_TRNG    = RCU_REGIDX_BIT(AHB2_REG_OFFSET, 3U),                 /*!< TRNG clock */
    RCU_CRC     = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 6U),                /*!< CRC clock */
    RCU_GPIOA   = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 17U),               /*!< GPIOA clock */
    RCU_GPIOB   = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 18U),               /*!< GPIOB clock */
    RCU_GPIOC   = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 19U),               /*!< GPIOC clock */
    RCU_GPIOD   = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 20U),               /*!< GPIOD clock */
    RCU_GPIOF   = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 22U),               /*!< GPIOF clock */

    /* APB2 peripherals */
    RCU_SYSCFG  = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 0U),               /*!< SYSCFG clock */
    RCU_CMP     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 1U),               /*!< CMP clock */
    RCU_ADC     = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 9U),               /*!< ADC clock */
    RCU_TIMER8  = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 11U),              /*!< TIMER0 clock */
    RCU_SPI0    = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 12U),              /*!< SPI0 clock */
    RCU_USART0  = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 14U),              /*!< USART0 clock */
    RCU_DBGMCU  = RCU_REGIDX_BIT(APB2EN_REG_OFFSET, 22U),              /*!< DBGMCU clock */

    /* APB1 peripherals */
    RCU_TIMER1  = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 0U),               /*!< TIMER1 clock */
    RCU_TIMER2  = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 1U),               /*!< TIMER2 clock */
    RCU_TIMER5  = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 4U),               /*!< TIMER5 clock */
    RCU_TIMER6  = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 5U),               /*!< TIMER6 clock */
    RCU_TIMER11 = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 8U),               /*!< TIMER11 clock */
    RCU_LPTIMER = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 9U),               /*!< LPTIMER clock */
    RCU_SLCD    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 10U),              /*!< SLCD clock */
    RCU_WWDGT   = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 11U),              /*!< WWDGT clock */
    RCU_SPI1    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 14U),              /*!< SPI1 clock */
    RCU_USART1  = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 17U),              /*!< USART1 clock */
    RCU_LPUART  = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 18U),              /*!< LPUART clock */
    RCU_UART3   = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 19U),              /*!< UART3 clock */
    RCU_UART4   = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 20U),              /*!< UART4 clock */
    RCU_I2C0    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 21U),              /*!< I2C0 clock */
    RCU_I2C1    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 22U),              /*!< I2C1 clock */
    RCU_USBD    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 23U),              /*!< USBD clock */
    RCU_I2C2    = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 24U),              /*!< I2C2 clock */
    RCU_PMU     = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 28U),              /*!< PMU clock */
    RCU_DAC     = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 29U),              /*!< DAC clock */
    RCU_CTC     = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 30U),              /*!< CTC clock */
    RCU_BKP     = RCU_REGIDX_BIT(APB1EN_REG_OFFSET, 31U),              /*!< BKP clock */

    /* Backup domain control(BDCTL) */
    RCU_RTC     = RCU_REGIDX_BIT(BDCTL_REG_OFFSET, 15U)                /*!< RTC clock */
} rcu_periph_enum;

/* peripheral clock enable when sleep mode*/
typedef enum {
    /* AHB peripherals */
    RCU_SRAM0_SLP    = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 2U),          /*!< SRAM0 clock */
    RCU_FMC_SLP      = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 4U),          /*!< FMC clock */
    RCU_SRAM1_SLP    = RCU_REGIDX_BIT(AHBEN_REG_OFFSET, 7U),          /*!< SRAM1 clock */
} rcu_periph_sleep_enum;

/* peripherals reset */
typedef enum {
    /* AHB peripherals reset */
    RCU_CAURST     = RCU_REGIDX_BIT(AHB2RST_REG_OFFSET, 1U),             /*!< CAU reset */
    RCU_TRNGRST    = RCU_REGIDX_BIT(AHB2RST_REG_OFFSET, 3U),             /*!< TRNG reset */
    RCU_CRCRST     = RCU_REGIDX_BIT(AHBRST_REG_OFFSET, 6U),              /*!< CRC reset */
    RCU_GPIOARST   = RCU_REGIDX_BIT(AHBRST_REG_OFFSET, 17U),             /*!< GPIOA reset */
    RCU_GPIOBRST   = RCU_REGIDX_BIT(AHBRST_REG_OFFSET, 18U),             /*!< GPIOB reset */
    RCU_GPIOCRST   = RCU_REGIDX_BIT(AHBRST_REG_OFFSET, 19U),             /*!< GPIOC reset */
    RCU_GPIODRST   = RCU_REGIDX_BIT(AHBRST_REG_OFFSET, 19U),             /*!< GPIOD reset */
    RCU_GPIOFRST   = RCU_REGIDX_BIT(AHBRST_REG_OFFSET, 22U),             /*!< GPIOF reset */

    /* APB2 peripherals reset */
    RCU_SYSCFGRST  = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 0U),             /*!< SYSCFG reset */
    RCU_CMPRST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 1U),             /*!< CMP reset */
    RCU_ADCRST     = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 9U),             /*!< ADC reset */
    RCU_TIMER8RST  = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 11U),            /*!< TIMER8 reset */
    RCU_SPI0RST    = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 12U),            /*!< SPI0 reset */
    RCU_USART0RST  = RCU_REGIDX_BIT(APB2RST_REG_OFFSET, 14U),            /*!< USART0 reset */

    /* APB1 peripherals reset */
    RCU_TIMER1RST  = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 0U),             /*!< TIMER1 reset */
    RCU_TIMER2RST  = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 1U),             /*!< TIMER2 reset */
    RCU_TIMER5RST  = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 4U),             /*!< TIMER5 reset */
    RCU_TIMER6RST  = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 5U),             /*!< TIMER6 reset */
    RCU_TIMER11RST = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 8U),             /*!< TIMER11 reset */
    RCU_LPTIMERRST = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 9U),             /*!< LPTIMER reset */
    RCU_SLCDRST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 10U),            /*!< SLCD reset */
    RCU_WWDGTRST   = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 11U),            /*!< WWDGT reset */
    RCU_SPI1RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 14U),            /*!< SPI1 reset */
    RCU_USART1RST  = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 17U),            /*!< USART1 reset */
    RCU_LPUARTRST  = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 18U),            /*!< LPUART reset */
    RCU_UART3RST   = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 19U),            /*!< UART3 reset */
    RCU_UART4RST   = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 20U),            /*!< UART4 reset */
    RCU_I2C0RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 21U),            /*!< I2C0 reset */
    RCU_I2C1RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 22U),            /*!< I2C1 reset */
    RCU_USBDRST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 23U),            /*!< USBD reset */
    RCU_I2C2RST    = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 24U),            /*!< I2C2 reset */
    RCU_PMURST     = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 28U),            /*!< PMU reset */
    RCU_DACRST     = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 29U),            /*!< DAC reset */
    RCU_CTCRST     = RCU_REGIDX_BIT(APB1RST_REG_OFFSET, 30U),            /*!< CTC reset */
} rcu_periph_reset_enum;

/* clock stabilization and peripheral reset flags */
typedef enum {
    RCU_FLAG_IRC32KSTB    = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 1U),       /*!< IRC32K stabilization flags */
    RCU_FLAG_LXTALSTB     = RCU_REGIDX_BIT(BDCTL_REG_OFFSET, 1U),        /*!< LXTAL stabilization flags */
    RCU_FLAG_IRC16MSTB    = RCU_REGIDX_BIT(CTL_REG_OFFSET, 1U),          /*!< IRC16M stabilization flags */
    RCU_FLAG_HXTALSTB     = RCU_REGIDX_BIT(CTL_REG_OFFSET, 17U),         /*!< HXTAL stabilization flags */
    RCU_FLAG_IRC48MSTB    = RCU_REGIDX_BIT(CTL_REG_OFFSET, 21U),         /*!< IRC48M stabilization flags */
    RCU_FLAG_PLLSTB       = RCU_REGIDX_BIT(CTL_REG_OFFSET, 25U),         /*!< PLL stabilization flags */

    RCU_FLAG_V12RST       = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 23U),      /*!< V12 reset flags */
    RCU_FLAG_EPRST        = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 26U),      /*!< EPR reset flags */
    RCU_FLAG_PORRST       = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 27U),      /*!< power reset flags */
    RCU_FLAG_SWRST        = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 28U),      /*!< SW reset flags */
    RCU_FLAG_FWDGTRST     = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 29U),      /*!< FWDGT reset flags */
    RCU_FLAG_WWDGTRST     = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 30U),      /*!< WWDGT reset flags */
    RCU_FLAG_LPRST        = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 31U)       /*!< LP reset flags */
} rcu_flag_enum;

/* clock stabilization and ckm interrupt flags */
typedef enum {
    RCU_INT_FLAG_IRC32KSTB = RCU_REGIDX_BIT(INT_REG_OFFSET, 0U),         /*!< IRC32K stabilization interrupt flag */
    RCU_INT_FLAG_LXTALSTB  = RCU_REGIDX_BIT(INT_REG_OFFSET, 1U),         /*!< LXTAL stabilization interrupt flag */
    RCU_INT_FLAG_IRC16MSTB = RCU_REGIDX_BIT(INT_REG_OFFSET, 2U),         /*!< IRC16M stabilization interrupt flag */
    RCU_INT_FLAG_HXTALSTB  = RCU_REGIDX_BIT(INT_REG_OFFSET, 3U),         /*!< HXTAL stabilization interrupt flag */
    RCU_INT_FLAG_PLLSTB    = RCU_REGIDX_BIT(INT_REG_OFFSET, 4U),         /*!< PLL stabilization interrupt flag */
    RCU_INT_FLAG_IRC48MSTB = RCU_REGIDX_BIT(INT_REG_OFFSET, 5U),         /*!< IRC48M stabilization interrupt flag */
    RCU_INT_FLAG_LXTALCKM = RCU_REGIDX_BIT(INT_REG_OFFSET, 6U),          /*!< LXTAL clock stuck interrupt flag */
    RCU_INT_FLAG_CKM       = RCU_REGIDX_BIT(INT_REG_OFFSET, 7U),         /*!< CKM interrupt flag */
} rcu_int_flag_enum;

/* clock stabilization and stuck interrupt flags clear */
typedef enum {
    RCU_INT_FLAG_IRC32KSTB_CLR = RCU_REGIDX_BIT(INT_REG_OFFSET, 16U),    /*!< IRC32K stabilization interrupt flags clear */
    RCU_INT_FLAG_LXTALSTB_CLR  = RCU_REGIDX_BIT(INT_REG_OFFSET, 17U),    /*!< LXTAL stabilization interrupt flags clear */
    RCU_INT_FLAG_IRC16MSTB_CLR = RCU_REGIDX_BIT(INT_REG_OFFSET, 18U),    /*!< IRC16M stabilization interrupt flags clear */
    RCU_INT_FLAG_HXTALSTB_CLR  = RCU_REGIDX_BIT(INT_REG_OFFSET, 19U),    /*!< HXTAL stabilization interrupt flags clear */
    RCU_INT_FLAG_PLLSTB_CLR    = RCU_REGIDX_BIT(INT_REG_OFFSET, 20U),    /*!< PLL stabilization interrupt flags clear */
    RCU_INT_FLAG_IRC48MSTB_CLR = RCU_REGIDX_BIT(INT_REG_OFFSET, 21U),    /*!< IRC48M stabilization interrupt flags clear */
    RCU_INT_FLAG_LXTALCKM_CLR  = RCU_REGIDX_BIT(INT_REG_OFFSET, 22U),    /*!< LXTAL clock stuck interrupt flag clear */
    RCU_INT_FLAG_CKM_CLR       = RCU_REGIDX_BIT(INT_REG_OFFSET, 23U),    /*!< CKM interrupt flags clear */
} rcu_int_flag_clear_enum;

/* clock stabilization interrupt enable or disable */
typedef enum {
    RCU_INT_IRC32KSTB       = RCU_REGIDX_BIT(INT_REG_OFFSET, 8U),        /*!< IRC32K stabilization interrupt */
    RCU_INT_LXTALSTB        = RCU_REGIDX_BIT(INT_REG_OFFSET, 9U),        /*!< LXTAL stabilization interrupt */
    RCU_INT_IRC16MSTB       = RCU_REGIDX_BIT(INT_REG_OFFSET, 10U),       /*!< IRC16M stabilization interrupt */
    RCU_INT_HXTALSTB        = RCU_REGIDX_BIT(INT_REG_OFFSET, 11U),       /*!< HXTAL stabilization interrupt */
    RCU_INT_PLLSTB          = RCU_REGIDX_BIT(INT_REG_OFFSET, 12U),       /*!< PLL stabilization interrupt */
    RCU_INT_IRC48MSTB       = RCU_REGIDX_BIT(INT_REG_OFFSET, 13U),       /*!< IRC48M stabilization interrupt */
    RCU_INT_LXTALCKM        = RCU_REGIDX_BIT(INT_REG_OFFSET, 14U),       /*!< LXTAL clock stuck interrupt */
} rcu_int_enum;

/* oscillator types */
typedef enum {
    RCU_HXTAL   = RCU_REGIDX_BIT(CTL_REG_OFFSET, 16U),                  /*!< HXTAL */
    RCU_LXTAL   = RCU_REGIDX_BIT(BDCTL_REG_OFFSET, 0U),                 /*!< LXTAL */
    RCU_IRC16M  = RCU_REGIDX_BIT(CTL_REG_OFFSET, 0U),                   /*!< IRC16M */
    RCU_IRC48M  = RCU_REGIDX_BIT(CTL_REG_OFFSET, 20U),                  /*!< IRC48M */
    RCU_IRC32K  = RCU_REGIDX_BIT(RSTSCK_REG_OFFSET, 0U),                /*!< IRC32K */
    RCU_PLL_CK  = RCU_REGIDX_BIT(CTL_REG_OFFSET, 24U)                   /*!< PLL */
} rcu_osci_type_enum;

/* rcu clock frequency */
typedef enum {
    CK_SYS      = 0U,                                             /*!< system clock */
    CK_AHB,                                                       /*!< AHB clock */
    CK_APB1,                                                      /*!< APB1 clock */
    CK_APB2,                                                      /*!< APB2 clock */
    CK_ADC,                                                       /*!< ADC clock */
    CK_USART0,                                                    /*!< USART0 clock */
    CK_I2C0,                                                      /*!< I2C0 clock */
    CK_I2C1,                                                      /*!< I2C1 clock */
    CK_I2C2,                                                      /*!< I2C2 clock */
    CK_LPUART,                                                    /*!< LPUART clock */
    CK_USART1,                                                    /*!< USART1 clock */
    CK_LPTIMER                                                    /*!< LPTIMER clock */
} rcu_clock_freq_enum;

typedef enum {
    IDX_USART0 = 0U,                                              /*!< idnex of USART0 */
    IDX_USART1                                                    /*!< idnex of USART1 */
} usart_idx_enum;

typedef enum {
    IDX_I2C0 = 0U,                                                /*!< idnex of I2C0 */
    IDX_I2C1,                                                     /*!< idnex of I2C1 */
    IDX_I2C2                                                      /*!< idnex of I2C2 */
} i2c_idx_enum;

/* system clock source select */
#define CFG0_SCS(regval)            (BITS(0,1) & ((uint32_t)(regval) << 0U))
#define RCU_CKSYSSRC_IRC16M          CFG0_SCS(0)                   /*!< system clock source select IRC16M */
#define RCU_CKSYSSRC_HXTAL           CFG0_SCS(1)                   /*!< system clock source select HXTAL */
#define RCU_CKSYSSRC_PLL             CFG0_SCS(2)                   /*!< system clock source select PLL */
#define RCU_CKSYSSRC_IRC48M          CFG0_SCS(3)                   /*!< system clock source select IRC48M */

/* system clock source select status */
#define CFG0_SCSS(regval)           (BITS(2,3) & ((uint32_t)(regval) << 2U))
#define RCU_SCSS_IRC16M              CFG0_SCSS(0)                  /*!< system clock source select IRC16M */
#define RCU_SCSS_HXTAL               CFG0_SCSS(1)                  /*!< system clock source select HXTAL */
#define RCU_SCSS_PLL                 CFG0_SCSS(2)                  /*!< system clock source select PLL */
#define RCU_SCSS_IRC48M              CFG0_SCSS(3)                  /*!< system clock source select IRC48M */

/* AHB prescaler selection */
#define CFG0_AHBPSC(regval)         (BITS(4,7) & ((uint32_t)(regval) << 4U))
#define RCU_AHB_CKSYS_DIV1          CFG0_AHBPSC(0)                /*!< AHB prescaler select CK_SYS */
#define RCU_AHB_CKSYS_DIV2          CFG0_AHBPSC(8)                /*!< AHB prescaler select CK_SYS/2 */
#define RCU_AHB_CKSYS_DIV4          CFG0_AHBPSC(9)                /*!< AHB prescaler select CK_SYS/4 */
#define RCU_AHB_CKSYS_DIV8          CFG0_AHBPSC(10)               /*!< AHB prescaler select CK_SYS/8 */
#define RCU_AHB_CKSYS_DIV16         CFG0_AHBPSC(11)               /*!< AHB prescaler select CK_SYS/16 */
#define RCU_AHB_CKSYS_DIV64         CFG0_AHBPSC(12)               /*!< AHB prescaler select CK_SYS/64 */
#define RCU_AHB_CKSYS_DIV128        CFG0_AHBPSC(13)               /*!< AHB prescaler select CK_SYS/128 */
#define RCU_AHB_CKSYS_DIV256        CFG0_AHBPSC(14)               /*!< AHB prescaler select CK_SYS/256 */
#define RCU_AHB_CKSYS_DIV512        CFG0_AHBPSC(15)               /*!< AHB prescaler select CK_SYS/512 */

/* APB1 prescaler selection */
#define CFG0_APB1PSC(regval)        (BITS(8,10) & ((uint32_t)(regval) << 8U))
#define RCU_APB1_CKAHB_DIV1         CFG0_APB1PSC(0)               /*!< APB1 prescaler select CK_AHB */
#define RCU_APB1_CKAHB_DIV2         CFG0_APB1PSC(4)               /*!< APB1 prescaler select CK_AHB/2 */
#define RCU_APB1_CKAHB_DIV4         CFG0_APB1PSC(5)               /*!< APB1 prescaler select CK_AHB/4 */
#define RCU_APB1_CKAHB_DIV8         CFG0_APB1PSC(6)               /*!< APB1 prescaler select CK_AHB/8 */
#define RCU_APB1_CKAHB_DIV16        CFG0_APB1PSC(7)               /*!< APB1 prescaler select CK_AHB/16 */

/* APB2 prescaler selection */
#define CFG0_APB2PSC(regval)        (BITS(11,13) & ((uint32_t)(regval) << 11U))
#define RCU_APB2_CKAHB_DIV1         CFG0_APB2PSC(0)               /*!< APB2 prescaler select CK_AHB */
#define RCU_APB2_CKAHB_DIV2         CFG0_APB2PSC(4)               /*!< APB2 prescaler select CK_AHB/2 */
#define RCU_APB2_CKAHB_DIV4         CFG0_APB2PSC(5)               /*!< APB2 prescaler select CK_AHB/4 */
#define RCU_APB2_CKAHB_DIV8         CFG0_APB2PSC(6)               /*!< APB2 prescaler select CK_AHB/8 */
#define RCU_APB2_CKAHB_DIV16        CFG0_APB2PSC(7)               /*!< APB2 prescaler select CK_AHB/16 */

/* ADC clock prescaler selection */
#define CFG0_ADCPSC(regval)         (BITS(14,15) & ((uint32_t)(regval) << 14U))
#define CFG2_ADCPSC(regval)         (BITS(30,31) & ((uint32_t)(regval) << 30U))
#define CFG_ADCPSC(regval)          (BITS(0,4)& ((uint32_t)(regval)))
#define RCU_ADCCK_IRC16M            CFG_ADCPSC(16)                /*!< ADC clock select IRC16M */
#define RCU_ADCCK_APB2_DIV2         CFG_ADCPSC(0)                 /*!< ADC clock prescaler select CK_APB2/2 */
#define RCU_ADCCK_APB2_DIV4         CFG_ADCPSC(1)                 /*!< ADC clock prescaler select CK_APB2/4 */
#define RCU_ADCCK_APB2_DIV6         CFG_ADCPSC(2)                 /*!< ADC clock prescaler select CK_APB2/6 */
#define RCU_ADCCK_APB2_DIV8         CFG_ADCPSC(3)                 /*!< ADC clock prescaler select CK_APB2/8 */
#define RCU_ADCCK_APB2_DIV10        CFG_ADCPSC(4)                 /*!< ADC clock prescaler select CK_APB2/10 */
#define RCU_ADCCK_APB2_DIV12        CFG_ADCPSC(5)                 /*!< ADC clock prescaler select CK_APB2/12 */
#define RCU_ADCCK_APB2_DIV14        CFG_ADCPSC(6)                 /*!< ADC clock prescaler select CK_APB2/14 */
#define RCU_ADCCK_APB2_DIV16        CFG_ADCPSC(7)                 /*!< ADC clock prescaler select CK_APB2/16 */
#define RCU_ADCCK_AHB_DIV3          CFG_ADCPSC(8)                 /*!< ADC clock prescaler select CK_AHB/3 */
#define RCU_ADCCK_AHB_DIV5          CFG_ADCPSC(9)                 /*!< ADC clock prescaler select CK_AHB/5 */
#define RCU_ADCCK_AHB_DIV7          CFG_ADCPSC(10)                /*!< ADC clock prescaler select CK_AHB/7 */
#define RCU_ADCCK_AHB_DIV9          CFG_ADCPSC(11)                /*!< ADC clock prescaler select CK_AHB/9 */
#define RCU_ADCCK_AHB_DIV11         CFG_ADCPSC(12)                /*!< ADC clock prescaler select CK_AHB/11 */
#define RCU_ADCCK_AHB_DIV13         CFG_ADCPSC(13)                /*!< ADC clock prescaler select CK_AHB/13 */
#define RCU_ADCCK_AHB_DIV15         CFG_ADCPSC(14)                /*!< ADC clock prescaler select CK_AHB/15 */
#define RCU_ADCCK_AHB_DIV17         CFG_ADCPSC(15)                /*!< ADC clock prescaler select CK_AHB/17 */

/* PLL clock source selection */
#define CFG0_PLLSRC(regval)         (BITS(16,17) & ((uint32_t)(regval) << 16U))
#define RCU_PLLSRC_IRC16M           CFG0_PLLSRC(0)               /*!< PLL clock source select IRC16M */
#define RCU_PLLSRC_HXTAL            CFG0_PLLSRC(1)               /*!< PLL clock source select HXTAL */
#define RCU_PLLSRC_IRC48M           CFG0_PLLSRC(2)               /*!< PLL clock source select IRC48M */

/* PLL multiply factor */
#define PLLMF_6                     RCU_CFG0_PLLMF_6                    /*!< bit 6 of PLLMF */
#define CFG0_PLLMF(regval)          (BITS(18,23) & ((uint32_t)(regval) << 18U))
#define RCU_PLL_MUL4                CFG0_PLLMF(2)                 /*!< PLL source clock multiply by 4 */
#define RCU_PLL_MUL5                CFG0_PLLMF(3)                 /*!< PLL source clock multiply by 5 */
#define RCU_PLL_MUL6                CFG0_PLLMF(4)                 /*!< PLL source clock multiply by 6 */
#define RCU_PLL_MUL7                CFG0_PLLMF(5)                 /*!< PLL source clock multiply by 7 */
#define RCU_PLL_MUL8                CFG0_PLLMF(6)                 /*!< PLL source clock multiply by 8 */
#define RCU_PLL_MUL9                CFG0_PLLMF(7)                 /*!< PLL source clock multiply by 9 */
#define RCU_PLL_MUL10               CFG0_PLLMF(8)                 /*!< PLL source clock multiply by 10 */
#define RCU_PLL_MUL11               CFG0_PLLMF(9)                 /*!< PLL source clock multiply by 11 */
#define RCU_PLL_MUL12               CFG0_PLLMF(10)                /*!< PLL source clock multiply by 12 */
#define RCU_PLL_MUL13               CFG0_PLLMF(11)                /*!< PLL source clock multiply by 13 */
#define RCU_PLL_MUL14               CFG0_PLLMF(12)                /*!< PLL source clock multiply by 14 */
#define RCU_PLL_MUL15               CFG0_PLLMF(13)                /*!< PLL source clock multiply by 15 */
#define RCU_PLL_MUL16               CFG0_PLLMF(14)                /*!< PLL source clock multiply by 16 */
#define RCU_PLL_MUL17               CFG0_PLLMF(16)                /*!< PLL source clock multiply by 17 */
#define RCU_PLL_MUL18               CFG0_PLLMF(17)                /*!< PLL source clock multiply by 18 */
#define RCU_PLL_MUL19               CFG0_PLLMF(18)                /*!< PLL source clock multiply by 19 */
#define RCU_PLL_MUL20               CFG0_PLLMF(19)                /*!< PLL source clock multiply by 20 */
#define RCU_PLL_MUL21               CFG0_PLLMF(20)                /*!< PLL source clock multiply by 21 */
#define RCU_PLL_MUL22               CFG0_PLLMF(21)                /*!< PLL source clock multiply by 22 */
#define RCU_PLL_MUL23               CFG0_PLLMF(22)                /*!< PLL source clock multiply by 23 */
#define RCU_PLL_MUL24               CFG0_PLLMF(23)                /*!< PLL source clock multiply by 24 */
#define RCU_PLL_MUL25               CFG0_PLLMF(24)                /*!< PLL source clock multiply by 25 */
#define RCU_PLL_MUL26               CFG0_PLLMF(25)                /*!< PLL source clock multiply by 26 */
#define RCU_PLL_MUL27               CFG0_PLLMF(26)                /*!< PLL source clock multiply by 27 */
#define RCU_PLL_MUL28               CFG0_PLLMF(27)                /*!< PLL source clock multiply by 28 */
#define RCU_PLL_MUL29               CFG0_PLLMF(28)                /*!< PLL source clock multiply by 29 */
#define RCU_PLL_MUL30               CFG0_PLLMF(29)                /*!< PLL source clock multiply by 30 */
#define RCU_PLL_MUL31               CFG0_PLLMF(30)                /*!< PLL source clock multiply by 31 */
#define RCU_PLL_MUL32               CFG0_PLLMF(31)                /*!< PLL source clock multiply by 32 */
#define RCU_PLL_MUL33               CFG0_PLLMF(32)                /*!< PLL source clock multiply by 33 */
#define RCU_PLL_MUL34               CFG0_PLLMF(33)                /*!< PLL source clock multiply by 34 */
#define RCU_PLL_MUL35               CFG0_PLLMF(34)                /*!< PLL source clock multiply by 35 */
#define RCU_PLL_MUL36               CFG0_PLLMF(35)                /*!< PLL source clock multiply by 36 */
#define RCU_PLL_MUL37               CFG0_PLLMF(36)                /*!< PLL source clock multiply by 37 */
#define RCU_PLL_MUL38               CFG0_PLLMF(37)                /*!< PLL source clock multiply by 38 */
#define RCU_PLL_MUL39               CFG0_PLLMF(38)                /*!< PLL source clock multiply by 39 */
#define RCU_PLL_MUL40               CFG0_PLLMF(39)                /*!< PLL source clock multiply by 40 */
#define RCU_PLL_MUL41               CFG0_PLLMF(40)                /*!< PLL source clock multiply by 41 */
#define RCU_PLL_MUL42               CFG0_PLLMF(41)                /*!< PLL source clock multiply by 42 */
#define RCU_PLL_MUL43               CFG0_PLLMF(42)                /*!< PLL source clock multiply by 43 */
#define RCU_PLL_MUL44               CFG0_PLLMF(43)                /*!< PLL source clock multiply by 44 */
#define RCU_PLL_MUL45               CFG0_PLLMF(44)                /*!< PLL source clock multiply by 45 */
#define RCU_PLL_MUL46               CFG0_PLLMF(45)                /*!< PLL source clock multiply by 46 */
#define RCU_PLL_MUL47               CFG0_PLLMF(46)                /*!< PLL source clock multiply by 47 */
#define RCU_PLL_MUL48               CFG0_PLLMF(47)                /*!< PLL source clock multiply by 48 */
#define RCU_PLL_MUL49               CFG0_PLLMF(48)                /*!< PLL source clock multiply by 49 */
#define RCU_PLL_MUL50               CFG0_PLLMF(49)                /*!< PLL source clock multiply by 50 */
#define RCU_PLL_MUL51               CFG0_PLLMF(50)                /*!< PLL source clock multiply by 51 */
#define RCU_PLL_MUL52               CFG0_PLLMF(51)                /*!< PLL source clock multiply by 52 */
#define RCU_PLL_MUL53               CFG0_PLLMF(52)                /*!< PLL source clock multiply by 53 */
#define RCU_PLL_MUL54               CFG0_PLLMF(53)                /*!< PLL source clock multiply by 54 */
#define RCU_PLL_MUL55               CFG0_PLLMF(54)                /*!< PLL source clock multiply by 55 */
#define RCU_PLL_MUL56               CFG0_PLLMF(55)                /*!< PLL source clock multiply by 56 */
#define RCU_PLL_MUL57               CFG0_PLLMF(56)                /*!< PLL source clock multiply by 57 */
#define RCU_PLL_MUL58               CFG0_PLLMF(57)                /*!< PLL source clock multiply by 58 */
#define RCU_PLL_MUL59               CFG0_PLLMF(58)                /*!< PLL source clock multiply by 59 */
#define RCU_PLL_MUL60               CFG0_PLLMF(59)                /*!< PLL source clock multiply by 60 */
#define RCU_PLL_MUL61               CFG0_PLLMF(60)                /*!< PLL source clock multiply by 61 */
#define RCU_PLL_MUL62               CFG0_PLLMF(61)                /*!< PLL source clock multiply by 62 */
#define RCU_PLL_MUL63               CFG0_PLLMF(62)                /*!< PLL source clock multiply by 63 */
#define RCU_PLL_MUL64               CFG0_PLLMF(63)                /*!< PLL source clock multiply by 64 */
#define RCU_PLL_MUL63               CFG0_PLLMF(62)                /*!< PLL source clock multiply by 63 */
#define RCU_PLL_MUL64               CFG0_PLLMF(63)                /*!< PLL source clock multiply by 64 */
#define RCU_PLL_MUL65              (PLLMF_6 | CFG0_PLLMF(0))      /*!< PLL source clock multiply by 65 */
#define RCU_PLL_MUL66              (PLLMF_6 | CFG0_PLLMF(1))      /*!< PLL source clock multiply by 66 */
#define RCU_PLL_MUL67              (PLLMF_6 | CFG0_PLLMF(2))      /*!< PLL source clock multiply by 67 */
#define RCU_PLL_MUL68              (PLLMF_6 | CFG0_PLLMF(3))      /*!< PLL source clock multiply by 68 */
#define RCU_PLL_MUL69              (PLLMF_6 | CFG0_PLLMF(4))      /*!< PLL source clock multiply by 69 */
#define RCU_PLL_MUL70              (PLLMF_6 | CFG0_PLLMF(5))      /*!< PLL source clock multiply by 70 */
#define RCU_PLL_MUL71              (PLLMF_6 | CFG0_PLLMF(6))      /*!< PLL source clock multiply by 71 */
#define RCU_PLL_MUL72              (PLLMF_6 | CFG0_PLLMF(7))      /*!< PLL source clock multiply by 72 */
#define RCU_PLL_MUL73              (PLLMF_6 | CFG0_PLLMF(8))      /*!< PLL source clock multiply by 73 */
#define RCU_PLL_MUL74              (PLLMF_6 | CFG0_PLLMF(9))      /*!< PLL source clock multiply by 74 */
#define RCU_PLL_MUL75              (PLLMF_6 | CFG0_PLLMF(10))     /*!< PLL source clock multiply by 75 */
#define RCU_PLL_MUL76              (PLLMF_6 | CFG0_PLLMF(11))     /*!< PLL source clock multiply by 76 */
#define RCU_PLL_MUL77              (PLLMF_6 | CFG0_PLLMF(12))     /*!< PLL source clock multiply by 77 */
#define RCU_PLL_MUL78              (PLLMF_6 | CFG0_PLLMF(13))     /*!< PLL source clock multiply by 78 */
#define RCU_PLL_MUL79              (PLLMF_6 | CFG0_PLLMF(14))     /*!< PLL source clock multiply by 79 */
#define RCU_PLL_MUL80              (PLLMF_6 | CFG0_PLLMF(15))     /*!< PLL source clock multiply by 80 */
#define RCU_PLL_MUL81              (PLLMF_6 | CFG0_PLLMF(16))     /*!< PLL source clock multiply by 81 */
#define RCU_PLL_MUL82              (PLLMF_6 | CFG0_PLLMF(17))     /*!< PLL source clock multiply by 82 */
#define RCU_PLL_MUL83              (PLLMF_6 | CFG0_PLLMF(18))     /*!< PLL source clock multiply by 83 */
#define RCU_PLL_MUL84              (PLLMF_6 | CFG0_PLLMF(19))     /*!< PLL source clock multiply by 84 */
#define RCU_PLL_MUL85              (PLLMF_6 | CFG0_PLLMF(20))     /*!< PLL source clock multiply by 85 */
#define RCU_PLL_MUL86              (PLLMF_6 | CFG0_PLLMF(21))     /*!< PLL source clock multiply by 86 */
#define RCU_PLL_MUL87              (PLLMF_6 | CFG0_PLLMF(22))     /*!< PLL source clock multiply by 87 */
#define RCU_PLL_MUL88              (PLLMF_6 | CFG0_PLLMF(23))     /*!< PLL source clock multiply by 88 */
#define RCU_PLL_MUL89              (PLLMF_6 | CFG0_PLLMF(24))     /*!< PLL source clock multiply by 89 */
#define RCU_PLL_MUL90              (PLLMF_6 | CFG0_PLLMF(25))     /*!< PLL source clock multiply by 90 */
#define RCU_PLL_MUL91              (PLLMF_6 | CFG0_PLLMF(26))     /*!< PLL source clock multiply by 91 */
#define RCU_PLL_MUL92              (PLLMF_6 | CFG0_PLLMF(27))     /*!< PLL source clock multiply by 92 */
#define RCU_PLL_MUL93              (PLLMF_6 | CFG0_PLLMF(28))     /*!< PLL source clock multiply by 93 */
#define RCU_PLL_MUL94              (PLLMF_6 | CFG0_PLLMF(29))     /*!< PLL source clock multiply by 94 */
#define RCU_PLL_MUL95              (PLLMF_6 | CFG0_PLLMF(30))     /*!< PLL source clock multiply by 95 */
#define RCU_PLL_MUL96              (PLLMF_6 | CFG0_PLLMF(31))     /*!< PLL source clock multiply by 96 */
#define RCU_PLL_MUL97              (PLLMF_6 | CFG0_PLLMF(32))     /*!< PLL source clock multiply by 97 */
#define RCU_PLL_MUL98              (PLLMF_6 | CFG0_PLLMF(33))     /*!< PLL source clock multiply by 98 */
#define RCU_PLL_MUL99              (PLLMF_6 | CFG0_PLLMF(34))     /*!< PLL source clock multiply by 99 */
#define RCU_PLL_MUL100             (PLLMF_6 | CFG0_PLLMF(35))     /*!< PLL source clock multiply by 100 */
#define RCU_PLL_MUL101             (PLLMF_6 | CFG0_PLLMF(36))     /*!< PLL source clock multiply by 101 */
#define RCU_PLL_MUL102             (PLLMF_6 | CFG0_PLLMF(37))     /*!< PLL source clock multiply by 102 */
#define RCU_PLL_MUL103             (PLLMF_6 | CFG0_PLLMF(38))     /*!< PLL source clock multiply by 103 */
#define RCU_PLL_MUL104             (PLLMF_6 | CFG0_PLLMF(39))     /*!< PLL source clock multiply by 104 */
#define RCU_PLL_MUL105             (PLLMF_6 | CFG0_PLLMF(40))     /*!< PLL source clock multiply by 105 */
#define RCU_PLL_MUL106             (PLLMF_6 | CFG0_PLLMF(41))     /*!< PLL source clock multiply by 106 */
#define RCU_PLL_MUL107             (PLLMF_6 | CFG0_PLLMF(42))     /*!< PLL source clock multiply by 107 */
#define RCU_PLL_MUL108             (PLLMF_6 | CFG0_PLLMF(43))     /*!< PLL source clock multiply by 108 */
#define RCU_PLL_MUL109             (PLLMF_6 | CFG0_PLLMF(44))     /*!< PLL source clock multiply by 109 */
#define RCU_PLL_MUL110             (PLLMF_6 | CFG0_PLLMF(45))     /*!< PLL source clock multiply by 110 */
#define RCU_PLL_MUL111             (PLLMF_6 | CFG0_PLLMF(46))     /*!< PLL source clock multiply by 111 */
#define RCU_PLL_MUL112             (PLLMF_6 | CFG0_PLLMF(47))     /*!< PLL source clock multiply by 112 */
#define RCU_PLL_MUL113             (PLLMF_6 | CFG0_PLLMF(48))     /*!< PLL source clock multiply by 113 */
#define RCU_PLL_MUL114             (PLLMF_6 | CFG0_PLLMF(49))     /*!< PLL source clock multiply by 114 */
#define RCU_PLL_MUL115             (PLLMF_6 | CFG0_PLLMF(50))     /*!< PLL source clock multiply by 115 */
#define RCU_PLL_MUL116             (PLLMF_6 | CFG0_PLLMF(51))     /*!< PLL source clock multiply by 116 */
#define RCU_PLL_MUL117             (PLLMF_6 | CFG0_PLLMF(52))     /*!< PLL source clock multiply by 117 */
#define RCU_PLL_MUL118             (PLLMF_6 | CFG0_PLLMF(53))     /*!< PLL source clock multiply by 118 */
#define RCU_PLL_MUL119             (PLLMF_6 | CFG0_PLLMF(54))     /*!< PLL source clock multiply by 119 */
#define RCU_PLL_MUL120             (PLLMF_6 | CFG0_PLLMF(55))     /*!< PLL source clock multiply by 120 */
#define RCU_PLL_MUL121             (PLLMF_6 | CFG0_PLLMF(56))     /*!< PLL source clock multiply by 121 */
#define RCU_PLL_MUL122             (PLLMF_6 | CFG0_PLLMF(57))     /*!< PLL source clock multiply by 122 */
#define RCU_PLL_MUL123             (PLLMF_6 | CFG0_PLLMF(58))     /*!< PLL source clock multiply by 123 */
#define RCU_PLL_MUL124             (PLLMF_6 | CFG0_PLLMF(59))     /*!< PLL source clock multiply by 124 */
#define RCU_PLL_MUL125             (PLLMF_6 | CFG0_PLLMF(60))     /*!< PLL source clock multiply by 125 */
#define RCU_PLL_MUL126             (PLLMF_6 | CFG0_PLLMF(61))     /*!< PLL source clock multiply by 126 */
#define RCU_PLL_MUL127             (PLLMF_6 | CFG0_PLLMF(62))     /*!< PLL source clock multiply by 127 */

/* CK_OUT clock source selection */
#define CFG0_CKOUTSEL(regval)       (BITS(24,26) & ((uint32_t)(regval) << 24U))
#define RCU_CKOUTSRC_NONE           CFG0_CKOUTSEL(0)                    /*!< no clock selected */
#define RCU_CKOUTSRC_IRC48M         CFG0_CKOUTSEL(1)                    /*!< CK_OUT clock source select IRC48M */
#define RCU_CKOUTSRC_IRC32K         CFG0_CKOUTSEL(2)                    /*!< CK_OUT clock source select IRC32K */
#define RCU_CKOUTSRC_LXTAL          CFG0_CKOUTSEL(3)                    /*!< CK_OUT clock source select LXTAL */
#define RCU_CKOUTSRC_CKSYS          CFG0_CKOUTSEL(4)                    /*!< CK_OUT clock source select CKSYS */
#define RCU_CKOUTSRC_IRC16M         CFG0_CKOUTSEL(5)                    /*!< CK_OUT clock source select IRC16M */
#define RCU_CKOUTSRC_HXTAL          CFG0_CKOUTSEL(6)                    /*!< CK_OUT clock source select HXTAL */
#define RCU_CKOUTSRC_CKPLL_DIV1     (RCU_CFG0_PLLDV | CFG0_CKOUTSEL(7)) /*!< CK_OUT clock source select CK_PLL */
#define RCU_CKOUTSRC_CKPLL_DIV2     CFG0_CKOUTSEL(7)                    /*!< CK_OUT clock source select CK_PLL/2 */

/* CK_OUT divider */
#define CFG0_CKOUTDIV(regval)       (BITS(28,30) & ((uint32_t)(regval) << 28U))
#define RCU_CKOUT_DIV1              CFG0_CKOUTDIV(0)                    /*!< CK_OUT is divided by 1 */
#define RCU_CKOUT_DIV2              CFG0_CKOUTDIV(1)                    /*!< CK_OUT is divided by 2 */
#define RCU_CKOUT_DIV4              CFG0_CKOUTDIV(2)                    /*!< CK_OUT is divided by 4 */
#define RCU_CKOUT_DIV8              CFG0_CKOUTDIV(3)                    /*!< CK_OUT is divided by 8 */
#define RCU_CKOUT_DIV16             CFG0_CKOUTDIV(4)                    /*!< CK_OUT is divided by 16 */
#define RCU_CKOUT_DIV32             CFG0_CKOUTDIV(5)                    /*!< CK_OUT is divided by 32 */
#define RCU_CKOUT_DIV64             CFG0_CKOUTDIV(6)                    /*!< CK_OUT is divided by 64 */
#define RCU_CKOUT_DIV128            CFG0_CKOUTDIV(7)                    /*!< CK_OUT is divided by 128 */

/* CK_PLL divide by 1 or 2 for CK_OUT */
#define RCU_PLLDV_CKPLL_DIV2        (uint32_t)0x00000000U               /*!< CK_PLL divide by 2 for CK_OUT */
#define RCU_PLLDV_CKPLL             RCU_CFG0_PLLDV                      /*!< CK_PLL divide by 1 for CK_OUT */

/* LXTAL drive capability */
#define BDCTL_LXTALDRI(regval)      (BITS(3,4) & ((uint32_t)(regval) << 3U))
#define RCU_LXTAL_LOWDRI            BDCTL_LXTALDRI(0)                   /*!< lower driving capability */
#define RCU_LXTAL_MED_LOWDRI        BDCTL_LXTALDRI(1)                   /*!< medium low driving capability */
#define RCU_LXTAL_MED_HIGHDRI       BDCTL_LXTALDRI(2)                   /*!< medium high driving capability */
#define RCU_LXTAL_HIGHDRI           BDCTL_LXTALDRI(3)                   /*!< higher driving capability */

/* RTC clock entry selection */
#define BDCTL_RTCSRC(regval)        (BITS(8,9) & ((uint32_t)(regval) << 8U))
#define RCU_RTCSRC_NONE             BDCTL_RTCSRC(0)                     /*!< no clock selected */
#define RCU_RTCSRC_LXTAL            BDCTL_RTCSRC(1)                     /*!< LXTAL selected as RTC source clock */
#define RCU_RTCSRC_IRC32K           BDCTL_RTCSRC(2)                     /*!< IRC32K selected as RTC source clock */
#define RCU_RTCSRC_HXTAL_DIV32      BDCTL_RTCSRC(3)                     /*!< HXTAL/32 selected as RTC source clock */

/* CK_HXTAL divider previous PLL */
#define CFG1_PREDV(regval)         (BITS(0,3) & ((uint32_t)(regval) << 0U))
#define RCU_PLL_PREDV1              CFG1_PREDV(0)                       /*!< PLL not divided */
#define RCU_PLL_PREDV2              CFG1_PREDV(1)                       /*!< PLL divided by 2 */
#define RCU_PLL_PREDV3              CFG1_PREDV(2)                       /*!< PLL divided by 3 */
#define RCU_PLL_PREDV4              CFG1_PREDV(3)                       /*!< PLL divided by 4 */
#define RCU_PLL_PREDV5              CFG1_PREDV(4)                       /*!< PLL divided by 5 */
#define RCU_PLL_PREDV6              CFG1_PREDV(5)                       /*!< PLL divided by 6 */
#define RCU_PLL_PREDV7              CFG1_PREDV(6)                       /*!< PLL divided by 7 */
#define RCU_PLL_PREDV8              CFG1_PREDV(7)                       /*!< PLL divided by 8 */
#define RCU_PLL_PREDV9              CFG1_PREDV(8)                       /*!< PLL divided by 9 */
#define RCU_PLL_PREDV10             CFG1_PREDV(9)                       /*!< PLL divided by 10 */
#define RCU_PLL_PREDV11             CFG1_PREDV(10)                      /*!< PLL divided by 11 */
#define RCU_PLL_PREDV12             CFG1_PREDV(11)                      /*!< PLL divided by 12 */
#define RCU_PLL_PREDV13             CFG1_PREDV(12)                      /*!< PLL divided by 13 */
#define RCU_PLL_PREDV14             CFG1_PREDV(13)                      /*!< PLL divided by 14 */
#define RCU_PLL_PREDV15             CFG1_PREDV(14)                      /*!< PLL divided by 15 */
#define RCU_PLL_PREDV16             CFG1_PREDV(15)                      /*!< PLL divided by 16 */

/* USART0 clock source selection */
#define CFG2_USART0SEL(regval)      (BITS(0,1) & ((uint32_t)(regval) << 0U))
#define RCU_USART0SRC_CKAPB2        CFG2_USART0SEL(0)                   /*!< CK_USART0 select CK_APB2 */
#define RCU_USART0SRC_CKSYS         CFG2_USART0SEL(1)                   /*!< CK_USART0 select CK_SYS */
#define RCU_USART0SRC_LXTAL         CFG2_USART0SEL(2)                   /*!< CK_USART0 select LXTAL */
#define RCU_USART0SRC_IRC16MDIV     CFG2_USART0SEL(3)                   /*!< CK_USART0 select IRC16MDIV */

/* LPUART clock source selection */
#define CFG2_LPUARTSEL(regval)      (BITS(11,12) & ((uint32_t)(regval) << 11U))
#define RCU_LPUARTSRC_CKAPB1        CFG2_LPUARTSEL(0)                   /*!< CK_LPUART select CK_APB1 */
#define RCU_LPUARTSRC_CKSYS         CFG2_LPUARTSEL(1)                   /*!< CK_LPUART select CK_SYS */
#define RCU_LPUARTSRC_LXTAL         CFG2_LPUARTSEL(2)                   /*!< CK_LPUART select LXTAL */
#define RCU_LPUARTSRC_IRC16MDIV     CFG2_LPUARTSEL(3)                   /*!< CK_LPUART select IRC16MDIV */

/* USART1 clock source selection */
#define CFG2_USART1SEL(regval)      (BITS(16,17) & ((uint32_t)(regval) << 16U))
#define RCU_USART1SRC_CKAPB1         CFG2_USART1SEL(0)                  /*!< CK_USART1 select CK_APB1 */
#define RCU_USART1SRC_CKSYS          CFG2_USART1SEL(1)                  /*!< CK_USART1 select CK_SYS */
#define RCU_USART1SRC_LXTAL          CFG2_USART1SEL(2)                  /*!< CK_USART1 select LXTAL */
#define RCU_USART1SRC_IRC16MDIV      CFG2_USART1SEL(3)                  /*!< CK_USART1 select IRC16MDIV */

/* USARTx(x=0,1) clock source selection */
#define CFG2_USART0SEL(regval)      (BITS(0,1) & ((uint32_t)(regval) << 0U))
#define RCU_USARTSRC_CKAPB           CFG2_USART0SEL(0)                  /*!< CK_USART select CK_APB1/2 */
#define RCU_USARTSRC_CKSYS           CFG2_USART0SEL(1)                  /*!< CK_USART select CK_SYS */
#define RCU_USARTSRC_LXTAL           CFG2_USART0SEL(2)                  /*!< CK_USART select LXTAL */
#define RCU_USARTSRC_IRC16MDIV       CFG2_USART0SEL(3)                  /*!< CK_USART select IRC16MDIV */

/* I2Cx(x=0,1,2) clock source selection */
#define CFG2_I2C0SEL(regval)        (BITS(2,3) & ((uint32_t)(regval) << 2U))
#define RCU_I2CSRC_CKAPB1            CFG2_I2C0SEL(0)                    /*!< CK_I2C select CK_APB1 */
#define RCU_I2CSRC_CKSYS             CFG2_I2C0SEL(1)                    /*!< CK_I2C select CK_SYS */
#define RCU_I2CSRC_IRC16MDIV         CFG2_I2C0SEL(2)                    /*!< CK_I2C select IRC16MDIV */

/* LPTIMER clock source selection */
#define CFG2_LPTIMERSEL(regval)      (BITS(9,10) & ((uint32_t)(regval) << 9U))
#define RCU_LPTIMERSRC_CKAPB1        CFG2_LPTIMERSEL(0)                 /*!< CK_LPTIMER select CK_APB1 */
#define RCU_LPTIMERSRC_IRC32K        CFG2_LPTIMERSEL(1)                 /*!< CK_LPTIMER select CK_IRC32K */
#define RCU_LPTIMERSRC_LXTAL         CFG2_LPTIMERSEL(2)                 /*!< CK_LPTIMER select LXTAL */
#define RCU_LPTIMERSRC_IRC16MDIV     CFG2_LPTIMERSEL(3)                 /*!< CK_LPTIMER select IRC16MDIV */

/* IRC16MDIV clock source selection */
#define CFG2_IRC16MDIVSEL(regval)    (BITS(18,20) & ((uint32_t)(regval) << 18U))
#define RCU_IRC16MDIV_NONE           CFG2_IRC16MDIVSEL(0)               /*!< CK_IRC16MDIV select CK_IRC16M */
#define RCU_IRC16MDIV_2              CFG2_IRC16MDIVSEL(4)               /*!< CK_IRC16MDIV select CK_IRC16M divided by 2 */
#define RCU_IRC16MDIV_4              CFG2_IRC16MDIVSEL(5)               /*!< CK_IRC16MDIV select CK_IRC16M divided by 4 */
#define RCU_IRC16MDIV_8              CFG2_IRC16MDIVSEL(6)               /*!< CK_IRC16MDIV select CK_IRC16M divided by 8 */
#define RCU_IRC16MDIV_16             CFG2_IRC16MDIVSEL(7)               /*!< CK_IRC16MDIV select CK_IRC16M divided by 16 */

/* USBD clock source selection */
#define RCU_USBDSRC_IRC48M           (uint32_t)0x00000000U              /*!< CK_USBD select CK_IRC48M */
#define RCU_USBDSRC_PLL              RCU_CFG2_USBDSEL                   /*!< CK_USBD select CK_PLL */

/* ADC clock source selection */
#define RCU_ADCSRC_IRC16M            (uint32_t)0x00000000U              /*!< ADC clock source select */
#define RCU_ADCSRC_AHB_APB2DIV       RCU_CFG2_ADCSEL                    /*!< ADC clock source select */

/* low power mode LDO voltage selection */
#define RCU_LP_LDO_V_0_8             (uint32_t)0x00000000U              /*!< LP_LDO output voltage 0.8V */
#define RCU_LP_LDO_V_0_9             RCU_LPLDO_LPLDOVOS                 /*!< LP_LDO output voltage 0.9V */

/* low power bandgap mode selection */
#define LPB_LPBMSEL(regval)          BITS(0,2) & ((uint32_t)(regval) << 0U))
#define RCU_LPBM_32CLK               LPB_LPBMSEL(3)                     /*!< The length of holding phase is 3.2ms, 32 clock cycles */
#define RCU_LPBM_64CLK               LPB_LPBMSEL(2)                     /*!< The length of holding phase is 6.4ms, 64 clock cycles */
#define RCU_LPBM_128CLK              LPB_LPBMSEL(1)                     /*!< The length of holding phase is 12.8ms, 128 clock cycles */
#define RCU_LPBM_256CLK              LPB_LPBMSEL(0)                     /*!< The length of holding phase is 25.6ms, 256 clock cycles */
#define RCU_LPBM_512CLK              LPB_LPBMSEL(7)                     /*!< The length of holding phase is 51.2ms, 512 clock cycles */
#define RCU_LPBM_1024CLK             LPB_LPBMSEL(6)                     /*!< The length of holding phase is 102.4ms, 1024 clock cycles */
#define RCU_LPBM_2048CLK             LPB_LPBMSEL(5)                     /*!< The length of holding phase is 204.8ms, 2048 clock cycles */

/* function declarations */
/* initialization, peripheral clock and reset configuration functions */
/* deinitialize the RCU */
void rcu_deinit(void);
/* enable the peripherals clock */
void rcu_periph_clock_enable(rcu_periph_enum periph);
/* disable the peripherals clock */
void rcu_periph_clock_disable(rcu_periph_enum periph);
/* enable the peripherals clock when sleep mode */
void rcu_periph_clock_sleep_enable(rcu_periph_sleep_enum periph);
/* disable the peripherals clock when sleep mode */
void rcu_periph_clock_sleep_disable(rcu_periph_sleep_enum periph);
/* reset the peripherals */
void rcu_periph_reset_enable(rcu_periph_reset_enum periph_reset);
/* disable reset the peripheral */
void rcu_periph_reset_disable(rcu_periph_reset_enum periph_reset);
/* reset the BKP */
void rcu_bkp_reset_enable(void);
/* disable the BKP reset */
void rcu_bkp_reset_disable(void);

/* system clock, AHB, APB1, APB2, ADC and clock out configuration functions */
/* configure the system clock source */
void rcu_system_clock_source_config(uint32_t ck_sys);
/* get the system clock source */
uint32_t rcu_system_clock_source_get(void);
/* configure the AHB prescaler selection */
void rcu_ahb_clock_config(uint32_t ck_ahb);
/* configure the APB1 prescaler selection */
void rcu_apb1_clock_config(uint32_t ck_apb1);
/* configure the APB2 prescaler selection */
void rcu_apb2_clock_config(uint32_t ck_apb2);
/* configure the ADC clock source and prescaler selection */
void rcu_adc_clock_config(uint32_t ck_adc);
/* configure the CK_OUT clock source and divider */
void rcu_ckout_config(uint32_t ckout_src, uint32_t ckout_div);

/* configure the PLL clock source selection and PLL multiply factor */
void rcu_pll_config(uint32_t pll_src, uint32_t pll_mul);
/* configure the USARTx(x=0,1) clock source selection */
void rcu_usart_clock_config(usart_idx_enum usart_idx, uint32_t ck_usart);
/* configure the I2Cx(x=0,1,2) clock source selection */
void rcu_i2c_clock_config(i2c_idx_enum i2c_idx, uint32_t ck_i2c);
/* configure the LPTIMER clock source selection */
void rcu_lptimer_clock_config(uint32_t ck_lptimer);
/* configure the LPUART clock source selection */
void rcu_lpuart_clock_config(uint32_t ck_lpuart);
/* configure the IRC16MDIV clock selection */
void rcu_irc16mdiv_clock_config(uint32_t ck_irc16mdiv);
/* configure the USBD clock source selection */
void rcu_usbd_clock_config(uint32_t ck_usbd);
/* configure the RTC clock source selection */
void rcu_rtc_clock_config(uint32_t rtc_clock_source);
/* configure PLL source clocks pre-divider */
void rcu_pll_source_ck_prediv_config(uint32_t pllsource_ck_prediv);
/* configure the LXTAL drive capability */
void rcu_lxtal_drive_capability_config(uint32_t lxtal_dricap);
/* configure the low power mode LDO voltage selection */
void rcu_lp_ldo_config(uint32_t lp_ldo_voltage);
/* configure low power bandgap mode selection */
void rcu_lp_bandgap_config(uint32_t lp_bandgap_clock);

/* flag and interrupt functions */
/* get the clock stabilization and periphral reset flags */
FlagStatus rcu_flag_get(rcu_flag_enum flag);
/* clear the reset flag */
void rcu_all_reset_flag_clear(void);
/* get the clock stabilization interrupt and ckm flags */
FlagStatus rcu_interrupt_flag_get(rcu_int_flag_enum int_flag);
/* clear the interrupt flags */
void rcu_interrupt_flag_clear(rcu_int_flag_clear_enum int_flag_clear);
/* enable the stabilization interrupt */
void rcu_interrupt_enable(rcu_int_enum stab_int);
/* disable the stabilization interrupt */
void rcu_interrupt_disable(rcu_int_enum stab_int);

/* oscillator configuration functions */
/* wait until oscillator stabilization flags is SET */
ErrStatus rcu_osci_stab_wait(rcu_osci_type_enum osci);
/* turn on the oscillator */
void rcu_osci_on(rcu_osci_type_enum osci);
/* turn off the oscillator */
void rcu_osci_off(rcu_osci_type_enum osci);
/* enable the oscillator bypass mode */
void rcu_osci_bypass_mode_enable(rcu_osci_type_enum osci);
/* disable the oscillator bypass mode */
void rcu_osci_bypass_mode_disable(rcu_osci_type_enum osci);
/* enable the HXTAL clock monitor */
void rcu_hxtal_clock_monitor_enable(void);
/* disable the HXTAL clock monitor */
void rcu_hxtal_clock_monitor_disable(void);
/* enable the LXTAL clock monitor */
void rcu_lxtal_clock_monitor_enable(void);
/* disable the LXTAL clock monitor */
void rcu_lxtal_clock_monitor_disable(void);

/* set the IRC16M adjust value */
void rcu_irc16m_adjust_value_set(uint8_t irc16m_adjval);
/* unlock the voltage key */
void rcu_voltage_key_unlock(void);

/* get the system clock, bus and peripheral clock frequency */
uint32_t rcu_clock_freq_get(rcu_clock_freq_enum clock);

#endif /* GD32L23X_RCU_H */
