/*!
    \file    gd32l23x_adc.c
    \brief   ADC driver

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

#include "gd32l23x_adc.h"

/* discontinuous mode macro*/
#define  ADC_CHANNEL_LENGTH_SUBTRACT_ONE            ((uint8_t)0x01U)

/* ADC regular channel macro */
#define  ADC_REGULAR_CHANNEL_RANK_SIX               ((uint8_t)0x06U)
#define  ADC_REGULAR_CHANNEL_RANK_TWELVE            ((uint8_t)0x0CU)
#define  ADC_REGULAR_CHANNEL_RANK_LENGTH            ((uint8_t)0x05U)

/* ADC sampling time macro */
#define  ADC_CHANNEL_SAMPLE_TEN                     ((uint8_t)0x0AU)
#define  ADC_CHANNEL_SAMPLE_NINETEEN                ((uint8_t)0x13U)
#define  ADC_CHANNEL_SAMPLE_LENGTH                  ((uint8_t)0x03U)

/* ADC inserted channel macro */
#define  ADC_INSERTED_CHANNEL_RANK_LENGTH           ((uint8_t)0x05U)
#define  ADC_INSERTED_CHANNEL_SHIFT_OFFSET          ((uint8_t)0x0FU)

/* ADC inserted channel offset macro */
#define  ADC_OFFSET_LENGTH                          ((uint8_t)0x03U)
#define  ADC_OFFSET_SHIFT_LENGTH                    ((uint8_t)0x04U)

/*!
    \brief      reset ADC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_deinit(void)
{
    rcu_periph_reset_enable(RCU_ADCRST);
    rcu_periph_reset_disable(RCU_ADCRST);
}

/*!
    \brief      enable ADC interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_enable(void)
{
    /* configure ADC charge pulse width counter */
    adc_charge_pulse_width_counter(320);

    if(RESET == (ADC_CTL1 & ADC_CTL1_ADCON)) {
        /* enable ADC */
        ADC_CTL1 |= (uint32_t)ADC_CTL1_ADCON;
    }
}

/*!
    \brief      disable ADC interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_disable(void)
{
    /* disable ADC */
    ADC_CTL1 &= ~((uint32_t)ADC_CTL1_ADCON);
}

/*!
    \brief      ADC calibration and reset calibration
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_calibration_enable(void)
{
    /* reset the selected ADC calibration registers */
    ADC_CTL1 |= (uint32_t) ADC_CTL1_RSTCLB;
    /* check the RSTCLB bit state */
    while((ADC_CTL1 & ADC_CTL1_RSTCLB)) {
    }
    /* enable ADC calibration process */
    ADC_CTL1 |= ADC_CTL1_CLB;
    /* check the CLB bit state */
    while((ADC_CTL1 & ADC_CTL1_CLB)) {
    }
}

/*!
    \brief      enable DMA request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_dma_mode_enable(void)
{
    /* enable DMA request */
    ADC_CTL1 |= (uint32_t)(ADC_CTL1_DMA);
}

/*!
    \brief      disable DMA request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_dma_mode_disable(void)
{
    /* disable DMA request */
    ADC_CTL1 &= ~((uint32_t)ADC_CTL1_DMA);
}

/*!
    \brief      configure ADC discontinuous mode
    \param[in]  adc_channel_group: select the channel group
                only one parameter can be selected which is shown as below:
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
      \arg        ADC_CHANNEL_DISCON_DISABLE: disable discontinuous mode of regular and inserted channel
    \param[in]  length: number of conversions in discontinuous mode,the number can be 1..16
                        for regular channel, the number has no effect for inserted channel
    \param[out] none
    \retval     none
*/
void adc_discontinuous_mode_config(uint8_t adc_channel_group, uint8_t length)
{
    /* disable discontinuous mode of regular & inserted channel */
    ADC_CTL0 &= ~((uint32_t)(ADC_CTL0_DISRC | ADC_CTL0_DISIC));
    switch(adc_channel_group) {
    case ADC_REGULAR_CHANNEL:
        /* configure the number of conversions in discontinuous mode */
        ADC_CTL0 &= ~((uint32_t)ADC_CTL0_DISNUM);
        if((length <= 16U) && (length >= 1U)) {
            ADC_CTL0 |= CTL0_DISNUM(((uint32_t)length - ADC_CHANNEL_LENGTH_SUBTRACT_ONE));
        }
        /* enable regular channel group discontinuous mode */
        ADC_CTL0 |= (uint32_t)ADC_CTL0_DISRC;
        break;
    case ADC_INSERTED_CHANNEL:
        /* enable inserted channel group discontinuous mode */
        ADC_CTL0 |= (uint32_t)ADC_CTL0_DISIC;
        break;
    case ADC_CHANNEL_DISCON_DISABLE:
    /* disable discontinuous mode of regular & inserted channel */
    default:
        break;
    }
}

