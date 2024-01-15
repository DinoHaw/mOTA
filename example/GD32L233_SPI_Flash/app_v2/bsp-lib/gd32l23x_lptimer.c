/*!
    \file    gd32l23x_lptimer.c
    \brief   LPTIMER driver

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

#include "gd32l23x_lptimer.h"

/*!
    \brief      deinit LPTIMER
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_deinit(void)
{
    /* reset LPTIMER */
    rcu_periph_reset_enable(RCU_LPTIMERRST);
    rcu_periph_reset_disable(RCU_LPTIMERRST);
}

/*!
    \brief      initialize LPTIMER init parameter struct with a default value
    \param[in]  initpara: init parameter struct
    \param[out] none
    \retval     none
*/
void lptimer_struct_para_init(lptimer_parameter_struct *initpara)
{
    /* initialize the init parameter struct member with the default value */
    initpara->clocksource      = LPTIMER_INTERNALCLK;
    initpara->prescaler        = LPTIMER_PSC_1;
    initpara->extclockpolarity = LPTIMER_EXTERNALCLK_RISING;
    initpara->extclockfilter   = LPTIMER_EXTERNALCLK_FILTEROFF;
    initpara->triggermode      = LPTIMER_TRIGGER_SOFTWARE;
    initpara->extriggersource  = LPTIMER_EXTRIGGER_GPIO;
    initpara->extriggerfilter  = LPTIMER_TRIGGER_FILTEROFF;
    initpara->outputpolarity   = LPTIMER_OUTPUT_NOTINVERTED;
    initpara->outputmode       = LPTIMER_OUTPUT_PWMORSINGLE;
    initpara->countersource    = LPTIMER_COUNTER_INTERNAL;
}

/*!
    \brief      initialize LPTIMER counter
    \param[in]  initpara: init parameter struct
                  clocksource: LPTIMER_INTERNALCLK, LPTIMER_EXTERNALCLK
                  prescaler: prescaler value of the counter clock, LPTIMER_PSC_x(x=1,2,4,8..128)
                  extclockpolarity: LPTIMER_EXTERNALCLK_RISING, LPTIMER_EXTERNALCLK_FALLING, LPTIMER_EXTERNALCLK_BOTH
                  extclockfilter: LPTIMER_EXTERNALCLK_FILTEROFF, LPTIMER_EXTERNALCLK_FILTER_2, LPTIMER_EXTERNALCLK_FILTER_4, LPTIMER_EXTERNALCLK_FILTER_8
                  triggermode: LPTIMER_TRIGGER_SOFTWARE, LPTIMER_TRIGGER_EXTERNALRISING, LPTIMER_TRIGGER_EXTERNALFALLING, LPTIMER_TRIGGER_EXTERNALBOTH
                  extriggersource: LPTIMER_EXTRIGGER_GPIO, LPTIMER_EXTRIGGER_RTCALARM0, LPTIMER_EXTRIGGER_RTCALARM1, LPTIMER_EXTRIGGER_RTCTAMP0, LPTIMER_EXTRIGGER_RTCTAMP1,
                                   LPTIMER_EXTRIGGER_RTCTAMP2, LPTIMER_EXTRIGGER_CMP0_OUT, LPTIMER_EXTRIGGER_CMP1_OUT
                  extriggerfilter: LPTIMER_TRIGGER_FILTEROFF, LPTIMER_TRIGGER_FILTER_2, LPTIMER_TRIGGER_FILTER_4, LPTIMER_TRIGGER_FILTER_8
                  outputpolarity: LPTIMER_OUTPUT_NOTINVERTED, LPTIMER_OUTPUT_INVERTED
                  outputmode: LPTIMER_OUTPUT_PWMORSINGLE, LPTIMER_OUTPUT_SET
                  countersource: LPTIMER_COUNTER_INTERNAL, LPTIMER_COUNTER_EXTERNAL
    \param[out] none
    \retval     none
*/
void lptimer_init(lptimer_parameter_struct *initpara)
{
    LPTIMER_CTL0 = initpara->clocksource | initpara->prescaler | initpara->triggermode | \
                   initpara->outputpolarity | initpara->outputmode | initpara->countersource;

    if(initpara->triggermode != LPTIMER_TRIGGER_SOFTWARE) {
        LPTIMER_CTL0 |= initpara->extriggersource | initpara->extriggerfilter;
    }

    if((initpara->clocksource == LPTIMER_EXTERNALCLK) || (initpara->countersource == LPTIMER_COUNTER_EXTERNAL)) {
        LPTIMER_CTL0 |= initpara->extclockpolarity | initpara->extclockfilter;
    }

    if((initpara->clocksource == LPTIMER_INTERNALCLK) && (initpara->countersource == LPTIMER_COUNTER_EXTERNAL)) {
        LPTIMER_CTL0 &= ~LPTIMER_CTL0_PSC;
    }
}

