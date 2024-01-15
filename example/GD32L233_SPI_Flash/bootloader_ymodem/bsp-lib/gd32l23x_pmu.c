/*!
    \file    gd32l23x_pmu.c
    \brief   PMU driver

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

#include "gd32l23x_pmu.h"

/* PMU register bit offset */
#define PAR_TWK_CORE1_OFFSET               ((uint32_t)0x00000015U)               /*!< bit offset of TWK_CORE1 in PMU_PAR */
#define PAR_TSW_IRC16MCNT_OFFSET           ((uint32_t)0x00000010U)               /*!< bit offset of TSW_IRC16MCNT in PMU_PAR */
#define PAR_TWK_SRAM1_OFFSET               ((uint32_t)0x00000008U)               /*!< bit offset of TWK_SRAM1 in PMU_PAR */

/*!
    \brief      reset PMU
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_deinit(void)
{
    /* reset PMU */
    rcu_periph_reset_enable(RCU_PMURST);
    rcu_periph_reset_disable(RCU_PMURST);
}

/*!
    \brief      select low voltage detector threshold
    \param[in]  lvdt_n: low voltage detector threshold
                only one parameter can be selected which is shown as below:
      \arg        PMU_LVDT_0: voltage threshold is 2.1V
      \arg        PMU_LVDT_1: voltage threshold is 2.3V
      \arg        PMU_LVDT_2: voltage threshold is 2.4V
      \arg        PMU_LVDT_3: voltage threshold is 2.6V
      \arg        PMU_LVDT_4: voltage threshold is 2.7V
      \arg        PMU_LVDT_5: voltage threshold is 2.9V
      \arg        PMU_LVDT_6: voltage threshold is 3.0V
      \arg        PMU_LVDT_7: input analog voltage on PB7 (compared with 0.8V)
    \param[out] none
    \retval     none
*/
void pmu_lvd_select(uint32_t lvdt_n)
{
    /* disable LVD */
    PMU_CTL0 &= ~PMU_CTL0_LVDEN;
    /* clear LVDT bits */
    PMU_CTL0 &= ~PMU_CTL0_LVDT;
    /* set LVDT bits according to lvdt_n */
    PMU_CTL0 |= lvdt_n;
    /* enable LVD */
    PMU_CTL0 |= PMU_CTL0_LVDEN;
}

/*!
    \brief      disable PMU lvd
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lvd_disable(void)
{
    PMU_CTL0 &= ~PMU_CTL0_LVDEN;
}

/*!
    \brief      select LDO output voltage
                this bit set by software when the main PLL closed, before closing PLL, change the system clock to IRC16M or HXTAL
    \param[in]  ldo_output:
                only one parameter can be selected which is shown as below:
      \arg        PMU_LDOVS_LOW: LDO output voltage low mode
      \arg        PMU_LDOVS_HIGH: LDO output voltage high mode
    \param[out] none
    \retval     none
*/
void pmu_ldo_output_select(uint32_t ldo_output)
{
    PMU_CTL0 &= ~PMU_CTL0_LDOVS;
    PMU_CTL0 |= ldo_output;
}

/*!
    \brief      enable VBAT battery charging
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_vc_enable(void)
{
    PMU_CTL0 |= PMU_CTL0_VCEN;
}

/*!
    \brief      disable VBAT battery charging
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_vc_disable(void)
{
    PMU_CTL0 &= ~PMU_CTL0_VCEN;
}

/*!
    \brief      select PMU VBAT battery charging resistor
    \param[in]  resistor: VBAT battery charging resistor
                only one parameter can be selected which is shown as below:
      \arg        PMU_VCRSEL_5K: 5 kOhms resistor is selected for charing VBAT battery
      \arg        PMU_VCRSEL_1P5K: 1.5 kOhms resistor is selected for charing VBAT battery
    \param[out] none
    \retval     none
*/
void pmu_vcr_select(uint32_t resistor)
{
    PMU_CTL0 &= ~PMU_CTL0_VCRSEL;
    PMU_CTL0 |= resistor;
}

