/*!
    \file    gd32l23x_lpuart.c
    \brief   LPUART driver

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

#include "gd32l23x_lpuart.h"

/*!
    \brief      reset LPUART
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_deinit(void)
{
    rcu_periph_reset_enable(RCU_LPUARTRST);
    rcu_periph_reset_disable(RCU_LPUARTRST);
}

/*!
    \brief      configure LPUART baud rate value
    \param[in]  baudval: baud rate value
    \param[out] none
    \retval     none
*/
void lpuart_baudrate_set(uint32_t baudval)
{
    uint32_t lpuclk = 0U, lpudiv = 0U;
    lpuclk = rcu_clock_freq_get(CK_LPUART);

    lpudiv = (lpuclk / 100U * 256U + baudval / 200U) / (baudval / 100U);

    LPUART_BAUD = (LPUART_BAUD_BRR & lpudiv);
}

/*!
    \brief      configure LPUART parity
    \param[in]  paritycfg: LPUART parity configure
                only one parameter can be selected which is shown as below:
      \arg        LPUART_PM_NONE: no parity
      \arg        LPUART_PM_ODD: odd parity
      \arg        LPUART_PM_EVEN: even parity
    \param[out] none
    \retval     none
*/
void lpuart_parity_config(uint32_t paritycfg)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* clear LPUART_CTL0 PM,PCEN bits */
    LPUART_CTL0 &= ~(LPUART_CTL0_PM | LPUART_CTL0_PCEN);
    /* configure LPUART parity mode */
    LPUART_CTL0 |= paritycfg;
}

/*!
    \brief      configure LPUART word length
    \param[in]  wlen: LPUART word length configure
                only one parameter can be selected which is shown as below:
      \arg        LPUART_WL_7BIT: 7 bits
      \arg        LPUART_WL_8BIT: 8 bits
      \arg        LPUART_WL_9BIT: 9 bits
    \param[out] none
    \retval     none
*/
void lpuart_word_length_set(uint32_t wlen)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* clear LPUART_CTL0 WL bit */
    LPUART_CTL0 &= ~(LPUART_CTL0_WL0 | LPUART_CTL0_WL1);
    /* configure LPUART word length */
    LPUART_CTL0 |= wlen;
}

/*!
    \brief      configure LPUART stop bit length
    \param[in]  stblen: LPUART stop bit configure
                only one parameter can be selected which is shown as below:
      \arg        LPUART_STB_1BIT: 1 bit
      \arg        LPUART_STB_2BIT: 2 bits
    \param[out] none
    \retval     none
*/
void lpuart_stop_bit_set(uint32_t stblen)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* clear LPUART_CTL1 STB bits */
    LPUART_CTL1 &= ~LPUART_CTL1_STB;
    LPUART_CTL1 |= stblen;
}

/*!
    \brief      enable LPUART
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_enable(void)
{
    LPUART_CTL0 |= LPUART_CTL0_UEN;
}

/*!
    \brief      disable LPUART
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_disable(void)
{
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
}

/*!
    \brief      configure LPUART transmitter
    \param[in]  txconfig: enable or disable LPUART transmitter
                only one parameter can be selected which is shown as below:
      \arg        LPUART_TRANSMIT_ENABLE: enable LPUART transmission
      \arg        LPUART_TRANSMIT_DISABLE: enable LPUART transmission
    \param[out] none
    \retval     none
*/
void lpuart_transmit_config(uint32_t txconfig)
{
    LPUART_CTL0 &= ~LPUART_CTL0_TEN;
    /* configure transfer mode */
    LPUART_CTL0 |= txconfig;
}

/*!
    \brief      configure LPUART receiver
    \param[in]  rxconfig: enable or disable LPUART receiver
                only one parameter can be selected which is shown as below:
      \arg        LPUART_RECEIVE_ENABLE: enable LPUART reception
      \arg        LPUART_RECEIVE_DISABLE: disable LPUART reception
    \param[out] none
    \retval     none
*/
void lpuart_receive_config(uint32_t rxconfig)
{
    LPUART_CTL0 &= ~LPUART_CTL0_REN;
    /* configure receiver mode */
    LPUART_CTL0 |= rxconfig;
}

