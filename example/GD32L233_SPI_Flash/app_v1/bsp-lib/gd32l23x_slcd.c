/*!
    \file    gd32l23x_slcd.c
    \brief   SLCD driver

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

#include "gd32l23x_slcd.h"

/*!
    \brief      reset SLCD register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_deinit(void)
{
    /* reset PMU */
    rcu_periph_reset_enable(RCU_SLCDRST);
    rcu_periph_reset_disable(RCU_SLCDRST);
}

/*!
    \brief      enable SLCD interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_enable(void)
{
    SLCD_CTL |= SLCD_CTL_SLCDON;
}

/*!
    \brief      disable SLCD interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_disable(void)
{
    SLCD_CTL &= ~SLCD_CTL_SLCDON;
}

/*!
    \brief      initialize SLCD interface
    \param[in]  prescaler: the SLCD prescaler
                only one parameters can be selected which are shown as below:
      \arg        SLCD_PRESCALER_x(x=1,2,4,8...32768)
    \param[in]  divider: the SLCD divider
                only one parameters can be selected which are shown as below:
      \arg        SLCD_DIVIDER_x(x=16,17,18,19...31)
    \param[in]  duty: the SLCD duty
                only one parameters can be selected which are shown as below:
    \arg        SLCD_DUTY_STATIC: static duty
    \arg        SLCD_DUTY_1_x(x=2,3,4,8,6): 1/x duty
    \param[in]  bias: the SLCD voltage bias
                only one parameters can be selected which are shown as below:
    \arg        SLCD_BIAS_1_x(x=2,3,4): 1/x voltage bias
    \param[out] none
    \retval     none
*/
void slcd_init(uint32_t prescaler, uint32_t divider, uint32_t duty, uint32_t bias)
{
    uint32_t reg;
    /* configure SLCD_CFG register */
    reg = SLCD_CFG;
    reg &= ~(SLCD_CFG_PSC | SLCD_CFG_DIV);
    reg |= prescaler | divider;
    SLCD_CFG = reg;
    /* configure SLCD_CTL register */
    reg = SLCD_CTL;
    reg &= ~(SLCD_CTL_BIAS | SLCD_CTL_DUTY);
    reg |= duty | bias;
    SLCD_CTL = reg;
}

/*!
    \brief      enable SLCD enhance mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_enhance_mode_enable(void)
{
    SLCD_CTL |= SLCD_CTL_VODEN;
}

/*!
    \brief      disable SLCD enhance mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_enhance_mode_disable(void)
{
    SLCD_CTL &= ~SLCD_CTL_VODEN;
}

/*!
    \brief      select SLCD bias voltage
    \param[in]  bias_voltage: the SLCD voltage bias
                only one parameter can be selected which is shown as below:
      \arg        SLCD_BIAS_1_4: 1/4 voltage bias
      \arg        SLCD_BIAS_1_2: 1/2 voltage bias
      \arg        SLCD_BIAS_1_3: 1/3 voltage bias
    \param[out] none
    \retval     none
*/
void slcd_bias_voltage_select(uint32_t bias_voltage)
{
    SLCD_CTL &= ~(SLCD_CTL_BIAS);
    SLCD_CTL |= bias_voltage;
}

/*!
    \brief      select SLCD duty
    \param[in]  duty: duty select
                only one parameter can be selected which is shown as below:
      \arg        SLCD_DUTY_STATIC: static duty
      \arg        SLCD_DUTY_1_2: 1/2 duty
      \arg        SLCD_DUTY_1_3: 1/3 duty
      \arg        SLCD_DUTY_1_4: 1/4 duty
      \arg        SLCD_DUTY_1_6: 1/6 duty
      \arg        SLCD_DUTY_1_8: 1/8 duty
    \param[out] none
    \retval     none
*/
void slcd_duty_select(uint32_t duty)
{
    SLCD_CTL &= ~(SLCD_CTL_DUTY);
    SLCD_CTL |= duty;
}