/*!
    \brief      enable low power in Run/Sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_low_power_enable(void)
{
    PMU_CTL0 |= PMU_CTL0_LDNP;
}

/*!
    \brief      disable low power in Run/Sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_low_power_disable(void)
{
    PMU_CTL0 &= ~PMU_CTL0_LDNP;
}

/*!
    \brief      PMU work at sleep mode
    \param[in]  lowdrive: low-driver mode when use normal power LDO in Run/Sleep mode.
                only one parameter can be selected which is shown as below:
      \arg        PMU_LDNP_NORMALDRIVE: low-driver mode disable
      \arg        PMU_LDNP_LOWDRIVE: low-driver mode enable
    \param[in]  sleepmodecmd: sleep mode command
                only one parameter can be selected which is shown as below:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_sleepmode(uint32_t lowdrive, uint8_t sleepmodecmd)
{
    /* clear sleepDeep bit of Cortex-M23 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);

    /* clear lowpower mode and low drive bits */
    PMU_CTL0 &= ~((uint32_t)(PMU_CTL0_LPMOD | PMU_CTL0_LDNP));

    /* configure low drive mode in Sleep mode */
    if(PMU_LDNP_LOWDRIVE == lowdrive) {
        PMU_CTL0 |= (uint32_t)(PMU_CTL0_LDNP);
    }

    /* select WFI or WFE command to enter sleep mode */
    if(WFI_CMD == sleepmodecmd) {
        __WFI();
    } else {
        __WFE();
    }
}

/*!
    \brief      PMU work at Deep-sleep mode
    \param[in]  lowdrive: low-driver mode when use normal power LDO in Deep-sleep mode
                only one parameter can be selected which is shown as below:
      \arg        PMU_LDNPDSP_NORMALDRIVE: low-driver mode disable
      \arg        PMU_LDNPDSP_LOWDRIVE: low-driver mode enable
    \param[in]  deepsleepmodecmd: deepsleep mode command
                only one parameter can be selected which is shown as below:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[in]  deepsleepmode: deepsleep mode
                only one parameter can be selected which is shown as below:
      \arg        PMU_DEEPSLEEP: Deep-sleep mode
      \arg        PMU_DEEPSLEEP1: Deep-sleep mode 1
      \arg        PMU_DEEPSLEEP2: Deep-sleep mode 2
    \param[out] none
    \retval     none
*/
void pmu_to_deepsleepmode(uint32_t lowdrive, uint8_t deepsleepmodecmd, uint8_t deepsleepmode)
{
    /* clear lowpower mode and low drive bits */
    PMU_CTL0 &= ~((uint32_t)(PMU_CTL0_LPMOD | PMU_CTL0_LDNPDSP));

    /* low drive mode config in Deep-sleep mode */
    /* PMU_CTL0_LDNPDSP only useful in PMU_DEEPSLEEP */
    if((PMU_LDNPDSP_LOWDRIVE == lowdrive) && (PMU_DEEPSLEEP == deepsleepmode)) {
        PMU_CTL0 |= (uint32_t)(PMU_CTL0_LDNPDSP);
    }

    /* configure Deep-sleep mode */
    PMU_CTL0 |= deepsleepmode;

    /* set sleepdeep bit of Cortex-M23 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* select WFI or WFE command to enter Deep-sleep mode */
    if(WFI_CMD == deepsleepmodecmd) {
        __WFI();
    } else {
        __SEV();
        __WFE();
        __WFE();
    }
    /* reset sleepdeep bit of Cortex-M23 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}

/*!
    \brief      pmu work at standby mode
    \param[in]  standbymodecmd:
                only one parameter can be selected which is shown as below:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_standbymode(uint8_t standbymodecmd)
{
    /* set sleepdeep bit of Cortex-M23 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* select the low-power mode to enter */
    PMU_CTL0 |= PMU_STANDBY;

    /* reset wakeup flag and standby flag */
    PMU_CTL0 |= PMU_CTL0_WURST | PMU_CTL0_STBRST;

    /* select WFI or WFE command to enter standby mode */
    if(WFI_CMD == standbymodecmd) {
        __WFI();
    } else {
        __WFE();
        __WFE();
    }
}

