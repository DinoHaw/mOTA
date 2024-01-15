/*!
    \file    gd32l23x_cmp.h
    \brief   definitions for the CMP

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

#ifndef GD32L23X_CMP_H
#define GD32L23X_CMP_H

#include "gd32l23x.h"

/* CMP definitions */
#define CMP0                                      CMP_BASE + 0x00000000U         /*!< CMP0 base address */
#define CMP1                                      CMP_BASE + 0x00000004U         /*!< CMP1 base address */

/* registers definitions */
#define CMP0_CS                                   REG32(CMP0 + 0x00000000U)      /*!< CMP0 control and status register */
#define CMP1_CS                                   REG32(CMP1 + 0x00000000U)      /*!< CMP1 control and status register */

/* bits definitions */
/* CMP0_CS */
#define CMP0_CS_EN                                BIT(0)                         /*!< CMP0 enable  */
#define CMP0_CS_PM                                BITS(2,3)                      /*!< CMP0 mode */
#define CMP0_CS_MSEL                              BITS(4,6)                      /*!< CMP0 minus input selection */
#define CMP0_CS_OSEL                              BITS(13,14)                    /*!< CMP0 output selection */
#define CMP0_CS_PL                                BIT(15)                        /*!< CMP0 output polarity */
#define CMP0_CS_HST                               BITS(16,17)                    /*!< CMP0 hysteresis */
#define CMP0_CS_BLK                               BITS(18,20)                    /*!< CMP0 blanking source selection */
#define CMP0_CS_BEN                               BIT(22)                        /*!< CMP0 scaler bridge enable bit */
#define CMP0_CS_SEN                               BIT(23)                        /*!< CMP0 voltage input scaler */
#define CMP0_CS_OUT                               BIT(30)                        /*!< CMP0 output state bit */
#define CMP0_CS_LK                                BIT(31)                        /*!< CMP0 lock */

/* CMP1_CS */
#define CMP1_CS_EN                                BIT(0)                         /*!< CMP1 enable  */
#define CMP1_CS_WEN                               BIT(1)                         /*!< CMP window mode enable */
#define CMP1_CS_PM                                BITS(2,3)                      /*!< CMP1 mode */
#define CMP1_CS_MSEL                              BITS(4,6)                      /*!< CMP1 minus input selection */
#define CMP1_CS_PSEL                              BITS(8,10)                     /*!< CMP1 plus input selection */
#define CMP1_CS_OSEL                              BITS(13,14)                    /*!< CMP1 output selection */
#define CMP1_CS_PL                                BIT(15)                        /*!< CMP1 output polarity */
#define CMP1_CS_HST                               BITS(16,17)                    /*!< CMP1 hysteresis */
#define CMP1_CS_BLK                               BITS(18,20)                    /*!< CMP1 blanking source selection */
#define CMP1_CS_BEN                               BIT(22)                        /*!< CMP1 scaler bridge enable bit */
#define CMP1_CS_SEN                               BIT(23)                        /*!< CMP1 voltage input scaler */
#define CMP1_CS_OUT                               BIT(30)                        /*!< CMP1 output state bit */
#define CMP1_CS_LK                                BIT(31)                        /*!< CMP1 lock */

/* constants definitions */
/* operating mode */
typedef enum {
    CMP_HIGHSPEED = 0,                                                           /*!< high speed mode */
    CMP_MIDDLESPEED,                                                             /*!< medium speed mode */
    CMP_LOWSPEED                                                                 /*!< low speed mode */
} operating_mode_enum;

/* inverting input */
typedef enum {
    CMP_1_4VREFINT = 0,                                                          /*!< VREFINT /4 input */
    CMP_1_2VREFINT,                                                              /*!< VREFINT /2 input */
    CMP_3_4VREFINT,                                                              /*!< VREFINT *3/4 input */
    CMP_VREFINT,                                                                 /*!< VREFINT input */
    CMP_PA0_PA2,                                                                 /*!< PA0 input when selecting CMP0, PA2 input when selecting CMP1 */
    CMP_DACOUT_PA4,                                                              /*!< DAC_OUT0(PA4) input */
    CMP_PB3                                                                      /*!< PB3 input only for CMP1 */
} inverting_input_enum;

/* plus input */
typedef enum {
    CMP1_PA3 = 0,                                                                /*!< PA3 */
    CMP1_PB4,                                                                    /*!< PB4 */
    CMP1_PB5,                                                                    /*!< PB5 */
    CMP1_PB6,                                                                    /*!< PB6 */
    CMP1_PB7                                                                     /*!< PB7 */
} CMP1_plus_input_enum;