/*!
    \brief      configure SLCD input clock
                fSLCD = finclk/(pow(2, PRE)* DIV)
    \param[in]  prescaler: the prescaler factor
                only one parameter can be selected which is shown as below:
      \arg        SLCD_PRESCALER_1: PRE = 0
      \arg        SLCD_PRESCALER_2: PRE = 1
      \arg        SLCD_PRESCALER_4: PRE = 2
      \arg        SLCD_PRESCALER_8: PRE = 3
      \arg        SLCD_PRESCALER_16: PRE = 4
      \arg        SLCD_PRESCALER_32: PRE = 5
      \arg        SLCD_PRESCALER_64: PRE = 6
      \arg        SLCD_PRESCALER_128: PRE = 7
      \arg        SLCD_PRESCALER_256: PRE = 8
      \arg        SLCD_PRESCALER_512: PRE = 9
      \arg        SLCD_PRESCALER_1024: PRE = 10
      \arg        SLCD_PRESCALER_2048: PRE = 11
      \arg        SLCD_PRESCALER_4096: PRE = 12
      \arg        SLCD_PRESCALER_8192: PRE = 13
      \arg        SLCD_PRESCALER_16384: PRE = 14
      \arg        SLCD_PRESCALER_32768: PRE = 15
    \param[in]  divider: the divider factor
                only one parameter can be selected which is shown as below:
      \arg        SLCD_DIVIDER_x: x= 16..31, DIV = 16..31
    \param[out] none
    \retval     none
*/
void slcd_clock_config(uint32_t prescaler, uint32_t divider)
{
    uint32_t reg;

    /* config the prescaler and the divider */
    reg = SLCD_CFG;
    reg &= ~(SLCD_CFG_PSC | SLCD_CFG_DIV);
    reg |= (prescaler | divider);
    SLCD_CFG = reg;
}

/*!
    \brief      configure SLCD blink mode
    \param[in]  mode: blink mode
                only one parameter can be selected which is shown as below:
      \arg        SLCD_BLINKMODE_OFF: blink disabled
      \arg        SLCD_BLINKMODE_SEG0_COM0: blink enabled on SEG[0], COM[0]
      \arg        SLCD_BLINKMODE_SEG0_ALLCOM: blink enabled on SEG[0], all COM
      \arg        SLCD_BLINKMODE_ALLSEG_ALLCOM: blink enabled on all SEG and all COM
    \param[in]  blink_divider: the divider factor
                only one parameter can be selected which is shown as below:
      \arg        SLCD_BLINK_FREQUENCY_DIV8: blink frequency = fSLCD/8
      \arg        SLCD_BLINK_FREQUENCY_DIV16: blink frequency = fSLCD/16
      \arg        SLCD_BLINK_FREQUENCY_DIV32: blink frequency = fSLCD/32
      \arg        SLCD_BLINK_FREQUENCY_DIV64: blink frequency = fSLCD/64
      \arg        SLCD_BLINK_FREQUENCY_DIV128: blink frequency = fSLCD/128
      \arg        SLCD_BLINK_FREQUENCY_DIV256: blink frequency = fSLCD/256
      \arg        SLCD_BLINK_FREQUENCY_DIV512: blink frequency = fSLCD/512
      \arg        SLCD_BLINK_FREQUENCY_DIV1024: blink frequency = fSLCD/1024
    \param[out] none
    \retval     none
*/
void slcd_blink_mode_config(uint32_t mode, uint32_t blink_divider)
{
    uint32_t reg;
    reg = SLCD_CFG;
    reg &= ~(SLCD_CFG_BLKMOD | SLCD_CFG_BLKDIV);
    reg |= mode | blink_divider;
    SLCD_CFG = reg;
}

/*!
    \brief      configure slcd_contrast_ratio
    \param[in]  contrast: the slcd contrast
                only one parameters can be selected which are shown as below:
      \arg        SLCD_CONTRAST_LEVEL0:Maximum Voltage = 2.65V
      \arg        SLCD_CONTRAST_LEVEL1:Maximum Voltage = 2.80V
      \arg        SLCD_CONTRAST_LEVEL2:Maximum Voltage = 2.92V
      \arg        SLCD_CONTRAST_LEVEL3:Maximum Voltage = 3.08V
      \arg        SLCD_CONTRAST_LEVEL4:Maximum Voltage = 3.23V
      \arg        SLCD_CONTRAST_LEVEL5:Maximum Voltage = 3.37V
      \arg        SLCD_CONTRAST_LEVEL6:Maximum Voltage = 3.52V
      \arg        SLCD_CONTRAST_LEVEL7:Maximum Voltage = 3.67V
    \param[out] none
    \retval     none
*/
void slcd_contrast_ratio_config(uint32_t contrast_ratio)
{
    uint32_t reg;
    reg = SLCD_CFG;
    reg &= ~SLCD_CFG_CONR;
    reg |= contrast_ratio;
    SLCD_CFG = reg;
}

