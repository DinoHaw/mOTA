/*!
    \file    gd32l23x_pmu.h
    \brief   definitions for the PMU

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

#ifndef GD32L23X_PMU_H
#define GD32L23X_PMU_H

#include "gd32l23x.h"

/* PMU definitions */
#define PMU                           PMU_BASE                              /*!< PMU base address */

/* registers definitions */
#define PMU_CTL0                      REG32((PMU) + 0x00000000U)            /*!< PMU control register 0 */
#define PMU_CS                        REG32((PMU) + 0x00000004U)            /*!< PMU control and status register */
#define PMU_CTL1                      REG32((PMU) + 0x00000008U)            /*!< PMU control register 1 */
#define PMU_STAT                      REG32((PMU) + 0x0000000CU)            /*!< PMU status register */
#define PMU_PAR                       REG32((PMU) + 0x00000010U)            /*!< PMU parameter register */

/* bits definitions */
/* PMU_CTL0 */
#define PMU_CTL0_LPMOD                BITS(0,1)                             /*!< select the low-power mode to enter */
#define PMU_CTL0_WURST                BIT(2)                                /*!< wakeup flag reset */
#define PMU_CTL0_STBRST               BIT(3)                                /*!< standby flag reset */
#define PMU_CTL0_LVDEN                BIT(4)                                /*!< low voltage detector enable */
#define PMU_CTL0_LVDT                 BITS(5,7)                             /*!< low voltage detector threshold */
#define PMU_CTL0_BKPWEN               BIT(8)                                /*!< backup domain write enable */
#define PMU_CTL0_LDNPDSP              BIT(10)                               /*!< low-driver mode when use normal power LDO in Deep-sleep mode */
#define PMU_CTL0_LDNP                 BIT(11)                               /*!< low-driver mode when use normal power LDO in Run/Sleep mode */
#define PMU_CTL0_VCEN                 BIT(12)                               /*!< VBAT battery charging enable */
#define PMU_CTL0_VCRSEL               BIT(13)                               /*!< VBAT battery charging resistor selection */
#define PMU_CTL0_LDOVS                BITS(14,15)                           /*!< LDO output voltage select */

/* PMU_CS */
#define PMU_CS_WUF                    BIT(0)                                /*!< wakeup flag */
#define PMU_CS_STBF                   BIT(1)                                /*!< standby flag */
#define PMU_CS_LVDF                   BIT(2)                                /*!< low voltage detector status flag */
#define PMU_CS_WUPEN0                 BIT(8)                                /*!< wakeup pin0 enable */
#define PMU_CS_WUPEN1                 BIT(9)                                /*!< wakeup pin1 enable */
#define PMU_CS_WUPEN2                 BIT(10)                               /*!< wakeup pin2 enable */
#define PMU_CS_WUPEN3                 BIT(11)                               /*!< wakeup pin3 enable */
#define PMU_CS_WUPEN4                 BIT(12)                               /*!< wakeup pin4 enable */
#define PMU_CS_LDOVSRF                BIT(14)                               /*!< LDO voltage select ready flag */
#define PMU_CS_NPRDY                  BIT(16)                               /*!< normal-power LDO ready flag */
#define PMU_CS_LPRDY                  BIT(17)                               /*!< low-power LDO ready flag */

/* PMU_CTL1 */
#define PMU_CTL1_SRAM1PSLEEP          BIT(0)                                /*!< SRAM1 go to power-off */
#define PMU_CTL1_SRAM1PWAKE           BIT(1)                                /*!< SRAM1 wakeup */
#define PMU_CTL1_CORE1SLEEP           BIT(4)                                /*!< COREOFF1 domain go to power-off */
#define PMU_CTL1_CORE1WAKE            BIT(5)                                /*!< COREOFF1 domain wakeup */
#define PMU_CTL1_NRRD2                BIT(16)                               /*!< no retention register in Deep-sleep 2 mode */
#define PMU_CTL1_SRAM1PD2             BIT(17)                               /*!< power state of SRAM1 when enters Deep-sleep 2 mode */