/*!
    \brief      configure external input remap
    \param[in]  input0remap: external input0 remap
                only one parameter can be selected which is shown as below:
      \arg        LPTIMER_INPUT0_GPIO
      \arg        LPTIMER_INPUT0_CMP0_OUT
    \param[in]  input1remap: external input1 remap
                only one parameter can be selected which is shown as below:
      \arg        LPTIMER_INPUT1_GPIO
      \arg        LPTIMER_INPUT1_CMP1_OUT
    \param[out] none
    \retval     none
*/
void lptimer_inputremap(uint32_t input0remap, uint32_t input1remap)
{
    LPTIMER_EIRMP = (uint32_t)(input0remap | input1remap);
}

/*!
    \brief      enable the LPTIMER_CAR and LPTIMER_CMPV registers shadow function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_register_shadow_enable(void)
{
    LPTIMER_CTL0 |= (uint32_t)LPTIMER_CTL0_SHWEN;
}

/*!
    \brief      disable the LPTIMER_CAR and LPTIMER_CMPV registers shadow function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_register_shadow_disable(void)
{
    LPTIMER_CTL0 &= ~(uint32_t)LPTIMER_CTL0_SHWEN;
}

/*!
    \brief      enable the LPTIMER TIMEOUT function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_timeout_enable(void)
{
    LPTIMER_CTL0 |= (uint32_t)LPTIMER_CTL0_TIMEOUT;
}

/*!
    \brief      disable the LPTIMER TIMEOUT function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_timeout_disable(void)
{
    LPTIMER_CTL0 &= ~(uint32_t)LPTIMER_CTL0_TIMEOUT;
}

/*!
    \brief      LPTIMER countinue start
    \param[in]  autoreload: auto reload value, 0x0~0xffffffff
    \param[in]  compare: compare value, 0x0~0xffffffff
    \param[out] none
    \retval     none
*/
void lptimer_countinue_start(uint32_t autoreload, uint32_t compare)
{
    LPTIMER_INTC |= (uint32_t)(LPTIMER_INTC_CMPVUPIC | LPTIMER_INTC_CARUPIC);
    LPTIMER_CTL1 |= (uint32_t)LPTIMER_CTL1_LPTEN;

    while(0U == (LPTIMER_CTL1 & LPTIMER_CTL1_LPTENF)) {
    }
    LPTIMER_CAR = autoreload;
    LPTIMER_CMPV = compare;
    while(0U == (LPTIMER_INTF & LPTIMER_INTF_CARUPIF)) {
    }
    while(0U == (LPTIMER_INTF & LPTIMER_INTF_CMPVUPIF)) {
    }
    LPTIMER_INTC = LPTIMER_INTC_CMPVUPIC | LPTIMER_INTC_CARUPIC;
    LPTIMER_CTL1 |= LPTIMER_CTL1_CTNMST;
}

/*!
    \brief      LPTIMER single start
    \param[in]  autoreload: auto reload value, 0x0~0xffffffff
    \param[in]  compare: compare value, 0x0~0xffffffff
    \param[out] none
    \retval     none
*/
void lptimer_single_start(uint32_t autoreload, uint32_t compare)
{
    LPTIMER_INTC |= (uint32_t)(LPTIMER_INTC_CMPVUPIC | LPTIMER_INTC_CARUPIC);
    LPTIMER_CTL1 |= (uint32_t)LPTIMER_CTL1_LPTEN;

    while(0U == (LPTIMER_CTL1 & LPTIMER_CTL1_LPTENF)) {
    }
    LPTIMER_CAR = autoreload;
    LPTIMER_CMPV = compare;
    while(0U == (LPTIMER_INTF & LPTIMER_INTF_CARUPIF)) {
    }
    while(0U == (LPTIMER_INTF & LPTIMER_INTF_CMPVUPIF)) {
    }
    LPTIMER_INTC = LPTIMER_INTC_CMPVUPIC | LPTIMER_INTC_CARUPIC;
    LPTIMER_CTL1 |= LPTIMER_CTL1_SMST;
}

