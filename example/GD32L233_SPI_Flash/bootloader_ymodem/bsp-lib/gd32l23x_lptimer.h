/*!
    \file    gd32l23x_lptimer.h
    \brief   definitions for the LPTIMER

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

#ifndef GD32L23X_LPTIMER_H
#define GD32L23X_LPTIMER_H

#include "gd32l23x.h"

/* LPTIMER definitions */
#define LPTIMER                             LPTIMER_BASE

/* registers definitions */
#define LPTIMER_INTF                        REG32(LPTIMER + 0x00U)              /*!< interrupt flag register */
#define LPTIMER_INTC                        REG32(LPTIMER + 0x04U)              /*!< interrupt flag clear register */
#define LPTIMER_INTEN                       REG32(LPTIMER + 0x08U)              /*!< interrupt enable register */
#define LPTIMER_CTL0                        REG32(LPTIMER + 0x0CU)              /*!< control register 0 */
#define LPTIMER_CTL1                        REG32(LPTIMER + 0x10U)              /*!< control register 1 */
#define LPTIMER_CMPV                        REG32(LPTIMER + 0x14U)              /*!< compare value register */
#define LPTIMER_CAR                         REG32(LPTIMER + 0x18U)              /*!< counter auto reload register */
#define LPTIMER_CNT                         REG32(LPTIMER + 0x1CU)              /*!< counter register */
#define LPTIMER_EIRMP                       REG32(LPTIMER + 0x20U)              /*!< external input remap register */
#define LPTIMER_INHLCMV                     REG32(LPTIMER + 0x24U)              /*!< input high level counter max value register */

/* bits definitions */
/* LPTIMER_INTF */
#define LPTIMER_INTF_CMPVMIF                BIT(0)                              /*!< compare value register match interrupt flag */
#define LPTIMER_INTF_CARMIF                 BIT(1)                              /*!< counter auto reload register match interrupt flag */
#define LPTIMER_INTF_ETEDEVIF               BIT(2)                              /*!< external trigger edge event interrupt flag */
#define LPTIMER_INTF_CMPVUPIF               BIT(3)                              /*!< compare value register update interrupt flag */
#define LPTIMER_INTF_CARUPIF                BIT(4)                              /*!< counter auto reload register update interrupt flag */
#define LPTIMER_INTF_UPIF                   BIT(5)                              /*!< LPTIMER counter direction change down to up interrupt flag */
#define LPTIMER_INTF_DOWNIF                 BIT(6)                              /*!< LPTIMER counter direction change up to down interrupt flag */
#define LPTIMER_INTF_HLCMVUPIF              BIT(26)                             /*!< input high level counter max value register update interrupt flag */
#define LPTIMER_INTF_INHLCOIF               BIT(27)                             /*!< LPTIMER_INx(x=0,1) high level counter overflow interrupt flag */
#define LPTIMER_INTF_INHLOEIF               BIT(28)                             /*!< the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag */
#define LPTIMER_INTF_INRFOEIF               BIT(29)                             /*!< the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag */
#define LPTIMER_INTF_IN0EIF                 BIT(30)                             /*!< LPTIMER_IN0 error interrupt flag */
#define LPTIMER_INTF_IN1EIF                 BIT(31)                             /*!< LPTIMER_IN1 error interrupt flag */