/*!
    \brief      configure ADC special function
    \param[in]  function: the function to configure
                one or more parameters can be selected which is shown as below:
      \arg        ADC_SCAN_MODE: scan mode select
      \arg        ADC_INSERTED_CHANNEL_AUTO: inserted channel group convert automatically
      \arg        ADC_CONTINUOUS_MODE: continuous mode select
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_special_function_config(uint32_t function, ControlStatus newvalue)
{
    if(newvalue) {
        /* enable ADC scan mode */
        if(RESET != (function & ADC_SCAN_MODE)) {
            ADC_CTL0 |= (uint32_t)ADC_SCAN_MODE;
        }
        /* enable ADC inserted channel group convert automatically */
        if(RESET != (function & ADC_INSERTED_CHANNEL_AUTO)) {
            ADC_CTL0 |= (uint32_t)ADC_INSERTED_CHANNEL_AUTO;
        }
        /* enable ADC continuous mode */
        if(RESET != (function & ADC_CONTINUOUS_MODE)) {
            ADC_CTL1 |= (uint32_t)ADC_CONTINUOUS_MODE;
        }
    } else {
        /* disable ADC scan mode */
        if(RESET != (function & ADC_SCAN_MODE)) {
            ADC_CTL0 &= ~((uint32_t)ADC_SCAN_MODE);
        }
        /* disable ADC inserted channel group convert automatically */
        if(RESET != (function & ADC_INSERTED_CHANNEL_AUTO)) {
            ADC_CTL0 &= ~((uint32_t)ADC_INSERTED_CHANNEL_AUTO);
        }
        /* disable ADC continuous mode */
        if(RESET != (function & ADC_CONTINUOUS_MODE)) {
            ADC_CTL1 &= ~((uint32_t)ADC_CONTINUOUS_MODE);
        }
    }
}

/*!
    \brief      configure temperature sensor, internal reference voltage channel, VBAT channel or VSLCD channel function
    \param[in]  function: temperature sensor or internal reference voltage channel or VBAT channel or VSLCD channel function
                only one parameter can be selected which is shown as below:
      \arg        ADC_TEMP_CHANNEL_SWITCH: channel 16 (temperature sensor) switch of ADC
      \arg        ADC_INTERNAL_CHANNEL_SWITCH: channel 17 (internal reference voltage) switch of ADC
      \arg        ADC_VBAT_CHANNEL_SWITCH: channel 18 (1/3 voltage of external battery) switch of ADC
      \arg        ADC_VSLCD_CHANNEL_SWITCH: channel 19 (1/3 voltage of VSLCD) switch of ADC
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_channel_16_to_19(uint32_t function, ControlStatus newvalue)
{
    if(newvalue) {
        if(RESET != (function & ADC_TEMP_CHANNEL_SWITCH)) {
            /* enable ADC VBAT channel */
            ADC_CTL1 |= ADC_TEMP_CHANNEL_SWITCH;
        }
        if(RESET != (function & ADC_INTERNAL_CHANNEL_SWITCH)) {
            /* enable ADC VREF and Temperature channel */
            ADC_CTL1 |= ADC_INTERNAL_CHANNEL_SWITCH;
        }
        if(RESET != (function & ADC_VBAT_CHANNEL_SWITCH)) {
            /* enable ADC VREF and Temperature channel */
            ADC_CTL1 |= ADC_VBAT_CHANNEL_SWITCH;
        }
        if(RESET != (function & ADC_VSLCD_CHANNEL_SWITCH)) {
            /* enable ADC VREF and Temperature channel */
            ADC_CTL1 |= ADC_VSLCD_CHANNEL_SWITCH;
        }
    } else {
        if(RESET != (function & ADC_TEMP_CHANNEL_SWITCH)) {
            /* disable ADC VBAT channel  */
            ADC_CTL1 &= ~ADC_TEMP_CHANNEL_SWITCH;
        }
        if(RESET != (function & ADC_INTERNAL_CHANNEL_SWITCH)) {
            /* disable ADC VREF and temperature channel */
            ADC_CTL1 &= ~ADC_INTERNAL_CHANNEL_SWITCH;
        }
        if(RESET != (function & ADC_VBAT_CHANNEL_SWITCH)) {
            /* disable ADC VREF and temperature channel */
            ADC_CTL1 &= ~ADC_VBAT_CHANNEL_SWITCH;
        }
        if(RESET != (function & ADC_VSLCD_CHANNEL_SWITCH)) {
            /* disable ADC VREF and temperature channel */
            ADC_CTL1 &= ~ADC_VSLCD_CHANNEL_SWITCH;
        }
    }
}