/*!
    \brief      stop LPTIMER
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_stop(void)
{
    LPTIMER_CTL1 &= ~(uint32_t)LPTIMER_CTL1_LPTEN;
}

/*!
    \brief      read LPTIMER current counter value
    \param[in]  none
    \param[out] none
    \retval     counter value
*/
uint32_t lptimer_counter_read(void)
{
    uint32_t count_value = 0U;
    count_value = LPTIMER_CNT;
    return (count_value);
}

/*!
    \brief      read LPTIMER auto reload value
    \param[in]  none
    \param[out] none
    \retval     auto reload value
*/
uint32_t lptimer_autoreload_read(void)
{
    uint32_t car_value = 0U;
    car_value = LPTIMER_CAR;
    return (car_value);
}

/*!
    \brief      read LPTIMER compare value
    \param[in]  none
    \param[out] none
    \retval     compare value
*/
uint32_t lptimer_compare_read(void)
{
    uint32_t comp_value = 0U;
    comp_value = LPTIMER_CMPV;
    return (comp_value);
}

/*!
    \brief      configure LPTIMER autoreload register value
    \param[in]  autoreload: autoreload value
    \param[out] none
    \retval     none
*/
void lptimer_autoreload_value_config(uint32_t autoreload)
{
    LPTIMER_CAR = (uint32_t)autoreload;
}

/*!
    \brief      configure LPTIMER compare value
    \param[in]  compare: compare value
    \param[out] none
    \retval     none
*/
void lptimer_compare_value_config(uint32_t compare)
{
    LPTIMER_CMPV = (uint32_t)compare;
}

/*!
    \brief      enable decode mode 0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_decodemode0_enable(void)
{
    LPTIMER_CTL0 &= ~(uint32_t)(LPTIMER_CTL0_DECMEN | LPTIMER_CTL0_DECMSEL);
    LPTIMER_CTL0 |= LPTIMER_CTL0_DECMEN;
}

/*!
    \brief      enable decode mode 1
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_decodemode1_enable(void)
{
    LPTIMER_CTL0 |= (uint32_t)(LPTIMER_CTL0_DECMEN | LPTIMER_CTL0_DECMSEL);
}

/*!
    \brief      disable decode mode 0/1
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_decodemode_disable(void)
{
    LPTIMER_CTL0 &= ~(uint32_t)(LPTIMER_CTL0_DECMEN | LPTIMER_CTL0_DECMSEL);
}

/*!
    \brief      enable external input high level counter
    \param[in]  maxvalue: input high level counter max value, 0x0~0x03ffffff
    \param[out] none
    \retval     none
*/
void lptimer_highlevelcounter_enable(uint32_t maxvalue)
{
    LPTIMER_CTL1 |= (uint32_t)LPTIMER_CTL1_INHLCEN;
    LPTIMER_INHLCMV = (uint32_t)maxvalue;
}

