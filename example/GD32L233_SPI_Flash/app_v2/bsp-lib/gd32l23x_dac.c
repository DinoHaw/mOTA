/*!
    \file    gd32l23x_dac.c
    \brief   DAC driver

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

#include "gd32l23x_dac.h"

/* DAC register bit offset */
#define OUT1_REG_OFFSET           ((uint32_t)0x00000010U)
#define DH_12BIT_OFFSET           ((uint32_t)0x00000010U)
#define DH_8BIT_OFFSET            ((uint32_t)0x00000008U)

/*!
    \brief      deinitialize DAC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_deinit(void)
{
    rcu_periph_reset_enable(RCU_DACRST);
    rcu_periph_reset_disable(RCU_DACRST);
}

/*!
    \brief      enable DAC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_enable(void)
{
    DAC_CTL0 |= DAC_CTL0_DEN;
}

/*!
    \brief      disable DAC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_disable(void)
{
    DAC_CTL0 &= ~DAC_CTL0_DEN;
}

/*!
    \brief      enable DAC DMA function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_dma_enable(void)
{
    DAC_CTL0 |= DAC_CTL0_DDMAEN;
}

/*!
    \brief      disable DAC DMA function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_dma_disable(void)
{
    DAC_CTL0 &= ~DAC_CTL0_DDMAEN;
}

/*!
    \brief      enable DAC output buffer
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_output_buffer_enable(void)
{
    DAC_CTL0 &= ~DAC_CTL0_DBOFF;
}

/*!
    \brief      disable DAC output buffer
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_output_buffer_disable(void)
{
    DAC_CTL0 |= DAC_CTL0_DBOFF;
}

/*!
    \brief      get DAC output value
    \param[in]  none
    \param[out] none
    \retval     DAC output data: 0~4095
*/
uint16_t dac_output_value_get(void)
{
    uint16_t data = 0U;
    data = (uint16_t)OUT_DO;

    return data;
}

/*!
    \brief      set DAC data holding register value
    \param[in]  dac_align: DAC data alignment mode
                only one parameter can be selected which is shown as below:
      \arg        DAC_ALIGN_12B_R: 12-bit right-aligned data
      \arg        DAC_ALIGN_12B_L: 12-bit left-aligned data
      \arg        DAC_ALIGN_8B_R: 8-bit right-aligned data
    \param[in]  data: data to be loaded, 0~4095
    \param[out] none
    \retval     none
*/
void dac_data_set(uint32_t dac_align, uint16_t data)
{
    switch(dac_align) {
    /* 12-bit right-aligned data */
    case DAC_ALIGN_12B_R:
        OUT_R12DH = data;
        break;
    /* 12-bit left-aligned data */
    case DAC_ALIGN_12B_L:
        OUT_L12DH = data;
        break;
    /* 8-bit right-aligned data */
    case DAC_ALIGN_8B_R:
        OUT_R8DH = data;
        break;
    default:
        break;
    }
}

/*!
    \brief      enable DAC trigger
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_trigger_enable(void)
{
    DAC_CTL0 |= DAC_CTL0_DTEN;
}

/*!
    \brief      disable DAC trigger
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_trigger_disable(void)
{
    DAC_CTL0 &= ~DAC_CTL0_DTEN;
}

/*!
    \brief      configure DAC trigger source
    \param[in]  triggersource: external triggers of DAC
                only one parameter can be selected which is shown as below:
      \arg        DAC_TRIGGER_T1_TRGO: TIMER1 TRGO
      \arg        DAC_TRIGGER_T2_TRGO: TIMER2 TRGO
      \arg        DAC_TRIGGER_T6_TRGO: TIMER6 TRGO
      \arg        DAC_TRIGGER_T5_TRGO: TIMER5 TRGO
      \arg        DAC_TRIGGER_EXTI_9: EXTI interrupt line9 event
      \arg        DAC_TRIGGER_SOFTWARE: software trigger
    \param[out] none
    \retval     none
*/
void dac_trigger_source_config(uint32_t triggersource)
{
    /* configure DAC trigger source */
    DAC_CTL0 &= (uint32_t)(~DAC_CTL0_DTSEL);
    DAC_CTL0 |= triggersource;
}

/*!
    \brief      enable DAC software trigger
    \param[in]  none
    \retval     none
*/
void dac_software_trigger_enable(void)
{
    DAC_SWT |= DAC_SWT_SWTR;
}