/*!
    \brief      data is transmitted/received with the LSB/MSB first
    \param[in]  msbf: LSB/MSB
                only one parameter can be selected which is shown as below:
      \arg        LPUART_MSBF_LSB: LSB first
      \arg        LPUART_MSBF_MSB: MSB first
    \param[out] none
    \retval     none
*/
void lpuart_data_first_config(uint32_t msbf)
{
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* configure LSB or MSB first */
    LPUART_CTL1 &= ~(LPUART_CTL1_MSBF);
    LPUART_CTL1 |= (LPUART_CTL1_MSBF & msbf);
}

/*!
    \brief      configure LPUART inverted
    \param[in]  invertpara: refer to lpuart_invert_enum
                only one parameter can be selected which is shown as below:
      \arg        LPUART_DINV_ENABLE: data bit level inversion
      \arg        LPUART_DINV_DISABLE: data bit level not inversion
      \arg        LPUART_TXPIN_ENABLE: TX pin level inversion
      \arg        LPUART_TXPIN_DISABLE: TX pin level not inversion
      \arg        LPUART_RXPIN_ENABLE: RX pin level inversion
      \arg        LPUART_RXPIN_DISABLE: RX pin level not inversion
      \arg        LPUART_SWAP_ENABLE: swap TX/RX pins
      \arg        LPUART_SWAP_DISABLE: not swap TX/RX pins
    \param[out] none
    \retval     none
*/
void lpuart_invert_config(lpuart_invert_enum invertpara)
{
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* inverted or not the specified signal */
    switch(invertpara) {
    case LPUART_DINV_ENABLE:
        LPUART_CTL1 |= LPUART_CTL1_DINV;
        break;
    case LPUART_DINV_DISABLE:
        LPUART_CTL1 &= ~(LPUART_CTL1_DINV);
        break;
    case LPUART_TXPIN_ENABLE:
        LPUART_CTL1 |= LPUART_CTL1_TINV;
        break;
    case LPUART_TXPIN_DISABLE:
        LPUART_CTL1 &= ~(LPUART_CTL1_TINV);
        break;
    case LPUART_RXPIN_ENABLE:
        LPUART_CTL1 |= LPUART_CTL1_RINV;
        break;
    case LPUART_RXPIN_DISABLE:
        LPUART_CTL1 &= ~(LPUART_CTL1_RINV);
        break;
    case LPUART_SWAP_ENABLE:
        LPUART_CTL1 |= LPUART_CTL1_STRP;
        break;
    case LPUART_SWAP_DISABLE:
        LPUART_CTL1 &= ~(LPUART_CTL1_STRP);
        break;
    default:
        break;
    }
}

/*!
    \brief      enable the LPUART overrun function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_overrun_enable(void)
{
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* enable overrun function */
    LPUART_CTL2 &= ~(LPUART_CTL2_OVRD);
}

/*!
    \brief      disable the LPUART overrun function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_overrun_disable(void)
{
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* disable overrun function */
    LPUART_CTL2 |= LPUART_CTL2_OVRD;
}

/*!
    \brief      LPUART transmit data function
    \param[in]  data: data of transmission
    \param[out] none
    \retval     none
*/
void lpuart_data_transmit(uint32_t data)
{
    LPUART_TDATA = (LPUART_TDATA_TDATA & data);
}

/*!
    \brief      LPUART receive data function
    \param[in]  none
    \param[out] none
    \retval     data of received
*/
uint16_t lpuart_data_receive(void)
{
    return (uint16_t)(GET_BITS(LPUART_RDATA, 0U, 8U));
}