/*!
    \brief      configure ADC data alignment
    \param[in]  data_alignment: data alignment select
                only one parameter can be selected which is shown as below:
      \arg        ADC_DATAALIGN_RIGHT: right alignment
      \arg        ADC_DATAALIGN_LEFT: left alignment
    \param[out] none
    \retval     none
*/
void adc_data_alignment_config(uint32_t data_alignment)
{
    if(ADC_DATAALIGN_RIGHT != data_alignment) {
        /* left alignment */
        ADC_CTL1 |= ADC_CTL1_DAL;
    } else {
        /* right alignment */
        ADC_CTL1 &= ~((uint32_t)ADC_CTL1_DAL);
    }
}

/*!
    \brief      configure the length of regular channel group or inserted channel group
    \param[in]  adc_channel_group: select the channel group
                only one parameter can be selected which is shown as below:
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[in]  length: the length of the channel
                        regular channel 1-16
                        inserted channel 1-4
    \param[out] none
    \retval     none
*/
void adc_channel_length_config(uint8_t adc_channel_group, uint32_t length)
{
    switch(adc_channel_group) {
    case ADC_REGULAR_CHANNEL:
        /* configure the length of regular channel group */
        if((length >= 1U) && (length <= 16U)) {
            ADC_RSQ0 &= ~((uint32_t)ADC_RSQ0_RL);
            ADC_RSQ0 |= RSQ0_RL((uint32_t)(length - ADC_CHANNEL_LENGTH_SUBTRACT_ONE));
        }
        break;
    case ADC_INSERTED_CHANNEL:
        /* configure the length of inserted channel group */
        if((length >= 1U) && (length <= 4U)) {
            ADC_ISQ &= ~((uint32_t)ADC_ISQ_IL);
            ADC_ISQ |= ISQ_IL((uint32_t)(length - ADC_CHANNEL_LENGTH_SUBTRACT_ONE));
        }
        break;
    default:
        break;
    }
}

/*!
    \brief      configure ADC regular channel
    \param[in]  rank: the regular group sequence rank, this parameter must be between 0 to 15
    \param[in]  adc_channel: the selected ADC channel
                only one parameter can be selected which is shown as below:
      \arg        ADC_CHANNEL_x(x=0..19): ADC Channelx
    \param[in]  sample_time: the sample time value
                only one parameter can be selected which is shown as below:
      \arg        ADC_SAMPLETIME_2POINT5: 2.5 cycles
      \arg        ADC_SAMPLETIME_7POINT5: 7.5 cycles
      \arg        ADC_SAMPLETIME_13POINT5: 13.5 cycles
      \arg        ADC_SAMPLETIME_28POINT5: 28.5 cycles
      \arg        ADC_SAMPLETIME_41POINT5: 41.5 cycles
      \arg        ADC_SAMPLETIME_55POINT5: 55.5 cycles
      \arg        ADC_SAMPLETIME_71POINT5: 71.5 cycles
      \arg        ADC_SAMPLETIME_239POINT5: 239.5 cycles
    \param[out] none
    \retval     none
*/
void adc_regular_channel_config(uint8_t rank, uint8_t adc_channel, uint32_t sample_time)
{
    uint32_t rsq, sampt;

    /* configure ADC regular sequence */
    if(rank < ADC_REGULAR_CHANNEL_RANK_SIX) {
        /* the regular group sequence rank is smaller than six */
        rsq = ADC_RSQ2;
        rsq &=  ~((uint32_t)(ADC_RSQX_RSQN << (ADC_REGULAR_CHANNEL_RANK_LENGTH * rank)));
        /* the channel number is written to these bits to select a channel as the nth conversion in the regular channel group */
        rsq |= ((uint32_t)adc_channel << (ADC_REGULAR_CHANNEL_RANK_LENGTH * rank));
        ADC_RSQ2 = rsq;
    } else if(rank < ADC_REGULAR_CHANNEL_RANK_TWELVE) {
        /* the regular group sequence rank is smaller than twelve */
        rsq = ADC_RSQ1;
        rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (ADC_REGULAR_CHANNEL_RANK_LENGTH * (rank - ADC_REGULAR_CHANNEL_RANK_SIX))));
        /* the channel number is written to these bits to select a channel as the nth conversion in the regular channel group */
        rsq |= ((uint32_t)adc_channel << (ADC_REGULAR_CHANNEL_RANK_LENGTH * (rank - ADC_REGULAR_CHANNEL_RANK_SIX)));
        ADC_RSQ1 = rsq;
    } else {
        /* the regular group sequence rank is larger than twelve */
        rsq = ADC_RSQ0;
        rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (ADC_REGULAR_CHANNEL_RANK_LENGTH * (rank - ADC_REGULAR_CHANNEL_RANK_TWELVE))));
        /* the channel number is written to these bits to select a channel as the nth conversion in the regular channel group */
        rsq |= ((uint32_t)adc_channel << (ADC_REGULAR_CHANNEL_RANK_LENGTH * (rank - ADC_REGULAR_CHANNEL_RANK_TWELVE)));
        ADC_RSQ0 = rsq;
    }

    /* configure ADC sampling time */
    if(adc_channel < ADC_CHANNEL_SAMPLE_TEN) {
        /* the regular group sequence rank is smaller than ten */
        sampt = ADC_SAMPT1;
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (ADC_CHANNEL_SAMPLE_LENGTH * adc_channel)));
        /* channel sample time set*/
        sampt |= (uint32_t)(sample_time << (ADC_CHANNEL_SAMPLE_LENGTH * adc_channel));
        ADC_SAMPT1 = sampt;
    } else if(adc_channel <= ADC_CHANNEL_SAMPLE_NINETEEN) {
        /* the regular group sequence rank is smaller than eleven */
        sampt = ADC_SAMPT0;
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (ADC_CHANNEL_SAMPLE_LENGTH * (adc_channel - ADC_CHANNEL_SAMPLE_TEN))));
        /* channel sample time set*/
        sampt |= (uint32_t)(sample_time << (ADC_CHANNEL_SAMPLE_LENGTH * (adc_channel - ADC_CHANNEL_SAMPLE_TEN)));
        ADC_SAMPT0 = sampt;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      configure ADC inserted channel
    \param[in]  rank: the inserted group sequencer rank,this parameter must be between 0 to 3
    \param[in]  adc_channel: the selected ADC channel
                only one parameter can be selected which is shown as below:
      \arg        ADC_CHANNEL_x(x=0..19): ADC Channelx
    \param[in]  sample_time: The sample time value
                only one parameter can be selected which is shown as below:
      \arg        ADC_SAMPLETIME_2POINT5: 2.5 cycles
      \arg        ADC_SAMPLETIME_7POINT5: 7.5 cycles
      \arg        ADC_SAMPLETIME_13POINT5: 13.5 cycles
      \arg        ADC_SAMPLETIME_28POINT5: 28.5 cycles
      \arg        ADC_SAMPLETIME_41POINT5: 41.5 cycles
      \arg        ADC_SAMPLETIME_55POINT5: 55.5 cycles
      \arg        ADC_SAMPLETIME_71POINT5: 71.5 cycles
      \arg        ADC_SAMPLETIME_239POINT5: 239.5 cycles
    \param[out] none
    \retval     none