/* LPTIMER_INTC */
#define LPTIMER_INTC_CMPVMIC                BIT(0)                              /*!< compare value register match interrupt flag clear bit */
#define LPTIMER_INTC_CARMIC                 BIT(1)                              /*!< counter auto reload register match interrupt flag clear bit */
#define LPTIMER_INTC_ETEDEVIC               BIT(2)                              /*!< external trigger edge event interrupt flag clear bit */
#define LPTIMER_INTC_CMPVUPIC               BIT(3)                              /*!< compare value register update interrupt flag clear bit */
#define LPTIMER_INTC_CARUPIC                BIT(4)                              /*!< counter auto reload register update interrupt flag clear bit */
#define LPTIMER_INTC_UPIC                   BIT(5)                              /*!< LPTIMER counter direction change down to up interrupt flag clear bit */
#define LPTIMER_INTC_DOWNIC                 BIT(6)                              /*!< LPTIMER counter direction change up to down interrupt flag clear bit */
#define LPTIMER_INTC_HLCMVUPIC              BIT(26)                             /*!< input high level counter max value register update interrupt flag clear bit */
#define LPTIMER_INTC_INHLCOIC               BIT(27)                             /*!< LPTIMER_INx(x=0, 1) high level counter overflow interrupt flag clear bit */
#define LPTIMER_INTC_INHLOEIC               BIT(28)                             /*!< the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag clear bit */
#define LPTIMER_INTC_INRFOEIC               BIT(29)                             /*!< the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag clear bit */
#define LPTIMER_INTC_IN0EIC                 BIT(30)                             /*!< LPTIMER_IN0 error interrupt flag clear bit */
#define LPTIMER_INTC_IN1EIC                 BIT(31)                             /*!< LPTIMER_IN1 error interrupt flag clear bit */

/* LPTIMER_INTEN */
#define LPTIMER_INTEN_CMPVMIE               BIT(0)                              /*!< compare value register match interrupt enable bit */
#define LPTIMER_INTEN_CARMIE                BIT(1)                              /*!< counter auto reload register match interrupt enable bit */
#define LPTIMER_INTEN_ETEDEVIE              BIT(2)                              /*!< external trigger edge event interrupt enable bit */
#define LPTIMER_INTEN_CMPVUPIE              BIT(3)                              /*!< compare value register update interrupt enable bit */
#define LPTIMER_INTEN_CARUPIE               BIT(4)                              /*!< counter auto reload register update interrupt enable bit */
#define LPTIMER_INTEN_UPIE                  BIT(5)                              /*!< LPTIMER counter direction change down to up interrupt enable bit */
#define LPTIMER_INTEN_DOWNIE                BIT(6)                              /*!< LPTIMER counter direction change up to down interrupt enable bit */
#define LPTIMER_INTEN_HLCMVUPIE             BIT(26)                             /*!< input high level counter max value register update interrupt enable bit */
#define LPTIMER_INTEN_INHLCOIE              BIT(27)                             /*!< LPTIMER_INx(x=0,1) high level counter overflow interrupt enable bit */
#define LPTIMER_INTEN_INHLOEIE              BIT(28)                             /*!< the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt enable bit */
#define LPTIMER_INTEN_INRFOEIE              BIT(29)                             /*!< the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt enable bit */
#define LPTIMER_INTEN_IN0EIE                BIT(30)                             /*!< LPTIMER_IN0 error interrupt enable bit */
#define LPTIMER_INTEN_IN1EIE                BIT(31)                             /*!< LPTIMER_IN1 error interrupt enable bit */

/* LPTIMER_CTL0 */
#define LPTIMER_CTL0_CKSSEL                 BIT(0)                              /*!< clock source select */
#define LPTIMER_CTL0_CKPSEL                 BITS(1, 2)                          /*!< clock polarity select */
#define LPTIMER_CTL0_ECKFLT                 BITS(3, 4)                          /*!< external clock filter */
#define LPTIMER_CTL0_TFLT                   BITS(6, 7)                          /*!< trigger filter */
#define LPTIMER_CTL0_PSC                    BITS(9, 11)                         /*!< clock prescaler selection */
#define LPTIMER_CTL0_ETSEL                  BITS(13, 15)                        /*!< external trigger select */
#define LPTIMER_CTL0_ETMEN                  BITS(17, 18)                        /*!< external trigger mode enable */
#define LPTIMER_CTL0_TIMEOUT                BIT(19)                             /*!< timeout enable */
#define LPTIMER_CTL0_OMSEL                  BIT(20)                             /*!< output mode select */
#define LPTIMER_CTL0_OPSEL                  BIT(21)                             /*!< output polarity select */
#define LPTIMER_CTL0_SHWEN                  BIT(22)                             /*!< LPTIMER_CAR and LPTIMER_CMPV shadow registers enable */
#define LPTIMER_CTL0_CNTMEN                 BIT(23)                             /*!< counter mode select */
#define LPTIMER_CTL0_DECMEN                 BIT(24)                             /*!< decoder mode enabled */
#define LPTIMER_CTL0_DECMSEL                BIT(25)                             /*!< decoder mode select */

