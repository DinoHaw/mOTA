/*!
    \file    gd32l23x_lpuart.h
    \brief   definitions for the LPUART

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

#ifndef GD32L23X_LPUART_H
#define GD32L23X_LPUART_H

#include "gd32l23x.h"

/* LPUART definitions */
#define LPUART                        LPUART_BASE

/* registers definitions */
#define LPUART_CTL0                   REG32(LPUART + 0x00000000U)         /*!< LPUART control register 0 */
#define LPUART_CTL1                   REG32(LPUART + 0x00000004U)         /*!< LPUART control register 1 */
#define LPUART_CTL2                   REG32(LPUART + 0x00000008U)         /*!< LPUART control register 2 */
#define LPUART_BAUD                   REG32(LPUART + 0x0000000CU)         /*!< LPUART baud rate register */
#define LPUART_CMD                    REG32(LPUART + 0x00000018U)         /*!< LPUART command register */
#define LPUART_STAT                   REG32(LPUART + 0x0000001CU)         /*!< LPUART status register */
#define LPUART_INTC                   REG32(LPUART + 0x00000020U)         /*!< LPUART status clear register */
#define LPUART_RDATA                  REG32(LPUART + 0x00000024U)         /*!< LPUART receive data register */
#define LPUART_TDATA                  REG32(LPUART + 0x00000028U)         /*!< LPUART transmit data register */
#define LPUART_CHC                    REG32(LPUART + 0x000000C0U)         /*!< LPUART coherence control register */

/* bits definitions */
/* LPUART CTL0 */
#define LPUART_CTL0_UEN               BIT(0)                              /*!< LPUART enable */
#define LPUART_CTL0_UESM              BIT(1)                              /*!< LPUART enable in deep-sleep mode */
#define LPUART_CTL0_REN               BIT(2)                              /*!< receiver enable */
#define LPUART_CTL0_TEN               BIT(3)                              /*!< transmitter enable */
#define LPUART_CTL0_IDLEIE            BIT(4)                              /*!< idle line detected interrupt enable */
#define LPUART_CTL0_RBNEIE            BIT(5)                              /*!< read data buffer not empty interrupt and overrun error interrupt enable */
#define LPUART_CTL0_TCIE              BIT(6)                              /*!< transmission complete interrupt enable */
#define LPUART_CTL0_TBEIE             BIT(7)                              /*!< transmitter register empty interrupt enable */
#define LPUART_CTL0_PERRIE            BIT(8)                              /*!< parity error interrupt enable */
#define LPUART_CTL0_PM                BIT(9)                              /*!< parity mode */
#define LPUART_CTL0_PCEN              BIT(10)                             /*!< parity control enable */
#define LPUART_CTL0_WM                BIT(11)                             /*!< wakeup method in mute mode */
#define LPUART_CTL0_WL0               BIT(12)                             /*!< word length low bit*/
#define LPUART_CTL0_MEN               BIT(13)                             /*!< mute mode enable */
#define LPUART_CTL0_AMIE              BIT(14)                             /*!< address match interrupt enable */
#define LPUART_CTL0_DED               BITS(16,20)                         /*!< driver enable deassertion time */
#define LPUART_CTL0_DEA               BITS(21,25)                         /*!< driver enable assertion time */
#define LPUART_CTL0_WL1               BIT(28)                             /*!< word length high bit*/

/* LPUART CTL1 */
#define LPUART_CTL1_ADDM              BIT(4)                              /*!< address detection mode */
#define LPUART_CTL1_STB               BITS(12,13)                         /*!< stop bits length */
#define LPUART_CTL1_STRP              BIT(15)                             /*!< swap TX/RX pins */
#define LPUART_CTL1_RINV              BIT(16)                             /*!< RX pin level inversion */
#define LPUART_CTL1_TINV              BIT(17)                             /*!< TX pin level inversion */
#define LPUART_CTL1_DINV              BIT(18)                             /*!< data bit level inversion */
#define LPUART_CTL1_MSBF              BIT(19)                             /*!< most significant bit first */
#define LPUART_CTL1_ADDR              BITS(24,31)                         /*!< address of the LPUART terminal */