*/
void adc_inserted_channel_config(uint8_t rank, uint8_t adc_channel, uint32_t sample_time)
{
    uint8_t inserted_length;
    uint32_t isq, sampt;

    /* get inserted channel group length */
    inserted_length = (uint8_t)GET_BITS(ADC_ISQ, 20U, 21U);
    /* the channel number is written to these bits to select a channel as the nth conversion in the inserted channel group */
    if(rank < 4U) {
        isq = ADC_ISQ;
        isq &= ~((uint32_t)(ADC_ISQ_ISQN << (ADC_INSERTED_CHANNEL_SHIFT_OFFSET - (inserted_length - rank) * ADC_INSERTED_CHANNEL_RANK_LENGTH)));
        isq |= ((uint32_t)adc_channel << (ADC_INSERTED_CHANNEL_SHIFT_OFFSET - (inserted_length - rank) * ADC_INSERTED_CHANNEL_RANK_LENGTH));
        ADC_ISQ = isq;
    }

    /* configure ADC sampling time */
    if(adc_channel < ADC_CHANNEL_SAMPLE_TEN) {
        /* the inserted group sequence rank is smaller than ten */
        sampt = ADC_SAMPT1;
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (ADC_CHANNEL_SAMPLE_LENGTH * adc_channel)));
        /* channel sample time set*/
        sampt |= (uint32_t) sample_time << (ADC_CHANNEL_SAMPLE_LENGTH * adc_channel);
        ADC_SAMPT1 = sampt;
    } else if(adc_channel <= ADC_CHANNEL_SAMPLE_NINETEEN) {
        /* the inserted group sequence rank is smaller than eighteen */
        sampt = ADC_SAMPT0;
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (ADC_CHANNEL_SAMPLE_LENGTH * (adc_channel - ADC_CHANNEL_SAMPLE_TEN))));
        /* channel sample time set*/
        sampt |= ((uint32_t)sample_time << (ADC_CHANNEL_SAMPLE_LENGTH * (adc_channel - ADC_CHANNEL_SAMPLE_TEN)));
        ADC_SAMPT0 = sampt;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      configure ADC inserted channel offset
    \param[in]  inserted_channel: inserted channel select
                only one parameter can be selected which is shown as below:
      \arg        ADC_INSERTED_CHANNEL_0: ADC inserted channel 0
      \arg        ADC_INSERTED_CHANNEL_1: ADC inserted channel 1
      \arg        ADC_INSERTED_CHANNEL_2: ADC inserted channel 2
      \arg        ADC_INSERTED_CHANNEL_3: ADC inserted channel 3
    \param[in]  offset: the offset data
    \param[out] none
    \retval     none
