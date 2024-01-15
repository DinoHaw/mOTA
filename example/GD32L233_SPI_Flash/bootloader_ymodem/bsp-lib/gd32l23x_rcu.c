/*!
    \file    gd32l23x_rcu.c
    \brief   RCU driver

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

#include "gd32l23x_rcu.h"

/* define clock source */
#define SEL_IRC16M      0x00U
#define SEL_HXTAL       0x01U
#define SEL_PLL         0x02U

/* define startup timeout count */
#define OSC_STARTUP_TIMEOUT         ((uint32_t)0x000FFFFFU)
#define LXTAL_STARTUP_TIMEOUT       ((uint32_t)0x03FFFFFFU)

/*!
    \brief      deinitialize the RCU
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_deinit(void)
{
    /* enable IRC16M */
    RCU_CTL |= RCU_CTL_IRC16MEN;
    while(0U == (RCU_CTL & RCU_CTL_IRC16MSTB)) {
    }
    /* reset RCU */
    RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC | \
                  RCU_CFG0_ADCPSC | RCU_CFG0_CKOUTSEL | RCU_CFG0_CKOUTDIV | RCU_CFG0_PLLDV);
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF  | RCU_CFG0_PLLDV);
    RCU_CTL &= ~(RCU_CTL_HXTALEN | RCU_CTL_CKMEN | RCU_CTL_PLLEN | RCU_CTL_HXTALBPS);
    RCU_CFG1 &= ~(RCU_CFG1_PREDV);
    RCU_CFG2 &= ~(RCU_CFG2_USART0SEL | RCU_CFG2_ADCSEL);
    RCU_CFG2 &= ~RCU_CFG2_ADCPSC2;

    RCU_INT = 0x00000000U;
}

/*!
    \brief      enable the peripherals clock
    \param[in]  periph: RCU peripherals, refer to rcu_periph_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_GPIOx (x=A,B,C,D,F): GPIO ports clock
      \arg        RCU_DMA: DMA clock
      \arg        RCU_CAU: CAU clock
      \arg        RCU_TRNG: TRNG clock
      \arg        RCU_CRC: CRC clock
      \arg        RCU_CMP: CMP clock
      \arg        RCU_SYSCFG: SYSCFG clock
      \arg        RCU_ADC: ADC clock
      \arg        RCU_TIMERx (x=1,2,5,6,8,11): TIMER clock
      \arg        RCU_LPTIMER: LPTIMER clock
      \arg        RCU_SPIx (x=0,1): SPI clock
      \arg        RCU_USARTx (x=0,1): USART clock
      \arg        RCU_UARTx (x=3,4): UART clock
      \arg        RCU_LPUART: LPUART clock
      \arg        RCU_WWDGT: WWDGT clock
      \arg        RCU_I2Cx (x=0,1,2): I2C clock
      \arg        RCU_PMU: PMU clock
      \arg        RCU_RTC: RTC clock
      \arg        RCU_DBGMCU: DBGMCU clock
      \arg        RCU_CAU: CAU clock
      \arg        RCU_DAC: DAC clock
      \arg        RCU_CTC: CTC clock
      \arg        RCU_BKP: BKP clock
      \arg        RCU_USBD: USBD clock
      \arg        RCU_SLCD: SLCD clock
    \param[out] none
    \retval     none
*/
void rcu_periph_clock_enable(rcu_periph_enum periph)
{
    RCU_REG_VAL(periph) |= BIT(RCU_BIT_POS(periph));
}

/*!
    \brief      disable the peripherals clock
    \param[in]  periph: RCU peripherals, refer to rcu_periph_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_GPIOx (x=A,B,C,D,F): GPIO ports clock
      \arg        RCU_DMA: DMA clock
      \arg        RCU_CAU: CAU clock
      \arg        RCU_TRNG: TRNG clock
      \arg        RCU_CRC: CRC clock
      \arg        RCU_CMP: CMP clock
      \arg        RCU_SYSCFG: SYSCFG clock
      \arg        RCU_ADC: ADC clock
      \arg        RCU_TIMERx (x=1,2,5,6,8,11): TIMER clock
      \arg        RCU_LPTIMER: LPTIMER clock
      \arg        RCU_SPIx (x=0,1): SPI clock
      \arg        RCU_USARTx (x=0,1): USART clock
      \arg        RCU_UARTx (x=3,4): UART clock
      \arg        RCU_LPUART: LPUART clock
      \arg        RCU_WWDGT: WWDGT clock
      \arg        RCU_I2Cx (x=0,1,2): I2C clock
      \arg        RCU_PMU: PMU clock
      \arg        RCU_RTC: RTC clock
      \arg        RCU_DBGMCU: DBGMCU clock
      \arg        RCU_CAU: CAU clock
      \arg        RCU_DAC: DAC clock
      \arg        RCU_CTC: CTC clock
      \arg        RCU_BKP: BKP clock
      \arg        RCU_USBD: USBD clock
      \arg        RCU_SLCD: SLCD clock
    \param[out] none
    \retval     none
*/
void rcu_periph_clock_disable(rcu_periph_enum periph)
{
    RCU_REG_VAL(periph) &= ~BIT(RCU_BIT_POS(periph));
}

/*!
    \brief      enable the peripherals clock when sleep mode
    \param[in]  periph: RCU peripherals, refer to rcu_periph_sleep_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_FMC_SLP: FMC clock
      \arg        RCU_SRAM0_SLP: SRAM0 clock
      \arg        RCU_SRAM1_SLP: SRAM1 clock
    \param[out] none
    \retval     none
*/
void rcu_periph_clock_sleep_enable(rcu_periph_sleep_enum periph)
{
    RCU_REG_VAL(periph) |= BIT(RCU_BIT_POS(periph));
}

