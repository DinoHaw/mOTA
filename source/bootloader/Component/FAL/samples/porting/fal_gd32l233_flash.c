/**
 * \file            fal_gd32l233_flash.c
 * \brief           gd32l233 onchip flash operate interface for FAL liabrary
 */

/*
 * Copyright (c) 2023
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
 * v1.0     2023-01-08     wade任       the first version
 */
 
/* Includes ------------------------------------------------------------------*/
#include <fal.h>
#include "bsp_common.h"


/* Extern function prototypes ------------------------------------------------*/
extern void Firmware_OperateCallback(uint16_t progress);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  读取 flash 的数据
 * @note   
 * @param[in]   offset: 偏移地址
 * @param[out]  buf: 数据缓存池
 * @param[in]   size: 数据长度，单位 byte
 * @retval 读到的数据长度，单位 byte
 */
int read(long offset, uint8_t *buf, size_t size)
{
    size_t i = 0;
#if (IS_ENABLE_SPI_FLASH)
    uint32_t addr = gd32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif

    FAL_PRINTF("read addr: 0x%.8x\r\n", addr);

    if ((addr + size) > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: read outrange flash size! addr is (0x%p)\n", (void *)(addr + size));
        return -1;
    }

    for (i = 0; i < size; i++, buf++, addr++)
    {
        *buf = *(uint8_t *) addr;
    }

    return size;
}


/**
 * @brief  
 * @note   
 * @param[in]  offset: 偏移地址 
 * @param[in]  buf: 数据池
 * @param[in]  size: 数据长度，单位 byte
 * @retval -1: 失败。 0: 成功
 */
int write(long offset, const uint8_t *buf, size_t size)
{
    uint8_t ret = 0;
    size_t  byte_step = 0;
    uint8_t index = 0;
    int8_t  status = 0;
    uint8_t byte_num = sizeof(uint32_t);
    uint32_t write_data = 0;
    uint32_t temp_data = 0;

#if (IS_ENABLE_SPI_FLASH)
    uint32_t addr = gd32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif
    
    if ((addr + size) > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: write outrange flash size! addr is (0x%p)\n", (void *)(addr + size));
        return -1;
    }

    if (size < 1) {
        return -1;
    }

    fmc_unlock();
    fmc_flag_clear(FMC_FLAG_BUSY | FMC_FLAG_PGAERR | FMC_FLAG_WPERR | FMC_FLAG_END);

    FAL_PRINTF("write addr: 0x%.8x\r\n", addr);
    for (byte_step = 0; byte_step < size; )
    {
        if ((size - byte_step) < byte_num)         /* The number of bytes less than a WORD/half WORD */
        {
            for (index = 0; (size - byte_step) > 0; index++)
            {
                temp_data  = *buf;
                write_data = (write_data) | (temp_data << 8*index);
                
                buf ++;
                byte_step ++;
            }
        }
        else
        {
            for (index = 0; index < byte_num; index++)
            {
                temp_data  = *buf;
                write_data = (write_data) | (temp_data << 8*index);
                
                buf ++;
                byte_step ++;
            }
        }

        /* write data */
        ret = fmc_word_program(addr, write_data);
        if (ret == FMC_READY)
        {
            /* Check the written value */
            if (*(uint32_t *)addr != write_data)
            {
                FAL_PRINTF("ERROR: write data != read data\r\n");
                status = -1;
                goto __exit;
            }
//            FAL_PRINTF("[D] %s: OK (%d)\r\n", __func__, __LINE__);
        }
        else
        {
            FAL_PRINTF("[D] %s: %d (%d)\r\n", __func__, ret, __LINE__);
            status = -1;
            goto __exit;
        }
        
        temp_data  = 0;
        write_data = 0;

        addr += byte_num;
    }

__exit:
    fmc_lock();
    
    if (status) {
        return status;
    }

    return size;
}


/**
 * @brief  
 * @note   
 * @param[in]   offset: 偏移地址 
 * @param[in]   size: 数据长度，单位 byte
 * @retval -2: 擦除失败。 -1: 超过 flash 大小。 0: 成功 
 */
int erase(long offset, size_t size)
{
    fmc_state_enum status = FMC_READY;
    uint32_t progress_unit = 0;
    uint32_t progress = 0;
    uint32_t page_addr = 0;

#if (IS_ENABLE_SPI_FLASH)
    uint32_t addr = gd32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif
    uint32_t end_addr = addr + size;

    if (end_addr > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: erase outrange flash size! addr is (0x%p)\n", (void *)end_addr);
        return -1;
    }

    fmc_unlock();
    progress_unit = 10000 * FMC_PAGE_SIZE / size;

    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

    page_addr = addr;

    for(; page_addr < end_addr; page_addr += FMC_PAGE_SIZE)
    {
        // status = fmc_page_erase(page_addr);
        // if (status != FMC_READY)
        //     break;
        if(FMC_READY == fmc_page_erase(page_addr)) {
            FAL_PRINTF("gd32l2xx erase finish!\r\n");
        } else {
            FAL_PRINTF("gd32l2xx erase error!\r\n");
        }

        progress += progress_unit;
        Firmware_OperateCallback(progress);
    }

    fmc_lock();
    
    if (status != FMC_READY) {
        return -2;
    }

    return size;
}


const struct fal_flash_dev gd32_onchip_flash = 
{
    .name      = FAL_ONCHIP_FLASH_DEV_NAME,
    .addr      = FLASH_BASE,
    .len       = ONCHIP_FLASH_SIZE,
    .blk_size  = FMC_PAGE_SIZE,
    
    .ops.init  = NULL,
    .ops.read  = read,
    .ops.write = write,
    .ops.erase = erase,
};