*/
void adc_inserted_channel_offset_config(uint8_t inserted_channel, uint16_t offset)
{
    uint8_t inserted_length;
    uint32_t num = 0U;

    inserted_length = (uint8_t)GET_BITS(ADC_ISQ, 20U, 21U);
    num = ((uint32_t)ADC_OFFSET_LENGTH - ((uint32_t)inserted_length - (uint32_t)inserted_channel));

    if(num <= ADC_OFFSET_LENGTH) {
        /* calculate the offset of the register */
        num = num * ADC_OFFSET_SHIFT_LENGTH;
        /* configure the offset of the selected channels */
        REG32(ADC + 0x14U + num) = IOFFX_IOFF((uint32_t)offset);
    }
}

/*!
    \brief      configure ADC external trigger
    \param[in]  adc_channel_group: select the channel group
                only one parameter can be selected which is shown as below:
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_external_trigger_config(uint8_t adc_channel_group, ControlStatus newvalue)
{
    if(newvalue) {
        /* external trigger enable for regular channel */
        if(RESET != (adc_channel_group & ADC_REGULAR_CHANNEL)) {
            ADC_CTL1 |= (uint32_t)ADC_CTL1_ETERC;
        }
        /* external trigger enable for inserted channel */
        if(RESET != (adc_channel_group & ADC_INSERTED_CHANNEL)) {
            ADC_CTL1 |= (uint32_t)ADC_CTL1_ETEIC;
        }
    } else {
        /* external trigger disable for regular channel */
        if(RESET != (adc_channel_group & ADC_REGULAR_CHANNEL)) {
            ADC_CTL1 &= ~((uint32_t)ADC_CTL1_ETERC);
        }
        /* external trigger disable for inserted channel */
        if(RESET != (adc_channel_group & ADC_INSERTED_CHANNEL)) {
            ADC_CTL1 &= ~((uint32_t)ADC_CTL1_ETEIC);
        }
    }
}

/*!
    \brief      configure ADC external trigger source
    \param[in]  adc_channel_group: select the channel group
                only one parameter can be selected which is shown as below:
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[in]  external_trigger_source: regular or inserted group trigger source
                only one parameter can be selected which is shown as below:
                for regular channel:
      \arg        ADC_EXTTRIG_REGULAR_T8_CH0: external trigger TIMER8 CH0 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T8_CH1: external trigger TIMER8 CH1 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T1_CH1: external trigger TIMER1 CH1 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T2_TRGO: external trigger TIMER2 TRGO event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_T11_CH0: external trigger TIMER11 CH0 event select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_EXTI_11: external trigger interrupt line 11 select for regular channel
      \arg        ADC_EXTTRIG_REGULAR_NONE: external trigger software event select for regular channel
                for inserted channel:
      \arg        ADC_EXTTRIG_INSERTED_T1_TRGO: TIMER1 TRGO event select for inserted channel
      \arg        ADC_EXTTRIG_INSERTED_T1_CH0: TIMER1 CH0 event select for inserted channel
      \arg        ADC_EXTTRIG_INSERTED_T2_CH3: TIMER2 CH3 event select for inserted channel
      \arg        ADC_EXTTRIG_INSERTED_EXTI_15: external interrupt line 15 event select for inserted channel
      \arg        ADC_EXTTRIG_INSERTED_NONE: external trigger software event select for inserted channel
    \param[out] none
    \retval     none
*/
void adc_external_trigger_source_config(uint8_t adc_channel_group, uint32_t external_trigger_source)
{
    switch(adc_channel_group) {
    case ADC_REGULAR_CHANNEL:
        /* configure ADC regular group external trigger source */
        ADC_CTL1 &= ~((uint32_t)ADC_CTL1_ETSRC);
        ADC_CTL1 |= (uint32_t)external_trigger_source;
        break;
    case ADC_INSERTED_CHANNEL:
        /* configure ADC inserted group external trigger source */
        ADC_CTL1 &= ~((uint32_t)ADC_CTL1_ETSIC);
        ADC_CTL1 |= (uint32_t)external_trigger_source;
        break;
    default:
        break;
    }
}

/*!
    \brief      enable ADC software trigger
    \param[in]  adc_channel_group: select the channel group
                only one parameter can be selected which is shown as below:
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
    \param[out] none
    \retval     none
*/
void adc_software_trigger_enable(uint8_t adc_channel_group)
{
    /* enable regular group channel software trigger */
    if(RESET != (adc_channel_group & ADC_REGULAR_CHANNEL)) {
        ADC_CTL1 |= (uint32_t)ADC_CTL1_SWRCST;
    }
    /* enable inserted channel group software trigger */
    if(RESET != (adc_channel_group & ADC_INSERTED_CHANNEL)) {
        ADC_CTL1 |= (uint32_t)ADC_CTL1_SWICST;
    }
}