/*!
    \brief      enable LPUART command
    \param[in]  cmdtype: command type
                only one parameter can be selected which is shown as below:
      \arg        LPUART_CMD_MMCMD: mute mode command
      \arg        LPUART_CMD_RXFCMD: receive data flush command
    \param[out] none
    \retval     none
*/
void lpuart_command_enable(uint32_t cmdtype)
{
    LPUART_CMD |= (cmdtype);
}

/*!
    \brief      configure address of the LPUART
    \param[in]  addr: 0x00-0xFF, address of LPUART terminal
    \param[out] none
    \retval     none
*/
void lpuart_address_config(uint8_t addr)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL1 &= ~(LPUART_CTL1_ADDR);
    LPUART_CTL1 |= (LPUART_CTL1_ADDR & (((uint32_t)addr) << 24U));
}

/*!
    \brief      configure address detection mode
    \param[in]  addmod: address detection mode
                only one parameter can be selected which is shown as below:
      \arg        LPUART_ADDM_4BIT: 4 bits
      \arg        LPUART_ADDM_FULLBIT: full bits
    \param[out] none
    \retval     none
*/
void lpuart_address_detection_mode_config(uint32_t addmod)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL1 &= ~(LPUART_CTL1_ADDM);
    LPUART_CTL1 |= LPUART_CTL1_ADDM & (addmod);
}

/*!
    \brief      enable mute mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_mute_mode_enable(void)
{
    LPUART_CTL0 |= LPUART_CTL0_MEN;
}

/*!
    \brief      disable mute mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_mute_mode_disable(void)
{
    LPUART_CTL0 &= ~(LPUART_CTL0_MEN);
}

/*!
    \brief      configure wakeup method in mute mode
    \param[in]  wmethod: two methods be used to enter or exit the mute mode
                only one parameter can be selected which is shown as below:
      \arg        LPUART_WM_IDLE: idle line
      \arg        LPUART_WM_ADDR: address mark
    \param[out] none
    \retval     none
*/
void lpuart_mute_mode_wakeup_config(uint32_t wmethod)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL0 &= ~(LPUART_CTL0_WM);
    LPUART_CTL0 |= wmethod;
}

/*!
    \brief      enable half-duplex mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_halfduplex_enable(void)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL2 |= LPUART_CTL2_HDEN;
}

/*!
    \brief      disable half-duplex mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_halfduplex_disable(void)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL2 &= ~(LPUART_CTL2_HDEN);
}

/*!
    \brief      configure hardware flow control RTS
    \param[in]  rtsconfig: enable or disable RTS
                only one parameter can be selected which is shown as below:
      \arg        LPUART_RTS_ENABLE:  enable RTS
      \arg        LPUART_RTS_DISABLE: disable RTS
    \param[out] none
    \retval     none
*/
void lpuart_hardware_flow_rts_config(uint32_t rtsconfig)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL2 &= ~(LPUART_CTL2_RTSEN);
    LPUART_CTL2 |= rtsconfig;
}

/*!
    \brief      configure hardware flow control CTS
    \param[in]  ctsconfig:  enable or disable CTS
                only one parameter can be selected which is shown as below:
      \arg        LPUART_CTS_ENABLE:  enable CTS
      \arg        LPUART_CTS_DISABLE: disable CTS
    \param[out] none
    \retval     none
*/
void lpuart_hardware_flow_cts_config(uint32_t ctsconfig)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL2 &= ~LPUART_CTL2_CTSEN;
    LPUART_CTL2 |= ctsconfig;
}

/*!
    \brief      configure hardware flow control coherence mode
    \param[in]  hcm:
                only one parameter can be selected which is shown as below:
      \arg        LPUART_HCM_NONE: nRTS signal equals to the RBNE status register
      \arg        LPUART_HCM_EN:   nRTS signal is set when the last data bit has been sampled
    \param[out] none
    \retval     none
*/
void lpuart_hardware_flow_coherence_config(uint32_t hcm)
{
    LPUART_CHC &= ~(LPUART_CHC_HCM);
    LPUART_CHC |= (LPUART_CHC_HCM & hcm);
}

