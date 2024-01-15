/**
 * \file            bsp_flash_drv.c
 * \brief           SPI FLASH DRIVE
 */

/*
 * Copyright (c) 2024
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of mOTA - The Over-The-Air technology component for MCU.
 *
 * Author:          wade任
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2024-01-11     wade任       the first version
 */

/* Includes ------------------------------------------------------------------*/
#include "bsp_flash_drv.h"

static void bsp_flash_gpio_config(void)
{
    /* 配置 SPI0_CS 引脚 */
    rcu_periph_clock_enable(FLASH_SPI_CS_CLK );
    gpio_mode_set(FLASH_SPI_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,  FLASH_SPI_CS_PIN);
    gpio_output_options_set(FLASH_SPI_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_SPI_CS_PIN);
    gpio_bit_set(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN);
    
    /* 配置 SPI0_CLK 引脚 */
    rcu_periph_clock_enable(FLASH_SPI_CLK_CLK);
    gpio_af_set(FLASH_SPI_CLK_PORT, GPIO_AF_5, FLASH_SPI_CLK_PIN);
    gpio_mode_set(FLASH_SPI_CLK_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_SPI_CLK_PIN);
    gpio_output_options_set(FLASH_SPI_CLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_SPI_CLK_PIN);

    /* 配置 SPI0_MISO 引脚 */
    rcu_periph_clock_enable(FLASH_SPI_MISO_CLK);
    gpio_af_set(FLASH_SPI_MISO_PORT, GPIO_AF_5, FLASH_SPI_MISO_PIN);
    gpio_mode_set(FLASH_SPI_MISO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_SPI_MISO_PIN);
    gpio_output_options_set(FLASH_SPI_MISO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_SPI_MISO_PIN);

    /* 配置 SPI0_MOSI 引脚 */
    rcu_periph_clock_enable(FLASH_SPI_MOSI_CLK);
    gpio_af_set(FLASH_SPI_MOSI_PORT, GPIO_AF_5, FLASH_SPI_MOSI_PIN);
    gpio_mode_set(FLASH_SPI_MOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FLASH_SPI_MOSI_PIN);
    gpio_output_options_set(FLASH_SPI_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FLASH_SPI_MOSI_PIN);

    SPI0_FLASH_CS_HIGH();
}

static void bsp_flash_spi_config(void)
{
    spi_parameter_struct spi_init_struct;
    rcu_periph_clock_enable(FLASH_SPI_CLK);

    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_2;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(FLASH_SPIx, &spi_init_struct);

    spi_fifo_access_size_config(FLASH_SPIx, SPI_BYTE_ACCESS);

    spi_crc_polynomial_set(FLASH_SPIx, 7);

//    qspi_io23_output_enable(FLASH_SPIx);

    spi_enable(FLASH_SPIx);
}

/* 初始化 Flash 的 GPIO 和 SPI 外设 */
void bsp_flash_hardware_init(void)
{
    bsp_flash_gpio_config();
    bsp_flash_spi_config();
}

/* 通过 SPI 接口发送一个字节，并返回从 SPI 总线接收的字节 */
uint8_t bsp_spi_flash_send_byte(uint8_t byte)
{
    while (RESET == spi_i2s_flag_get(FLASH_SPIx, SPI_FLAG_TBE)) {}

    spi_i2s_data_transmit(FLASH_SPIx, byte);

    while (RESET == spi_i2s_flag_get(FLASH_SPIx, SPI_FLAG_RBNE)) {}

    return (spi_i2s_data_receive(FLASH_SPIx));
}

/* 向闪存写入多个字节 */
void bsp_spi_flash_page_write(const uint8_t *pbuffer, uint16_t num_byte_to_write)
{
    while (num_byte_to_write--)
    {
        bsp_spi_flash_send_byte(*pbuffer);
        pbuffer++;
    }
}

/* 从闪存读取一个数据块 */
void bsp_spi_flash_buffer_read(uint8_t *pbuffer, uint16_t num_byte_to_read)
{
    while (num_byte_to_read--)
    {
        *pbuffer = bsp_spi_flash_send_byte(DUMMY_BYTE);
        pbuffer++;
    }
}