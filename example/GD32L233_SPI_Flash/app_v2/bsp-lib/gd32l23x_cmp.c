/*!
    \file    gd32l23x_cmp.c
    \brief   CMP driver

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

#include "gd32l23x_cmp.h"

#define CMP_MODE_DEFAULT                         ((uint32_t)0xFFFFCF83)         /*!< cmp mode default */
#define CMP_OUTPUT_DEFAULT                       ((uint32_t)0xFFFFF0FF)         /*!< cmp output default */
/*!
    \brief      deinitialize comparator
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_deinit(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP0_CS &= ~(uint32_t)CMP0_CS_EN;
    }else{
        CMP1_CS &= ~(uint32_t)CMP1_CS_EN;
    }
}

/*!
    \brief      initialize comparator mode
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[in]  operating_mode
      \arg        CMP_HIGHSPEED: high speed mode
      \arg        CMP_MEDIUMSPEED: medium speed mode
      \arg        CMP_LOWSPEED: low speed mode
    \param[in]  inverting_input
      \arg        CMP_1_4VREFINT: VREFINT *1/4 input
      \arg        CMP_1_2VREFINT: VREFINT *1/2 input
      \arg        CMP_3_4VREFINT: VREFINT *3/4 input
      \arg        CMP_VREFINT: VREFINT input
      \arg        CMP_PA0_PA2: PA0 input when selecting CMP0, PA2 input when selecting CMP1
      \arg        CMP_DACOUT_PA4: DAC_OUT(PA4) input
      \arg        CMP_PB3: PB3 input when selecting CMP1
    \param[in]  output_hysteresis
      \arg        CMP_HYSTERESIS_NO: output no hysteresis
      \arg        CMP_HYSTERESIS_LOW: output low hysteresis
      \arg        CMP_HYSTERESIS_MEDIUM: output medium hysteresis
      \arg        CMP_HYSTERESIS_HIGH: output high hysteresis
    \param[out] none
    \retval     none
*/
void cmp_mode_init(uint32_t cmp_periph, operating_mode_enum operating_mode, inverting_input_enum inverting_input, cmp_hysteresis_enum output_hysteresis)
{
    uint32_t CMPx_CS = 0x00000000U;
    if(CMP0 == cmp_periph){
        /* initialize comparator 0 mode */
        CMPx_CS = CMP0_CS;
        CMPx_CS &= ~(uint32_t)(CMP0_CS_PM | CMP0_CS_MSEL | CMP0_CS_HST);
        CMPx_CS |= (uint32_t)(CS_CMP0PM(operating_mode) | CS_CMP0MSEL(inverting_input) | CS_CMP0HST(output_hysteresis));
        CMP0_CS = CMPx_CS;
    }else if(CMP1 == cmp_periph){
        /* initialize comparator 1 mode */
        CMPx_CS = CMP1_CS;
        CMPx_CS &= ~(uint32_t)(CMP1_CS_PM | CMP1_CS_MSEL | CMP1_CS_HST);
        CMPx_CS |= (uint32_t)(CS_CMP1PM(operating_mode) | CS_CMP1MSEL(inverting_input) | CS_CMP1HST(output_hysteresis));
        CMP1_CS = CMPx_CS;
    }else{
    }
}

/*!
    \brief      Selecte the plus input for CMP1
    \param[in]  plus_input
      \arg        CMP1_PA3: selecte PA3 as plus input for CMP1
      \arg        CMP1_PB4: selecte PB4 as plus input for CMP1
      \arg        CMP1_PB5: selecte PB5 as plus input for CMP1
      \arg        CMP1_PB6: selecte PB6 as plus input for CMP1
      \arg        CMP1_PB7: selecte PB7 as plus input for CMP1
    \param[out] none
    \retval     none
*/
void cmp1_plus_selection(CMP1_plus_input_enum plus_input)
{
    uint32_t CMPx_CS = 0x00000000U;
    CMPx_CS = CMP1_CS;
    CMPx_CS &= ~(uint32_t)(CMP1_CS_PSEL);
    CMPx_CS |= CS_CMP1PSEL(plus_input);
    CMP1_CS = CMPx_CS;
}