/*!
    \brief      disable external input high level counter
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lptimer_highlevelcounter_disable(void)
{
    LPTIMER_CTL1 &= ~(uint32_t)LPTIMER_CTL1_INHLCEN;
}

/*!
    \brief      get LPTIMER flags
    \param[in]  flag: the LPTIMER flag
                only one parameter can be selected which is shown as below:
      \arg        LPTIMER_FLAG_CMPVM: compare value register match flag
      \arg        LPTIMER_FLAG_CARM: counter auto reload register match flag
      \arg        LPTIMER_FLAG_ETEDEV: external trigger edge event flag
      \arg        LPTIMER_FLAG_CMPVUP: compare value register update flag
      \arg        LPTIMER_FLAG_CARUP: counter auto reload register update flag
      \arg        LPTIMER_FLAG_UP: LPTIMER counter direction change down to up flag
      \arg        LPTIMER_FLAG_DOWN: LPTIMER counter direction change up to down flag
      \arg        LPTIMER_FLAG_HLCMVUP: input high level counter max value register update flag
      \arg        LPTIMER_FLAG_INHLCO: LPTIMER_INx(x=0,1) high level counter overflow flag
      \arg        LPTIMER_FLAG_INHLOE: the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error flag
      \arg        LPTIMER_FLAG_INRFOE: the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error flag
      \arg        LPTIMER_FLAG_IN0E: LPTIMER_IN0 error flag
      \arg        LPTIMER_FLAG_IN1E: LPTIMER_IN1 error flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus lptimer_flag_get(uint32_t flag)
{
    if(RESET != (LPTIMER_INTF & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear LPTIMER flags
    \param[in]  flag: the LPTIMER flag
                one or more parameters can be selected which is shown as below:
      \arg        LPTIMER_FLAG_CMPVM: compare value register match flag
      \arg        LPTIMER_FLAG_CARM: counter auto reload register match flag
      \arg        LPTIMER_FLAG_ETEDEV: external trigger edge event flag
      \arg        LPTIMER_FLAG_CMPVUP: compare value register update flag
      \arg        LPTIMER_FLAG_CARUP: counter auto reload register update flag
      \arg        LPTIMER_FLAG_UP: LPTIMER counter direction change down to up flag
      \arg        LPTIMER_FLAG_DOWN: LPTIMER counter direction change up to down flag
      \arg        LPTIMER_FLAG_HLCMVUP: input high level counter max value register update flag
      \arg        LPTIMER_FLAG_INHLCO: LPTIMER_INx(x=0,1) high level counter overflow flag
      \arg        LPTIMER_FLAG_INHLOE: the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error flag
      \arg        LPTIMER_FLAG_INRFOE: the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error flag
      \arg        LPTIMER_FLAG_IN0E: LPTIMER_IN0 error flag
      \arg        LPTIMER_FLAG_IN1E: LPTIMER_IN1 error flag
    \param[out] none
    \retval     none
*/
void lptimer_flag_clear(uint32_t flag)
{
    LPTIMER_INTC |= (uint32_t)flag;
}

/*!
    \brief      enable the LPTIMER interrupt
    \param[in]  interrupt: LPTIMER interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        LPTIMER_INT_CMPVM: compare value register match interrupt
      \arg        LPTIMER_INT_CARM: counter auto reload register match interrupt
      \arg        LPTIMER_INT_ETEDEV: external trigger edge event interrupt
      \arg        LPTIMER_INT_CMPVUP: compare value register update interrupt
      \arg        LPTIMER_INT_CARUP: counter auto reload register update interrupt
      \arg        LPTIMER_INT_UP: LPTIMER counter direction change down to up interrupt
      \arg        LPTIMER_INT_DOWN: LPTIMER counter direction change up to down interrupt
      \arg        LPTIMER_INT_HLCMVUP: input high level counter max value register update interrupt
      \arg        LPTIMER_INT_INHLCO: LPTIMER_INx(x=0,1) high level counter overflow interrupt
      \arg        LPTIMER_INT_INHLOE: the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt
      \arg        LPTIMER_INT_INRFOE: the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt
      \arg        LPTIMER_INT_IN0E: LPTIMER_IN0 error interrupt
      \arg        LPTIMER_INT_IN1E: LPTIMER_IN1 error interrupt
    \param[out] none
    \retval     none
*/
void lptimer_interrupt_enable(uint32_t interrupt)
{
    LPTIMER_INTEN |= (uint32_t)interrupt;
}