/*!
    \brief      disable DAC software trigger
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_software_trigger_disable(void)
{
    DAC_SWT &= ~DAC_SWT_SWTR;
}

/*!
    \brief      configure DAC wave mode
    \param[in]  wave_mode: DAC wave mode
                only one parameter can be selected which is shown as below:
      \arg        DAC_WAVE_DISABLE: wave mode disable
      \arg        DAC_WAVE_MODE_LFSR: LFSR noise mode
      \arg        DAC_WAVE_MODE_TRIANGLE: triangle noise mode
    \param[out] none
    \retval     none
*/
void dac_wave_mode_config(uint32_t wave_mode)
{
    /* configure DAC wave mode */
    DAC_CTL0 &= ~DAC_CTL0_DWM;
    DAC_CTL0 |= wave_mode;
}

/*!
    \brief      configure DAC wave bit width
    \param[in]  bit_width: DAC noise wave bit width
                only one parameter can be selected which is shown as below:
      \arg        DAC_WAVE_BIT_WIDTH_1: bit width of the wave signal is 1
      \arg        DAC_WAVE_BIT_WIDTH_2: bit width of the wave signal is 2
      \arg        DAC_WAVE_BIT_WIDTH_3: bit width of the wave signal is 3
      \arg        DAC_WAVE_BIT_WIDTH_4: bit width of the wave signal is 4
      \arg        DAC_WAVE_BIT_WIDTH_5: bit width of the wave signal is 5
      \arg        DAC_WAVE_BIT_WIDTH_6: bit width of the wave signal is 6
      \arg        DAC_WAVE_BIT_WIDTH_7: bit width of the wave signal is 7
      \arg        DAC_WAVE_BIT_WIDTH_8: bit width of the wave signal is 8
      \arg        DAC_WAVE_BIT_WIDTH_9: bit width of the wave signal is 9
      \arg        DAC_WAVE_BIT_WIDTH_10: bit width of the wave signal is 10
      \arg        DAC_WAVE_BIT_WIDTH_11: bit width of the wave signal is 11
      \arg        DAC_WAVE_BIT_WIDTH_12: bit width of the wave signal is 12
    \param[out] none
    \retval     none
*/
void dac_wave_bit_width_config(uint32_t bit_width)
{
    /* configure DAC wave bit width */
    DAC_CTL0 &= ~DAC_CTL0_DWBW;
    DAC_CTL0 |= bit_width;
}

/*!
    \brief      configure DAC LFSR noise mode
    \param[in]  unmask_bits: LFSR noise unmask bits
                only one parameter can be selected which is shown as below:
      \arg        DAC_LFSR_BIT0: unmask the LFSR bit0
      \arg        DAC_LFSR_BITS1_0: unmask the LFSR bits[1:0]
      \arg        DAC_LFSR_BITS2_0: unmask the LFSR bits[2:0]
      \arg        DAC_LFSR_BITS3_0: unmask the LFSR bits[3:0]
      \arg        DAC_LFSR_BITS4_0: unmask the LFSR bits[4:0]
      \arg        DAC_LFSR_BITS5_0: unmask the LFSR bits[5:0]
      \arg        DAC_LFSR_BITS6_0: unmask the LFSR bits[6:0]
      \arg        DAC_LFSR_BITS7_0: unmask the LFSR bits[7:0]
      \arg        DAC_LFSR_BITS8_0: unmask the LFSR bits[8:0]
      \arg        DAC_LFSR_BITS9_0: unmask the LFSR bits[9:0]
      \arg        DAC_LFSR_BITS10_0: unmask the LFSR bits[10:0]
      \arg        DAC_LFSR_BITS11_0: unmask the LFSR bits[11:0]
    \param[out] none
    \retval     none
*/
void dac_lfsr_noise_config(uint32_t unmask_bits)
{
    /* configure DAC LFSR noise mode */
    DAC_CTL0 &= ~DAC_CTL0_DWBW;
    DAC_CTL0 |= unmask_bits;
}

