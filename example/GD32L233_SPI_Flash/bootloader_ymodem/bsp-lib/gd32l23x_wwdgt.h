/*!
    \file    gd32l23x_wwdgt.h
    \brief   definitions for the WWDGT

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


#ifndef GD32L23X_WWDGT_H
#define GD32L23X_WWDGT_H

#include "gd32l23x.h"

/* WWDGT definitions */
#define WWDGT                       WWDGT_BASE

/* registers definitions */
#define WWDGT_CTL                   REG32((WWDGT) + 0x00000000U)              /*!< WWDGT control register */
#define WWDGT_CFG                   REG32((WWDGT) + 0x00000004U)              /*!< WWDGT configuration register */
#define WWDGT_STAT                  REG32((WWDGT) + 0x00000008U)              /*!< WWDGT status register */

/* bits definitions */
/* WWDGT_CTL */
#define WWDGT_CTL_CNT               BITS(0,6)                                 /*!< WWDGT counter value */
#define WWDGT_CTL_WDGTEN            BIT(7)                                    /*!< WWDGT counter enable */

/* WWDGT_CFG */
#define WWDGT_CFG_WIN               BITS(0,6)                                 /*!< WWDGT counter window value */
#define WWDGT_CFG_PSC_0_1           BITS(7,8)                                 /*!< WWDGT prescaler divider value low bit */
#define WWDGT_CFG_EWIE              BIT(9)                                    /*!< WWDGT early wakeup interrupt enable */
#define WWDGT_CFG_PSC_2_3           BITS(16,17)                               /*!< WWDGT prescaler divider value high bit */

/* WWDGT_STAT */
#define WWDGT_STAT_EWIF             BIT(0)                                    /*!< WWDGT early wakeup interrupt flag */

/* constants definitions */
#define CFG_PSC(regval1, regval2)   ((BITS(7,8) & ((uint32_t)(regval1) << 7U)) | (BITS(16, 17) & ((regval2) << 16)))
#define WWDGT_CFG_PSC_DIV1          ((uint32_t)CFG_PSC(0,0))                  /*!< the time base of WWDGT = (PCLK1/4096)/1 */
#define WWDGT_CFG_PSC_DIV2          ((uint32_t)CFG_PSC(1,0))                  /*!< the time base of WWDGT = (PCLK1/4096)/2 */
#define WWDGT_CFG_PSC_DIV4          ((uint32_t)CFG_PSC(2,0))                  /*!< the time base of WWDGT = (PCLK1/4096)/4 */
#define WWDGT_CFG_PSC_DIV8          ((uint32_t)CFG_PSC(3,0))                  /*!< the time base of WWDGT = (PCLK1/4096)/8 */
#define WWDGT_CFG_PSC_DIV16         ((uint32_t)CFG_PSC(0,1))                  /*!< the time base of WWDGT = (PCLK1/4096)/16 */
#define WWDGT_CFG_PSC_DIV32         ((uint32_t)CFG_PSC(1,1))                  /*!< the time base of WWDGT = (PCLK1/4096)/32 */
#define WWDGT_CFG_PSC_DIV64         ((uint32_t)CFG_PSC(2,1))                  /*!< the time base of WWDGT = (PCLK1/4096)/64 */
#define WWDGT_CFG_PSC_DIV128        ((uint32_t)CFG_PSC(3,1))                  /*!< the time base of WWDGT = (PCLK1/4096)/128 */
#define WWDGT_CFG_PSC_DIV256        ((uint32_t)CFG_PSC(0,2))                  /*!< the time base of WWDGT = (PCLK1/4096)/256 */
#define WWDGT_CFG_PSC_DIV512        ((uint32_t)CFG_PSC(1,2))                  /*!< the time base of WWDGT = (PCLK1/4096)/512 */
#define WWDGT_CFG_PSC_DIV1024       ((uint32_t)CFG_PSC(2,2))                  /*!< the time base of WWDGT = (PCLK1/4096)/1024 */
#define WWDGT_CFG_PSC_DIV2048       ((uint32_t)CFG_PSC(3,2))                  /*!< the time base of WWDGT = (PCLK1/4096)/2048 */
#define WWDGT_CFG_PSC_DIV4096       ((uint32_t)CFG_PSC(0,3))                  /*!< the time base of WWDGT = (PCLK1/4096)/4096 */
#define WWDGT_CFG_PSC_DIV8192       ((uint32_t)CFG_PSC(1,3))                  /*!< the time base of WWDGT = (PCLK1/4096)/8192 */

/* function declarations */
/* reset the WWDGT configuration */
void wwdgt_deinit(void);
/* start the WWDGT counter */
void wwdgt_enable(void);

/* configure the WWDGT counter prescaler value */
void wwdgt_prescaler_value_config(uint16_t prescaler);
/* configure the WWDGT counter window value */
void wwdgt_window_value_config(uint16_t window);
/* configure the WWDGT counter value */
void wwdgt_counter_update(uint16_t counter_value);
/* configure counter value, window value, and prescaler divider value */
void wwdgt_config(uint16_t counter, uint16_t window, uint32_t prescaler);

/* enable early wakeup interrupt of WWDGT */
void wwdgt_interrupt_enable(void);
/* check early wakeup interrupt state of WWDGT */
FlagStatus wwdgt_flag_get(void);
/* clear early wakeup interrupt state of WWDGT */
void wwdgt_flag_clear(void);

#endif /* GD32L23X_WWDGT_H */