/*!
    \brief      disable the peripherals clock when sleep mode
    \param[in]  periph: RCU peripherals, refer to rcu_periph_sleep_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_FMC_SLP: FMC clock
      \arg        RCU_SRAM0_SLP: SRAM clock
      \arg        RCU_SRAM1_SLP: SRAM clock
    \param[out] none
    \retval     none
*/
void rcu_periph_clock_sleep_disable(rcu_periph_sleep_enum periph)
{
    RCU_REG_VAL(periph) &= ~BIT(RCU_BIT_POS(periph));
}
/*!
    \brief      reset the peripherals
    \param[in]  periph_reset: RCU peripherals reset, refer to rcu_periph_reset_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_GPIOxRST (x=A,B,C,D,F): reset GPIO ports
      \arg        RCU_CAU: reset CAU
      \arg        RCU_CRC: reset CRC
      \arg        RCU_TRNG: reset TRNG
      \arg        RCU_CMPRST: reset CMP
      \arg        RCU_SYSCFGRST: reset SYSCFG
      \arg        RCU_ADCRST: reset ADC
      \arg        RCU_TIMERxRST (x=1,2,5,6,8,11): reset TIMER
      \arg        RCU_SPIxRST (x=0,1): reset SPI
      \arg        RCU_USARTxRST (x=0,1): reset USART
      \arg        RCU_LPTIMERRST: reset LPTIMER
      \arg        RCU_SLCDRRST: reset SLCD
      \arg        RCU_WWDGTRST: reset WWDGT
      \arg        RCU_LPUARTRST: reset LPUART
      \arg        RCU_UARTxRST (x=3,4): reset UART
      \arg        RCU_I2CxRST (x=0,1,2): reset I2C
      \arg        RCU_USBDRST: reset USBD
      \arg        RCU_PMURST: reset PMU
      \arg        RCU_DACRST : reset DAC
      \arg        RCU_CTCRST : reset CTC
    \param[out] none
    \retval     none
*/
void rcu_periph_reset_enable(rcu_periph_reset_enum periph_reset)
{
    RCU_REG_VAL(periph_reset) |= BIT(RCU_BIT_POS(periph_reset));
}

/*!
    \brief      disable reset the peripheral
    \param[in]  periph_reset: RCU peripherals reset, refer to rcu_periph_reset_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_GPIOxRST (x=A,B,C,D,F): reset GPIO ports
      \arg        RCU_CAU: reset CAU
      \arg        RCU_CRC: reset CRC
      \arg        RCU_TRNG: reset TRNG
      \arg        RCU_CMPRST: reset CMP
      \arg        RCU_SYSCFGRST: reset SYSCFG
      \arg        RCU_ADCRST: reset ADC
      \arg        RCU_TIMERxRST (x=1,2,5,6,8,11): reset TIMER
      \arg        RCU_SPIxRST (x=0,1): reset SPI
      \arg        RCU_USARTxRST (x=0,1): reset USART
      \arg        RCU_LPTIMERRST: reset LPTIMER
      \arg        RCU_SLCDRRST: reset LPTIMER
      \arg        RCU_WWDGTRST: reset WWDGT
      \arg        RCU_LPUARTRST: reset LPUART
      \arg        RCU_UARTxRST (x=3,4): reset UART
      \arg        RCU_I2CxRST (x=0,1,2): reset I2C
      \arg        RCU_USBDRST: reset USBD
      \arg        RCU_PMURST: reset PMU
      \arg        RCU_DACRST : reset DAC
      \arg        RCU_CTCRST : reset CTC
    \param[out] none
    \retval     none
*/
void rcu_periph_reset_disable(rcu_periph_reset_enum periph_reset)
{
    RCU_REG_VAL(periph_reset) &= ~BIT(RCU_BIT_POS(periph_reset));
}

/*!
    \brief      reset the BKP
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_bkp_reset_enable(void)
{
    RCU_BDCTL |= RCU_BDCTL_BKPRST;
}

/*!
    \brief      disable the BKP reset
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_bkp_reset_disable(void)
{
    RCU_BDCTL &= ~RCU_BDCTL_BKPRST;
}

/*!
    \brief      configure the system clock source
    \param[in]  ck_sys: system clock source select
                only one parameter can be selected which is shown as below:
      \arg        RCU_CKSYSSRC_IRC16M: select CK_IRC16M as the CK_SYS source
      \arg        RCU_CKSYSSRC_HXTAL: select CK_HXTAL as the CK_SYS source
      \arg        RCU_CKSYSSRC_PLL: select CK_PLL as the CK_SYS source
      \arg        RCU_CKSYSSRC_IRC48M: select CK_IRC48M as the CK_SYS source
    \param[out] none
    \retval     none
*/
void rcu_system_clock_source_config(uint32_t ck_sys)
{
    uint32_t cksys_source = 0U;
    cksys_source = RCU_CFG0;
    /* reset the SCS bits and set according to ck_sys */
    cksys_source &= ~RCU_CFG0_SCS;
    RCU_CFG0 = (ck_sys | cksys_source);
}

/*!
    \brief      get the system clock source
    \param[in]  none
    \param[out] none
    \retval     which clock is selected as CK_SYS source
      \arg        RCU_SCSS_IRC16M: select CK_IRC16M as the CK_SYS source
      \arg        RCU_SCSS_HXTAL: select CK_HXTAL as the CK_SYS source
      \arg        RCU_SCSS_PLL: select CK_PLL as the CK_SYS source
      \arg        RCU_SCSS_IRC48M: select CK_IRC48M as the CK_SYS source
*/
uint32_t rcu_system_clock_source_get(void)
{
    return (RCU_CFG0 & RCU_CFG0_SCSS);
}

/*!
    \brief      configure the AHB clock prescaler selection
    \param[in]  ck_ahb: AHB clock prescaler selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_AHB_CKSYS_DIVx, x=1, 2, 4, 8, 16, 64, 128, 256, 512
    \param[out] none
    \retval     none
*/
void rcu_ahb_clock_config(uint32_t ck_ahb)
{
    uint32_t ahbpsc = 0U;
    ahbpsc = RCU_CFG0;
    /* reset the AHBPSC bits and set according to ck_ahb */
    ahbpsc &= ~RCU_CFG0_AHBPSC;
    RCU_CFG0 = (ck_ahb | ahbpsc);
}

/*!
    \brief      configure the APB1 clock prescaler selection
    \param[in]  ck_apb1: APB1 clock prescaler selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_APB1_CKAHB_DIV1: select CK_AHB as CK_APB1
      \arg        RCU_APB1_CKAHB_DIV2: select CK_AHB/2 as CK_APB1
      \arg        RCU_APB1_CKAHB_DIV4: select CK_AHB/4 as CK_APB1
      \arg        RCU_APB1_CKAHB_DIV8: select CK_AHB/8 as CK_APB1
      \arg        RCU_APB1_CKAHB_DIV16: select CK_AHB/16 as CK_APB1
    \param[out] none
    \retval     none
*/
void rcu_apb1_clock_config(uint32_t ck_apb1)
{
    uint32_t apb1psc = 0U;
    apb1psc = RCU_CFG0;
    /* reset the APB1PSC and set according to ck_apb1 */
    apb1psc &= ~RCU_CFG0_APB1PSC;
    RCU_CFG0 = (ck_apb1 | apb1psc);
}

