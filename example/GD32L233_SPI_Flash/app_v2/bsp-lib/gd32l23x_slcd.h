/*!
    \file    gd32l23x_slcd.h
    \brief   definitions for the SLCD

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

#ifndef GD32L23X_SLCD_H
#define GD32L23X_SLCD_H

#include "gd32l23x.h"

/* SLCD definitions */
#define SLCD                                SLCD_BASE

/* registers definitions */
#define SLCD_CTL                            REG32(SLCD + 0x00000000U)       /*!< SLCD control register */
#define SLCD_CFG                            REG32(SLCD + 0x00000004U)       /*!< SLCD configuration register */
#define SLCD_STAT                           REG32(SLCD + 0x00000008U)       /*!< SLCD status flag register */
#define SLCD_STATC                          REG32(SLCD + 0x0000000CU)       /*!< SLCD status flag clear register */
#define SLCD_DATA0                          REG32(SLCD + 0x00000014U)       /*!< SLCD display data registers 0 */
#define SLCD_DATA1                          REG32(SLCD + 0x0000001CU)       /*!< SLCD display data registers 1 */
#define SLCD_DATA2                          REG32(SLCD + 0x00000024U)       /*!< SLCD display data registers 2 */
#define SLCD_DATA3                          REG32(SLCD + 0x0000002CU)       /*!< SLCD display data registers 3 */
#define SLCD_DATA4                          REG32(SLCD + 0x00000034U)       /*!< SLCD display data registers 4 */
#define SLCD_DATA5                          REG32(SLCD + 0x0000003CU)       /*!< SLCD display data registers 5 */
#define SLCD_DATA6                          REG32(SLCD + 0x00000044U)       /*!< SLCD display data registers 6 */
#define SLCD_DATA7                          REG32(SLCD + 0x0000004CU)       /*!< SLCD display data registers 7 */

/* bits definitions */
/* SLCD_CTL */
#define SLCD_CTL_SLCDON                     BIT(0)                          /*!< controller start */
#define SLCD_CTL_VSRC                       BIT(1)                          /*!< voltage source */
#define SLCD_CTL_DUTY                       BITS(2,4)                       /*!< duty select */
#define SLCD_CTL_BIAS                       BITS(5,6)                       /*!< bias select */
#define SLCD_CTL_COMS                       BIT(7)                          /*!< common segment pad select */
#define SLCD_CTL_VODEN                      BIT(8)                          /*!< voltage output driver enable */

/* SLCD_CFG */
#define SLCD_CFG_HDEN                       BIT(0)                          /*!< high drive enable */
#define SLCD_CFG_SOFIE                      BIT(1)                          /*!< start of frame interrupt enable */
#define SLCD_CFG_UPDIE                      BIT(3)                          /*!< update done interrupt enable */
#define SLCD_CFG_PULSE                      BITS(4,6)                       /*!< pulse on duration */
#define SLCD_CFG_DTD                        BITS(7,9)                       /*!< dead time duration */
#define SLCD_CFG_CONR                       BITS(10,12)                     /*!< contrast ratio */
#define SLCD_CFG_BLKDIV                     BITS(13,15)                     /*!< blink frequency divider */
#define SLCD_CFG_BLKMOD                     BITS(16,17)                     /*!< blink mode */
#define SLCD_CFG_DIV                        BITS(18,21)                     /*!< clock divider */
#define SLCD_CFG_PSC                        BITS(22,25)                     /*!< clock prescaler */

/* SLCD_STAT */
#define SLCD_STAT_ONF                       BIT(0)                          /*!< controller on flag */
#define SLCD_STAT_SOF                       BIT(1)                          /*!< start of frame flag */
#define SLCD_STAT_UPRF                      BIT(2)                          /*!< update data request flag */
#define SLCD_STAT_UPDF                      BIT(3)                          /*!< update data done flag */
#define SLCD_STAT_VRDYF                     BIT(4)                          /*!< voltage ready flag */
#define SLCD_STAT_SYNF                      BIT(5)                          /*!< SLCD_CFG register synchronization flag */