/* hysteresis */
typedef enum {
    CMP_HYSTERESIS_NO = 0,                                                       /*!< output no hysteresis */
    CMP_HYSTERESIS_LOW,                                                          /*!< output low hysteresis */
    CMP_HYSTERESIS_MIDDLE,                                                       /*!< output middle hysteresis */
    CMP_HYSTERESIS_HIGH                                                          /*!< output high hysteresis */
} cmp_hysteresis_enum;

/* output */
typedef enum {
    CMP_OUTPUT_NONE = 0,                                                         /*!< output no selection */
    CMP_OUTPUT_TIMER1IC3,                                                        /*!< TIMER 1 channel3 input capture */
    CMP_OUTPUT_TIMER2IC0,                                                         /*!< TIMER 2 channel0 input capture */
    CMP_OUTPUT_LPTIMERIC0_IC1                                                    /*!< LPTIMER channel0 or channel1 input capture */
} cmp_output_enum;

/* output inv */
typedef enum {
    CMP_OUTPUT_POLARITY_INVERTED = 0,                                            /*!< output is inverted */
    CMP_OUTPUT_POLARITY_NOINVERTED                                               /*!< output is not inverted */
} cmp_output_inv_enum;

/* blanking_sourc */
typedef enum {
    CMP_BLANKING_NONE = 0,                                                       /*!< output no selection */
    CMP_BLANKING_TIMER1_OC1 = 1,                                                 /*!< TIMER 1 output channel 1 */
    CMP_BLANKING_TIMER2_OC1 = 2,                                                 /*!< TIMER 2 output channel 1 */
    CMP_BLANKING_TIMER8_OC1 = 4,                                                 /*!< TIMER 8 output channel 1 */
    CMP_BLANKING_TIMER11_OC1 = 5                                                 /*!< TIMER 11 output channel 1 */
} blanking_source_enum;

/* output state */
typedef enum {
    CMP_OUTPUTLEVEL_LOW = 0,                                                     /*!< the output is low */
    CMP_OUTPUTLEVEL_HIGH                                                         /*!< the output is high */
} cmp_output_state_enum;

/* CMP0 enable bit*/
#define CS_CMP0EN(regval)                         (BIT(0) & ((uint32_t)(regval) << 0))
#define CS_CMP0EN_DISABLE                         CS_CMP0EN(0)                   /*!< CMP0 enable */
#define CS_CMP0EN_ENABLE                          CS_CMP0EN(1)                   /*!< CMP0 disable */

/* CMP0 mode */
#define CS_CMP0PM(regval)                         (BITS(2,3) & ((uint32_t)(regval) << 2))
#define CS_CMP0PM_HIGHSPEED                       CS_CMP0PM(0)                   /*!< CMP0 mode high speed */
#define CS_CMP0PM_MEDIUMSPEED                     CS_CMP0PM(1)                   /*!< CMP0 mode medium speed */
#define CS_CMP0PM_LOWSPEED                        CS_CMP0PM(3)                   /*!< CMP0 mode low speed */

/* CMP0 inverting input */
#define CS_CMP0MSEL(regval)                       (BITS(4,6) & ((uint32_t)(regval) << 4))
#define CS_CMP0MSEL_1_4VREFINT                    CS_CMP0MSEL(0)                 /*!< CMP0 inverting input 1/4 Vrefint */
#define CS_CMP0MSEL_1_2VREFINT                    CS_CMP0MSEL(1)                 /*!< CMP0 inverting input 1/2 Vrefint */
#define CS_CMP0MSEL_3_4VREFINT                    CS_CMP0MSEL(2)                 /*!< CMP0 inverting input 3/4 Vrefint */
#define CS_CMP0MSEL_VREFINT                       CS_CMP0MSEL(3)                 /*!< CMP0 inverting input Vrefint */
#define CS_CMP0MSEL_PA0                           CS_CMP0MSEL(4)                 /*!< CMP0 inverting input PA0 */
#define CS_CMP0MSEL_DAC_OUT                       CS_CMP0MSEL(5)                 /*!< CMP0 inverting input DAC_OUT/PA4 */

/* CMP0 output selection */
#define CS_CMP0OSEL(regval)                       (BITS(13,14) & ((uint32_t)(regval) << 13))
#define CS_CMP0OSEL_NOSELECTION                   CS_CMP0OSEL(0)                 /*!< CMP0 no output selection */
#define CS_CMP0OSEL_TIMER1CI3                     CS_CMP0OSEL(1)                 /*!< CMP0 as TIMER1 channel 3 input capture source */
#define CS_CMP0OSEL_TIMER2CI0                     CS_CMP0OSEL(2)                 /*!< CMP0 as TIMER2 channel 0 input capture source */
#define CS_CMP0OSEL_LPTIMERCI0                    CS_CMP0OSEL(3)                 /*!< CMP0 as LPTIMER channel 0 input capture source */