/*!
    \brief      configure the APB2 clock prescaler selection
    \param[in]  ck_apb2: APB2 clock prescaler selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_APB2_CKAHB_DIV1: select CK_AHB as CK_APB2
      \arg        RCU_APB2_CKAHB_DIV2: select CK_AHB/2 as CK_APB2
      \arg        RCU_APB2_CKAHB_DIV4: select CK_AHB/4 as CK_APB2
      \arg        RCU_APB2_CKAHB_DIV8: select CK_AHB/8 as CK_APB2
      \arg        RCU_APB2_CKAHB_DIV16: select CK_AHB/16 as CK_APB2
    \param[out] none
    \retval     none
*/
void rcu_apb2_clock_config(uint32_t ck_apb2)
{
    uint32_t apb2psc = 0U;
    apb2psc = RCU_CFG0;
    /* reset the APB2PSC and set according to ck_apb2 */
    apb2psc &= ~RCU_CFG0_APB2PSC;
    RCU_CFG0 = (ck_apb2 | apb2psc);
}

/*!
    \brief      configure the ADC clock prescaler selection
    \param[in]  ck_adc: ADC clock prescaler selection, refer to rcu_adc_clock_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_ADCCK_IRC16M: select CK_IRC16M as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV2: select CK_APB2/2 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV4: select CK_APB2/4 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV6: select CK_APB2/6 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV8: select CK_APB2/8 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV10: select CK_APB2/10 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV12: select CK_APB2/12 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV14: select CK_APB2/14 as CK_ADC
      \arg        RCU_ADCCK_APB2_DIV16: select CK_APB2/16 as CK_ADC
      \arg        RCU_ADCCK_AHB_DIV3: select CK_AHB/3 as CK_ADC
      \arg        RCU_ADCCK_AHB_DIV5: select CK_AHB/5 as CK_ADC
      \arg        RCU_ADCCK_AHB_DIV7: select CK_AHB/7 as CK_ADC
      \arg        RCU_ADCCK_AHB_DIV9: select CK_AHB/9 as CK_ADC
      \arg        RCU_ADCCK_AHB_DIV11: select CK_AHB/11 as CK_ADC
      \arg        RCU_ADCCK_AHB_DIV13: select CK_AHB/13 as CK_ADC
      \arg        RCU_ADCCK_AHB_DIV15: select CK_AHB/15 as CK_ADC
    \param[out] none
    \retval     none
*/
void rcu_adc_clock_config(uint32_t ck_adc)
{
    /* reset the ADCPSC, ADCSEL bits */
    RCU_CFG0 &= ~RCU_CFG0_ADCPSC;
    RCU_CFG2 &= ~(RCU_CFG2_ADCSEL | RCU_CFG2_ADCPSC2);

    /* set the ADC clock according to ck_adc */
    if(ck_adc <= 15U) {
        RCU_CFG0 |= ((ck_adc & 0x3U) << 14U);
        RCU_CFG2 |= (((ck_adc & 0xCU) >> 2U) << 30U);
        RCU_CFG2 |= RCU_CFG2_ADCSEL;
    }
    if(ck_adc == 16U) {
        RCU_CFG2 &= ~RCU_CFG2_ADCSEL;
    }
}

/*!
    \brief      configure the CK_OUT clock source and divider
    \param[in]  ckout_src: CK_OUT clock source selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_CKOUTSRC_NONE: no clock selected
      \arg        RCU_CKOUTSRC_IRC48M: IRC48M selected
      \arg        RCU_CKOUTSRC_IRC32K: IRC32K selected
      \arg        RCU_CKOUTSRC_LXTAL: LXTAL selected
      \arg        RCU_CKOUTSRC_CKSYS: CKSYS selected
      \arg        RCU_CKOUTSRC_IRC16M: IRC16M selected
      \arg        RCU_CKOUTSRC_HXTAL: HXTAL selected
      \arg        RCU_CKOUTSRC_CKPLL_DIV1: CK_PLL selected
      \arg        RCU_CKOUTSRC_CKPLL_DIV2: CK_PLL/2 selected
    \param[in]  ckout_div: CK_OUT divider
      \arg        RCU_CKOUT_DIVx(x=1,2,4,8,16,32,64,128): CK_OUT is divided by x
    \param[out] none
    \retval     none
*/
void rcu_ckout_config(uint32_t ckout_src, uint32_t ckout_div)
{
    uint32_t ckout = 0U;
    ckout = RCU_CFG0;
    /* reset the CKOUTSEL, CKOUTDIV and PLLDV bits and set according to ckout_src and ckout_div */
    ckout &= ~(RCU_CFG0_CKOUTSEL | RCU_CFG0_CKOUTDIV | RCU_CFG0_PLLDV);
    RCU_CFG0 = (ckout | ckout_src | ckout_div);
}

/*!
    \brief      configure the PLL clock source selection and PLL multiply factor
    \param[in]  pll_src: PLL clock source selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_PLLSRC_IRC16M: select CK_IRC16M as PLL source clock
      \arg        RCU_PLLSRC_HXTAL: select HXTAL as PLL source clock
      \arg        RCU_PLLSRC_IRC48M: select CK_IRC48M as PLL source clock
    \param[in]  pll_mul: PLL multiply factor
                only one parameter can be selected which is shown as below:
      \arg        RCU_PLL_MULx(x=4..127): PLL source clock * x
    \param[out] none
    \retval     none
*/
void rcu_pll_config(uint32_t pll_src, uint32_t pll_mul)
{
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
    RCU_CFG0 |= (pll_src | pll_mul);
}

/*!
    \brief      configure the USARTx(x=0,1) clock source selection
    \param[in]  usart_idx: IDX_USARTx(x=0,1)
    \param[in]  ck_usart: USART clock source selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_USARTSRC_CKAPB: CK_USART select CK_APB1/2
      \arg        RCU_USARTSRC_CKSYS: CK_USART select CK_SYS
      \arg        RCU_USARTSRC_LXTAL: CK_USART select CK_LXTAL
      \arg        RCU_USARTSRC_IRC16MDIV: CK_USART select CK_IRC16MDIV
    \param[out] none
    \retval     none
*/
void rcu_usart_clock_config(usart_idx_enum usart_idx, uint32_t ck_usart)
{
    switch(usart_idx) {
    case IDX_USART0:
        /* reset the USART0SEL bits and set according to ck_usart */
        RCU_CFG2 &= ~RCU_CFG2_USART0SEL;
        RCU_CFG2 |= ck_usart;
        break;
    case IDX_USART1:
        /* reset the USART1SEL bits and set according to ck_usart */
        RCU_CFG2 &= ~RCU_CFG2_USART1SEL;
        RCU_CFG2 |= (uint32_t)ck_usart << 16U;
        break;
    default:
        break;
    }
}