/*!
    \brief      enable wakeup pin
    \param[in]  wakeup_pin: wakeup pin
                one or more parameters can be selected which are shown as below:
      \arg        PMU_WAKEUP_PIN0: WKUP Pin 0 (PA0)
      \arg        PMU_WAKEUP_PIN1: WKUP Pin 1 (PC13)
      \arg        PMU_WAKEUP_PIN2: WKUP Pin 2 (PA2)
      \arg        PMU_WAKEUP_PIN3: WKUP Pin 3 (PB2)
      \arg        PMU_WAKEUP_PIN4: WKUP Pin 4 (PC6)
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_enable(uint32_t wakeup_pin)
{
    PMU_CS |= wakeup_pin;
}

/*!
    \brief      disable wakeup pin
    \param[in]  wakeup_pin: wakeup pin
                one or more parameters can be selected which are shown as below:
      \arg        PMU_WAKEUP_PIN0: WKUP Pin 0 (PA0)
      \arg        PMU_WAKEUP_PIN1: WKUP Pin 1 (PC13)
      \arg        PMU_WAKEUP_PIN2: WKUP Pin 2 (PA2)
      \arg        PMU_WAKEUP_PIN3: WKUP Pin 3 (PB2)
      \arg        PMU_WAKEUP_PIN4: WKUP Pin 4 (PC6)
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_disable(uint32_t wakeup_pin)
{
    PMU_CS &= ~(wakeup_pin);
}

/*!
    \brief      enable backup domain write
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_enable(void)
{
    PMU_CTL0 |= PMU_CTL0_BKPWEN;
}

/*!
    \brief      disable backup domain write
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_disable(void)
{
    PMU_CTL0 &= ~PMU_CTL0_BKPWEN;
}

/*!
    \brief      configure power state of SRAM1
    \param[in]  state: power state of SRAM1
                only one parameter can be selected which is shown as below:
      \arg        PMU_SRAM1_SLEEP: SRAM1 go to power-off
      \arg        PMU_SRAM1_WAKE: SRAM1 wakeup
    \param[out] none
    \retval     none
*/
void pmu_sram_power_config(uint32_t state)
{
    PMU_CTL1 |= state;
}

/*!
    \brief      configure power state of COREOFF1 domain
    \param[in]  state: power state of COREOFF1 domain
                only one parameter can be selected which is shown as below:
      \arg        PMU_CORE1_SLEEP: COREOFF1 domain go to power-off
      \arg        PMU_CORE1_WAKE: COREOFF1 domain wakeup
    \param[out] none
    \retval     none
*/
void pmu_core1_power_config(uint32_t state)
{
    PMU_CTL1 |= state;
}

/*!
    \brief      have retention register in Deep-sleep 2
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_deepsleep2_retention_enable(void)
{
    PMU_CTL1 &= ~PMU_CTL1_NRRD2;
}

/*!
    \brief      no retention register in Deep-sleep 2
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_deepsleep2_retention_disable(void)
{
    PMU_CTL1 |= PMU_CTL1_NRRD2;
}

/*!
    \brief      configure SRAM1 power state when enter Deep-sleep 2
    \param[in]  state: power state of SRAM1 when enters Deep-sleep 2 mode
                only one parameter can be selected which is shown as below:
      \arg        PMU_SRAM1_POWER_OFF: SRAM1 power-off
      \arg        PMU_SRAM1_POWER_REMAIN: SRAM1 power same as Run/Run1/Run2 mode
    \param[out] none
    \retval     none
*/
void pmu_deepsleep2_sram_power_config(uint32_t state)
{
    PMU_CTL1 &= ~PMU_CTL1_SRAM1PD2;
    PMU_CTL1 |= state;
}

/*!
    \brief      configure IRC16M counter before enter Deep-sleep mode
    \param[in]  wait_time: 0x0~0x1F, IRC16M counter before enter Deep-sleep mode
    \param[out] none
    \retval     none
*/
void pmu_deepsleep_wait_time_config(uint32_t wait_time)
{
    PMU_PAR &= ~PMU_PAR_TSW_IRC16MCNT;
    PMU_PAR |= (uint32_t)(wait_time << PAR_TSW_IRC16MCNT_OFFSET);
}

/*!
    \brief      use software value signal when wake up COREOFF1 domain
    \param[in]  wakeup_time: 0x0~0xFF, wakeup time of power switch of COREOFF1 domain. 4 IRC16M clock step and the max value is 64us.
    \param[out] none
    \retval     none
*/
void pmu_wakeuptime_core1_software_enable(uint32_t wakeup_time)
{
    PMU_PAR &= ~PMU_PAR_TWK_CORE1;
    PMU_PAR |= (uint32_t)(wakeup_time << PAR_TWK_CORE1_OFFSET);
    PMU_PAR |= PMU_PAR_TWKCORE1EN;
}