/* LPUART CTL2 */
#define LPUART_CTL2_ERRIE             BIT(0)                              /*!< error interrupt enable in multibuffer communication */
#define LPUART_CTL2_HDEN              BIT(3)                              /*!< half-duplex enable */
#define LPUART_CTL2_DENR              BIT(6)                              /*!< DMA enable for reception */
#define LPUART_CTL2_DENT              BIT(7)                              /*!< DMA enable for transmission */
#define LPUART_CTL2_RTSEN             BIT(8)                              /*!< RTS enable */
#define LPUART_CTL2_CTSEN             BIT(9)                              /*!< CTS enable */
#define LPUART_CTL2_CTSIE             BIT(10)                             /*!< CTS interrupt enable */
#define LPUART_CTL2_OVRD              BIT(12)                             /*!< overrun disable */
#define LPUART_CTL2_DDRE              BIT(13)                             /*!< disable DMA on reception error */
#define LPUART_CTL2_DEM               BIT(14)                             /*!< driver enable mode */
#define LPUART_CTL2_DEP               BIT(15)                             /*!< driver enable polarity mode */
#define LPUART_CTL2_WUM               BITS(20,21)                         /*!< wakeup mode from deep-sleep mode */
#define LPUART_CTL2_WUIE              BIT(22)                             /*!< wakeup from deep-sleep mode interrupt enable */
#define LPUART_CTL2_UCESM             BIT(23)                             /*!< LPUART clock enable in Deep-sleep mode */

/* LPUART BAUD */
#define LPUART_BAUD_BRR               BITS(0,19)                          /*!< baud-rate divider */

/* LPUART CMD */
#define LPUART_CMD_MMCMD              BIT(2)                              /*!< mute mode command */
#define LPUART_CMD_RXFCMD             BIT(3)                              /*!< receive data flush command */

/* LPUART STAT */
#define LPUART_STAT_PERR              BIT(0)                              /*!< parity error flag */
#define LPUART_STAT_FERR              BIT(1)                              /*!< frame error flag */
#define LPUART_STAT_NERR              BIT(2)                              /*!< noise error flag */
#define LPUART_STAT_ORERR             BIT(3)                              /*!< overrun error */
#define LPUART_STAT_IDLEF             BIT(4)                              /*!< idle line detected flag */
#define LPUART_STAT_RBNE              BIT(5)                              /*!< read data buffer not empty */
#define LPUART_STAT_TC                BIT(6)                              /*!< transmission completed */
#define LPUART_STAT_TBE               BIT(7)                              /*!< transmit data register empty */
#define LPUART_STAT_CTSF              BIT(9)                              /*!< CTS change flag */
#define LPUART_STAT_CTS               BIT(10)                             /*!< CTS level */
#define LPUART_STAT_BSY               BIT(16)                             /*!< busy flag */
#define LPUART_STAT_AMF               BIT(17)                             /*!< address match flag */
#define LPUART_STAT_RWU               BIT(19)                             /*!< receiver wakeup from mute mode */
#define LPUART_STAT_WUF               BIT(20)                             /*!< wakeup from deep-sleep mode flag */
#define LPUART_STAT_TEA               BIT(21)                             /*!< transmit enable acknowledge flag */
#define LPUART_STAT_REA               BIT(22)                             /*!< receive enable acknowledge flag */

/* LPUART INTC */
#define LPUART_INTC_PEC               BIT(0)                              /*!< parity error clear */
#define LPUART_INTC_FEC               BIT(1)                              /*!< frame error flag clear */
#define LPUART_INTC_NEC               BIT(2)                              /*!< noise detected clear */
#define LPUART_INTC_OREC              BIT(3)                              /*!< overrun error clear */
#define LPUART_INTC_IDLEC             BIT(4)                              /*!< idle line detected clear */
#define LPUART_INTC_TCC               BIT(6)                              /*!< transmission complete clear */
#define LPUART_INTC_CTSC              BIT(9)                              /*!< CTS change clear */
#define LPUART_INTC_AMC               BIT(17)                             /*!< address match clear */
#define LPUART_INTC_WUC               BIT(20)                             /*!< wakeup from deep-sleep mode clear */