/* LPTIMER_CTL1 */
#define LPTIMER_CTL1_LPTEN                  BIT(0)                              /*!< LPTIMER enable */
#define LPTIMER_CTL1_SMST                   BIT(1)                              /*!< LPTIMER start in single counting mode */
#define LPTIMER_CTL1_CTNMST                 BIT(2)                              /*!< LPTIMER start in continus counting mode */
#define LPTIMER_CTL1_LPTENF                 BIT(30)                             /*!< LPTIMER enable from LPTIMER core */
#define LPTIMER_CTL1_INHLCEN                BIT(31)                             /*!< LPTIMER external input high level counter enable */

/* LPTIMER_CMPV */
#define LPTIMER_CMPV_CMPVAL                 BITS(0, 31)                          /*!< compare value */

/* LPTIMER_CAR */
#define LPTIMER_CAR_CARL                    BITS(0, 31)                          /*!< counter auto reload value */

/* LPTIMER_CNT */
#define LPTIMER_CNT_CNT                     BITS(0, 31)                          /*!< counter value */

/* LPTIMER_EIRMP */
#define LPTIMER_EIRMP_IN0RMP                BIT(0)                              /*!< external input remap 1 */
#define LPTIMER_EIRMP_IN1RMP                BIT(1)                              /*!< external input remap 1 */

/* LPTIMER_INHLCMV */
#define LPTIMER_INHLCMV_INHLCMVAL           BITS(0, 25)                         /*!< input high level counter max value */

/* constants definitions */
/* LPTIMER init parameter struct definitions */
typedef struct {
    uint32_t clocksource;                                                       /*!< clock source */
    uint32_t prescaler;                                                         /*!< counter clock prescaler */
    uint32_t extclockpolarity;                                                  /*!< external clock polarity of the active edge for the counter */
    uint32_t extclockfilter;                                                    /*!< external clock sampling time to configure the clock glitch filter */
    uint32_t triggermode;                                                       /*!< trigger mode */
    uint32_t extriggersource;                                                   /*!< external trigger source */
    uint32_t extriggerfilter;                                                   /*!< external trigger sampling time to configure the trigger glitch filter */
    uint32_t outputpolarity;                                                    /*!< output polarity */
    uint32_t outputmode;                                                        /*!< output mode */
    uint32_t countersource;                                                     /*!< whether the counter is incremented each internal event or each external event */
} lptimer_parameter_struct;

/* clock source select */
#define LPTIMER_INTERNALCLK                 ((uint32_t)0x00000000U)             /*!< LPTIMER is clocked by internal clock source */
#define LPTIMER_EXTERNALCLK                 LPTIMER_CTL0_CKSSEL                 /*!< LPTIMER is clocked by external clock source on the LPTIMER_IN0 */

/* external clock polarity select */
#define CTL0_CKPSEL(regval)                 (BITS(1, 2) & ((uint32_t)(regval) << 1U))
#define LPTIMER_EXTERNALCLK_RISING          CTL0_CKPSEL(0)                      /*!< the rising edge is the active edge used for counting */
#define LPTIMER_EXTERNALCLK_FALLING         CTL0_CKPSEL(1)                      /*!< the falling edge is the active edge used for counting */
#define LPTIMER_EXTERNALCLK_BOTH            CTL0_CKPSEL(2)                      /*!< both edges are the active edge used for counting */

