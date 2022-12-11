/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <stdarg.h>

/* User Add */
#include "common.h"

typedef struct 
{
    SPI_HandleTypeDef   *handle;
    GPIO_TypeDef        *cs_port;
    uint16_t            cs_pin;
#if (USING_RTOS_TYPE == RTOS_USING_RTTHREAD)
    struct rt_mutex     lock;
#endif

} spi_user_data, *spi_user_data_t;


static char log_buf[128];
static spi_user_data spi = 
{
    .handle  = &hspi2,
    .cs_port = FLASH_CS_GPIO_Port,
    .cs_pin  = FLASH_CS_Pin,
};


void sfud_log_debug(const char *file, const long line, const char *format, ...);


/* User Code */
static void spi_lock(const sfud_spi *spi) {

//    BSP_INT_DIS();
}


static void spi_unlock(const sfud_spi *spi) {
    
//    BSP_INT_EN();
}


static void retry_delay_100us(void) {
    /* 100 microsecond delay */
    /* 此处使用 perf_counter 的 delay_us() 函数 */
    delay_us(100);
}


/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {

    sfud_err  err = SFUD_SUCCESS;
    spi_user_data *spi_dev = (spi_user_data *)spi->user_data;
    HAL_StatusTypeDef status = HAL_OK;

    HAL_GPIO_WritePin(spi_dev->cs_port, spi_dev->cs_pin, GPIO_PIN_RESET);

    status = HAL_SPI_Transmit(spi_dev->handle, (uint8_t *)write_buf, write_size, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        BSP_Printf("[ %s ] line %d: transmit error : %d\r\n", __func__, __LINE__, status);
        err = SFUD_ERR_WRITE;
        goto __exit;
    }

    if (read_buf == NULL || read_size == 0)
        goto __exit;

    status = HAL_SPI_Receive(spi_dev->handle, (uint8_t *)read_buf, read_size, HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        BSP_Printf("[ %s ] line %d: receive error : %d\r\n", __func__, __LINE__, status);
        err = SFUD_ERR_READ;
        goto __exit;
    }
    
__exit:
    HAL_GPIO_WritePin(spi_dev->cs_port, spi_dev->cs_pin, GPIO_PIN_SET);

    return err;
}


#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err qspi_read(const struct __sfud_spi *spi, uint32_t addr, sfud_qspi_read_cmd_format *qspi_read_cmd_format,
        uint8_t *read_buf, size_t read_size) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your qspi read flash data code
     */

    return result;
}
#endif /* SFUD_USING_QSPI */


sfud_err sfud_spi_port_init(sfud_flash *flash) {

    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock;
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */
    switch (flash->index) 
    {
        case SFUD_W25Q128_DEVICE_INDEX: {
            /* RCC 初始化 */
            /* GPIO 初始化 */
            /* SPI 外设初始化 */
            /* 同步 Flash 移植所需的接口及数据 */
            flash->spi.wr        = spi_write_read;
            flash->spi.lock      = spi_lock;
            flash->spi.unlock    = spi_unlock;
            flash->spi.user_data = &spi;
            /* about 100 microsecond delay */
            flash->retry.delay   = retry_delay_100us;
            /* adout 60 seconds timeout */
            flash->retry.times   = 60 * 10000;

            /* 信号锁也在此初始化 */
            break;
        }
    }

    return SFUD_SUCCESS;
}


/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    SFUD_PRINT("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    SFUD_PRINT("%s\r\n", log_buf);
    va_end(args);
}


/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    SFUD_PRINT("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    SFUD_PRINT("%s\r\n", log_buf);
    va_end(args);
}