/* LPUART RDATA */
#define LPUART_RDATA_RDATA            BITS(0,8)                           /*!< receive data value */

/* LPUART TDATA */
#define LPUART_TDATA_TDATA            BITS(0,8)                           /*!< transmit data value */

/* LPUART CHC */
#define LPUART_CHC_HCM                BIT(0)                              /*!< hardware flow control coherence mode */
#define LPUART_CHC_EPERR              BIT(8)                              /*!< early parity error flag */

/* constants definitions */
/* define the LPUART bit position and its register index offset */
#define LPUART_REGIDX_BIT(regidx, bitpos)   (((uint32_t)(regidx) << 6) | (uint32_t)(bitpos))
#define LPUART_REG_VAL(lpuart, offset)      (REG32((lpuart) + (((uint32_t)(offset) & 0x0000FFFFU) >> 6)))
#define LPUART_BIT_POS(val)                 ((uint32_t)(val) & 0x0000001FU)
#define LPUART_REGIDX_BIT2(regidx, bitpos, regidx2, bitpos2)   (((uint32_t)(regidx2) << 22) | (uint32_t)((bitpos2) << 16)\
        | (((uint32_t)(regidx) << 6) | (uint32_t)(bitpos)))
#define LPUART_REG_VAL2(lpuart, offset)     (REG32((lpuart) + ((uint32_t)(offset) >> 22)))
#define LPUART_BIT_POS2(val)                (((uint32_t)(val) & 0x001F0000U) >> 16)

/* register offset */
#define LPUART_CTL0_REG_OFFSET             ((uint32_t)0x00000000U)         /*!< CTL0 register offset */
#define LPUART_CTL1_REG_OFFSET             ((uint32_t)0x00000004U)         /*!< CTL1 register offset */
#define LPUART_CTL2_REG_OFFSET             ((uint32_t)0x00000008U)         /*!< CTL2 register offset */
#define LPUART_STAT_REG_OFFSET             ((uint32_t)0x0000001CU)         /*!< STAT register offset */
#define LPUART_CHC_REG_OFFSET              ((uint32_t)0x000000C0U)         /*!< CHC register offset */

/* LPUART flags */
typedef enum {
    /* flags in STAT register */
    LPUART_FLAG_REA = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 22U),      /*!< receive enable acknowledge flag */
    LPUART_FLAG_TEA = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 21U),      /*!< transmit enable acknowledge flag */
    LPUART_FLAG_WU = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 20U),       /*!< wakeup from Deep-sleep mode flag */
    LPUART_FLAG_RWU = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 19U),      /*!< receiver wakeup from mute mode */
    LPUART_FLAG_AM = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 17U),       /*!< ADDR match flag */
    LPUART_FLAG_BSY = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 16U),      /*!< busy flag */
    LPUART_FLAG_CTS = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 10U),      /*!< CTS level */
    LPUART_FLAG_CTSF = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 9U),      /*!< CTS change flag */
    LPUART_FLAG_TBE = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 7U),       /*!< transmit data buffer empty */
    LPUART_FLAG_TC = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 6U),        /*!< transmission complete */
    LPUART_FLAG_RBNE = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 5U),      /*!< read data buffer not empty */
    LPUART_FLAG_IDLE = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 4U),      /*!< IDLE line detected flag */
    LPUART_FLAG_ORERR = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 3U),     /*!< overrun error */
    LPUART_FLAG_NERR = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 2U),      /*!< noise error flag */
    LPUART_FLAG_FERR = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 1U),      /*!< frame error flag */
    LPUART_FLAG_PERR = LPUART_REGIDX_BIT(LPUART_STAT_REG_OFFSET, 0U),      /*!< parity error flag */
    /* flags in CHC register */
    LPUART_FLAG_EPERR = LPUART_REGIDX_BIT(LPUART_CHC_REG_OFFSET, 8U),      /*!< early parity error flag */
} lpuart_flag_enum;