/*!
    \brief      read ADC regular group data register
    \param[in]  none
    \param[out] none
    \retval     the conversion value: 0~0xFFFF
*/
uint16_t adc_regular_data_read(void)
{
    return ((uint16_t)(ADC_RDATA));
}

/*!
    \brief      read ADC inserted group data register
    \param[in]  inserted_channel: inserted channel select
                only one parameter can be selected which is shown as below:
      \arg        ADC_INSERTED_CHANNEL_0: ADC inserted channel 0
      \arg        ADC_INSERTED_CHANNEL_1: ADC inserted channel 1
      \arg        ADC_INSERTED_CHANNEL_2: ADC inserted channel 2
      \arg        ADC_INSERTED_CHANNEL_3: ADC inserted channel 3
    \param[out] none
    \retval     the conversion value: 0~0xFFFF
*/
uint16_t adc_inserted_data_read(uint8_t inserted_channel)
{
    uint32_t idata;
    /* read the data of the selected channel */
    switch(inserted_channel) {
    case ADC_INSERTED_CHANNEL_0:
        /* read the data of channel 0 */
        idata = ADC_IDATA0;
        break;
    case ADC_INSERTED_CHANNEL_1:
        /* read the data of channel 1 */
        idata = ADC_IDATA1;
        break;
    case ADC_INSERTED_CHANNEL_2:
        /* read the data of channel 2 */
        idata = ADC_IDATA2;
        break;
    case ADC_INSERTED_CHANNEL_3:
        /* read the data of channel 3 */
        idata = ADC_IDATA3;
        break;
    default:
        idata = 0U;
        break;
    }
    return ((uint16_t)idata);
}

/*!
    \brief      enable ADC analog watchdog single channel
    \param[in]  adc_channel: the selected ADC channel
                only one parameter can be selected which is shown as below:
      \arg        ADC_CHANNEL_x: ADC Channelx(x=0..19)
    \param[out] none
    \retval     none
*/
void adc_watchdog_single_channel_enable(uint8_t adc_channel)
{
    ADC_CTL0 &= (uint32_t)~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC | ADC_CTL0_WDCHSEL);

    ADC_CTL0 |= (uint32_t)adc_channel;
    ADC_CTL0 |= (uint32_t)(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC);
}

/*!
    \brief      enable ADC analog watchdog group channel
    \param[in]  adc_channel_group: the channel group use analog watchdog
                only one parameter can be selected which is shown as below:
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
      \arg        ADC_REGULAR_INSERTED_CHANNEL: both regular and inserted group
    \param[out] none
    \retval     none
*/
void adc_watchdog_group_channel_enable(uint8_t adc_channel_group)
{
    ADC_CTL0 &= ~((uint32_t)(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC));
    /* select the group */
    switch(adc_channel_group) {
    case ADC_REGULAR_CHANNEL:
        /* regular channel analog watchdog enable */
        ADC_CTL0 |= (uint32_t) ADC_CTL0_RWDEN;
        break;
    case ADC_INSERTED_CHANNEL:
        /* inserted channel analog watchdog enable */
        ADC_CTL0 |= (uint32_t) ADC_CTL0_IWDEN;
        break;
    case ADC_REGULAR_INSERTED_CHANNEL:
        /* regular and inserted channel analog watchdog enable */
        ADC_CTL0 |= (uint32_t)(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN);
        break;
    default:
        break;
    }
}

/*!
    \brief      disable ADC analog watchdog
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_watchdog_disable(void)
{
    ADC_CTL0 &= (uint32_t)~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC | ADC_CTL0_WDCHSEL);
}

/*!
    \brief      configure ADC analog watchdog threshold
    \param[in]  low_threshold: analog watchdog low threshold, 0..4095
    \param[in]  high_threshold: analog watchdog high threshold, 0..4095
    \param[out] none
    \retval     none
*/
void adc_watchdog_threshold_config(uint16_t low_threshold, uint16_t high_threshold)
{
    /* configure ADC analog watchdog low threshold */
    ADC_WDLT = (uint32_t)WDLT_WDLT(low_threshold);
    /* configure ADC analog watchdog high threshold */
    ADC_WDHT = (uint32_t)WDHT_WDHT(high_threshold);
}

/*!
    \brief      configure ADC resolution
    \param[in]  resolution: ADC resolution
                only one parameter can be selected which is shown as below:
      \arg        ADC_RESOLUTION_12B: 12-bit ADC resolution
      \arg        ADC_RESOLUTION_10B: 10-bit ADC resolution
      \arg        ADC_RESOLUTION_8B: 8-bit ADC resolution
      \arg        ADC_RESOLUTION_6B: 6-bit ADC resolution
    \param[out] none
    \retval     none
*/
void adc_resolution_config(uint32_t resolution)
{
    ADC_CTL0 &= ~((uint32_t)ADC_CTL0_DRES);
    ADC_CTL0 |= (uint32_t)resolution;
}