/* external clock filter select */
#define CTL0_ECKFLT(regval)                 (BITS(3, 4) & ((uint32_t)(regval) << 3U))
#define LPTIMER_EXTERNALCLK_FILTEROFF       CTL0_ECKFLT(0)                      /*!< external clock filter disabled */
#define LPTIMER_EXTERNALCLK_FILTER_2        CTL0_ECKFLT(1)                      /*!< the active level change of the external clock need to be maintained at least 2 clock periods */
#define LPTIMER_EXTERNALCLK_FILTER_4        CTL0_ECKFLT(2)                      /*!< the active level change of the external clock need to be maintained at least 4 clock periods */
#define LPTIMER_EXTERNALCLK_FILTER_8        CTL0_ECKFLT(3)                      /*!< the active level change of the external clock need to be maintained at least 8 clock periods */

/* trigger filter select */
#define CTL0_TFLT(regval)                   (BITS(6, 7) & ((uint32_t)(regval) << 6U))
#define LPTIMER_TRIGGER_FILTEROFF           CTL0_TFLT(0)                        /*!< trigger filter disabled */
#define LPTIMER_TRIGGER_FILTER_2            CTL0_TFLT(1)                        /*!< the active level change of the trigger need to be maintained at least 2 clock periods */
#define LPTIMER_TRIGGER_FILTER_4            CTL0_TFLT(2)                        /*!< the active level change of the trigger need to be maintained at least 4 clock periods */
#define LPTIMER_TRIGGER_FILTER_8            CTL0_TFLT(3)                        /*!< the active level change of the trigger need to be maintained at least 8 clock periods */

/* clock prescaler select */
#define CTL0_CLKPSC(regval)                 (BITS(9, 11) & ((uint32_t)(regval) << 9U))
#define LPTIMER_PSC_1                       CTL0_CLKPSC(0)                      /*!< no prescaler */
#define LPTIMER_PSC_2                       CTL0_CLKPSC(1)                      /*!< divided by 2 */
#define LPTIMER_PSC_4                       CTL0_CLKPSC(2)                      /*!< divided by 4 */
#define LPTIMER_PSC_8                       CTL0_CLKPSC(3)                      /*!< divided by 8 */
#define LPTIMER_PSC_16                      CTL0_CLKPSC(4)                      /*!< divided by 16 */
#define LPTIMER_PSC_32                      CTL0_CLKPSC(5)                      /*!< divided by 32 */
#define LPTIMER_PSC_64                      CTL0_CLKPSC(6)                      /*!< divided by 64 */
#define LPTIMER_PSC_128                     CTL0_CLKPSC(7)                      /*!< divided by 128 */

/* external trigger select */
#define CTL0_ETSEL(regval)                  (BITS(13, 15) & ((uint32_t)(regval) << 13U))
#define LPTIMER_EXTRIGGER_GPIO              CTL0_ETSEL(0)                       /*!< external trigger for GPIO */
#define LPTIMER_EXTRIGGER_RTCALARM0         CTL0_ETSEL(1)                       /*!< external trigger for RTC Alarm 0 */
#define LPTIMER_EXTRIGGER_RTCALARM1         CTL0_ETSEL(2)                       /*!< external trigger for RTC Alarm 1 */
#define LPTIMER_EXTRIGGER_RTCTAMP0          CTL0_ETSEL(3)                       /*!< external trigger for RTC_TAMP0 input detection */
#define LPTIMER_EXTRIGGER_RTCTAMP1          CTL0_ETSEL(4)                       /*!< external trigger for RTC_TAMP1 input detection */
#define LPTIMER_EXTRIGGER_RTCTAMP2          CTL0_ETSEL(5)                       /*!< external trigger for RTC_TAMP2 input detection */
#define LPTIMER_EXTRIGGER_CMP0_OUT          CTL0_ETSEL(6)                       /*!< external trigger for CMP0_OUT */
#define LPTIMER_EXTRIGGER_CMP1_OUT          CTL0_ETSEL(7)                       /*!< external trigger for CMP1_OUT */