/* PMU_STAT */
#define PMU_STAT_DPF2                 BIT(1)                                /*!< Deep-sleep 2 mode status, set by hardware when enter Deep-sleep 2 mode. */
#define PMU_STAT_SRAM1PS_SLEEP        BIT(2)                                /*!< SRAM1 is in sleep state */
#define PMU_STAT_SRAM1PS_ACTIVE       BIT(3)                                /*!< SRAM1 is in active state */
#define PMU_STAT_CORE1PS_SLEEP        BIT(4)                                /*!< COREOFF1 domain is in sleep state */
#define PMU_STAT_CORE1PS_ACTIVE       BIT(5)                                /*!< COREOFF1 domain is in active state */

/* PMU_PAR */
#define PMU_PAR_TWK_CORE0             BITS(0,7)                             /*!< wakeup time of power switch of COREOFF0 domain */
#define PMU_PAR_TWK_SRAM1             BITS(8,15)                            /*!< wakeup time of power switch of SRAM1 domain */
#define PMU_PAR_TSW_IRC16MCNT         BITS(16,20)                           /*!< wait the IRC16M COUNTER and then set Deep-sleep signal */
#define PMU_PAR_TWK_CORE1             BITS(21,28)                           /*!< wakeup time of power switch of COREOFF1 domain */
#define PMU_PAR_TWKCORE1EN            BIT(29)                               /*!< use software value when wake up COREOFF1 or not */
#define PMU_PAR_TWKSRAM1EN            BIT(30)                               /*!< use software value when wake up SRAM1 power domain or not */
#define PMU_PAR_TWKEN                 BIT(31)                               /*!< use software value when wake up Deep-sleep or not */

/* constants definitions */
/* PMU low voltage detector threshold definitions */
#define CTL0_LVDT(regval)             (BITS(5,7)&((uint32_t)(regval)<<5))
#define PMU_LVDT_0                    CTL0_LVDT(0)                          /*!< voltage threshold is 2.1V */
#define PMU_LVDT_1                    CTL0_LVDT(1)                          /*!< voltage threshold is 2.3V */
#define PMU_LVDT_2                    CTL0_LVDT(2)                          /*!< voltage threshold is 2.4V */
#define PMU_LVDT_3                    CTL0_LVDT(3)                          /*!< voltage threshold is 2.6V */
#define PMU_LVDT_4                    CTL0_LVDT(4)                          /*!< voltage threshold is 2.7V */
#define PMU_LVDT_5                    CTL0_LVDT(5)                          /*!< voltage threshold is 2.9V */
#define PMU_LVDT_6                    CTL0_LVDT(6)                          /*!< voltage threshold is 3.0V */
#define PMU_LVDT_7                    CTL0_LVDT(7)                          /*!< input analog voltage on PB7 (compared with 0.8V) */

/* PMU LDO output voltage select definitions */
#define CTL0_LDOVS(regval)            (BITS(14,15)&((uint32_t)(regval)<<14))
#define PMU_LDOVS_LOW                 CTL0_LDOVS(0)                         /*!< LDO output voltage low mode */
#define PMU_LDOVS_HIGH                CTL0_LDOVS(2)                         /*!< LDO output voltage high mode */

/* PMU low-driver mode when use normal power LDO in Deep-sleep mode */
#define CTL0_LDNPDSP(regval)          (BIT(10)&((uint32_t)(regval)<<10))
#define PMU_LDNPDSP_NORMALDRIVE       CTL0_LDNPDSP(0)                       /*!< normal driver when use normal power LDO in Deep-sleep mode */
#define PMU_LDNPDSP_LOWDRIVE          CTL0_LDNPDSP(1)                       /*!< low-driver mode enabled when use normal power LDO in Deep-sleep mode */

/* PMU low-driver mode when use normal power LDO in Run/Sleep mode */
#define CTL0_LDNP(regval)             (BIT(11)&((uint32_t)(regval)<<11))
#define PMU_LDNP_NORMALDRIVE          CTL0_LDNP(0)                          /*!< normal driver when use normal power LDO  in run/sleep mode */
#define PMU_LDNP_LOWDRIVE             CTL0_LDNP(1)                          /*!< low-driver mode enabled when use normal power LDO in run/sleep mode */