/* CMP0 output polarity*/
#define CS_CMP0PL(regval)                         (BIT(15) & ((uint32_t)(regval) << 15))
#define CS_CMP0PL_NOL                             CS_CMP0PL(0)                   /*!< CMP0 output not inverted */
#define CS_CMP0PL_INV                             CS_CMP0PL(1)                   /*!< CMP0 output inverted */

/* CMP0 hysteresis */
#define CS_CMP0HST(regval)                        (BITS(16,17) & ((uint32_t)(regval) << 16))
#define CS_CMP0HST_HYSTERESIS_NO                  CS_CMP0HST(0)                  /*!< CMP0 output no hysteresis */
#define CS_CMP0HST_HYSTERESIS_LOW                 CS_CMP0HST(1)                  /*!< CMP0 output low hysteresis */
#define CS_CMP0HST_HYSTERESIS_MEDIUM              CS_CMP0HST(2)                  /*!< CMP0 output medium hysteresis */
#define CS_CMP0HST_HYSTERESIS_HIGH                CS_CMP0HST(3)                  /*!< CMP0 output high hysteresis */

/* CMP0 Blanking suorce */
#define CS_CMP0BLK(regval)                        (BITS(18,20) & ((uint32_t)(regval) << 18))
#define CS_CMP0BLK_NOBLK                          CS_CMP0BLK(0)                  /*!< CMP0 no blanking */
#define CS_CMP0BLK_TIMER1_OC1                     CS_CMP0BLK(1)                  /*!< CMP0 TIMER1 OC1 selected as blanking source */
#define CS_CMP0BLK_TIMER2_OC1                     CS_CMP0BLK(2)                  /*!< CMP0 TIMER2 OC1 selected as blanking source */
#define CS_CMP0BLK_TIMER8_OC1                     CS_CMP0BLK(4)                  /*!< CMP0 TIMER8 OC1 selected as blanking source */
#define CS_CMP0BLK_TIMER11_OC1                    CS_CMP0BLK(5)                  /*!< CMP0 TIMER11 OC1 selected as blanking source */

/* CMP0 bridge enable*/
#define CS_CMP0BEN(regval)                        (BIT(22) & ((uint32_t)(regval) << 22))
#define CS_CMP0BEN_DISABLE                        CS_CMP0BEN(0)                  /*!< CMP0 scaler bridge enable */
#define CS_CMP0BEN_ENABLE                         CS_CMP0BEN(1)                  /*!< CMP0 scaler bridge disable */

/* CMP0 voltage scaler*/
#define CS_CMP0SEN(regval)                        (BIT(23) & ((uint32_t)(regval) << 23))
#define CS_CMP0SEN_DISABLE                        CS_CMP0SEN(0)                  /*!< CMP0 voltage scaler enable */
#define CS_CMP0SEN_ENABLE                         CS_CMP0SEN(1)                  /*!< CMP0 voltage scaler disable */

/* CMP0 output state bit*/
#define CS_CMP0OUT(regval)                        (BIT(30) & ((uint32_t)(regval) << 30))
#define CS_CMP0OUT_DISABLE                        CS_CMP0LK(0)                   /*!< Non-inverting input below inverting input and the output is low */
#define CS_CMP0OUT_ENABLE                         CS_CMP0LK(1)                   /*!< Non-inverting input above inverting input and the output is high */

/* CMP0 lock bit*/
#define CS_CMP0LK(regval)                         (BIT(31) & ((uint32_t)(regval) << 31))
#define CS_CMP0LK_DISABLE                         CS_CMP0LK(0)                   /*!< CMP0 voltage scaler enable */
#define CS_CMP0LK_ENABLE                          CS_CMP0LK(1)                   /*!< CMP0 voltage scaler disable */

/* CMP1 enable bit*/
#define CS_CMP1EN(regval)                         (BIT(0) & ((uint32_t)(regval) << 0))
#define CS_CMP1EN_DISABLE                         CS_CMP1EN(0)                   /*!< CMP1 enable */
#define CS_CMP1EN_ENABLE                          CS_CMP1EN(1)                   /*!< CMP1 disable */