/*!
    \brief      configure ADC oversample mode
    \param[in]  mode: ADC oversampling mode
                only one parameter can be selected which is shown as below:
      \arg        ADC_OVERSAMPLING_ALL_CONVERT: all oversampled conversions for a channel are done consecutively after a trigger
      \arg        ADC_OVERSAMPLING_ONE_CONVERT: each oversampled conversion for a channel needs a trigger
    \param[in]  shift: ADC oversampling shift
                only one parameter can be selected which is shown as below:
      \arg        ADC_OVERSAMPLING_SHIFT_NONE: no oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_1B: 1-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_2B: 2-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_3B: 3-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_4B: 3-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_5B: 5-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_6B: 6-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_7B: 7-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_8B: 8-bit oversampling shift
    \param[in]  ratio: ADC oversampling ratio
                only one parameter can be selected which is shown as below:
      \arg        ADC_OVERSAMPLING_RATIO_MUL2: oversampling ratio multiple 2
      \arg        ADC_OVERSAMPLING_RATIO_MUL4: oversampling ratio multiple 4
      \arg        ADC_OVERSAMPLING_RATIO_MUL8: oversampling ratio multiple 8
      \arg        ADC_OVERSAMPLING_RATIO_MUL16: oversampling ratio multiple 16
      \arg        ADC_OVERSAMPLING_RATIO_MUL32: oversampling ratio multiple 32
      \arg        ADC_OVERSAMPLING_RATIO_MUL64: oversampling ratio multiple 64
      \arg        ADC_OVERSAMPLING_RATIO_MUL128: oversampling ratio multiple 128
      \arg        ADC_OVERSAMPLING_RATIO_MUL256: oversampling ratio multiple 256
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_config(uint32_t mode, uint16_t shift, uint8_t ratio)
{
    /* configure ADC oversampling mode */
    if(ADC_OVERSAMPLING_ONE_CONVERT == mode) {
        ADC_OVSAMPCTL |= (uint32_t)ADC_OVSAMPCTL_TOVS;
    } else {
        ADC_OVSAMPCTL &= ~((uint32_t)ADC_OVSAMPCTL_TOVS);
    }
    /* configure the shift and ratio */
    ADC_OVSAMPCTL &= ~((uint32_t)(ADC_OVSAMPCTL_OVSR | ADC_OVSAMPCTL_OVSS));
    ADC_OVSAMPCTL |= ((uint32_t)shift | (uint32_t)ratio);
}

/*!
    \brief      enable ADC oversample mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_enable(void)
{
    ADC_OVSAMPCTL |= ADC_OVSAMPCTL_OVSEN;
}

/*!
    \brief      disable ADC oversample mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_disable(void)
{
    ADC_OVSAMPCTL &= ~((uint32_t)ADC_OVSAMPCTL_OVSEN);
}

/*!
    \brief      configure ADC charge pulse width counter
    \param[in]  value: ADC charge pulse width counter value
    \param[out] none
    \retval     none
*/
void adc_charge_pulse_width_counter(uint32_t value)
{
    ADC_CCTL = (uint32_t)CCTL_CCNT(value);
}

/*!
    \brief      get the ADC charge flag
    \param[in]  flag: the ADC charge flag
      \arg        ADC_FLAG_CHARGE: ADC charge flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_charge_flag_get(uint32_t flag)
{
    FlagStatus reval = RESET;
    if(ADC_CCTL & flag) {
        reval = SET;
    }
    return reval;
}

/*!
    \brief      get the flag of ADC regular channel software start conversion
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_regular_software_startconv_flag_get(void)
{
    FlagStatus reval = RESET;
    if((uint32_t)RESET != (ADC_STAT & ADC_STAT_STRC)) {
        reval = SET;
    }
    return reval;
}

/*!
    \brief      get the flag of ADC inserted channel software start conversion
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_inserted_software_startconv_flag_get(void)
{
    FlagStatus reval = RESET;
    if((uint32_t)RESET != (ADC_STAT & ADC_STAT_STIC)) {
        reval = SET;
    }
    return reval;
}

/*!
    \brief      get ADC flag
    \param[in]  flag: ADC flag
                only one parameter can be selected which is shown as below:
      \arg        ADC_FLAG_WDE: analog watchdog event flag
      \arg        ADC_FLAG_EOC: end of group conversion flag
      \arg        ADC_FLAG_EOIC: end of inserted group conversion flag
      \arg        ADC_FLAG_STIC: start flag of inserted channel group
      \arg        ADC_FLAG_STRC: start flag of regular channel group
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_flag_get(uint32_t flag)
{
    FlagStatus reval = RESET;
    if(ADC_STAT & flag) {
        reval = SET;
    }
    return reval;
}

/*!
    \brief      clear ADC flag
    \param[in]  flag: ADC flag
                only one parameter can be selected which is shown as below:
      \arg        ADC_FLAG_WDE: analog watchdog event flag
      \arg        ADC_FLAG_EOC: end of group conversion flag
      \arg        ADC_FLAG_EOIC: end of inserted group conversion flag
      \arg        ADC_FLAG_STIC: start flag of inserted channel group
      \arg        ADC_FLAG_STRC: start flag of regular channel group
    \param[out] none
    \retval     none
*/
void adc_flag_clear(uint32_t flag)
{
    ADC_STAT &= ~((uint32_t)flag);
}