/*!
    \brief      configure SLCD dead time duration
    \param[in]  dead_time: configure the length of the dead time between frames
                only one parameter can be selected which is shown as below:
      \arg        SLCD_DEADTIME_PERIOD_0: no dead time
      \arg        SLCD_DEADTIME_PERIOD_1: 1 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_2: 2 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_3: 3 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_4: 4 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_5: 5 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_6: 6 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_7: 7 phase inserted between couple of frame
    \param[out] none
    \retval     none
*/
void slcd_dead_time_config(uint32_t dead_time)
{
    uint32_t reg;

    /* config dead time duration */
    reg = SLCD_CFG;
    reg &= ~(SLCD_CFG_DTD);
    reg |= dead_time;
    SLCD_CFG = reg;
}

/*!
    \brief      configure SLCD pulse on duration
    \param[in]  pulseonduration: specifies the slcd pulse on duration
                only one parameters can be selected which are shown as below:
      \arg        SLCD_PULSEON_DURATION_0: pulse on duration = 0
      \arg        SLCD_PULSEON_DURATION_1: pulse on duration = 1*1/fPRE
      \arg        SLCD_PULSEON_DURATION_2: pulse on duration = 2*1/fPRE
      \arg        SLCD_PULSEON_DURATION_3: pulse on duration = 3*1/fPRE
      \arg        SLCD_PULSEON_DURATION_4: pulse on duration = 4*1/fPRE
      \arg        SLCD_PULSEON_DURATION_5: pulse on duration = 5*1/fPRE
      \arg        SLCD_PULSEON_DURATION_6: pulse on duration = 6*1/fPRE
      \arg        SLCD_PULSEON_DURATION_7: pulse on duration = 7*1/fPRE
    \param[out] none
    \retval     none
*/
void slcd_pulse_on_duration_config(uint32_t duration)
{
    uint32_t reg;
    reg = SLCD_CFG;
    reg &= ~SLCD_CFG_PULSE;
    reg |= duration;
    SLCD_CFG = reg;
}

/*!
    \brief      select SLCD common/segment pad
    \param[in]  NewValue: ENABLE or DISABLE
                only one parameter can be selected which is shown as below:
      \arg        ENABLE: LCD_COM[7:4] pad select LCD_SEG[31:28]
      \arg        DISABLE: LCD_COM[7:4] pad select LCD_COM[7:4]
    \param[out] none
    \retval     none
*/
void slcd_com_seg_remap(ControlStatus newvalue)
{
    if(ENABLE == newvalue) {
        SLCD_CTL |= SLCD_CTL_COMS;
    } else {
        SLCD_CTL &= ~(SLCD_CTL_COMS);
    }
}

/*!
    \brief      select SLCD voltage source
    \param[in]  vsrc: specifies the slcd voltage source
                only one parameters can be selected which are shown as below:
      \arg        SLCD_VOLTAGE_INTERNAL
      \arg        SLCD_VOLTAGE_EXTERNAL
    \param[out] none
    \retval     none
*/
void slcd_voltage_source_select(uint8_t voltage_source)
{
    if(SLCD_VOLTAGE_INTERNAL == voltage_source) {
        SLCD_CTL &= ~SLCD_CTL_VSRC;
    } else {
        SLCD_CTL |= SLCD_CTL_VSRC;
    }
}

/*!
    \brief      enable or disable permanent high drive
    \param[in]  newvalue: ENABLE or DISABLE
                only one parameter can be selected which is shown as below:
    \arg          ENBALE: enable permanent high drive
    \arg          DISBALE: disable permanent high drive
    \param[out] none
    \retval     none
*/
void slcd_high_drive_config(ControlStatus newvalue)
{
    if(ENABLE == newvalue) {
        SLCD_CFG |= SLCD_CFG_HDEN;
    } else {
        SLCD_CFG &= ~(SLCD_CFG_HDEN);
    }
}