/* SLCD_STATC */
#define SLCD_STATC_SOFC                     BIT(1)                          /*!< start of frame flag clear */
#define SLCD_STATC_UPDC                     BIT(3)                          /*!< update data done flag clear */

/* constants definitions */
/* SLCD voltage source definitions */
#define CTL_VSRC(regval)                    (BIT(1)&((uint32_t)(regval)<<1))
#define SLCD_VOLTAGE_INTERNAL               CTL_VSRC(0)                     /*!< internal source */
#define SLCD_VOLTAGE_EXTERNAL               CTL_VSRC(1)                     /*!< external source (VSLCD pin) */

/* SLCD duty select definitions */
#define CTL_DUTY(regval)                    (BITS(2,4)&((uint32_t)(regval)<<2))
#define SLCD_DUTY_STATIC                    CTL_DUTY(0)                     /*!< static duty */
#define SLCD_DUTY_1_2                       CTL_DUTY(1)                     /*!< 1/2 duty */
#define SLCD_DUTY_1_3                       CTL_DUTY(2)                     /*!< 1/3 duty */
#define SLCD_DUTY_1_4                       CTL_DUTY(3)                     /*!< 1/4 duty */
#define SLCD_DUTY_1_8                       CTL_DUTY(4)                     /*!< 1/8 duty */
#define SLCD_DUTY_1_6                       CTL_DUTY(5)                     /*!< 1/6 duty */

/* SLCD bias select definitions */
#define CTL_BIAS(regval)                    (BITS(5,6)&((uint32_t)(regval)<<5))
#define SLCD_BIAS_1_4                       CTL_BIAS(0)                     /*!< 1/4 bias */
#define SLCD_BIAS_1_2                       CTL_BIAS(1)                     /*!< 1/2 bias */
#define SLCD_BIAS_1_3                       CTL_BIAS(2)                     /*!< 1/3 bias */

/* SLCD pulse on duration definitions */
#define CFG_PULSE(regval)                   (BITS(4,6)&((uint32_t)(regval)<<4))
#define SLCD_PULSEON_DURATION_0             CFG_PULSE(0)                    /*!< pulse on duration = 0 */
#define SLCD_PULSEON_DURATION_1             CFG_PULSE(1)                    /*!< pulse on duration = 1*1/fPRE */
#define SLCD_PULSEON_DURATION_2             CFG_PULSE(2)                    /*!< pulse on duration = 2*1/fPRE */
#define SLCD_PULSEON_DURATION_3             CFG_PULSE(3)                    /*!< pulse on duration = 3*1/fPRE */
#define SLCD_PULSEON_DURATION_4             CFG_PULSE(4)                    /*!< pulse on duration = 4*1/fPRE */
#define SLCD_PULSEON_DURATION_5             CFG_PULSE(5)                    /*!< pulse on duration = 5*1/fPRE */
#define SLCD_PULSEON_DURATION_6             CFG_PULSE(6)                    /*!< pulse on duration = 6*1/fPRE */
#define SLCD_PULSEON_DURATION_7             CFG_PULSE(7)                    /*!< pulse on duration = 7*1/fPRE */

/* SLCD dead time definitions */
#define CFG_DTD(regval)                     (BITS(7,9)&((uint32_t)(regval)<<7))
#define SLCD_DEADTIME_PERIOD_0              CFG_DTD(0)                      /*!< no dead time */
#define SLCD_DEADTIME_PERIOD_1              CFG_DTD(1)                      /*!< 1 phase inserted between couple of frame */
#define SLCD_DEADTIME_PERIOD_2              CFG_DTD(2)                      /*!< 2 phase inserted between couple of frame */
#define SLCD_DEADTIME_PERIOD_3              CFG_DTD(3)                      /*!< 3 phase inserted between couple of frame */
#define SLCD_DEADTIME_PERIOD_4              CFG_DTD(4)                      /*!< 4 phase inserted between couple of frame */
#define SLCD_DEADTIME_PERIOD_5              CFG_DTD(5)                      /*!< 5 phase inserted between couple of frame */
#define SLCD_DEADTIME_PERIOD_6              CFG_DTD(6)                      /*!< 6 phase inserted between couple of frame */
#define SLCD_DEADTIME_PERIOD_7              CFG_DTD(7)                      /*!< 7 phase inserted between couple of frame */