/*!
    \brief      enable ADC interrupt
    \param[in]  interrupt: ADC interrupt
                only one parameter can be selected which is shown as below:
      \arg        ADC_INT_WDE: analog watchdog interrupt
      \arg        ADC_INT_EOC: end of group conversion interrupt
      \arg        ADC_INT_EOIC: end of inserted group conversion interrupt
    \param[out] none
    \retval     none
*/
void adc_interrupt_enable(uint32_t interrupt)
{
    switch(interrupt) {
    case ADC_INT_WDE:
        /* enable analog watchdog interrupt */
        ADC_CTL0 |= (uint32_t) ADC_CTL0_WDEIE;
        break;
    case ADC_INT_EOC:
        /* enable end of group conversion interrupt */
        ADC_CTL0 |= (uint32_t) ADC_CTL0_EOCIE;
        break;
    case ADC_INT_EOIC:
        /* enable end of inserted group conversion interrupt */
        ADC_CTL0 |= (uint32_t) ADC_CTL0_EOICIE;
        break;
    default:
        break;
    }
}

/*!
    \brief      disable ADC interrupt
    \param[in]  interrupt: ADC interrupt
                only one parameter can be selected which is shown as below:
      \arg        ADC_INT_WDE: analog watchdog interrupt
      \arg        ADC_INT_EOC: end of group conversion interrupt
      \arg        ADC_INT_EOIC: end of inserted group conversion interrupt
    \param[out] none
    \retval     none
*/
void adc_interrupt_disable(uint32_t interrupt)
{
    switch(interrupt) {
    /* select the interrupt source */
    case ADC_INT_WDE:
        ADC_CTL0 &= ~((uint32_t)ADC_CTL0_WDEIE);
        break;
    case ADC_INT_EOC:
        ADC_CTL0 &= ~((uint32_t)ADC_CTL0_EOCIE);
        break;
    case ADC_INT_EOIC:
        ADC_CTL0 &= ~((uint32_t)ADC_CTL0_EOICIE);
        break;
    default:
        break;
    }
}

/*!
    \brief      get ADC interrupt flag
    \param[in]  int_flag: ADC interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        ADC_INT_FLAG_WDE: analog watchdog interrupt flag
      \arg        ADC_INT_FLAG_EOC: end of group conversion interrupt flag
      \arg        ADC_INT_FLAG_EOIC: end of inserted group conversion interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_interrupt_flag_get(uint32_t int_flag)
{
    FlagStatus interrupt_flag = RESET;
    uint32_t state;
    /* check the interrupt flags */
    switch(int_flag) {
    case ADC_INT_FLAG_WDE:
        /* get the ADC analog watchdog interrupt flags */
        state = ADC_STAT & ADC_STAT_WDE;
        if((ADC_CTL0 & ADC_CTL0_WDEIE) && state) {
            interrupt_flag = SET;
        }
        break;
    case ADC_INT_FLAG_EOC:
        /* get the ADC end of group conversion interrupt flags */
        state = ADC_STAT & ADC_STAT_EOC;
        if((ADC_CTL0 & ADC_CTL0_EOCIE) && state) {
            interrupt_flag = SET;
        }
        break;
    case ADC_INT_FLAG_EOIC:
        /* get the ADC end of inserted group conversion interrupt flags */
        state = ADC_STAT & ADC_STAT_EOIC;
        if((ADC_CTL0 & ADC_CTL0_EOICIE) && state) {
            interrupt_flag = SET;
        }
        break;
    default:
        break;
    }
    return interrupt_flag;
}

/*!
    \brief      clear ADC interrupt flag
    \param[in]  int_flag: ADC interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        ADC_INT_FLAG_WDE: analog watchdog interrupt flag
      \arg        ADC_INT_FLAG_EOC: end of group conversion interrupt flag
      \arg        ADC_INT_FLAG_EOIC: end of inserted group conversion interrupt flag
    \param[out] none
    \retval     none
*/
void adc_interrupt_flag_clear(uint32_t int_flag)
{
    ADC_STAT &= ~((uint32_t)int_flag);
}