/*!
    \brief      configure the I2Cx(x=0,1,2) clock source selection
    \param[in]  i2c_idx: IDX_I2Cx(x=0,1,2)
    \param[in]  ck_i2c: I2C clock source selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_I2CSRC_CKAPB1: CK_I2C select CK_APB1
      \arg        RCU_I2CSRC_CKSYS: CK_I2C select CK_SYS
      \arg        RCU_I2CSRC_IRC16MDIV: CK_I2C select IRC16MDIV
    \param[out] none
    \retval     none
*/
void rcu_i2c_clock_config(i2c_idx_enum i2c_idx, uint32_t ck_i2c)
{
    switch(i2c_idx) {
    case IDX_I2C0:
        /* reset the I2C0SEL bits and set according to ck_i2c */
        RCU_CFG2 &= ~RCU_CFG2_I2C0SEL;
        RCU_CFG2 |= ck_i2c;
        break;
    case IDX_I2C1:
        /* reset the I2C1SEL bits and set according to ck_i2c */
        RCU_CFG2 &= ~RCU_CFG2_I2C1SEL;
        RCU_CFG2 |= (uint32_t)ck_i2c << 2U;
        break;
    case IDX_I2C2:
        /* reset the I2C2SEL bits and set according to ck_i2c */
        RCU_CFG2 &= ~RCU_CFG2_I2C2SEL;
        RCU_CFG2 |= (uint32_t)ck_i2c << 4U;
        break;
    default:
        break;
    }
}
/*!
    \brief      configure the LPTIMER clock source selection
    \param[in]  ck_lptimer: LPTIMER clock source selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_LPTIMERSRC_CKAPB1: CK_LPTIMER select CK_APB1
      \arg        RCU_LPTIMERSRC_IRC32K: CK_LPTIMER select CK_IRC32K
      \arg        RCU_LPTIMERSRC_LXTAL: CK_LPTIMER select CK_LXTAL
      \arg        RCU_LPTIMERSRC_IRC16MDIV: CK_LPTIMER select CK_IRC16MDIV
    \param[out] none
    \retval     none
*/
void rcu_lptimer_clock_config(uint32_t ck_lptimer)
{
    /* reset the LPTIMERSEL bits and set according to ck_lptimer */
    RCU_CFG2 &= ~RCU_CFG2_LPTIMERSEL;
    RCU_CFG2 |= ck_lptimer;
}

/*!
    \brief      configure the LPUART clock source selection
    \param[in]  ck_lpusart: LPUART clock source selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_LPUARTSRC_CKAPB1: LPUART select CK_APB1
      \arg        RCU_LPUARTSRC_CKSYS: LPUART select CK_SYS
      \arg        RCU_LPUARTSRC_LXTAL: LPUART select CK_LXTAL
      \arg        RCU_LPUARTSRC_IRC16MDIV: LPUART select CK_IRC16MDIV
    \param[out] none
    \retval     none
*/
void rcu_lpuart_clock_config(uint32_t ck_lpuart)
{
    /* reset the LPUARTSEL bits and set according to ck_lpuart */
    RCU_CFG2 &= ~RCU_CFG2_LPUARTSEL;
    RCU_CFG2 |= ck_lpuart;
}

/*!
    \brief      configure the IRC16MDIV clock selection
    \param[in]  ck_irc16mdiv: IRC16MDIV clock selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_IRC16MDIV_NONE: CK_IRC16MDIV select CK_IRC16M
      \arg        RCU_IRC16MDIV_2: CK_IRC16MDIV select CK_IRC16M divided by 2
      \arg        RCU_IRC16MDIV_4: CK_IRC16MDIV select CK_IRC16M divided by 4
      \arg        RCU_IRC16MDIV_8: CK_IRC16MDIV select CK_IRC16M divided by 8
      \arg        RCU_IRC16MDIV_16: CK_IRC16MDIV select CK_IRC16M divided by 16
    \param[out] none
    \retval     none
*/
void rcu_irc16mdiv_clock_config(uint32_t ck_irc16mdiv)
{
    /* reset the LPUARTSEL bits and set according to ck_lpuart */
    RCU_CFG2 &= ~RCU_CFG2_IRC16MDIVSEL;
    RCU_CFG2 |= ck_irc16mdiv;
}

/*!
    \brief      configure the USBD clock source selection
    \param[in]  ck_usart: USBD clock source selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_USBDSRC_IRC48M: USBD select CK_IRC48M
      \arg        RCU_USBDSRC_PLL: USBD select CK_PLL
    \param[out] none
    \retval     none
*/
void rcu_usbd_clock_config(uint32_t ck_usbd)
{
    /* reset the USBDSEL bits and set according to ck_usbd */
    RCU_CFG2 &= ~RCU_CFG2_USBDSEL;
    RCU_CFG2 |= ck_usbd;
}

/*!
    \brief      configure the RTC clock source selection
    \param[in]  rtc_clock_source: RTC clock source selection
                only one parameter can be selected which is shown as below:
      \arg        RCU_RTCSRC_NONE: no clock selected
      \arg        RCU_RTCSRC_LXTAL: CK_LXTAL selected as RTC source clock
      \arg        RCU_RTCSRC_IRC32K: CK_IRC32K selected as RTC source clock
      \arg        RCU_RTCSRC_HXTAL_DIV32: CK_HXTAL/32 selected as RTC source clock
    \param[out] none
    \retval     none
*/
void rcu_rtc_clock_config(uint32_t rtc_clock_source)
{
    /* reset the RTCSRC bits and set according to rtc_clock_source */
    RCU_BDCTL &= ~RCU_BDCTL_RTCSRC;
    RCU_BDCTL |= rtc_clock_source;
}

/*!
    \brief      configure PLL source clocks pre-divider
    \param[in]  pllsource_ck_prediv: PLL source clocks divider used as input of PLL
                only one parameter can be selected which is shown as below:
      \arg        RCU_PLL_PREDVx(x=1..16): PLL source clocks divided x used as input of PLL
    \param[out] none
    \retval     none
*/
void rcu_pll_source_ck_prediv_config(uint32_t pllsource_ck_prediv)
{
    uint32_t prediv = 0U;
    prediv = RCU_CFG1;
    /* reset the PREDV bits and set according to pllsource_ck_prediv */
    prediv &= ~RCU_CFG1_PREDV;
    RCU_CFG1 = (prediv | pllsource_ck_prediv);
}

/*!
    \brief      configure the LXTAL drive capability
    \param[in]  lxtal_dricap: drive capability of LXTAL
                only one parameter can be selected which is shown as below:
      \arg        RCU_LXTAL_LOWDRI: lower driving capability
      \arg        RCU_LXTAL_MED_LOWDRI: medium low driving capability
      \arg        RCU_LXTAL_MED_HIGHDRI: medium high driving capability
      \arg        RCU_LXTAL_HIGHDRI: higher driving capability
    \param[out] none
    \retval     none
*/
void rcu_lxtal_drive_capability_config(uint32_t lxtal_dricap)
{
    /* reset the LXTALDRI bits and set according to lxtal_dricap */
    RCU_BDCTL &= ~RCU_BDCTL_LXTALDRI;
    RCU_BDCTL |= lxtal_dricap;
}