/* SLCD contrast definitions */
#define CFG_CONR(regval)                    (BITS(10,12)&((uint32_t)(regval)<<10))
#define SLCD_CONTRAST_LEVEL_0               CFG_CONR(0)                     /* contrast ratio level0:Maximum Voltage = 2.65V */
#define SLCD_CONTRAST_LEVEL_1               CFG_CONR(1)                     /* contrast ratio level1:Maximum Voltage = 2.80V */
#define SLCD_CONTRAST_LEVEL_2               CFG_CONR(2)                     /* contrast ratio level2:Maximum Voltage = 2.92V */
#define SLCD_CONTRAST_LEVEL_3               CFG_CONR(3)                     /* contrast ratio level3:Maximum Voltage = 3.08V */
#define SLCD_CONTRAST_LEVEL_4               CFG_CONR(4)                     /* contrast ratio level4:Maximum Voltage = 3.23V */
#define SLCD_CONTRAST_LEVEL_5               CFG_CONR(5)                     /* contrast ratio level5:Maximum Voltage = 3.37V */
#define SLCD_CONTRAST_LEVEL_6               CFG_CONR(6)                     /* contrast ratio level6:Maximum Voltage = 3.52V */
#define SLCD_CONTRAST_LEVEL_7               CFG_CONR(7)                     /* contrast ratio level7:Maximum Voltage = 3.67V */

/* SLCD blink frequency definitions */
#define CFG_BLKDIV(regval)                  (BITS(13,15)&((uint32_t)(regval)<<13))
#define SLCD_BLINK_FREQUENCY_DIV8           CFG_BLKDIV(0)                   /*!< blink frequency = fSLCD/8 */
#define SLCD_BLINK_FREQUENCY_DIV16          CFG_BLKDIV(1)                   /*!< blink frequency = fSLCD/16 */
#define SLCD_BLINK_FREQUENCY_DIV32          CFG_BLKDIV(2)                   /*!< blink frequency = fSLCD/32 */
#define SLCD_BLINK_FREQUENCY_DIV64          CFG_BLKDIV(3)                   /*!< blink frequency = fSLCD/64 */
#define SLCD_BLINK_FREQUENCY_DIV128         CFG_BLKDIV(4)                   /*!< blink frequency = fSLCD/128 */
#define SLCD_BLINK_FREQUENCY_DIV256         CFG_BLKDIV(5)                   /*!< blink frequency = fSLCD/256 */
#define SLCD_BLINK_FREQUENCY_DIV512         CFG_BLKDIV(6)                   /*!< blink frequency = fSLCD/512 */
#define SLCD_BLINK_FREQUENCY_DIV1024        CFG_BLKDIV(7)                   /*!< blink frequency = fSLCD/1024 */


/* SLCD blink mode definitions */
#define CFG_BLKMOD(regval)                  (BITS(16,17)&((uint32_t)(regval)<<16))
#define SLCD_BLINK_OFF                      CFG_BLKMOD(0)                   /* blink disabled */
#define SLCD_BLINK_SEG0_COM0                CFG_BLKMOD(1)                   /* blink enabled on SEG[0], COM[0] */
#define SLCD_BLINK_SEG0_ALLCOM              CFG_BLKMOD(2)                   /* blink enabled on SEG[0], all COM */
#define SLCD_BLINK_ALLSEG_ALLCOM            CFG_BLKMOD(3)                   /* blink enabled on all SEG and all COM */