/* CMP1 window mode*/
#define CS_CMP1WEN(regval)                        (BIT(1) & ((uint32_t)(regval) << 1))
#define CS_CMP1WEN_DISABLE                        CS_CMP1WEN(0)                  /*!< Input plus of CMP1 is not connected to CMP0 */
#define CS_CMP1WEN_ENABLE                         CS_CMP1WEN(1)                  /*!< Input plus of CMP1 is connected with input plus of CMP0 */

/* CMP1 mode */
#define CS_CMP1PM(regval)                         (BITS(2,3) & ((uint32_t)(regval) << 2))
#define CS_CMP1PM_HIGHSPEED                       CS_CMP1PM(0)                   /*!< CMP1 mode high speed */
#define CS_CMP1PM_MEDIUMSPEED                     CS_CMP1PM(1)                   /*!< CMP1 mode medium speed */
#define CS_CMP1PM_LOWSPEED                        CS_CMP1PM(3)                   /*!< CMP1 mode low speed */

/* CMP1 minus input */
#define CS_CMP1MSEL(regval)                       (BITS(4,6) & ((uint32_t)(regval) << 4))
#define CS_CMP1MSEL_1_4VREFINT                    CS_CMP1MSEL(0)                 /*!< CMP1 inverting input 1/4 Vrefint */
#define CS_CMP1MSEL_1_2VREFINT                    CS_CMP1MSEL(1)                 /*!< CMP1 inverting input 1/2 Vrefint */
#define CS_CMP1MSEL_3_4VREFINT                    CS_CMP1MSEL(2)                 /*!< CMP1 inverting input 3/4 Vrefint */
#define CS_CMP1MSEL_VREFINT                       CS_CMP1MSEL(3)                 /*!< CMP1 inverting input Vrefint */
#define CS_CMP1MSEL_PA2                           CS_CMP1MSEL(4)                 /*!< CMP1 inverting input PA2 */
#define CS_CMP1MSEL_DAC_OUT                       CS_CMP1MSEL(5)                 /*!< CMP1 inverting input DAC_OUT/PA4 */
#define CS_CMP1MSEL_PB3                           CS_CMP1MSEL(6)                 /*!< CMP1 inverting input PB3*/

/* CMP1 plus input*/
#define CS_CMP1PSEL(regval)                       (BITS(8,10) & ((uint32_t)(regval) << 8))
#define CS_CMP1PSEL_PA3                           CS_CMP1PSEL(0)                 /*!< CMP1 plus input PA3 */
#define CS_CMP1PSEL_PB4                           CS_CMP1PSEL(1)                 /*!< CMP1 plus input PB4 */
#define CS_CMP1PSEL_PB5                           CS_CMP1PSEL(2)                 /*!< CMP1 plus input PB5 */
#define CS_CMP1PSEL_PB6                           CS_CMP1PSEL(3)                 /*!< CMP1 plus input PB6 */
#define CS_CMP1PSEL_PB7                           CS_CMP1PSEL(4)                 /*!< CMP1 plus input PB7 */

/* CMP1 output selection */
#define CS_CMP1OSEL(regval)                       (BITS(13,14) & ((uint32_t)(regval) << 13))
#define CS_CMP1OSEL_NOSELECTION                   CS_CMP1OSEL(0)                 /*!< CMP1 no output selection */
#define CS_CMP1OSEL_TIMER1CI3                     CS_CMP1OSEL(1)                 /*!< CMP1 as TIMER1 channel 3 input capture source */
#define CS_CMP1OSEL_TIMER2CI0                     CS_CMP1OSEL(2)                 /*!< CMP1 as TIMER2 channel 0 input capture source */
#define CS_CMP1OSEL_LPTIMERCI1                    CS_CMP1OSEL(3)                 /*!< CMP1 as LPTIMER channel 1 input capture source */

/* CMP1 output polarity*/
#define CS_CMP1PL(regval)                         (BIT(15) & ((uint32_t)(regval) << 15))
#define CS_CMP1PL_NOL                             CS_CMP1PL(0)                   /*!< CMP1 output not inverted */
#define CS_CMP1PL_INV                             CS_CMP1PL(1)                   /*!< CMP1 output inverted */

/* CMP1 hysteresis */
#define CS_CMP1HST(regval)                        (BITS(16,17) & ((uint32_t)(regval) << 16))
#define CS_CMP1HST_HYSTERESIS_NO                  CS_CMP1HST(0)                  /*!< CMP1 output no hysteresis */
#define CS_CMP1HST_HYSTERESIS_LOW                 CS_CMP1HST(1)                  /*!< CMP1 output low hysteresis */
#define CS_CMP1HST_HYSTERESIS_MEDIUM              CS_CMP1HST(2)                  /*!< CMP1 output medium hysteresis */
#define CS_CMP1HST_HYSTERESIS_HIGH                CS_CMP1HST(3)                  /*!< CMP1 output high hysteresis */