/* trigger mode select */
#define CTL0_ETMEN(regval)                  (BITS(17, 18) & ((uint32_t)(regval) << 17U))
#define LPTIMER_TRIGGER_SOFTWARE            CTL0_ETMEN(0)                       /*!< external trigger disable (software trigger) */
#define LPTIMER_TRIGGER_EXTERNALRISING      CTL0_ETMEN(1)                       /*!< rising edge of external trigger enable */
#define LPTIMER_TRIGGER_EXTERNALFALLING     CTL0_ETMEN(2)                       /*!< falling edge of external trigger enable */
#define LPTIMER_TRIGGER_EXTERNALBOTH        CTL0_ETMEN(3)                       /*!< rising and falling edges of external trigger enable */

/* timeout select */
#define LPTIMER_TIMEOUT_DISABLE             ((uint32_t)0x00000000U)             /*!< a new trigger event will be ignored after LPTIMER started */
#define LPTIMER_TIMEOUT_ENABLE              LPTIMER_CTL0_TIMEOUT                /*!< a new trigger event will reset and restart the count after LPTIMER started */

/* output mode select */
#define LPTIMER_OUTPUT_PWMORSINGLE          ((uint32_t)0x00000000U)             /*!< PWM mode or single pulse mode */
#define LPTIMER_OUTPUT_SET                  LPTIMER_CTL0_OMSEL                  /*!< set mode */

/* clock polarity  select */
#define LPTIMER_OUTPUT_NOTINVERTED          ((uint32_t)0x00000000U)             /*!< the output is non-inverted */
#define LPTIMER_OUTPUT_INVERTED             LPTIMER_CTL0_OPSEL                  /*!< the output is inverted */

/* CAR and CMPV shadow registers enable */
#define LPTIMER_SHADOW_DISABLE              ((uint32_t)0x00000000U)             /*!< the shadow registers are disable */
#define LPTIMER_SHADOW_ENABLE               LPTIMER_CTL0_SHWEN                  /*!< the shadow registers are enable */

/* counter mode select */
#define LPTIMER_COUNTER_INTERNAL            ((uint32_t)0x00000000U)             /*!< the counter is count with each internal clock pulse */
#define LPTIMER_COUNTER_EXTERNAL            LPTIMER_CTL0_CNTMEN                 /*!< the counter is count with each active clock pulse on the LPTIMER_IN0 */

/* external input remap */
#define LPTIMER_INPUT0_GPIO                 ((uint32_t)0x00000000U)             /*!< external input 0 is remaped to GPIO */
#define LPTIMER_INPUT0_CMP0_OUT             ((uint32_t)0x00000001U)             /*!< external input 0 is remaped to CMP0_OUT */
#define LPTIMER_INPUT1_GPIO                 ((uint32_t)0x00000000U)             /*!< external input 1 is remaped to GPIO */
#define LPTIMER_INPUT1_CMP1_OUT             ((uint32_t)0x00000002U)             /*!< external input 1 is remaped to CMP1_OUT */