/*!
    \brief      configure the low power mode LDO voltage selection
    \param[in]  lp_ldo_voltage: low power mode LDO voltage
                only one parameter can be selected which is shown as below:
      \arg        RCU_LP_LDO_V_0_8: LP_LDO output voltage 0.8V
      \arg        RCU_LP_LDO_V_0_9: LP_LDO output voltage 0.9V
    \param[out] none
    \retval     none
*/
void rcu_lp_ldo_config(uint32_t lp_ldo_voltage)
{
    /* reset the RCU_LPB_LPBMODE bits and set according to lp_ldo_voltage */
    RCU_LPLDO &= ~RCU_LPB_LPBMSEL;
    RCU_LPLDO |= lp_ldo_voltage;
}

/*!
    \brief      configure low power bandgap mode selection
    \param[in]  lp_bandgap_clock: low power bandgap clock
                only one parameter can be selected which is shown as below:
      \arg        RCU_LPBM_32CLK: The length of holding phase is 3.2ms, 32 clock cycles
      \arg        RCU_LPBM_64CLK: The length of holding phase is 6.4ms, 64 clock cycles
      \arg        RCU_LPBM_128CLK: The length of holding phase is 12.8ms, 128 clock cycles
      \arg        RCU_LPBM_256CLK: The length of holding phase is 25.6ms, 256 clock cycles
      \arg        RCU_LPBM_512CLK: The length of holding phase is 51.2ms, 512 clock cycles
      \arg        RCU_LPBM_1024CLK: The length of holding phase is 102.4ms, 1024 clock cycles
      \arg        RCU_LPBM_2048CLK: The length of holding phase is 204.8ms, 2048 clock cycles
    \param[out] none
    \retval     none
*/
void rcu_lp_bandgap_config(uint32_t lp_bandgap_clock)
{
    /* reset the RCU_LPB_LPBMODE bits and set according to lp_ldo_voltage */
    RCU_LPB &= ~RCU_LPB_LPBMSEL;
    RCU_LPB |= lp_bandgap_clock;
}

/*!
    \brief      get the clock stabilization and periphral reset flags
    \param[in]  flag: the clock stabilization and periphral reset flags, refer to rcu_flag_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_FLAG_IRC32KSTB: IRC32K stabilization flag
      \arg        RCU_FLAG_LXTALSTB: LXTAL stabilization flag
      \arg        RCU_FLAG_IRC16MSTB: IRC16M stabilization flag
      \arg        RCU_FLAG_HXTALSTB: HXTAL stabilization flag
      \arg        RCU_FLAG_PLLSTB: PLL stabilization flag
      \arg        RCU_FLAG_IRC48MSTB: IRC48M stabilization flag
      \arg        RCU_FLAG_V12RST: V12 domain power reset flag
      \arg        RCU_FLAG_EPRST: external pin reset flag
      \arg        RCU_FLAG_PORRST: power reset flag
      \arg        RCU_FLAG_SWRST: software reset flag
      \arg        RCU_FLAG_FWDGTRST: free watchdog timer reset flag
      \arg        RCU_FLAG_WWDGTRST: window watchdog timer reset flag
      \arg        RCU_FLAG_LPRST: low-power reset flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus rcu_flag_get(rcu_flag_enum flag)
{
    if(RESET != (RCU_REG_VAL(flag) & BIT(RCU_BIT_POS(flag)))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear the reset flag
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_all_reset_flag_clear(void)
{
    RCU_RSTSCK |= RCU_RSTSCK_RSTFC;
}

/*!
    \brief      get the clock stabilization interrupt and ckm flags
    \param[in]  int_flag: interrupt and ckm flags, refer to rcu_int_flag_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_INT_FLAG_IRC32KSTB: IRC32K stabilization interrupt flag
      \arg        RCU_INT_FLAG_LXTALSTB: LXTAL stabilization interrupt flag
      \arg        RCU_INT_FLAG_IRC16MSTB: IRC16M stabilization interrupt flag
      \arg        RCU_INT_FLAG_HXTALSTB: HXTAL stabilization interrupt flag
      \arg        RCU_INT_FLAG_PLLSTB: PLL stabilization interrupt flag
      \arg        RCU_INT_FLAG_IRC48MSTB: IRC48M stabilization interrupt flag
      \arg        RCU_INT_FLAG_LXTALCKM: LXTAL clock stuck interrupt flag
      \arg        RCU_INT_FLAG_CKM: HXTAL clock stuck interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus rcu_interrupt_flag_get(rcu_int_flag_enum int_flag)
{
    if(RESET != (RCU_REG_VAL(int_flag) & BIT(RCU_BIT_POS(int_flag)))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear the interrupt flags
    \param[in]  int_flag_clear: clock stabilization and stuck interrupt flags clear, refer to rcu_int_flag_clear_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_INT_FLAG_IRC32KSTB_CLR: IRC32K stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_LXTALSTB_CLR: LXTAL stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_IRC16MSTB_CLR: IRC16M stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_HXTALSTB_CLR: HXTAL stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_PLLSTB_CLR: PLL stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_IRC48MSTB_CLR: IRC48M stabilization interrupt flag clear
      \arg        RCU_INT_FLAG_LXTALCKM_CLR: LXTAL clock stuck interrupt flag clear
      \arg        RCU_INT_FLAG_CKM_CLR: clock stuck interrupt flag clear
    \param[out] none
    \retval     none
*/
void rcu_interrupt_flag_clear(rcu_int_flag_clear_enum int_flag_clear)
{
    RCU_REG_VAL(int_flag_clear) |= BIT(RCU_BIT_POS(int_flag_clear));
}

/*!
    \brief      enable the stabilization interrupt
    \param[in]  stab_int: clock stabilization interrupt, refer to rcu_int_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_INT_IRC32KSTB: IRC32K stabilization interrupt enable
      \arg        RCU_INT_LXTALSTB: LXTAL stabilization interrupt enable
      \arg        RCU_INT_IRC16MSTB: IRC16M stabilization interrupt enable
      \arg        RCU_INT_HXTALSTB: HXTAL stabilization interrupt enable
      \arg        RCU_INT_PLLSTB: PLL stabilization interrupt enable
      \arg        RCU_INT_IRC48MSTB: IRC48M stabilization interrupt enable
      \arg        RCU_INT_LXTALCKM: LXTAL clock stuck interrup enable
    \param[out] none
    \retval     none
*/
void rcu_interrupt_enable(rcu_int_enum stab_int)
{
    RCU_REG_VAL(stab_int) |= BIT(RCU_BIT_POS(stab_int));
}

