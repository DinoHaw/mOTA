/*
 * File      : fal_flash_sfud_port.c
 * This file is part of FAL (Flash Abstraction Layer) package
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     armink       the first version
 */

#include <fal.h>
#include <sfud.h>


#if (IS_ENABLE_SPI_FLASH && defined(FAL_USING_SFUD_PORT)) 
#ifdef RT_USING_SFUD
#include <spi_flash_sfud.h>
#endif


static int init(void);
static int read(long offset, uint8_t *buf, size_t size);
static int write(long offset, const uint8_t *buf, size_t size);
static int erase(long offset, size_t size);


static sfud_flash *sfud_spi_flash1;
/* 若 flash 支持 SFDP 且已经开启 SFUD_USING_SFDP ，则 len 和 blk_size 不填或填错都无问题，
   这两个参数都会被读到的 SFDP 更新 */
struct fal_flash_dev spi_flash1 = 
{
    .name      = FAL_SPI_FLASH_DEV_NAME, 
    .addr      = 0, 
    .len       = 16 * 1024 * 1024, 
    .blk_size  = SPI_FLASH_ERASE_GRANULARITY, 

    .ops.init  = init,
    .ops.read  = read,
    .ops.write = write,
    .ops.erase = erase,
};


static int init(void)
{

#ifdef RT_USING_SFUD
    /* RT-Thread RTOS platform */
    sfud_dev = rt_sfud_flash_find_by_dev_name(FAL_USING_NOR_FLASH_DEV_NAME);
#else
    /* bare metal platform */
#endif
    sfud_init();        // initialize all of the SFUD flash
    
    sfud_spi_flash1 = sfud_get_device(SFUD_W25Q128_DEVICE_INDEX);   // according device name to get flash handle

    if (NULL == sfud_spi_flash1)
    {
        log_e("FAL Flash initialize failed.\r\n");
        return -1;
    }

    /* update the flash chip information */
    spi_flash1.blk_size = sfud_spi_flash1->chip.erase_gran;
    spi_flash1.len = sfud_spi_flash1->chip.capacity;

    return 0;
}


static int read(long offset, uint8_t *buf, size_t size)
{
    assert(sfud_spi_flash1);
    assert(sfud_spi_flash1->init_ok);
    BSP_Printf("[FAL SFUD] read addr: 0x%.8X, size: %d\r\n", spi_flash1.addr + offset, size);
    sfud_read(sfud_spi_flash1, spi_flash1.addr + offset, size, buf);

    return size;
}


static int write(long offset, const uint8_t *buf, size_t size)
{
    assert(sfud_dev);
    assert(sfud_dev->init_ok);
    BSP_Printf("[FAL SFUD] write addr: 0x%.8X, size: %d\r\n", spi_flash1.addr + offset, size);
    if (sfud_write(sfud_spi_flash1, spi_flash1.addr + offset, size, buf) != SFUD_SUCCESS)
    {
        return -1;
    }

    return size;
}


static int erase(long offset, size_t size)
{
    assert(sfud_dev);
    assert(sfud_dev->init_ok);
    BSP_Printf("[FAL SFUD] erase addr: 0x%.8X, size: %d\r\n", spi_flash1.addr + offset, size);
    if (sfud_erase(sfud_spi_flash1, spi_flash1.addr + offset, size) != SFUD_SUCCESS)
    {
        return -1;
    }

    return size;
}

#endif /* FAL_USING_SFUD_PORT */

