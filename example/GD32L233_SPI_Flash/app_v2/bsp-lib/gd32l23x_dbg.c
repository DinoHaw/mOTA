/*!
    \file    gd32l23x_dbg.c
    \brief   DBG driver

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

#include "gd32l23x_dbg.h"

#define DBG_RESET_VAL       ((uint32_t)0x00000000U)   /*!< DBG reset value */

/*!
    \brief      deinitialize the DBG
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dbg_deinit(void)
{
    DBG_CTL0 = DBG_RESET_VAL;
    DBG_CTL1 = DBG_RESET_VAL;
}

/*!
    \brief      read DBG_ID code register
    \param[in]  none
    \param[out] none
    \retval     DBG_ID code
*/
uint32_t dbg_id_get(void)
{
    return DBG_ID;
}

/*!
    \brief      enable low power behavior when the MCU is in debug mode
    \param[in]  dbg_low_power:
                one or more parameters can be selected which are shown as below:
      \arg        DBG_LOW_POWER_SLEEP: keep debugger connection during sleep mode
      \arg        DBG_LOW_POWER_DEEPSLEEP: keep debugger connection during deepsleep mode
      \arg        DBG_LOW_POWER_STANDBY: keep debugger connection during standby mode
    \param[out] none
    \retval     none
*/
void dbg_low_power_enable(uint32_t dbg_low_power)
{
    DBG_CTL0 |= dbg_low_power;
}

/*!
    \brief      disable low power behavior when the MCU is in debug mode
    \param[in]  dbg_low_power:
                one or more parameters can be selected which are shown as below:
      \arg        DBG_LOW_POWER_SLEEP: keep debugger connection during sleep mode
      \arg        DBG_LOW_POWER_DEEPSLEEP: keep debugger connection during deepsleep mode
      \arg        DBG_LOW_POWER_STANDBY: keep debugger connection during standby mode
    \param[out] none
    \retval     none
*/
void dbg_low_power_disable(uint32_t dbg_low_power)
{
    DBG_CTL0 &= ~dbg_low_power;
}

/*!
    \brief      enable peripheral behavior when the MCU is in debug mode
    \param[in]  dbg_periph: DBG peripheral
                only one parameter can be selected which is shown as below:
      \arg        DBG_FWDGT_HOLD: hold FWDGT counter when core is halted
      \arg        DBG_WWDGT_HOLD: hold WWDGT counter when core is halted
      \arg        DBG_TIMER1_HOLD: hold TIMER1 counter when core is halted
      \arg        DBG_TIMER2_HOLD: hold TIMER2 counter when core is halted
      \arg        DBG_TIMER5_HOLD: hold TIMER5 counter when core is halted
      \arg        DBG_TIMER6_HOLD: hold TIMER6 counter when core is halted
      \arg        DBG_TIMER8_HOLD: hold TIMER8 counter when core is halted
      \arg        DBG_TIMER11_HOLD: hold TIMER11 counter when core is halted
      \arg        DBG_LPTIMER_HOLD: hold LPTIMER counter when core is halted
      \arg        DBG_I2C0_HOLD: hold I2C0 SMBUS when core is halted
      \arg        DBG_I2C1_HOLD: hold I2C1 SMBUS when core is halted
      \arg        DBG_I2C2_HOLD: hold I2C2 SMBUS when core is halted
      \arg        DBG_RTC_HOLD: hold RTC calendar and wakeup counter when core is halted
    \param[out] none
    \retval     none
*/
void dbg_periph_enable(dbg_periph_enum dbg_periph)
{
    DBG_REG_VAL(dbg_periph) |= BIT(DBG_BIT_POS(dbg_periph));
}

/*!
    \brief      disable peripheral behavior when the MCU is in debug mode
    \param[in]  dbg_periph: DBG peripheral
                only one parameter can be selected which is shown as below:
      \arg        DBG_FWDGT_HOLD: hold FWDGT counter when core is halted
      \arg        DBG_WWDGT_HOLD: hold WWDGT counter when core is halted
      \arg        DBG_TIMER1_HOLD: hold TIMER1 counter when core is halted
      \arg        DBG_TIMER2_HOLD: hold TIMER2 counter when core is halted
      \arg        DBG_TIMER5_HOLD: hold TIMER5 counter when core is halted
      \arg        DBG_TIMER6_HOLD: hold TIMER6 counter when core is halted
      \arg        DBG_TIMER8_HOLD: hold TIMER8 counter when core is halted
      \arg        DBG_TIMER11_HOLD: hold TIMER11 counter when core is halted
      \arg        DBG_LPTIMER_HOLD: hold LPTIMER counter when core is halted
      \arg        DBG_I2C0_HOLD: hold I2C0 SMBUS when core is halted
      \arg        DBG_I2C1_HOLD: hold I2C1 SMBUS when core is halted
      \arg        DBG_I2C2_HOLD: hold I2C2 SMBUS when core is halted
      \arg        DBG_RTC_HOLD: hold RTC calendar and wakeup counter when core is halted
    \param[out] none
    \retval     none
*/
void dbg_periph_disable(dbg_periph_enum dbg_periph)
{
    DBG_REG_VAL(dbg_periph) &= ~BIT(DBG_BIT_POS(dbg_periph));
}