/*!
    \brief      disable the LPTIMER interrupt
    \param[in]  interrupt: LPTIMER interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        LPTIMER_INT_CMPVM: compare value register match interrupt
      \arg        LPTIMER_INT_CARM: counter auto reload register match interrupt
      \arg        LPTIMER_INT_ETEDEV: external trigger edge event interrupt
      \arg        LPTIMER_INT_CMPVUP: compare value register update interrupt
      \arg        LPTIMER_INT_CARUP: counter auto reload register update interrupt
      \arg        LPTIMER_INT_UP: LPTIMER counter direction change down to up interrupt
      \arg        LPTIMER_INT_DOWN: LPTIMER counter direction change up to down interrupt
      \arg        LPTIMER_INT_HLCMVUP: input high level counter max value register update interrupt
      \arg        LPTIMER_INT_INHLCO: LPTIMER_INx(x=0,1) high level counter overflow interrupt
      \arg        LPTIMER_INT_INHLOE: the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt
      \arg        LPTIMER_INT_INRFOE: the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt
      \arg        LPTIMER_INT_IN0E: LPTIMER_IN0 error interrupt
      \arg        LPTIMER_INT_IN1E: LPTIMER_IN1 error interrupt
    \param[out] none
    \retval     none
*/
void lptimer_interrupt_disable(uint32_t interrupt)
{
    LPTIMER_INTEN &= (~(uint32_t)interrupt);
}

/*!
    \brief      get LPTIMER interrupt flag
    \param[in]  int_flag: the LPTIMER interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        LPTIMER_INT_FLAG_CMPVM: compare value register match interrupt flag
      \arg        LPTIMER_INT_FLAG_CARM: counter auto reload register match interrupt flag
      \arg        LPTIMER_INT_FLAG_ETEDEV: external trigger edge event interrupt flag
      \arg        LPTIMER_INT_FLAG_CMPVUP: compare value register update interrupt flag
      \arg        LPTIMER_INT_FLAG_CARUP: counter auto reload register update interrupt flag
      \arg        LPTIMER_INT_FLAG_UP: LPTIMER counter direction change down to up interrupt flag
      \arg        LPTIMER_INT_FLAG_DOWN: LPTIMER counter direction change up to down interrupt flag
      \arg        LPTIMER_INT_FLAG_HLCMVUP: input high level counter max value register update interrupt flag
      \arg        LPTIMER_INT_FLAG_INHLCO: LPTIMER_INx(x=0,1) high level counter overflow interrupt flag
      \arg        LPTIMER_INT_FLAG_INHLOE: the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag
      \arg        LPTIMER_INT_FLAG_INRFOE: the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag
      \arg        LPTIMER_INT_FLAG_IN0E: LPTIMER_IN0 error interrupt flag
      \arg        LPTIMER_INT_FLAG_IN1E: LPTIMER_IN1 error interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus lptimer_interrupt_flag_get(uint32_t int_flag)
{
    uint32_t val;
    val = (LPTIMER_INTEN & int_flag);
    if((RESET != (LPTIMER_INTF & int_flag)) && (RESET != val)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear LPTIMER interrupt flag
    \param[in]  int_flag: the LPTIMER interrupt flag
                one or more parameters can be selected which is shown as below:
      \arg        LPTIMER_INT_FLAG_CMPVM: compare value register match interrupt flag
      \arg        LPTIMER_INT_FLAG_CARM: counter auto reload register match interrupt flag
      \arg        LPTIMER_INT_FLAG_ETEDEV: external trigger edge event interrupt flag
      \arg        LPTIMER_INT_FLAG_CMPVUP: compare value register update interrupt flag
      \arg        LPTIMER_INT_FLAG_CARUP: counter auto reload register update interrupt flag
      \arg        LPTIMER_INT_FLAG_UP: LPTIMER counter direction change down to up interrupt flag
      \arg        LPTIMER_INT_FLAG_DOWN: LPTIMER counter direction change up to down interrupt flag
      \arg        LPTIMER_INT_FLAG_HLCMVUP: input high level counter max value register update interrupt flag
      \arg        LPTIMER_INT_FLAG_INHLCO: LPTIMER_INx(x=0,1) high level counter overflow interrupt flag
      \arg        LPTIMER_INT_FLAG_INHLOE: the high level of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag
      \arg        LPTIMER_INT_FLAG_INRFOE: the falling and rising edges of LPTIMER_IN0 and LPTIMER_IN1 overlap error interrupt flag
      \arg        LPTIMER_INT_FLAG_IN0E: LPTIMER_IN0 error interrupt flag
      \arg        LPTIMER_INT_FLAG_IN1E: LPTIMER_IN1 error interrupt flag
    \param[out] none
    \retval     none
*/
void lptimer_interrupt_flag_clear(uint32_t int_flag)
{
    LPTIMER_INTC |= (uint32_t)int_flag;
}