/* CMP1 Blanking suorce */
#define CS_CMP1BLK(regval)                        (BITS(18,20) & ((uint32_t)(regval) << 18))
#define CS_CMP1BLK_NOBLK                          CS_CMP1BLK(0)                  /*!< CMP1 no blanking */
#define CS_CMP1BLK_TIMER1_OC1                     CS_CMP1BLK(1)                  /*!< CMP1 TIMER1 OC1 selected as blanking source */
#define CS_CMP1BLK_TIMER2_OC1                     CS_CMP1BLK(2)                  /*!< CMP1 TIMER2 OC1 selected as blanking source */
#define CS_CMP1BLK_TIMER8_OC1                     CS_CMP1BLK(4)                  /*!< CMP1 TIMER8 OC1 selected as blanking source */
#define CS_CMP1BLK_TIMER11_OC1                    CS_CMP1BLK(5)                  /*!< CMP1 TIMER11 OC1 selected as blanking source */

/* CMP1 bridge enable*/
#define CS_CMP1BEN(regval)                        (BIT(22) & ((uint32_t)(regval) << 22))
#define CS_CMP1BEN_DISABLE                        CS_CMP1BEN(0)                  /*!< CMP1 scaler bridge enable */
#define CS_CMP1BEN_ENABLE                         CS_CMP1BEN(1)                  /*!< CMP1 scaler bridge disable */

/* CMP1 voltage scaler*/
#define CS_CMP1SEN(regval)                        (BIT(23) & ((uint32_t)(regval) << 23))
#define CS_CMP1SEN_DISABLE                        CS_CMP1SEN(0)                  /*!< CMP1 voltage scaler enable */
#define CS_CMP1SEN_ENABLE                         CS_CMP1SEN(1)                  /*!< CMP1 voltage scaler disable */

/* CMP1 output state bit*/
#define CS_CMP1OUT(regval)                        (BIT(30) & ((uint32_t)(regval) << 30))
#define CS_CMP1OUT_DISABLE                        CS_CMP1LK(0)                   /*!< Non-inverting input below inverting input and the output is low */
#define CS_CMP1OUT_ENABLE                         CS_CMP1LK(1)                   /*!< Non-inverting input above inverting input and the output is high */

/* CMP1 lock bit*/
#define CS_CMP1LK(regval)                         (BIT(31) & ((uint32_t)(regval) << 31))
#define CS_CMP1LK_DISABLE                         CS_CMP1LK(0)                   /*!< CMP1 voltage scaler enable */
#define CS_CMP1LK_ENABLE                          CS_CMP1LK(1)                   /*!< CMP1 voltage scaler disable */

/* function declarations */
/* initialization functions */
/* deinitialize comparator  */
void cmp_deinit(uint32_t cmp_periph);
/* initialize comparator mode */
void cmp_mode_init(uint32_t cmp_periph, operating_mode_enum operating_mode, inverting_input_enum inverting_input, cmp_hysteresis_enum output_hysteresis);
/* selecte the plus input for CMP1 */
void cmp1_plus_selection(CMP1_plus_input_enum plus_input);
/* initialize comparator output */
void cmp_output_init(uint32_t cmp_periph, cmp_output_enum output_selection, cmp_output_inv_enum output_polarity);
/* initialize comparator blanking function */
void cmp_blanking_init(uint32_t cmp_periph, blanking_source_enum blanking_source_selection);


/* enable functions */
/* enable comparator */
void cmp_enable(uint32_t cmp_periph);
/* disable comparator */
void cmp_disable(uint32_t cmp_periph);
/* enable the window mode */
void cmp_window_enable(void);
/* disable the window mode */
void cmp_window_disable(void);
/* enable the voltage scaler */
void cmp_voltage_scaler_enable(uint32_t cmp_periph);
/* disable the voltage scaler */
void cmp_voltage_scaler_disable(uint32_t cmp_periph);
/* enable the scaler bridge */
void cmp_scaler_bridge_enable(uint32_t cmp_periph);
/* disable the scaler bridge */
void cmp_scaler_bridge_disable(uint32_t cmp_periph);
/* lock the comparator */
void cmp_lock_enable(uint32_t cmp_periph);

/* output functions */
/* get output level */
uint32_t cmp_output_level_get(uint32_t cmp_periph);

#endif /* GD32L23X_CMP_H */