/*!
    \brief      disable the stabilization interrupt
    \param[in]  stab_int: clock stabilization interrupt, refer to rcu_int_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_INT_IRC32KSTB: IRC32K stabilization interrupt disable
      \arg        RCU_INT_LXTALSTB: LXTAL stabilization interrupt disable
      \arg        RCU_INT_IRC16MSTB: IRC16M stabilization interrupt disable
      \arg        RCU_INT_HXTALSTB: HXTAL stabilization interrupt disable
      \arg        RCU_INT_PLLSTB: PLL stabilization interrupt disable
      \arg        RCU_INT_IRC48MSTB: IRC48M stabilization interrupt disable
      \arg        RCU_INT_LXTALCKM: LXTAL clock stuck interrup disable
    \param[out] none
    \retval     none
*/
void rcu_interrupt_disable(rcu_int_enum stab_int)
{
    RCU_REG_VAL(stab_int) &= ~BIT(RCU_BIT_POS(stab_int));
}

/*!
    \brief      wait until oscillator stabilization flags is SET
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
      \arg        RCU_IRC16M: IRC16M
      \arg        RCU_IRC48M: IRC48M
      \arg        RCU_IRC32K: IRC32K
      \arg        RCU_PLL_CK: PLL
    \param[out] none
    \retval     ErrStatus: SUCCESS or ERROR
*/
ErrStatus rcu_osci_stab_wait(rcu_osci_type_enum osci)
{
    uint32_t stb_cnt = 0U;
    ErrStatus reval = ERROR;
    FlagStatus osci_stat = RESET;
    switch(osci) {
    case RCU_HXTAL:
        /* wait until HXTAL is stabilization and osci_stat is not more than timeout */
        while((RESET == osci_stat) && (HXTAL_STARTUP_TIMEOUT != stb_cnt)) {
            osci_stat = rcu_flag_get(RCU_FLAG_HXTALSTB);
            stb_cnt++;
        }
        /* check whether flag is set or not */
        if(RESET != rcu_flag_get(RCU_FLAG_HXTALSTB)) {
            reval = SUCCESS;
        }
        break;

    /* wait LXTAL stable */
    case RCU_LXTAL:
        while((RESET == osci_stat) && (LXTAL_STARTUP_TIMEOUT != stb_cnt)) {
            osci_stat = rcu_flag_get(RCU_FLAG_LXTALSTB);
            stb_cnt++;
        }
        /* check whether flag is set or not */
        if(RESET != rcu_flag_get(RCU_FLAG_LXTALSTB)) {
            reval = SUCCESS;
        }
        break;

    /* wait IRC16M stable */
    case RCU_IRC16M:
        while((RESET == osci_stat) && (IRC16M_STARTUP_TIMEOUT != stb_cnt)) {
            osci_stat = rcu_flag_get(RCU_FLAG_IRC16MSTB);
            stb_cnt++;
        }
        /* check whether flag is set or not */
        if(RESET != rcu_flag_get(RCU_FLAG_IRC16MSTB)) {
            reval = SUCCESS;
        }
        break;

    /* wait IRC48M stable */
    case RCU_IRC48M:
        while((RESET == osci_stat) && (OSC_STARTUP_TIMEOUT != stb_cnt)) {
            osci_stat = rcu_flag_get(RCU_FLAG_IRC48MSTB);
            stb_cnt++;
        }
        /* check whether flag is set or not */
        if(RESET != rcu_flag_get(RCU_FLAG_IRC48MSTB)) {
            reval = SUCCESS;
        }
        break;

    /* wait IRC32K stable */
    case RCU_IRC32K:
        while((RESET == osci_stat) && (OSC_STARTUP_TIMEOUT != stb_cnt)) {
            osci_stat = rcu_flag_get(RCU_FLAG_IRC32KSTB);
            stb_cnt++;
        }
        /* check whether flag is set or not */
        if(RESET != rcu_flag_get(RCU_FLAG_IRC32KSTB)) {
            reval = SUCCESS;
        }
        break;

    /* wait PLL stable */
    case RCU_PLL_CK:
        while((RESET == osci_stat) && (OSC_STARTUP_TIMEOUT != stb_cnt)) {
            osci_stat = rcu_flag_get(RCU_FLAG_PLLSTB);
            stb_cnt++;
        }
        /* check whether flag is set or not */
        if(RESET != rcu_flag_get(RCU_FLAG_PLLSTB)) {
            reval = SUCCESS;
        }
        break;

    default:
        break;
    }
    /* return value */
    return reval;
}

/*!
    \brief      turn on the oscillator
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
      \arg        RCU_IRC16M: IRC16M
      \arg        RCU_IRC48M: IRC48M
      \arg        RCU_IRC32K: IRC32K
      \arg        RCU_PLL_CK: PLL
    \param[out] none
    \retval     none
*/
void rcu_osci_on(rcu_osci_type_enum osci)
{
    RCU_REG_VAL(osci) |= BIT(RCU_BIT_POS(osci));
}

/*!
    \brief      turn off the oscillator
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
      \arg        RCU_IRC16M: IRC16M
      \arg        RCU_IRC48M: IRC48M
      \arg        RCU_IRC32K: IRC32K
      \arg        RCU_PLL_CK: PLL
    \param[out] none
    \retval     none
*/
void rcu_osci_off(rcu_osci_type_enum osci)
{
    RCU_REG_VAL(osci) &= ~BIT(RCU_BIT_POS(osci));
}

/*!
    \brief      enable the oscillator bypass mode, HXTALEN or LXTALEN must be reset before it
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
    \param[out] none
    \retval     none
*/
void rcu_osci_bypass_mode_enable(rcu_osci_type_enum osci)
{
    uint32_t reg;
    switch(osci) {
    case RCU_HXTAL:
        /* HXTALEN must be reset before enable the oscillator bypass mode */
        reg = RCU_CTL;
        RCU_CTL &= ~RCU_CTL_HXTALEN;
        RCU_CTL = (reg | RCU_CTL_HXTALBPS);
        break;
    case RCU_LXTAL:
        /* LXTALEN must be reset before enable the oscillator bypass mode */
        reg = RCU_BDCTL;
        RCU_BDCTL &= ~RCU_BDCTL_LXTALEN;
        RCU_BDCTL = (reg | RCU_BDCTL_LXTALBPS);
        break;
    case RCU_IRC16M:
    case RCU_IRC48M:
    case RCU_IRC32K:
    case RCU_PLL_CK:
        break;
    default:
        break;
    }
}