/* LPUART interrupt flags */
typedef enum {
    /* interrupt flags in CTL0 register */
    LPUART_INT_FLAG_AM = LPUART_REGIDX_BIT2(LPUART_CTL0_REG_OFFSET, 14U, LPUART_STAT_REG_OFFSET, 17U),       /*!< address match interrupt and flag */
    LPUART_INT_FLAG_PERR = LPUART_REGIDX_BIT2(LPUART_CTL0_REG_OFFSET, 8U, LPUART_STAT_REG_OFFSET, 0U),       /*!< parity error interrupt and flag */
    LPUART_INT_FLAG_TBE = LPUART_REGIDX_BIT2(LPUART_CTL0_REG_OFFSET, 7U, LPUART_STAT_REG_OFFSET, 7U),        /*!< transmitter buffer empty interrupt and flag */
    LPUART_INT_FLAG_TC = LPUART_REGIDX_BIT2(LPUART_CTL0_REG_OFFSET, 6U, LPUART_STAT_REG_OFFSET, 6U),         /*!< transmission complete interrupt and flag */
    LPUART_INT_FLAG_RBNE = LPUART_REGIDX_BIT2(LPUART_CTL0_REG_OFFSET, 5U, LPUART_STAT_REG_OFFSET, 5U),       /*!< read data buffer not empty interrupt and flag */
    LPUART_INT_FLAG_RBNE_ORERR = LPUART_REGIDX_BIT2(LPUART_CTL0_REG_OFFSET, 5U, LPUART_STAT_REG_OFFSET, 3U), /*!< read data buffer not empty interrupt and overrun error flag */
    LPUART_INT_FLAG_IDLE = LPUART_REGIDX_BIT2(LPUART_CTL0_REG_OFFSET, 4U, LPUART_STAT_REG_OFFSET, 4U),       /*!< IDLE line detected interrupt and flag */
    /* interrupt flags in CTL2 register */
    LPUART_INT_FLAG_WU = LPUART_REGIDX_BIT2(LPUART_CTL2_REG_OFFSET, 22U, LPUART_STAT_REG_OFFSET, 20U),       /*!< wakeup from deep-sleep mode interrupt and flag */
    LPUART_INT_FLAG_CTS = LPUART_REGIDX_BIT2(LPUART_CTL2_REG_OFFSET, 10U, LPUART_STAT_REG_OFFSET, 9U),       /*!< CTS interrupt and flag */
    LPUART_INT_FLAG_ERR_NERR = LPUART_REGIDX_BIT2(LPUART_CTL2_REG_OFFSET, 0U, LPUART_STAT_REG_OFFSET, 2U),   /*!< error interrupt and noise error flag */
    LPUART_INT_FLAG_ERR_ORERR = LPUART_REGIDX_BIT2(LPUART_CTL2_REG_OFFSET, 0U, LPUART_STAT_REG_OFFSET, 3U),  /*!< error interrupt and overrun error flag */
    LPUART_INT_FLAG_ERR_FERR = LPUART_REGIDX_BIT2(LPUART_CTL2_REG_OFFSET, 0U, LPUART_STAT_REG_OFFSET, 1U),   /*!< error interrupt and frame error flag */
} lpuart_interrupt_flag_enum;