/* PMU VBAT battery charging resistor selection */
#define CTL0_VCRSEL(regval)           (BIT(13)&((uint32_t)(regval)<<13))
#define PMU_VCRSEL_5K                 CTL0_VCRSEL(0)                        /*!< 5 kOhms resistor is selected for charing VBAT battery */
#define PMU_VCRSEL_1P5K               CTL0_VCRSEL(1)                        /*!< 1.5 kOhms resistor is selected for charing VBAT battery */

/* select the low-power mode to enter */
#define CTL0_LPMOD(regval)            (BITS(0,1)&((uint32_t)(regval)<<0))
#define PMU_DEEPSLEEP                 CTL0_LPMOD(0)                         /*!< Deep-sleep mode */
#define PMU_DEEPSLEEP1                CTL0_LPMOD(1)                         /*!< Deep-sleep mode 1 */
#define PMU_DEEPSLEEP2                CTL0_LPMOD(2)                         /*!< Deep-sleep mode 2 */
#define PMU_STANDBY                   CTL0_LPMOD(3)                         /*!< standby mode */

/* power state of SRAM1 when enters Deep-sleep2 mode */
#define CTL1_SRAM1PD2(regval)         (BIT(17)&((uint32_t)(regval)<<17))
#define PMU_SRAM1_POWER_OFF           CTL1_SRAM1PD2(0)                      /*!< SRAM1 power-off */
#define PMU_SRAM1_POWER_REMAIN        CTL1_SRAM1PD2(1)                      /*!< SRAM1 power same as Run/Run1/Run2 mode */

/* PMU flag definitions */
#define PMU_FLAG_WAKEUP               PMU_CS_WUF                            /*!< wakeup flag status */
#define PMU_FLAG_STANDBY              PMU_CS_STBF                           /*!< standby flag status */
#define PMU_FLAG_LVD                  PMU_CS_LVDF                           /*!< lvd flag status */
#define PMU_FLAG_LDOVSRF              PMU_CS_LDOVSRF                        /*!< LDO voltage select ready flag */
#define PMU_FLAG_NPRDY                PMU_CS_NPRDY                          /*!< normal-power LDO ready flag */
#define PMU_FLAG_LPRDY                PMU_CS_LPRDY                          /*!< low-power LDO ready flag */
#define PMU_FLAG_SRAM1_SLEEP          (BIT(31) | PMU_STAT_SRAM1PS_SLEEP)    /*!< SRAM1 is in sleep state flag */
#define PMU_FLAG_SRAM1_ACTIVE         (BIT(31) | PMU_STAT_SRAM1PS_ACTIVE)   /*!< SRAM1 is in active state flag */
#define PMU_FLAG_CORE1_SLEEP          (BIT(31) | PMU_STAT_CORE1PS_SLEEP)    /*!< COREOFF1 domain is in sleep state flag */
#define PMU_FLAG_CORE1_ACTIVE         (BIT(31) | PMU_STAT_CORE1PS_ACTIVE)   /*!< COREOFF1 domain is in active state flag */
#define PMU_FLAG_DEEPSLEEP_2          (BIT(31) | PMU_STAT_DPF2)             /*!< Deep-sleep 2 mode status flag */

/* PMU wakeup pin definitions */
#define PMU_WAKEUP_PIN0               PMU_CS_WUPEN0                         /*!< WKUP Pin 0 (PA0) */
#define PMU_WAKEUP_PIN1               PMU_CS_WUPEN1                         /*!< WKUP Pin 1 (PC13) */
#define PMU_WAKEUP_PIN2               PMU_CS_WUPEN2                         /*!< WKUP Pin 2 (PA2) */
#define PMU_WAKEUP_PIN3               PMU_CS_WUPEN3                         /*!< WKUP Pin 3 (PB2) */
#define PMU_WAKEUP_PIN4               PMU_CS_WUPEN4                         /*!< WKUP Pin 4 (PC6) */