/*!
    \brief      use hardware ack signal when wake up COREOFF1 domain
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_wakeuptime_core1_software_disable(void)
{
    PMU_PAR &= ~PMU_PAR_TWKCORE1EN;
}

/*!
    \brief      use software value signal when wake up SRAM1
    \param[in]  wakeup_time: 0x0~0xFF, wakeup time of power switch of SRAM1. The step is 4 IRC16M clock and the max value is 64us.
    \param[out] none
    \retval     none
*/
void pmu_wakeuptime_sram_software_enable(uint32_t wakeup_time)
{
    PMU_PAR &= ~PMU_PAR_TWK_SRAM1;
    PMU_PAR |= (uint32_t)(wakeup_time << PAR_TWK_SRAM1_OFFSET);
    PMU_PAR |= PMU_PAR_TWKSRAM1EN;
}

/*!
    \brief      use hardware ack signal when wake up SRAM1
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_wakeuptime_sram_software_disable(void)
{
    PMU_PAR &= ~PMU_PAR_TWKSRAM1EN;
}

/*!
    \brief      use software value signal when wake up Deep-sleep 2
    \param[in]  wakeup_time: 0x0~0xFF, wakeup time of power switch of COREOFF0 domain. The step is 2 IRC16M clock and the max value is 32us.
    \param[out] none
    \retval     none
*/
void pmu_wakeuptime_deepsleep2_software_enable(uint32_t wakeup_time)
{
    PMU_PAR &= ~PMU_PAR_TWK_CORE0;
    PMU_PAR |= wakeup_time;
    PMU_PAR |= PMU_PAR_TWKEN;
}

/*!
    \brief      use hardware ack signal when wake up Deep-sleep 2
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_wakeuptime_deepsleep2_software_disable(void)
{
    PMU_PAR &= ~PMU_PAR_TWKEN;
}

/*!
    \brief      get PMU flag status
    \param[in]  flag: PMU flags
                only one parameter can be selected which is shown as below:
      \arg        PMU_FLAG_WAKEUP: wakeup flag
      \arg        PMU_FLAG_STANDBY: standby flag
      \arg        PMU_FLAG_LVD: lvd flag
      \arg        PMU_FLAG_LDOVSRF: LDO voltage select ready flag
      \arg        PMU_FLAG_NPRDY: normal-power LDO ready flag
      \arg        PMU_FLAG_LPRDY: low-power LDO ready flag
      \arg        PMU_FLAG_SRAM1_SLEEP: SRAM1 is in sleep state flag
      \arg        PMU_FLAG_SRAM1_ACTIVE: SRAM1 is in active state flag
      \arg        PMU_FLAG_CORE1_SLEEP: COREOFF1 domain is in sleep state flag
      \arg        PMU_FLAG_CORE1_ACTIVE: COREOFF1 domain is in active state flag
      \arg        PMU_FLAG_DEEPSLEEP_2: Deep-sleep 2 mode status flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus pmu_flag_get(uint32_t flag)
{
    FlagStatus ret = RESET;

    if(RESET != (flag & BIT(31))) {
        flag &= ~BIT(31);
        if(PMU_STAT & flag) {
            ret = SET;
        }
    } else {
        if(PMU_CS & flag) {
            ret = SET;
        }
    }
    return ret;
}

/*!
    \brief      clear PMU flag status
    \param[in]  flag: PMU flags
                only one parameter can be selected which is shown as below:
      \arg        PMU_FLAG_WAKEUP: wakeup flag
      \arg        PMU_FLAG_STANDBY: standby flag
      \arg        PMU_FLAG_DEEPSLEEP_2: Deep-sleep 2 mode status flag
    \param[out] none
    \retval     none
*/
void pmu_flag_clear(uint32_t flag)
{
    switch(flag) {
    case PMU_FLAG_WAKEUP:
        /* clear wakeup flag */
        PMU_CTL0 |= PMU_CTL0_WURST;
        break;
    case PMU_FLAG_STANDBY:
        /* clear standby flag */
        PMU_CTL0 |= PMU_CTL0_STBRST;
        break;
    case PMU_FLAG_DEEPSLEEP_2:
        /* clear Deep-sleep 2 mode status flag */
        PMU_STAT &= ~PMU_STAT_DPF2;
        break;
    default :
        break;
    }
}
