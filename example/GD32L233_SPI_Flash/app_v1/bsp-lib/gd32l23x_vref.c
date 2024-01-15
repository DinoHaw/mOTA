/*!
    \file    gd32l23x_vref.c
    \brief   VREF driver

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

#include "gd32l23x_vref.h"

/*!
    \brief      deinitialize the VREF
    \param[in]  none
    \param[out] none
    \retval     none
*/
void vref_deinit(void)
{
    /* reset the value of all the VREF registers */
    VREF_CS = (uint32_t)0x00000002U;
}

/*!
    \brief      enable VREF
    \param[in]  none
    \param[out] none
    \retval     none
*/
void vref_enable(void)
{
    VREF_CS |= VREF_EN;
}

/*!
    \brief      disable VREF
    \param[in]  none
    \param[out] none
    \retval     none
*/
void vref_disable(void)
{
    VREF_CS &= (uint32_t)~VREF_EN;
}

/*!
    \brief      enable VREF high impendance mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void vref_high_impedance_mode_enable(void)
{
    VREF_CS |= VREF_HIGH_IMPEDANCE_MODE;
}

/*!
    \brief      disable VREF high impendance mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void vref_high_impedance_mode_disable(void)
{
    VREF_CS &= (uint32_t)~VREF_HIGH_IMPEDANCE_MODE;
}

/*!
    \brief      get the status of VREF
    \param[in]  none
    \param[out] none
    \retval     the status of VREF output
      \arg        SET: the VREF output is ready
      \arg        RESET: the VREF output is not ready
*/
FlagStatus vref_status_get(void)
{
    uint32_t temp_status = VREF_CS;
    temp_status = temp_status >> 3U;
    if((temp_status & 0x01U) != 0x00U) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      set the calibration value of VREF
    \param[in]  value: calibration value (0x00 - 0x3F)
    \param[out] none
    \retval     none
*/
void vref_calib_value_set(uint8_t value)
{
    VREF_CALIB |= (uint32_t)value;
}

/*!
    \brief      get the calibration value of VREF
    \param[in]  none
    \param[out] none
    \retval     calibration value (0x00 - 0x3F)
*/
uint8_t vref_calib_value_get(void)
{
    uint8_t temp = (uint8_t)VREF_CALIB;
    return temp;
}