/*!
    \brief      enable RS485 driver
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_rs485_driver_enable(void)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL2 |= LPUART_CTL2_DEM;
}

/*!
    \brief      disable RS485 driver
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_rs485_driver_disable(void)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL2 &= ~(LPUART_CTL2_DEM);
}

/*!
    \brief      configure driver enable assertion time
    \param[in]  deatime: 0x00000000-0x0000001F
    \param[out] none
    \retval     none
*/
void lpuart_driver_assertime_config(uint32_t deatime)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL0 &= ~(LPUART_CTL0_DEA);
    LPUART_CTL0 |= (LPUART_CTL0_DEA & ((deatime) << 21U));
}

/*!
    \brief      configure driver enable de-assertion time
    \param[in]  dedtime: 0x00000000-0x0000001F
    \param[out] none
    \retval     none
*/
void lpuart_driver_deassertime_config(uint32_t dedtime)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL0 &= ~(LPUART_CTL0_DED);
    LPUART_CTL0 |= (LPUART_CTL0_DED & ((dedtime) << 16U));
}

/*!
    \brief      configure driver enable polarity mode
    \param[in]  dep: DE signal
                only one parameter can be selected which is shown as below:
      \arg        LPUART_DEP_HIGH: DE signal is active high
      \arg        LPUART_DEP_LOW: DE signal is active low
    \param[out] none
    \retval     none
*/
void lpuart_depolarity_config(uint32_t dep)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* reset DEP bit */
    LPUART_CTL2 &= ~(LPUART_CTL2_DEP);
    LPUART_CTL2 |= (LPUART_CTL2_DEP & dep);
}

/*!
    \brief      configure LPUART DMA for reception
    \param[in]  dmacmd: enable or disable DMA for reception
                only one parameter can be selected which is shown as below:
      \arg        LPUART_DENR_ENABLE: DMA enable for reception
      \arg        LPUART_DENR_DISABLE: DMA disable for reception
    \param[out] none
    \retval     none
*/
void lpuart_dma_receive_config(uint32_t dmacmd)
{
    LPUART_CTL2 &= ~LPUART_CTL2_DENR;
    /* configure DMA reception */
    LPUART_CTL2 |= dmacmd;
}

/*!
    \brief      configure LPUART DMA for transmission
    \param[in]  dmacmd: enable or disable DMA for transmission
                only one parameter can be selected which is shown as below:
      \arg        LPUART_DENT_ENABLE: DMA enable for transmission
      \arg        LPUART_DENT_DISABLE: DMA disable for transmission
    \param[out] none
    \retval     none
*/
void lpuart_dma_transmit_config(uint32_t dmacmd)
{
    LPUART_CTL2 &= ~LPUART_CTL2_DENT;
    /* configure DMA transmission */
    LPUART_CTL2 |= dmacmd;
}

/*!
    \brief      disable DMA on reception error
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_reception_error_dma_disable(void)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL2 |= LPUART_CTL2_DDRE;
}

/*!
    \brief      enable DMA on reception error
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_reception_error_dma_enable(void)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);

    LPUART_CTL2 &= ~(LPUART_CTL2_DDRE);
}

/*!
    \brief      enable LPUART to wakeup the mcu from deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_wakeup_enable(void)
{
    LPUART_CTL0 |= LPUART_CTL0_UESM;
}

/*!
    \brief      disable LPUART to wakeup the mcu from deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lpuart_wakeup_disable(void)
{
    LPUART_CTL0 &= ~(LPUART_CTL0_UESM);
}

/*!
    \brief      configure the LPUART wakeup mode from deep-sleep mode
    \param[in]  wum: wakeup mode
                only one parameter can be selected which is shown as below:
      \arg        LPUART_WUM_ADDR: WUF active on address match
      \arg        LPUART_WUM_STARTB: WUF active on start bit
      \arg        LPUART_WUM_RBNE: WUF active on RBNE
    \param[out] none
    \retval     none
*/
void lpuart_wakeup_mode_config(uint32_t wum)
{
    /* disable LPUART */
    LPUART_CTL0 &= ~(LPUART_CTL0_UEN);
    /* reset WUM bit */
    LPUART_CTL2 &= ~(LPUART_CTL2_WUM);
    LPUART_CTL2 |= LPUART_CTL2_WUM & (wum);
}