/* interrupt enable or disable */
#define LPTIMER_INT_CMPVM                   LPTIMER_INTEN_CMPVMIE               /*!< compare value register match interrupt */
#define LPTIMER_INT_CARM                    LPTIMER_INTEN_CARMIE                /*!< counter auto reload register match interrupt */
#define LPTIMER_INT_ETEDEV                  LPTIMER_INTEN_ETEDEVIE              /*!< external trigger edge event interrupt */
#define LPTIMER_INT_CMPVUP                  LPTIMER_INTEN_CMPVUPIE              /*!< compare value register update interrupt */
#define LPTIMER_INT_CARUP                   LPTIMER_INTEN_CARUPIE               /*!< counter auto reload register update interrupt */
#define LPTIMER_INT_UP                      LPTIMER_INTEN_UPIE                  /*!< LPTIMER counter direction change down to up interrupt */
#define LPTIMER_INT_DOWN                    LPTIMER_INTEN_DOWNIE                /*!< LPTIMER counter direction change up to down interrupt */
#define LPTIMER_INT_HLCMVUP                 LPTIMER_INTEN_HLCMVUPIE             /*!< input high level counter max value register update interrupt */
#define LPTIMER_INT_INHLCO                  LPTIMER_INTEN_INHLCOIE              /*!< LPTIMER_INx(x=0,1) high level counter overflow interrupt */
#define LPTIMER_INT_INHLOE                  LPTIMER_INTEN_INHLOEIE              /*!< the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt */
#define LPTIMER_INT_INRFOE                  LPTIMER_INTEN_INRFOEIE              /*!< the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt */
#define LPTIMER_INT_IN0E                    LPTIMER_INTEN_IN0EIE                /*!< LPTIMER_IN0 error interrupt */
#define LPTIMER_INT_IN1E                    LPTIMER_INTEN_IN1EIE                /*!< LPTIMER_IN1 error interrupt */

/* flag */
#define LPTIMER_FLAG_CMPVM                  LPTIMER_INTF_CMPVMIF                /*!< compare value register match flag */
#define LPTIMER_FLAG_CARM                   LPTIMER_INTF_CARMIF                 /*!< counter auto reload register match flag */
#define LPTIMER_FLAG_ETEDEV                 LPTIMER_INTF_ETEDEVIF               /*!< external trigger edge event flag */
#define LPTIMER_FLAG_CMPVUP                 LPTIMER_INTF_CMPVUPIF               /*!< compare value register update flag */
#define LPTIMER_FLAG_CARUP                  LPTIMER_INTF_CARUPIF                /*!< counter auto reload register update flag */
#define LPTIMER_FLAG_UP                     LPTIMER_INTF_UPIF                   /*!< LPTIMER counter direction change down to up flag */
#define LPTIMER_FLAG_DOWN                   LPTIMER_INTF_DOWNIF                 /*!< LPTIMER counter direction change up to down flag */
#define LPTIMER_FLAG_HLCMVUP                LPTIMER_INTF_HLCMVUPIF              /*!< input high level counter max value register update flag */
#define LPTIMER_FLAG_INHLCO                 LPTIMER_INTF_INHLCOIF               /*!< LPTIMER_INx(x=0,1) high level counter overflow flag */
#define LPTIMER_FLAG_INHLOE                 LPTIMER_INTF_INHLOEIF               /*!< the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error flag */
#define LPTIMER_FLAG_INRFOE                 LPTIMER_INTF_INRFOEIF               /*!< the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error flag */
#define LPTIMER_FLAG_IN0E                   LPTIMER_INTF_IN0EIF                 /*!< LPTIMER_IN0 error flag */
#define LPTIMER_FLAG_IN1E                   LPTIMER_INTF_IN1EIF                 /*!< LPTIMER_IN1 error flag */

/* interrupt flag */
#define LPTIMER_INT_FLAG_CMPVM              LPTIMER_INTF_CMPVMIF                /*!< compare value register match interrupt flag */
#define LPTIMER_INT_FLAG_CARM               LPTIMER_INTF_CARMIF                 /*!< counter auto reload register match interrupt flag */
#define LPTIMER_INT_FLAG_ETEDEV             LPTIMER_INTF_ETEDEVIF               /*!< external trigger edge event interrupt flag */
#define LPTIMER_INT_FLAG_CMPVUP             LPTIMER_INTF_CMPVUPIF               /*!< compare value register update interrupt flag */
#define LPTIMER_INT_FLAG_CARUP              LPTIMER_INTF_CARUPIF                /*!< counter auto reload register update interrupt flag */
#define LPTIMER_INT_FLAG_UP                 LPTIMER_INTF_UPIF                   /*!< LPTIMER counter direction change down to up interrupt flag */
#define LPTIMER_INT_FLAG_DOWN               LPTIMER_INTF_DOWNIF                 /*!< LPTIMER counter direction change up to down interrupt flag */
#define LPTIMER_INT_FLAG_HLCMVUP            LPTIMER_INTF_HLCMVUPIF              /*!< input high level counter max value register update interrupt flag */
#define LPTIMER_INT_FLAG_INHLCO             LPTIMER_INTF_INHLCOIF               /*!< LPTIMER_INx(x=0,1) high level counter overflow interrupt flag */
#define LPTIMER_INT_FLAG_INHLOE             LPTIMER_INTF_INHLOEIF               /*!< the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag */
#define LPTIMER_INT_FLAG_INRFOE             LPTIMER_INTF_INRFOEIF               /*!< the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag */
#define LPTIMER_INT_FLAG_IN0E               LPTIMER_INTF_IN0EIF                 /*!< LPTIMER_IN0 error interrupt flag */
#define LPTIMER_INT_FLAG_IN1E               LPTIMER_INTF_IN1EIF                 /*!< LPTIMER_IN1 error interrupt flag */