/* LPUART interrupt enable or disable */
typedef enum {
    /* interrupt in CTL0 register */
    LPUART_INT_AM = LPUART_REGIDX_BIT(LPUART_CTL0_REG_OFFSET, 14U),         /*!< address match interrupt */
    LPUART_INT_PERR = LPUART_REGIDX_BIT(LPUART_CTL0_REG_OFFSET, 8U),        /*!< parity error interrupt */
    LPUART_INT_TBE = LPUART_REGIDX_BIT(LPUART_CTL0_REG_OFFSET, 7U),         /*!< transmitter buffer empty interrupt */
    LPUART_INT_TC = LPUART_REGIDX_BIT(LPUART_CTL0_REG_OFFSET, 6U),          /*!< transmission complete interrupt */
    LPUART_INT_RBNE = LPUART_REGIDX_BIT(LPUART_CTL0_REG_OFFSET, 5U),        /*!< read data buffer not empty interrupt and overrun error interrupt */
    LPUART_INT_IDLE = LPUART_REGIDX_BIT(LPUART_CTL0_REG_OFFSET, 4U),        /*!< IDLE line detected interrupt */
    /* interrupt in CTL2 register */
    LPUART_INT_WU = LPUART_REGIDX_BIT(LPUART_CTL2_REG_OFFSET, 22U),         /*!< wakeup from deep-sleep mode interrupt */
    LPUART_INT_CTS = LPUART_REGIDX_BIT(LPUART_CTL2_REG_OFFSET, 10U),        /*!< CTS interrupt */
    LPUART_INT_ERR = LPUART_REGIDX_BIT(LPUART_CTL2_REG_OFFSET, 0U),         /*!< error interrupt */
} lpuart_interrupt_enum;

/* LPUART invert configure */
typedef enum {
    /* data bit level inversion */
    LPUART_DINV_ENABLE = 0,                                          /*!< data bit level inversion */
    LPUART_DINV_DISABLE,                                             /*!< data bit level not inversion */
    /* TX pin level inversion */
    LPUART_TXPIN_ENABLE,                                             /*!< TX pin level inversion */
    LPUART_TXPIN_DISABLE,                                            /*!< TX pin level not inversion */
    /* RX pin level inversion */
    LPUART_RXPIN_ENABLE,                                             /*!< RX pin level inversion */
    LPUART_RXPIN_DISABLE,                                            /*!< RX pin level not inversion */
    /* swap TX/RX pins */
    LPUART_SWAP_ENABLE,                                              /*!< swap TX/RX pins */
    LPUART_SWAP_DISABLE,                                             /*!< not swap TX/RX pins */
} lpuart_invert_enum;

/* LPUART receiver configure */
#define CTL0_REN(regval)              (BIT(2) & ((uint32_t)(regval) << 2))
#define LPUART_RECEIVE_ENABLE         CTL0_REN(1)                    /*!< enable receiver */
#define LPUART_RECEIVE_DISABLE        CTL0_REN(0)                    /*!< disable receiver */

/* LPUART transmitter configure */
#define CTL0_TEN(regval)              (BIT(3) & ((uint32_t)(regval) << 3))
#define LPUART_TRANSMIT_ENABLE        CTL0_TEN(1)                    /*!< enable transmitter */
#define LPUART_TRANSMIT_DISABLE       CTL0_TEN(0)                    /*!< disable transmitter */

/* LPUART parity bits definitions */
#define CTL0_PM(regval)               (BITS(9,10) & ((uint32_t)(regval) << 9))
#define LPUART_PM_NONE                CTL0_PM(0)                     /*!< no parity */
#define LPUART_PM_EVEN                CTL0_PM(2)                     /*!< even parity */
#define LPUART_PM_ODD                 CTL0_PM(3)                     /*!< odd parity */

/* LPUART wakeup method in mute mode */
#define CTL0_WM(regval)               (BIT(11) & ((uint32_t)(regval) << 11))
#define LPUART_WM_IDLE                CTL0_WM(0)                     /*!< idle line */
#define LPUART_WM_ADDR                CTL0_WM(1)                     /*!< address match */

/* LPUART word length definitions */
#define CTL0_WL01(regval1, regval2)   ((BIT(12) & ((uint32_t)(regval1) << 12))|(BIT(28) & ((uint32_t)(regval2) << 28)))
#define LPUART_WL_7BIT                CTL0_WL01(0,1)                 /*!< 7 bits */
#define LPUART_WL_8BIT                CTL0_WL01(0,0)                 /*!< 8 bits */
#define LPUART_WL_9BIT                CTL0_WL01(1,0)                 /*!< 9 bits */