/* PMU SRAM1 and COREOFF1 power control */
#define PMU_SRAM1_SLEEP               PMU_CTL1_SRAM1PSLEEP                  /*!< SRAM1 go to power-off */
#define PMU_SRAM1_WAKE                PMU_CTL1_SRAM1PWAKE                   /*!< SRAM1 wakeup */
#define PMU_CORE1_SLEEP               PMU_CTL1_CORE1SLEEP                   /*!< COREOFF1 domain go to power-off */
#define PMU_CORE1_WAKE                PMU_CTL1_CORE1WAKE                    /*!< COREOFF1 domain wakeup */

/* PMU command constants definitions */
#define WFI_CMD                       ((uint8_t)0x00U)                      /*!< use WFI command */
#define WFE_CMD                       ((uint8_t)0x01U)                      /*!< use WFE command */

/* function declarations */
/* reset PMU */
void pmu_deinit(void);
/* select low voltage detector threshold */
void pmu_lvd_select(uint32_t lvdt_n);
/* disable PMU lvd */
void pmu_lvd_disable(void);
/* select LDO output voltage */
void pmu_ldo_output_select(uint32_t ldo_output);
/* enable VBAT battery charging */
void pmu_vc_enable(void);
/* disable VBAT battery charging */
void pmu_vc_disable(void);
/* select PMU VBAT battery charging resistor */
void pmu_vcr_select(uint32_t resistor);

/* configure PMU mode */
/* enable low power in Run/Sleep mode */
void pmu_low_power_enable(void);
/* disable low power in Run/Sleep mode */
void pmu_low_power_disable(void);
/* PMU work at sleep mode */
void pmu_to_sleepmode(uint32_t lowdrive, uint8_t sleepmodecmd);
/* PMU work at Deep-sleep mode */
void pmu_to_deepsleepmode(uint32_t lowdrive, uint8_t deepsleepmodecmd, uint8_t deepsleepmode);
/* PMU work at standby mode */
void pmu_to_standbymode(uint8_t standbymodecmd);
/* enable wakeup pin */
void pmu_wakeup_pin_enable(uint32_t wakeup_pin);
/* disable wakeup pin */
void pmu_wakeup_pin_disable(uint32_t wakeup_pin);
/* enable backup domain write */
void pmu_backup_write_enable(void);
/* disable backup domain write */
void pmu_backup_write_disable(void);
/* configure power state of SRAM1 */
void pmu_sram_power_config(uint32_t state);
/* configure power state of COREOFF1 domain */
void pmu_core1_power_config(uint32_t state);
/* have retention register in Deep-sleep 2 */
void pmu_deepsleep2_retention_enable(void);
/* no retention register in Deep-sleep 2 */
void pmu_deepsleep2_retention_disable(void);
/* configure SRAM1 power state when enter Deep-sleep 2 */
void pmu_deepsleep2_sram_power_config(uint32_t state);
/* configure IRC16M counter before enter Deep-sleep mode */
void pmu_deepsleep_wait_time_config(uint32_t wait_time);
/* use software value signal when wake up COREOFF1 domain */
void pmu_wakeuptime_core1_software_enable(uint32_t wakeup_time);
/* use hardware ack signal when wake up COREOFF1 domain */
void pmu_wakeuptime_core1_software_disable(void);
/* use software value signal when wake up SRAM1 */
void pmu_wakeuptime_sram_software_enable(uint32_t wakeup_time);
/* use hardware ack signal when wake up SRAM1 */
void pmu_wakeuptime_sram_software_disable(void);
/* use software value signal when wake up Deep-sleep2 */
void pmu_wakeuptime_deepsleep2_software_enable(uint32_t wakeup_time);
/* use hardware ack signal when wake up Deep-sleep2 */
void pmu_wakeuptime_deepsleep2_software_disable(void);

/* flag functions */
/* get PMU flag status */
FlagStatus pmu_flag_get(uint32_t flag);
/* clear PMU flag status */
void pmu_flag_clear(uint32_t flag);

#endif /* GD32L23X_PMU_H */
