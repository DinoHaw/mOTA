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
//#include <rthw.h>
//#include <rtthread.h>
/* User Add */
#include "stm32f1xx_hal.h"
#include "bsp_common.h"

typedef struct {
//    SPI_HandleTypeDef   *spix;
//    GPIO_TypeDef        *cs_port;
//    uint16_t            cs_pin;
    
//    struct SPI_FLASH_STRUCT  drv;
//    struct rt_mutex     lock;
    uint8_t reserved;
} spi_user_data, *spi_user_data_t;

//extern SPI_HandleTypeDef    hspi2;

static char log_buf[256];

void sfud_log_debug(const char *file, const long line, const char *format, ...);

/* User Code */
static void spi_lock(const sfud_spi *spi) {
//    spi_user_data_t spi_dev = (spi_user_data_t) spi->user_data;

//    RT_ASSERT(spi);
//    RT_ASSERT(sfud_dev);

//    rt_mutex_take(&(spi_dev->lock), RT_WAITING_FOREVER);
}

static void spi_unlock(const sfud_spi *spi) {
//    spi_user_data_t spi_dev = (spi_user_data_t) spi->user_data;

//    RT_ASSERT(spi);
//    RT_ASSERT(sfud_dev);
//    
//    rt_mutex_release(&(spi_dev->lock));
}

static void retry_delay_100us(void) {
    /* 100 microsecond delay */
//    rt_thread_delay((RT_TICK_PER_SECOND * 1 + 9999) / 10000);
}

/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
//    uint8_t send_data, read_data;
            
//    spi_user_data_t spi_dev = (spi_user_data_t) spi->user_data;

    /**
     * add your spi write and read code
     */
//    if (write_size && read_size)
//    {
//        BSP_SPI_Flash_WriteData(&spi_dev->drv, write_buf, write_size)
//        if ()
//            result = SFUD_ERR_TIMEOUT;
//    }
//    else if (write_size)
//    {
//        if ()
//            result = SFUD_ERR_TIMEOUT;
//    }
//    else
//    {
//        if ()
//            result = SFUD_ERR_TIMEOUT;
//    }

            
//    SPI_Flash_ChipSelect(&spi_dev->drv, 0);									/* 使能片选 */
//            
//    if (write_size)
//        HAL_SPI_Transmit(spi_dev->drv.handle, (uint8_t *)write_buf, write_size, 0xFFFFFF);
//    
//    if (read_size)
//        HAL_SPI_Receive(spi_dev->drv.handle, (uint8_t *)read_buf, read_size, 0xFFFFFF);
//            
//    SPI_Flash_ChipSelect(&spi_dev->drv, 1);									/* 禁能片选 */

    return result;
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

//static spi_user_data spi2 = { .spix = &hspi2, .cs_port = GPIOF, .cs_pin = GPIO_PIN_11 };
static spi_user_data spi2;

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;

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
            flash->spi.wr = spi_write_read;
            flash->spi.lock = spi_lock;
            flash->spi.unlock = spi_unlock;
            flash->spi.user_data = &spi2;
            /* about 100 microsecond delay */
            flash->retry.delay = retry_delay_100us;
            /* adout 60 seconds timeout */
            flash->retry.times = 60 * 10000;
            
//            rt_mutex_init(&spi2.lock, "sf1_lock", RT_IPC_FLAG_FIFO);
//            
//            BSP_SPI_Flash_Init(&spi2.drv, &hspi2, 1);

            break;
        }
    }

    return result;
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