/* LPUART address detection mode */
#define CTL1_ADDM(regval)             (BIT(4) & ((uint32_t)(regval) << 4))
#define LPUART_ADDM_4BIT              CTL1_ADDM(0)                   /*!< 4-bit address detection */
#define LPUART_ADDM_FULLBIT           CTL1_ADDM(1)                   /*!< full-bit address detection */

/* LPUART stop bits definitions */
#define CTL1_STB(regval)              (BITS(12,13) & ((uint32_t)(regval) << 12))
#define LPUART_STB_1BIT               CTL1_STB(0)                    /*!< 1 bit */
#define LPUART_STB_2BIT               CTL1_STB(2)                    /*!< 2 bits */

/* LPUART data is transmitted/received with the LSB/MSB first */
#define CTL1_MSBF(regval)             (BIT(19) & ((uint32_t)(regval) << 19))
#define LPUART_MSBF_LSB               CTL1_MSBF(0)                   /*!< LSB first */
#define LPUART_MSBF_MSB               CTL1_MSBF(1)                   /*!< MSB first */

/* DMA enable for reception */
#define CTL2_DENR(regval)             (BIT(6) & ((uint32_t)(regval) << 6))
#define LPUART_DENR_ENABLE            CTL2_DENR(1)                   /*!< enable for reception */
#define LPUART_DENR_DISABLE           CTL2_DENR(0)                   /*!< disable for reception */

/* DMA enable for transmission */
#define CTL2_DENT(regval)             (BIT(7) & ((uint32_t)(regval) << 7))
#define LPUART_DENT_ENABLE            CTL2_DENT(1)                   /*!< enable for transmission */
#define LPUART_DENT_DISABLE           CTL2_DENT(0)                   /*!< disable for transmission */

/* LPUART RTS hardware flow control configure */
#define CTL2_RTSEN(regval)            (BIT(8) & ((uint32_t)(regval) << 8))
#define LPUART_RTS_ENABLE             CTL2_RTSEN(1)                  /*!< RTS hardware flow control enabled */
#define LPUART_RTS_DISABLE            CTL2_RTSEN(0)                  /*!< RTS hardware flow control disabled */

/* LPUART CTS hardware flow control configure */
#define CTL2_CTSEN(regval)            (BIT(9) & ((uint32_t)(regval) << 9))
#define LPUART_CTS_ENABLE             CTL2_CTSEN(1)                  /*!< CTS hardware flow control enabled */
#define LPUART_CTS_DISABLE            CTL2_CTSEN(0)                  /*!< CTS hardware flow control disabled */

/* LPUART driver enable polarity mode */
#define CTL2_DEP(regval)              (BIT(15) & ((uint32_t)(regval) << 15))
#define LPUART_DEP_HIGH               CTL2_DEP(0)                    /*!< DE signal is active high */
#define LPUART_DEP_LOW                CTL2_DEP(1)                    /*!< DE signal is active low */

/* LPUART wakeup mode from deep-sleep mode */
#define CTL2_WUM(regval)              (BITS(20,21) & ((uint32_t)(regval) << 20))
#define LPUART_WUM_ADDR               CTL2_WUM(0)                    /*!< WUF active on address match */
#define LPUART_WUM_STARTB             CTL2_WUM(2)                    /*!< WUF active on start bit */
#define LPUART_WUM_RBNE               CTL2_WUM(3)                    /*!< WUF active on RBNE */

/* LPUART hardware flow control coherence mode */
#define CHC_HCM(regval)               (BIT(0) & ((uint32_t)(regval) << 0))
#define LPUART_HCM_NONE               CHC_HCM(0)                     /*!< nRTS signal equals to the rxne status register */
#define LPUART_HCM_EN                 CHC_HCM(1)                     /*!< nRTS signal is set when the last data bit has been sampled */

/* function declarations */
/* initialization functions */
/* reset LPUART */
void lpuart_deinit(void);
/* configure LPUART baud rate value */
void lpuart_baudrate_set(uint32_t baudval);
/* configure LPUART parity */
void lpuart_parity_config(uint32_t paritycfg);
/* configure LPUART word length */
void lpuart_word_length_set(uint32_t wlen);
/* configure LPUART stop bit length */
void lpuart_stop_bit_set(uint32_t stblen);
/* enable LPUART */
void lpuart_enable(void);
/* disable LPUART */
void lpuart_disable(void);
/* configure LPUART transmitter */
void lpuart_transmit_config(uint32_t txconfig);
/* configure LPUART receiver */
void lpuart_receive_config(uint32_t rxconfig);