/*!
    \brief      get flag in STAT/CHC register
    \param[in]  flag: flag type
                only one parameter can be selected which is shown as below:
      \arg        LPUART_FLAG_PERR: parity error flag
      \arg        LPUART_FLAG_FERR: frame error flag
      \arg        LPUART_FLAG_NERR: noise error flag
      \arg        LPUART_FLAG_ORERR: overrun error
      \arg        LPUART_FLAG_IDLE: idle line detected flag
      \arg        LPUART_FLAG_RBNE: read data buffer not empty
      \arg        LPUART_FLAG_TC: transmission completed
      \arg        LPUART_FLAG_TBE: transmit data register empty
      \arg        LPUART_FLAG_CTSF: CTS change flag
      \arg        LPUART_FLAG_CTS: CTS level
      \arg        LPUART_FLAG_BSY: busy flag
      \arg        LPUART_FLAG_AM: address match flag
      \arg        LPUART_FLAG_RWU: receiver wakeup from mute mode.
      \arg        LPUART_FLAG_WU: wakeup from deep-sleep mode flag
      \arg        LPUART_FLAG_TEA: transmit enable acknowledge flag
      \arg        LPUART_FLAG_REA: receive enable acknowledge flag
      \arg        LPUART_FLAG_EPERR: early parity error flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus lpuart_flag_get(lpuart_flag_enum flag)
{
    if(RESET != (LPUART_REG_VAL(LPUART, flag) & BIT(LPUART_BIT_POS(flag)))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear LPUART status
    \param[in]  flag: flag type
                only one parameter can be selected which is shown as below:
      \arg        LPUART_FLAG_PERR: parity error flag
      \arg        LPUART_FLAG_FERR: frame error flag
      \arg        LPUART_FLAG_NERR: noise detected flag
      \arg        LPUART_FLAG_ORERR: overrun error flag
      \arg        LPUART_FLAG_IDLE: idle line detected flag
      \arg        LPUART_FLAG_TC: transmission complete flag
      \arg        LPUART_FLAG_CTSF: CTS change flag
      \arg        LPUART_FLAG_AM: address match flag
      \arg        LPUART_FLAG_WU: wakeup from deep-sleep mode flag
      \arg        LPUART_FLAG_EPERR: early parity error flag
    \param[out] none
    \retval     none
*/
void lpuart_flag_clear(lpuart_flag_enum flag)
{
    LPUART_INTC |= BIT(LPUART_BIT_POS(flag));
}

/*!
    \brief      enable LPUART interrupt
    \param[in]  interrupt: interrupt type
                only one parameter can be selected which is shown as below:
      \arg        LPUART_INT_IDLE: idle interrupt
      \arg        LPUART_INT_RBNE: read data buffer not empty interrupt and
                                  overrun error interrupt enable interrupt
      \arg        LPUART_INT_TC: transmission complete interrupt
      \arg        LPUART_INT_TBE: transmit data register empty interrupt
      \arg        LPUART_INT_PERR: parity error interrupt
      \arg        LPUART_INT_AM: address match interrupt
      \arg        LPUART_INT_ERR: error interrupt enable in multibuffer communication
      \arg        LPUART_INT_CTS: CTS interrupt
      \arg        LPUART_INT_WU: wakeup from deep-sleep mode interrupt
    \param[out] none
    \retval     none
*/
void lpuart_interrupt_enable(lpuart_interrupt_enum interrupt)
{
    LPUART_REG_VAL(LPUART, interrupt) |= BIT(LPUART_BIT_POS(interrupt));
}

