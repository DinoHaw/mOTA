/*!
    \file    gd32l23x_vref.h
    \brief   definitions for the VREF

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

#ifndef GD32L23X_VREF_H
#define GD32L23X_VREF_H

#include "gd32l23x.h"

/* VREF definitions */
#define VREF                      VREF_BASE                      /*!< VREF base address */

/* registers definitions */
#define VREF_CS                   REG32(VREF + 0x00000000U)      /*!< VREF Control and status register */
#define VREF_CALIB                REG32(VREF + 0x00000004U)      /*!< VREF Calibration register */

/* bits definitions */
/* VREF_CS */
#define VREF_CS_VREFEN            BIT(0)                         /*!< VREF enable */
#define VREF_CS_HIPM              BIT(1)                         /*!< High impedance mode */
#define VREF_CS_VREFRDY           BIT(3)                         /*!< VREF ready */

/* VREF_CALIB */
#define VREF_CALIB_VREFCAL        BITS(0,5)                      /*!< VREF calibration */

/* VREF bit devinitions */
#define VREF_EN                      VREF_CS_VREFEN              /*!< VREF enable */
#define VREF_HIGH_IMPEDANCE_MODE     VREF_CS_HIPM                /*!< High impedance mode */
#define VREF_RDY                     VREF_CS_VREFRDY             /*!< VREF ready */

/* function declarations */
/* deinitialize the VREF */
void vref_deinit(void);
/* enable VREF */
void vref_enable(void);
/* disable VREF */
void vref_disable(void);
/* enable VREF high impendance mode */
void vref_high_impedance_mode_enable(void);
/* disable VREF high impendance mode */
void vref_high_impedance_mode_disable(void);
/* get the status of VREF */
FlagStatus vref_status_get(void);
/* set the calibration value of VREF */
void syscfg_vref_calib_value_set(uint8_t value);
/* get the calibration value of VREF */
uint8_t syscfg_vref_calib_value_get(void);

#endif /* GD32L23X_VREF_H */