/* LPUART normal mode communication */
/* data is transmitted/received with the LSB/MSB first */
void lpuart_data_first_config(uint32_t msbf);
/* configure LPUART inverted */
void lpuart_invert_config(lpuart_invert_enum invertpara);
/* enable the LPUART overrun function */
void lpuart_overrun_enable(void);
/* disable the LPUART overrun function */
void lpuart_overrun_disable(void);
/* LPUART transmit data function */
void lpuart_data_transmit(uint32_t data);
/* LPUART receive data function */
uint16_t lpuart_data_receive(void);
/* enable LPUART command */
void lpuart_command_enable(uint32_t cmdtype);

/* multi-processor communication */
/* configure address of the LPUART */
void lpuart_address_config(uint8_t addr);
/* configure address detection mode */
void lpuart_address_detection_mode_config(uint32_t addmod);
/* enable mute mode */
void lpuart_mute_mode_enable(void);
/* disable mute mode */
void lpuart_mute_mode_disable(void);
/* configure wakeup method in mute mode */
void lpuart_mute_mode_wakeup_config(uint32_t wmethod);

/* half-duplex communication */
/* enable half-duplex mode */
void lpuart_halfduplex_enable(void);
/* disable half-duplex mode */
void lpuart_halfduplex_disable(void);

/* hardware flow communication */
/* configure hardware flow control RTS */
void lpuart_hardware_flow_rts_config(uint32_t rtsconfig);
/* configure hardware flow control CTS */
void lpuart_hardware_flow_cts_config(uint32_t ctsconfig);

/* coherence control */
/* configure hardware flow control coherence mode */
void lpuart_hardware_flow_coherence_config(uint32_t hcm);

/* enable RS485 driver */
void lpuart_rs485_driver_enable(void);
/* disable RS485 driver */
void lpuart_rs485_driver_disable(void);
/* configure driver enable assertion time */
void lpuart_driver_assertime_config(uint32_t deatime);
/* configure driver enable de-assertion time */
void lpuart_driver_deassertime_config(uint32_t dedtime);
/* configure driver enable polarity mode */
void lpuart_depolarity_config(uint32_t dep);

/* LPUART DMA */
/* configure LPUART DMA for reception */
void lpuart_dma_receive_config(uint32_t dmacmd);
/* configure LPUART DMA for transmission */
void lpuart_dma_transmit_config(uint32_t dmacmd);
/* disable DMA on reception error */
void lpuart_reception_error_dma_disable(void);
/* enable DMA on reception error */
void lpuart_reception_error_dma_enable(void);

/* enable LPUART to wakeup the mcu from deep-sleep mode */
void lpuart_wakeup_enable(void);
/* disable LPUART to wakeup the mcu from deep-sleep mode */
void lpuart_wakeup_disable(void);
/* configure the LPUART wakeup mode from deep-sleep mode */
void lpuart_wakeup_mode_config(uint32_t wum);

/* flag & interrupt functions */
/* get flag in STAT/CHC register */
FlagStatus lpuart_flag_get(lpuart_flag_enum flag);
/* clear LPUART status */
void lpuart_flag_clear(lpuart_flag_enum flag);
/* enable LPUART interrupt */
void lpuart_interrupt_enable(lpuart_interrupt_enum interrupt);
/* disable LPUART interrupt */
void lpuart_interrupt_disable(lpuart_interrupt_enum interrupt);
/* get LPUART interrupt and flag status */
FlagStatus lpuart_interrupt_flag_get(lpuart_interrupt_flag_enum int_flag);
/* clear LPUART interrupt flag */
void lpuart_interrupt_flag_clear(lpuart_interrupt_flag_enum int_flag);

#endif /* GD32L23X_LPUART_H */