/*!
    \brief      disable the oscillator bypass mode, HXTALEN or LXTALEN must be reset before it
    \param[in]  osci: oscillator types, refer to rcu_osci_type_enum
                only one parameter can be selected which is shown as below:
      \arg        RCU_HXTAL: HXTAL
      \arg        RCU_LXTAL: LXTAL
    \param[out] none
    \retval     none
*/
void rcu_osci_bypass_mode_disable(rcu_osci_type_enum osci)
{
    uint32_t reg;
    switch(osci) {
    case RCU_HXTAL:
        /* HXTALEN must be reset before disable the oscillator bypass mode */
        reg = RCU_CTL;
        RCU_CTL &= ~RCU_CTL_HXTALEN;
        RCU_CTL = (reg & (~RCU_CTL_HXTALBPS));
        break;
    case RCU_LXTAL:
        /* LXTALEN must be reset before disable the oscillator bypass mode */
        reg = RCU_BDCTL;
        RCU_BDCTL &= ~RCU_BDCTL_LXTALEN;
        RCU_BDCTL = (reg & (~RCU_BDCTL_LXTALBPS));
        break;
    case RCU_IRC16M:
    case RCU_IRC48M:
    case RCU_IRC32K:
    case RCU_PLL_CK:
        break;
    default:
        break;
    }
}

/*!
    \brief      enable the HXTAL clock monitor
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_hxtal_clock_monitor_enable(void)
{
    RCU_CTL |= RCU_CTL_CKMEN;
}

/*!
    \brief      disable the HXTAL clock monitor
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_hxtal_clock_monitor_disable(void)
{
    RCU_CTL &= ~RCU_CTL_CKMEN;
}

/*!
    \brief      enable the LXTAL clock monitor
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_lxtal_clock_monitor_enable(void)
{
    RCU_CTL |= RCU_CTL_LXTALCKMEN;
}

/*!
    \brief      disable the LXTAL clock monitor
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_lxtal_clock_monitor_disable(void)
{
    RCU_CTL &= ~RCU_CTL_LXTALCKMEN;
}

/*!
    \brief      set the IRC16M adjust value
    \param[in]  irc16m_adjval: IRC16M adjust value, must be between 0 and 0x1F
    \param[out] none
    \retval     none
*/
void rcu_irc16m_adjust_value_set(uint8_t irc16m_adjval)
{
    uint32_t adjust = 0U;
    adjust = RCU_CTL;
    /* reset the IRC16MADJ bits and set according to irc16m_adjval */
    adjust &= ~RCU_CTL_IRC16MADJ;
    RCU_CTL = (adjust | (((uint32_t)irc16m_adjval) << 3));
}


/*!
    \brief      unlock the voltage key
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_voltage_key_unlock(void)
{
    /* reset the KEY bits and set 0x1A2B3C4D */
    RCU_VKEY &= ~RCU_VKEY_KEY;
    RCU_VKEY |= RCU_VKEY_UNLOCK;
}