/* SLCD divider definitions */
#define CFG_DIV(regval)                     (BITS(18,21)&((uint32_t)(regval)<<18))
#define SLCD_DIVIDER_16                     CFG_DIV(0)                      /*!< DIV = 16 */
#define SLCD_DIVIDER_17                     CFG_DIV(1)                      /*!< DIV = 17 */
#define SLCD_DIVIDER_18                     CFG_DIV(2)                      /*!< DIV = 18 */
#define SLCD_DIVIDER_19                     CFG_DIV(3)                      /*!< DIV = 19 */
#define SLCD_DIVIDER_20                     CFG_DIV(4)                      /*!< DIV = 20 */
#define SLCD_DIVIDER_21                     CFG_DIV(5)                      /*!< DIV = 21 */
#define SLCD_DIVIDER_22                     CFG_DIV(6)                      /*!< DIV = 22 */
#define SLCD_DIVIDER_23                     CFG_DIV(7)                      /*!< DIV = 23 */
#define SLCD_DIVIDER_24                     CFG_DIV(8)                      /*!< DIV = 24 */
#define SLCD_DIVIDER_25                     CFG_DIV(9)                      /*!< DIV = 25 */
#define SLCD_DIVIDER_26                     CFG_DIV(10)                     /*!< DIV = 26 */
#define SLCD_DIVIDER_27                     CFG_DIV(11)                     /*!< DIV = 27 */
#define SLCD_DIVIDER_28                     CFG_DIV(12)                     /*!< DIV = 28 */
#define SLCD_DIVIDER_29                     CFG_DIV(13)                     /*!< DIV = 29 */
#define SLCD_DIVIDER_30                     CFG_DIV(14)                     /*!< DIV = 30 */
#define SLCD_DIVIDER_31                     CFG_DIV(15)                     /*!< DIV = 31 */


/* SLCD prescaler definitions */
#define CFG_PRE(regval)                     (BITS(22,25)&((uint32_t)(regval)<<22))
#define SLCD_PRESCALER_1                    CFG_PRE(0)                      /*!< PRE = 0 */
#define SLCD_PRESCALER_2                    CFG_PRE(1)                      /*!< PRE = 1 */
#define SLCD_PRESCALER_4                    CFG_PRE(2)                      /*!< PRE = 2 */
#define SLCD_PRESCALER_8                    CFG_PRE(3)                      /*!< PRE = 3 */
#define SLCD_PRESCALER_16                   CFG_PRE(4)                      /*!< PRE = 4 */
#define SLCD_PRESCALER_32                   CFG_PRE(5)                      /*!< PRE = 5 */
#define SLCD_PRESCALER_64                   CFG_PRE(6)                      /*!< PRE = 6 */
#define SLCD_PRESCALER_128                  CFG_PRE(7)                      /*!< PRE = 7 */
#define SLCD_PRESCALER_256                  CFG_PRE(8)                      /*!< PRE = 8 */
#define SLCD_PRESCALER_512                  CFG_PRE(9)                      /*!< PRE = 9 */
#define SLCD_PRESCALER_1024                 CFG_PRE(10)                     /*!< PRE = 10 */
#define SLCD_PRESCALER_2048                 CFG_PRE(11)                     /*!< PRE = 11 */
#define SLCD_PRESCALER_4096                 CFG_PRE(12)                     /*!< PRE = 12 */
#define SLCD_PRESCALER_8192                 CFG_PRE(13)                     /*!< PRE = 13 */
#define SLCD_PRESCALER_16384                CFG_PRE(14)                     /*!< PRE = 14 */
#define SLCD_PRESCALER_32768                CFG_PRE(15)                     /*!< PRE = 15 */

/* SLCD data register */
#define SLCD_DATA0_7(number)                REG32((SLCD) + (uint32_t)0x14U + (number) * (uint32_t)0x08U)

/* SCLD interrupt enable or disable */
#define SLCD_INT_SOF                        SLCD_CFG_SOFIE                  /*!< start of frame interrupt enable */
#define SLCD_INT_UPD                        SLCD_CFG_UPDIE                  /*!< update done interrupt enable */