/* function declarations */
/* deinit the LPTIMER */
void lptimer_deinit(void);
/* initialize LPTIMER init parameter struct with a default value */
void lptimer_struct_para_init(lptimer_parameter_struct *initpara);
/* initialize LPTIMER counter */
void lptimer_init(lptimer_parameter_struct *initpara);
/* configure external input remap */
void lptimer_inputremap(uint32_t input0remap, uint32_t input1remap);
/* enable the LPTIMER_CAR and LPTIMER_CMPV registers shadow function */
void lptimer_register_shadow_enable(void);
/* disable the LPTIMER_CAR and LPTIMER_CMPV registers shadow function */
void lptimer_register_shadow_disable(void);
/* enable the LPTIMER TIMEOUT function */
void lptimer_timeout_enable(void);
/* disable the LPTIMER TIMEOUT function */
void lptimer_timeout_disable(void);

/* LPTIMER start and stop */
/* LPTIMER start with countinue mode */
void lptimer_countinue_start(uint32_t autoreload, uint32_t compare);
/* LPTIMER start with single mode */
void lptimer_single_start(uint32_t autoreload, uint32_t compare);
/* stop LPTIMER */
void lptimer_stop(void);

/* read and configure LPTIMER register value */
/* read LPTIMER current counter value */
uint32_t lptimer_counter_read(void);
/* read LPTIMER auto reload value */
uint32_t lptimer_autoreload_read(void);
/* read the compare value */
uint32_t lptimer_compare_read(void);
/* configure LPTIMER autoreload register value */
void lptimer_autoreload_value_config(uint32_t autoreload);
/* configure LPTIMER compare value */
void lptimer_compare_value_config(uint32_t compare);

/* LPTIMER decode mode */
/* enable decode mode 0 */
void lptimer_decodemode0_enable(void);
/* enable decode mode 1 */
void lptimer_decodemode1_enable(void);
/* disable decode mode 0/1 */
void lptimer_decodemode_disable(void);

/* LPTIMER input high level counter */
/* enable external input high level counter */
void lptimer_highlevelcounter_enable(uint32_t maxvalue);
/* disable external input high level counter */
void lptimer_highlevelcounter_disable(void);

/* LPTIMER interrupt and flag */
/* get LPTIMER flags */
FlagStatus lptimer_flag_get(uint32_t flag);
/* clear LPTIMER flags */
void lptimer_flag_clear(uint32_t flag);
/* enable the LPTIMER interrupt */
void lptimer_interrupt_enable(uint32_t interrupt);
/* disable the LPTIMER interrupt */
void lptimer_interrupt_disable(uint32_t interrupt);
/* get LPTIMER interrupt flag */
FlagStatus lptimer_interrupt_flag_get(uint32_t int_flag);
/* clear LPTIMER interrupt flag */
void lptimer_interrupt_flag_clear(uint32_t int_flag);

#endif /* GD32L23X_LPTIMER_H */