/*!
    \brief      write SLCD data register
    \param[in]  register_number: refer to slcd_data_register_enum
                only one parameter can be selected which is shown as below:
      \arg        SLCD_DATA_REG0: SLCD_DATA register 0
      \arg        SLCD_DATA_REG1: SLCD_DATA Register 1
      \arg        SLCD_DATA_REG2: SLCD_DATA register 2
      \arg        SLCD_DATA_REG3: SLCD_DATA Register 3
      \arg        SLCD_DATA_REG4: SLCD_DATA register 4
      \arg        SLCD_DATA_REG5: SLCD_DATA Register 5
      \arg        SLCD_DATA_REG6: SLCD_DATA register 6
      \arg        SLCD_DATA_REG7: SLCD_DATA Register 7
    \param[in]  data: the data write to the register
    \param[out] none
    \retval     none
*/
void slcd_data_register_write(slcd_data_register_enum register_number, uint32_t data)
{
    /* wtite data word to DATA register */
    SLCD_DATA0_7((uint32_t)register_number) = data;
}

/*!
    \brief      update SLCD data request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_data_update_request(void)
{
    SLCD_STAT |= SLCD_STAT_UPRF;
}

/*!
    \brief      get SLCD flags
    \param[in]  flag: the slcd flags
                only one parameter can be selected which is shown as below:
      \arg        SLCD_FLAG_ON: controller on flag
      \arg        SLCD_FLAG_SO: start of frame flag
      \arg        SLCD_FLAG_UPR: update data request flag
      \arg        SLCD_FLAG_UPD: update data done flag
      \arg        SLCD_FLAG_VRDY: voltage ready flag
      \arg        SLCD_FLAG_SYN: SLCD_CFG register synchronization flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus slcd_flag_get(uint32_t flag)
{
    if(RESET != (SLCD_STAT & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear SLCD flags
    \param[in]  flag: the slcd flags
                one or more parameters can be selected which is shown as below:
      \arg        SLCD_FLAG_SOF: start of frame flag
      \arg        SLCD_FLAG_UPDF: update done flag
    \param[out] none
    \retval     none
*/
void slcd_flag_clear(uint32_t flag)
{
    SLCD_STATC = flag;
}

/*!
    \brief      enable SLCD interrupt
    \param[in]  interrupt: slcd interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        SLCD_INT_SOF: start of frame interrupt
      \arg        SLCD_INT_UPD: update done interrupt
    \param[out] none
    \retval     none
*/
void slcd_interrupt_enable(uint32_t interrupt)
{
    SLCD_CFG |= interrupt;
}

/*!
    \brief      disable SLCD interrupt
    \param[in]  interrupt: slcd interrupt source
                one or more parameters can be selected which is shown as below:
      \arg        SLCD_INT_SOF: start of frame interrupt
      \arg        SLCD_INT_UPD: update done interrupt
    \param[out] none
    \retval     none
*/
void slcd_interrupt_disable(uint32_t interrupt)
{
    SLCD_CFG &= ~interrupt;
}

/*!
    \brief      get SLCD interrupt flags
    \param[in]  int_flag: the slcd interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        SLCD_INT_FLAG_SOF: start of frame flag
      \arg        SLCD_INT_FLAG_UPD: update data done flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus slcd_interrupt_flag_get(uint32_t int_flag)
{
    uint32_t val;
    val = (SLCD_CFG & int_flag);
    if((RESET != (SLCD_STAT & int_flag)) && (RESET != val)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear SLCD interrupt flag
    \param[in]  int_flag: the slcd interrupt flag
                one or more parameters can be selected which is shown as below:
      \arg        SLCD_INT_FLAG_SOF: start of frame flag
      \arg        SLCD_INT_FLAG_UPD: update data done flag
    \param[out] none
    \retval     none
*/
void slcd_interrupt_flag_clear(uint32_t int_flag)
{
    SLCD_STATC = int_flag;
}