/* SCLD flag */
#define SLCD_FLAG_ON                        SLCD_STAT_ONF                   /*!< controller on flag */
#define SLCD_FLAG_SO                        SLCD_STAT_SOF                   /*!< start of frame flag */
#define SLCD_FLAG_UPR                       SLCD_STAT_UPRF                  /*!< update data request flag */
#define SLCD_FLAG_UPD                       SLCD_STAT_UPDF                  /*!< update data done flag */
#define SLCD_FLAG_VRDY                      SLCD_STAT_VRDYF                 /*!< voltage ready flag */
#define SLCD_FLAG_SYN                       SLCD_STAT_SYNF                  /*!< SLCD_CFG register synchronization flag */

/* SLCD interrupt flag */
#define SLCD_INT_FLAG_SO                    SLCD_STAT_SOF                   /*!< start of frame flag */
#define SLCD_INT_FLAG_UPD                   SLCD_STAT_UPDF                  /*!< update data done flag */

/*data register number */
typedef enum {
    SLCD_DATA_REG0,                                                         /*!< SLCD display data register 0 */
    SLCD_DATA_REG1,                                                         /*!< SLCD display data register 1 */
    SLCD_DATA_REG2,                                                         /*!< SLCD display data register 2 */
    SLCD_DATA_REG3,                                                         /*!< SLCD display data register 3 */
    SLCD_DATA_REG4,                                                         /*!< SLCD display data register 4 */
    SLCD_DATA_REG5,                                                         /*!< SLCD display data register 5 */
    SLCD_DATA_REG6,                                                         /*!< SLCD display data register 6 */
    SLCD_DATA_REG7,                                                         /*!< SLCD display data register 7 */
} slcd_data_register_enum;

/* function declarations */
/* initialization functions */
/* reset SLCD interface */
void slcd_deinit(void);
/* enable SLCD interface */
void slcd_enable(void);
/* disable SLCD interface */
void slcd_disable(void);

/* configure SLCD functions */
/* initialize SLCD interface */
void slcd_init(uint32_t prescaler, uint32_t divider, uint32_t duty, uint32_t bias);
/* enable SLCD enhance mode */
void slcd_enhance_mode_enable(void);
/* disable SLCD enhance mode */
void slcd_enhance_mode_disable(void);
/* select SLCD bias voltage */
void slcd_bias_voltage_select(uint32_t bias_voltage);
/* select SLCD duty */
void slcd_duty_select(uint32_t duty);
/* configure SLCD input clock */
void slcd_clock_config(uint32_t prescaler, uint32_t divider);
/* configure SLCD blink mode */
void slcd_blink_mode_config(uint32_t mode, uint32_t blink_divider);
/* configure SLCD contrast ratio */
void slcd_contrast_ratio_config(uint32_t contrast_ratio);
/* configure SLCD dead time duration */
void slcd_dead_time_config(uint32_t dead_time);
/* configure SLCD pulse on duration */
void slcd_pulse_on_duration_config(uint32_t duration);
/* select SLCD common/segment pad */
void slcd_com_seg_remap(ControlStatus newvalue);
/* select SLCD voltage source */
void slcd_voltage_source_select(uint8_t voltage_source);
/* enable or disable permanent high drive */
void slcd_high_drive_config(ControlStatus newvalue);
/* write SLCD data register */
void slcd_data_register_write(slcd_data_register_enum register_number, uint32_t data);
/* update SLCD data request */
void slcd_data_update_request(void);

/* SLCD interrupt and flag */
/* get SLCD flags */
FlagStatus slcd_flag_get(uint32_t flag);
/* clear SLCD flags */
void slcd_flag_clear(uint32_t flag);
/* enable SLCD interrupt */
void slcd_interrupt_enable(uint32_t interrupt);
/* disable SLCD interrupt */
void slcd_interrupt_disable(uint32_t interrupt);
/* get SLCD interrupt flag */
FlagStatus slcd_interrupt_flag_get(uint32_t int_flag);
/* clear SLCD interrupt flag */
void slcd_interrupt_flag_clear(uint32_t int_flag);

#endif /* GD32L23X_SLCD_H */