/*!
    \brief      initialize comparator output
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
      \param[in]  output_selection
      \arg        CMP_OUTPUT_NONE: output no selection
      \arg        CMP_OUTPUT_TIMER1IC3: TIMER 1 channel3 input capture
      \arg        CMP_OUTPUT_TIMER2IC0: TIMER 2 channel0 input capture
      \arg        CMP_OUTPUT_LPTIMERIC0_IC1: LPTIMER channel0 or channel1 input capture
    \param[in]  output_polarity
      \arg        CMP_OUTPUT_POLARITY_INVERTED: output is inverted
      \arg        CMP_OUTPUT_POLARITY_NOINVERTED: output is not inverted
    \param[out] none
    \retval     none
*/
void cmp_output_init(uint32_t cmp_periph, cmp_output_enum output_selection, cmp_output_inv_enum output_polarity)
{
    uint32_t CMPx_CS = 0x00000000U;
    if(CMP0 == cmp_periph){
        /* initialize comparator 0 output */
        CMPx_CS = CMP0_CS;
        CMPx_CS &= ~(uint32_t)CMP0_CS_OSEL;
        CMPx_CS |= (uint32_t)CS_CMP0OSEL(output_selection);
        /* output polarity */
        if(CMP_OUTPUT_POLARITY_INVERTED == output_polarity){
            CMPx_CS |= (uint32_t)CMP0_CS_PL;
        }else{
            CMPx_CS &= ~(uint32_t)CMP0_CS_PL;
        }
        CMP0_CS = CMPx_CS;
    }else if(CMP1 == cmp_periph){
        /* initialize comparator 1 output */
        CMPx_CS = CMP1_CS;
        CMPx_CS &= ~(uint32_t)CMP1_CS_OSEL;
        CMPx_CS |= (uint32_t)CS_CMP1OSEL(output_selection);
        /* output polarity */
        if(CMP_OUTPUT_POLARITY_INVERTED == output_polarity){
            CMPx_CS |= (uint32_t)CMP1_CS_PL;
        }else{
            CMPx_CS &= ~(uint32_t)CMP1_CS_PL;
        }
        CMP1_CS = CMPx_CS;
    } else {
    }
}

/*!
    \brief      initialize comparator blanking function
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[in]  blanking_source_selection
      \arg        CMP_BLANKING_NONE: output no selection
      \arg        CMP_BLANKING_TIMER1_OC1: TIMER 1 output channel 1
      \arg        CMP_BLANKING_TIMER2_OC1: TIMER 2 output channel 1
      \arg        CMP_BLANKING_TIMER8_OC1: TIMER 8 output channel 1
      \arg        CMP_BLANKING_TIMER11_OC1: TIMER 11 output channel 1
    \param[out] none
    \retval     none
*/
void cmp_blanking_init(uint32_t cmp_periph, blanking_source_enum blanking_source_selection)
{
    uint32_t CMPx_CS = 0x00000000U;
    if(CMP0 == cmp_periph){
        CMPx_CS = CMP0_CS;
        CMP0_CS |= (uint32_t)CS_CMP0BLK(blanking_source_selection);
        CMP0_CS = CMPx_CS;
    }else if(CMP1 == cmp_periph){
        CMPx_CS = CMP1_CS;
        CMP1_CS |= (uint32_t)CS_CMP1BLK(blanking_source_selection);
        CMP1_CS = CMPx_CS;
    }else{
    }
}

/*!
    \brief      enable comparator
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_enable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP0_CS |= (uint32_t)CMP0_CS_EN;
    }else{
        CMP1_CS |= (uint32_t)CMP1_CS_EN;
    }
}

/*!
    \brief      disable comparator
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_disable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP0_CS &= ~(uint32_t)CMP0_CS_EN;
    }else{
        CMP1_CS &= ~(uint32_t)CMP1_CS_EN;
    }
}

/*!
    \brief      enable the window mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_window_enable(void)
{
    CMP1_CS |= (uint32_t)CS_CMP1WEN_ENABLE;
}

/*!
    \brief      disable the window mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_window_disable(void)
{
    CMP1_CS &= ~(uint32_t)CS_CMP1WEN_ENABLE;
}

/*!
    \brief      enable the voltage scaler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_voltage_scaler_enable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP0_CS |= (uint32_t)CMP0_CS_SEN;
    }else{
        CMP1_CS |= (uint32_t)CMP1_CS_SEN;
    }
}
/*!
    \brief      disable the voltage scaler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_voltage_scaler_disable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP0_CS &= ~(uint32_t)CMP0_CS_SEN;
    }else{
        CMP1_CS &= ~(uint32_t)CMP1_CS_SEN;
    }
}
/*!
    \brief      enable the scaler bridge
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_scaler_bridge_enable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP0_CS |= (uint32_t)CMP0_CS_BEN;
    }else{
        CMP1_CS |= (uint32_t)CMP1_CS_BEN;
    }
}
/*!
    \brief      disable the scaler bridge
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_scaler_bridge_disable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP0_CS &= ~(uint32_t)CMP0_CS_BEN;
    }else{
        CMP1_CS &= ~(uint32_t)CMP1_CS_BEN;
    }
}

/*!
    \brief      lock the comparator
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_lock_enable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        /* lock CMP0 */
        CMP0_CS |= (uint32_t)CMP0_CS_LK;
    }else{
        /* lock CMP1 */
        CMP1_CS |= (uint32_t)CMP1_CS_LK;
    }
}

/*!
    \brief      get output level
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     the output level
*/
uint32_t cmp_output_level_get(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        /* get output level of CMP0 */
        if(CMP0_CS & CMP0_CS_OUT){
            return CMP_OUTPUTLEVEL_HIGH;
        }else{
            return CMP_OUTPUTLEVEL_LOW;
        }
    }else{
        /* get output level of CMP1 */
        if(CMP1_CS & CMP1_CS_OUT){
            return CMP_OUTPUTLEVEL_HIGH;
        }else{
            return CMP_OUTPUTLEVEL_LOW;
        }
    }
}