/*!
    \brief      get the system clock, bus and peripheral clock frequency
    \param[in]  clock: the clock frequency which to get
                only one parameter can be selected which is shown as below:
      \arg        CK_SYS: system clock frequency
      \arg        CK_AHB: AHB clock frequency
      \arg        CK_APB1: APB1 clock frequency
      \arg        CK_APB2: APB2 clock frequency
      \arg        CK_ADC: ADC clock frequency
      \arg        CK_USART0: USART0 clock frequency
      \arg        CK_USART1: USART1 clock frequency
      \arg        CK_LPTIMER: LPTIMER clock frequency
      \arg        CK_LPUART: LPUART clock frequency
    \param[out] none
    \retval     clock frequency of system, AHB, APB1, APB2, ADC or USRAT0/1, LPUART, LPTIMER
*/
uint32_t rcu_clock_freq_get(rcu_clock_freq_enum clock)
{
    uint32_t sws = 0U, adcps = 0U, adcps2 = 0U, ck_freq = 0U;
    uint32_t cksys_freq = 0U, ahb_freq = 0U, apb1_freq = 0U, apb2_freq = 0U;
    uint32_t adc_freq = 0U, usart_freq = 0U, lptimer_freq = 0U;
    uint32_t pllmf = 0U, pllmf6 = 0U, pllsel = 0U, prediv = 0U, idx = 0U, clk_exp = 0U;
    /* exponent of AHB, APB1 and APB2 clock divider */
    const uint8_t ahb_exp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
    const uint8_t apb1_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};
    const uint8_t apb2_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};
    const uint8_t IRC16M_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};

    sws = GET_BITS(RCU_CFG0, 2, 3);
    switch(sws) {
    /* IRC16M is selected as CK_SYS */
    case SEL_IRC16M:
        cksys_freq = IRC16M_VALUE;
        break;
    /* HXTAL is selected as CK_SYS */
    case SEL_HXTAL:
        cksys_freq = HXTAL_VALUE;
        break;
    /* PLL is selected as CK_SYS */
    case SEL_PLL:
        /* get the value of PLLMF[5:0] */
        pllmf  = GET_BITS(RCU_CFG0, 18, 23);
        pllmf6 = GET_BITS(RCU_CFG0, 27, 27);
        pllmf  = ((pllmf6 << 6) + pllmf);
        /* high 16 bits */
        if(14U <= pllmf) {
            pllmf += 1U;
        } else if(15U == pllmf) {
            pllmf = 16U;
        } else {
            pllmf += 2U;
        }

        /* PLL clock source selection, HXTAL or IRC16M_VALUE or IRC48M_VALUE */
        pllsel = GET_BITS(RCU_CFG0, 16, 17);
        if(0U == pllsel) {
            prediv = (GET_BITS(RCU_CFG1, 0, 3) + 1U);
            cksys_freq = (IRC16M_VALUE / prediv) * pllmf;
        } else if(1U == pllsel) {
            prediv = (GET_BITS(RCU_CFG1, 0, 3) + 1U);
            cksys_freq = (HXTAL_VALUE / prediv) * pllmf;
        } else {
            prediv = (GET_BITS(RCU_CFG1, 0, 3) + 1U);
            cksys_freq = (IRC48M_VALUE / prediv) * pllmf;
        }
        break;
    /* IRC16M is selected as CK_SYS */
    default:
        cksys_freq = IRC16M_VALUE;
        break;
    }
    /* calculate AHB clock frequency */
    idx = GET_BITS(RCU_CFG0, 4, 7);
    clk_exp = ahb_exp[idx];
    ahb_freq = cksys_freq >> clk_exp;

    /* calculate APB1 clock frequency */
    idx = GET_BITS(RCU_CFG0, 8, 10);
    clk_exp = apb1_exp[idx];
    apb1_freq = ahb_freq >> clk_exp;

    /* calculate APB2 clock frequency */
    idx = GET_BITS(RCU_CFG0, 11, 13);
    clk_exp = apb2_exp[idx];
    apb2_freq = ahb_freq >> clk_exp;

    /* return the clocks frequency */
    switch(clock) {
    case CK_SYS:
        ck_freq = cksys_freq;
        break;
    case CK_AHB:
        ck_freq = ahb_freq;
        break;
    case CK_APB1:
        ck_freq = apb1_freq;
        break;
    case CK_APB2:
        ck_freq = apb2_freq;
        break;
    case CK_ADC:
        /* calculate ADC clock frequency */
        if(RCU_ADCSRC_AHB_APB2DIV != (RCU_CFG2 & RCU_CFG2_ADCSEL)) {
            adc_freq = IRC48M_VALUE;
        } else {
            /* ADC clock select CK_APB2 divided by 2/4/6/8 or CK_AHB divided by 3/5/7/9 */
            adcps = GET_BITS(RCU_CFG0, 14, 15);
            adcps2 = GET_BITS(RCU_CFG2, 30, 31);
            switch(adcps) {
            case 0:
                if(0U == adcps2) {
                    adc_freq = apb2_freq / 2U;
                } else if(1U == adcps2) {
                    adc_freq = apb2_freq / 10U;
                } else if(2U == adcps2) {
                    adc_freq = ahb_freq / 3U;
                } else if(3U == adcps2) {
                    adc_freq = ahb_freq / 11U;
                } else {
                }
                break;
            case 1:
                if(0U == adcps2) {
                    adc_freq = apb2_freq / 4U;
                } else if(1U == adcps2) {
                    adc_freq = apb2_freq / 12U;
                } else if(2U == adcps2) {
                    adc_freq = ahb_freq / 5U;
                } else if(3U == adcps2) {
                    adc_freq = ahb_freq / 13U;
                } else {
                }
                break;
            case 2:
                if(0U == adcps2) {
                    adc_freq = apb2_freq / 6U;
                } else if(1U == adcps2) {
                    adc_freq = apb2_freq / 14U;
                } else if(2U == adcps2) {
                    adc_freq = ahb_freq /  7U;
                } else if(3U == adcps2) {
                    adc_freq = ahb_freq / 15U;
                } else {
                }
                break;
            case 3:
                if(0U == adcps2) {
                    adc_freq = apb2_freq / 8U;
                } else if(1U == adcps2) {
                    adc_freq = apb2_freq / 16U;
                } else if(2U == adcps2) {
                    adc_freq = ahb_freq /  9U;
                } else if(3U == adcps2) {
                    adc_freq = ahb_freq / 17U;
                } else {
                }
                break;
            default:
                break;
            }
        }
        ck_freq = adc_freq;
        break;
    case CK_USART0:
        /* calculate USART0 clock frequency */
        if(RCU_USART0SRC_CKAPB2 == (RCU_CFG2 & RCU_CFG2_USART0SEL)) {
            usart_freq = apb2_freq;
        } else if(RCU_USART0SRC_CKSYS == (RCU_CFG2 & RCU_CFG2_USART0SEL)) {
            usart_freq = cksys_freq;
        } else if(RCU_USART0SRC_LXTAL == (RCU_CFG2 & RCU_CFG2_USART0SEL)) {
            usart_freq = LXTAL_VALUE;
        } else if(RCU_USART0SRC_IRC16MDIV == (RCU_CFG2 & RCU_CFG2_USART0SEL)) {
            /* calculate IRC16MDIV clock frequency */
            idx = GET_BITS(RCU_CFG2, 18, 20);
            clk_exp = IRC16M_exp[idx];
            usart_freq = IRC16M_VALUE >> clk_exp;
        } else {
        }
        ck_freq = usart_freq;
        break;
    case CK_USART1:
        /* calculate USART1 clock frequency */
        if(RCU_USART1SRC_CKAPB1 == (RCU_CFG2 & RCU_CFG2_USART1SEL)) {
            usart_freq = apb1_freq;
        } else if(RCU_USART1SRC_CKSYS == (RCU_CFG2 & RCU_CFG2_USART1SEL)) {
            usart_freq = cksys_freq;
        } else if(RCU_USART1SRC_LXTAL == (RCU_CFG2 & RCU_CFG2_USART1SEL)) {
            usart_freq = LXTAL_VALUE;
        } else if(RCU_USART1SRC_IRC16MDIV == (RCU_CFG2 & RCU_CFG2_USART1SEL)) {
            /* calculate IRC16MDIV clock frequency */
            idx = GET_BITS(RCU_CFG2, 18, 20);
            clk_exp = IRC16M_exp[idx];
            usart_freq = IRC16M_VALUE >> clk_exp;
        } else {
        }
        ck_freq = usart_freq;
        break;
    case CK_LPUART:
        /* calculate LPUART clock frequency */
        if(RCU_LPUARTSRC_CKAPB1 == (RCU_CFG2 & RCU_CFG2_LPUARTSEL)) {
            usart_freq = apb1_freq;
        } else if(RCU_LPUARTSRC_CKSYS == (RCU_CFG2 & RCU_CFG2_LPUARTSEL)) {
            usart_freq = cksys_freq;
        } else if(RCU_LPUARTSRC_LXTAL == (RCU_CFG2 & RCU_CFG2_LPUARTSEL)) {
            usart_freq = LXTAL_VALUE;
        } else if(RCU_LPUARTSRC_IRC16MDIV == (RCU_CFG2 & RCU_CFG2_LPUARTSEL)) {
            /* calculate IRC16MDIV clock frequency */
            idx = GET_BITS(RCU_CFG2, 18, 20);
            clk_exp = IRC16M_exp[idx];
            usart_freq = IRC16M_VALUE >> clk_exp;
        } else {
        }
        ck_freq = usart_freq;
        break;
    case CK_LPTIMER:
        /* calculate LPTIMER clock frequency */
        if(RCU_LPTIMERSRC_CKAPB1 == (RCU_CFG2 & RCU_CFG2_LPTIMERSEL)) {
            lptimer_freq = apb1_freq;
        } else if(RCU_LPTIMERSRC_IRC32K == (RCU_CFG2 & RCU_CFG2_LPTIMERSEL)) {
            lptimer_freq = IRC32K_VALUE;
        } else if(RCU_LPTIMERSRC_LXTAL == (RCU_CFG2 & RCU_CFG2_LPTIMERSEL)) {
            lptimer_freq = LXTAL_VALUE;
        } else if(RCU_LPTIMERSRC_IRC16MDIV == (RCU_CFG2 & RCU_CFG2_LPTIMERSEL)) {
            /* calculate IRC16MDIV clock frequency */
            idx = GET_BITS(RCU_CFG2, 18, 20);
            clk_exp = IRC16M_exp[idx];
            usart_freq = IRC16M_VALUE >> clk_exp;
        } else {
        }
        ck_freq = lptimer_freq;
        break;
    default:
        break;
    }
    return ck_freq;
}