/*!
    \brief      disable LPUART interrupt
    \param[in]  interrupt: interrupt type
                only one parameter can be selected which is shown as below:
      \arg        LPUART_INT_IDLE: idle interrupt
      \arg        LPUART_INT_RBNE: read data buffer not empty interrupt and
                                  overrun error interrupt enable interrupt
      \arg        LPUART_INT_TC: transmission complete interrupt
      \arg        LPUART_INT_TBE: transmit data register empty interrupt
      \arg        LPUART_INT_PERR: parity error interrupt
      \arg        LPUART_INT_AM: address match interrupt
      \arg        LPUART_INT_LBD: LIN break detection interrupt
      \arg        LPUART_INT_ERR: error interrupt enable in multibuffer communication
      \arg        LPUART_INT_CTS: CTS interrupt
      \arg        LPUART_INT_WU: wakeup from deep-sleep mode interrupt
    \param[out] none
    \retval     none
*/
void lpuart_interrupt_disable(lpuart_interrupt_enum interrupt)
{
    LPUART_REG_VAL(LPUART, interrupt) &= ~BIT(LPUART_BIT_POS(interrupt));
}

/*!
    \brief      get LPUART interrupt and flag status
    \param[in]  int_flag: interrupt and flag type, refer to lpuart_interrupt_flag_enum
                only one parameter can be selected which is shown as below:
      \arg        LPUART_INT_FLAG_AM: address match interrupt and flag
      \arg        LPUART_INT_FLAG_PERR: parity error interrupt and flag
      \arg        LPUART_INT_FLAG_TBE: transmitter buffer empty interrupt and flag
      \arg        LPUART_INT_FLAG_TC: transmission complete interrupt and flag
      \arg        LPUART_INT_FLAG_RBNE: read data buffer not empty interrupt and flag
      \arg        LPUART_INT_FLAG_RBNE_ORERR: read data buffer not empty interrupt and overrun error flag
      \arg        LPUART_INT_FLAG_IDLE: IDLE line detected interrupt and flag
      \arg        LPUART_INT_FLAG_WU: wakeup from deep-sleep mode interrupt and flag
      \arg        LPUART_INT_FLAG_CTS: CTS interrupt and flag
      \arg        LPUART_INT_FLAG_ERR_NERR: error interrupt and noise error flag
      \arg        LPUART_INT_FLAG_ERR_ORERR: error interrupt and overrun error
      \arg        LPUART_INT_FLAG_ERR_FERR: error interrupt and frame error flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus lpuart_interrupt_flag_get(lpuart_interrupt_flag_enum int_flag)
{
    uint32_t intenable = 0U, flagstatus = 0U;
    /* get the interrupt enable bit status */
    intenable = (LPUART_REG_VAL(LPUART, int_flag) & BIT(LPUART_BIT_POS(int_flag)));
    /* get the corresponding flag bit status */
    flagstatus = (LPUART_REG_VAL2(LPUART, int_flag) & BIT(LPUART_BIT_POS2(int_flag)));

    if(flagstatus && intenable) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear LPUART interrupt flag
    \param[in]  int_flag: LPUART interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        LPUART_INT_FLAG_PERR: parity error flag
      \arg        LPUART_INT_FLAG_ERR_FERR: frame error flag
      \arg        LPUART_INT_FLAG_ERR_NERR: noise detected flag
      \arg        LPUART_INT_FLAG_RBNE_ORERR: read data buffer not empty interrupt and overrun error flag
      \arg        LPUART_INT_FLAG_ERR_ORERR: error interrupt and overrun error
      \arg        LPUART_INT_FLAG_IDLE: idle line detected flag
      \arg        LPUART_INT_FLAG_TC: transmission complete flag
      \arg        LPUART_INT_FLAG_CTS: CTS change flag
      \arg        LPUART_INT_FLAG_AM: address match flag
      \arg        LPUART_INT_FLAG_WU: wakeup from deep-sleep mode flag
    \param[out] none
    \retval     none
*/
void lpuart_interrupt_flag_clear(lpuart_interrupt_flag_enum int_flag)
{
    LPUART_INTC |= BIT(LPUART_BIT_POS2(int_flag));
}