/*!
    \brief      configure DAC triangle noise mode
    \param[in]  amplitude: the amplitude of the triangle
                only one parameter can be selected which is shown as below:
      \arg        DAC_TRIANGLE_AMPLITUDE_1: triangle amplitude is 1
      \arg        DAC_TRIANGLE_AMPLITUDE_3: triangle amplitude is 3
      \arg        DAC_TRIANGLE_AMPLITUDE_7: triangle amplitude is 7
      \arg        DAC_TRIANGLE_AMPLITUDE_15: triangle amplitude is 15
      \arg        DAC_TRIANGLE_AMPLITUDE_31: triangle amplitude is 31
      \arg        DAC_TRIANGLE_AMPLITUDE_63: triangle amplitude is 63
      \arg        DAC_TRIANGLE_AMPLITUDE_127: triangle amplitude is 127
      \arg        DAC_TRIANGLE_AMPLITUDE_255: triangle amplitude is 255
      \arg        DAC_TRIANGLE_AMPLITUDE_511: triangle amplitude is 511
      \arg        DAC_TRIANGLE_AMPLITUDE_1023: triangle amplitude is 1023
      \arg        DAC_TRIANGLE_AMPLITUDE_2047: triangle amplitude is 2047
      \arg        DAC_TRIANGLE_AMPLITUDE_4095: triangle amplitude is 4095
    \param[out] none
    \retval     none
*/
void dac_triangle_noise_config(uint32_t amplitude)
{
    /* configure DAC triangle noise mode */
    DAC_CTL0 &= ~DAC_CTL0_DWBW;
    DAC_CTL0 |= amplitude;
}

/*!
    \brief      get DAC flag
    \param[in]  dac_flag: DAC flag
                only one parameter can be selected which is shown as below:
      \arg        DAC_FLAG_DDUDR: DMA underrun flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dac_flag_get(uint32_t flag)
{
    /* check DAC flag */
    if(DAC_FLAG_DDUDR == flag) {
        if(RESET != (DAC_STAT0 & DAC_STAT0_DDUDR)) {
            return SET;
        }
    }
    return RESET;
}

/*!
    \brief      clear DAC flag
    \param[in]  flag: DAC flag
                only one parameter can be selected which is shown as below:
      \arg        DAC_FLAG_DDUDR: DMA underrun flag
    \param[out] none
    \retval     none
*/
void dac_flag_clear(uint32_t flag)
{
    /* clear DAC_OUT flag */
    if(DAC_FLAG_DDUDR == flag) {
        DAC_STAT0 |= (uint32_t)DAC_STAT0_DDUDR;
    }
}

/*!
    \brief      enable DAC interrupt
    \param[in]  interrupt: the DAC interrupt
                only one parameter can be selected which is shown as below:
      \arg        DAC_INT_DDUDRIE: DMA underrun interrupt enable
    \param[out] none
    \retval     none
*/
void dac_interrupt_enable(uint32_t interrupt)
{
    /* enable DAC interrupt */
    if(DAC_INT_DDUDRIE == interrupt) {
        DAC_CTL0 |= (uint32_t)DAC_CTL0_DDUDRIE;
    }
}

/*!
    \brief      disable DAC interrupt
    \param[in]  interrupt: the DAC interrupt
                only one parameter can be selected which is shown as below:
      \arg        DAC_INT_DDUDRIE: DMA underrun interrupt disable
    \param[out] none
    \retval     none
*/
void dac_interrupt_disable(uint32_t interrupt)
{
    /* disable DAC interrupt */
    if(DAC_INT_DDUDRIE == interrupt) {
        DAC_CTL0 &= (uint32_t)(~DAC_CTL0_DDUDRIE);
    }
}

/*!
    \brief      get DAC interrupt flag
    \param[in]  int_flag: DAC interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        DAC_INT_FLAG_DDUDR: DMA underrun interrupt flag
    \param[out] none
    \retval     the state of DAC interrupt flag(SET or RESET)
*/
FlagStatus dac_interrupt_flag_get(uint32_t int_flag)
{
    uint32_t reg1 = 0U, reg2 = 0U;

    /* check DAC interrupt flag */
    if(DAC_INT_FLAG_DDUDR == int_flag) {
        reg1 = DAC_STAT0 & DAC_STAT0_DDUDR;
        reg2 = DAC_CTL0 & DAC_CTL0_DDUDRIE;
    }

    /*get DAC interrupt flag status */
    if((RESET != reg1) && (RESET != reg2)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear DAC interrupt flag
    \param[in]  int_flag: DAC interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        DAC_INT_FLAG_DDUDR: DMA underrun interrupt flag
    \param[out] none
    \retval     none
*/
void dac_interrupt_flag_clear(uint32_t int_flag)
{
    /* clear DAC interrupt flag */
    if(DAC_INT_FLAG_DDUDR == int_flag) {
        DAC_STAT0 |= (uint32_t)DAC_STAT0_DDUDR;
    }
}
