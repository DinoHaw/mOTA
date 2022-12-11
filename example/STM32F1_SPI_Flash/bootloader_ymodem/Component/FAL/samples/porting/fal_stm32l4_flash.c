/**
 * \file            fal_stm32l4_flash.c
 * \brief           stm32l4 onchip flash operate interface for FAL liabrary
 */

/*
 * Copyright (c) 2022 Dino Haw
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
 * Author:          Dino Haw <347341799@qq.com>
 * Version:         v1.0.0
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
#if (ENABLE_SPI_FLASH)
    uint32_t addr = stm32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif

    FAL_PRINTF("read addr: 0x%.8X\r\n", addr);

    if ((addr + size) > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: read outrange flash size! addr is (0x%p)\n", (void*)(addr + size));
        return -1;
    }

    for (size_t i = 0; i < size; i++, buf++, addr++)
    {
        *buf = *(uint8_t *) addr;
    }

    return size;
}


/**
 * @brief  向 flash 写入数据
 * @note   传入的 buff 至少是 flash 最小写入 byte 的整数倍，否则会发生数组溢出
 * @param[in]  offset: 偏移地址 
 * @param[in]  buf: 数据池
 * @param[in]  size: 数据长度，单位 byte
 * @retval -1: 失败。 0: 成功
 */
int write(long offset, const uint8_t *buf, size_t size)
{
    int status = 0;
#if (ENABLE_SPI_FLASH)
    uint32_t addr = stm32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif
    uint32_t end_addr = addr + size;
    HAL_StatusTypeDef ret = HAL_OK;
    
    if (end_addr > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: write outrange flash size! addr is (0x%p)\n", (void*)end_addr);
        return -1;
    }
    
    if (size < 1)
        return -1;

    HAL_FLASH_Unlock();
    
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);
    
#if 1
    uint8_t  is_mod = size % sizeof(uint32_t);
    size_t   data_len = size / sizeof(uint32_t);
    uint32_t *data = (uint32_t *)buf;
    
    if (is_mod)
        data_len += 2;
    
    FAL_PRINTF("write addr: 0x%.8X , size: %d\r\n", addr, size);
    
    for (uint32_t i = 0; 
         (i < data_len / 2) && (addr <= (end_addr - 8)); 
         i++)
    {
        
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
           be done by word */ 
        ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, *((uint64_t *)(data + 2*i)));
        if (ret == HAL_OK)      
        {
            /* Check the written value */
            if (*(uint64_t *)addr != *(uint64_t *)(data + 2*i))
            {
                /* Flash content doesn't match SRAM content */
                FAL_PRINTF("ERROR: write data != read data\r\n");
                status = -1;
                break;
            }
            /* Increment FLASH addr address */
            addr += 8;
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            FAL_PRINTF("[D] %s: %d (%d)\r\n", __func__, ret, __LINE__);
            status = -2;
            break;
        }
    }
    
#else
    uint8_t  index      = 0;
    uint8_t  byte_num   = sizeof(uint64_t);
    size_t   byte_step  = 0;
    uint64_t write_data = 0;
    uint64_t temp_data  = 0;

    FAL_PRINTF("write addr: 0x%.8X\r\n", addr);
    for (byte_step = 0; byte_step < size; )
    {
        /* The number of bytes less than a WORD/half WORD */
        if ((size - byte_step) < byte_num)         
        {
            for (index = 0; (size - byte_step) > 0; index++)
            {
                temp_data = *buf;
                write_data = (write_data) | (temp_data << 8*index);
                
                buf ++;
                byte_step ++;
            }
        }
        else
        {
            for (index = 0; index < byte_num; index++)
            {
                temp_data = *buf;
                write_data = (write_data) | (temp_data << 8*index);
                
                buf ++;
                byte_step ++;
            }
        }
        
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
           be done by double word */ 
        /* DataLength must be a multiple of 64 bit */
        for (uint32_t i = 0; 
             (i < data_len / 2) && (addr <= (end_addr - 8)); 
             i++)
        {
            FAL_PRINTF("write addr: 0x%.8X\r\n", addr);
            /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
               be done by word */ 
            ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, *((uint64_t *)(data + 2*i)));
            if (ret == HAL_OK)      
            {
                /* Check the written value */
                if (*(uint64_t *)addr != *(uint64_t *)(data + 2*i))
                {
                    /* Flash content doesn't match SRAM content */
                    FAL_PRINTF("ERROR: write data != read data\r\n");
                    status = -1;
                    break;
                }
                /* Increment FLASH addr address */
                addr += 8;
            }
            else
            {
                /* Error occurred while writing data in Flash memory */
                FAL_PRINTF("[D] %s: %d (%d)\r\n", __func__, ret, __LINE__);
                status = -2;
                break;
            }
        }
        
        temp_data  = 0;
        write_data = 0;

        addr += byte_num;
    }
#endif

__exit:
    HAL_FLASH_Lock();
    
    if (status)
        return status;

    return size;
}


/**
 * @brief  擦除 flash
 * @note   会按照 addr + size 所在的 flash 的 sector 或 page 进行擦除 
 * @param[in]  offset: 偏移地址 
 * @param[in]  size: 数据长度，单位 byte
 * @retval -1: 失败。 0: 成功
 */
int erase(long offset, size_t size)
{
    uint8_t  is_bank2 = 0;
    uint16_t start_page_num;
    uint16_t end_page_num;
    uint16_t page_count_temp;
    uint16_t page_count;
    uint32_t page_error = 0;
#if (ENABLE_SPI_FLASH)
    uint32_t addr = stm32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif
    uint32_t end_addr = addr + size;
    uint32_t progress_unit;
    const uint8_t bank1_page_count = FLASH_BANK_SIZE / FLASH_PAGE_SIZE;
    HAL_StatusTypeDef status = HAL_OK;
    FLASH_EraseInitTypeDef erase_init;

    if (end_addr > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: erase outrange flash size! addr is (0x%p)\n", (void *)end_addr);
        return -1;
    }
    
    progress_unit = 10000 * FLASH_PAGE_SIZE / size;
    
    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Unlock();
    
    /* Get the number of page to erase */
    start_page_num  = (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    end_page_num    = (end_addr - 1 - FLASH_BASE) / FLASH_PAGE_SIZE;
    page_count      = end_page_num - start_page_num + 1;
    page_count_temp = page_count;
    
    BSP_Printf("%s---------------------------------------\r\n", __func__);
    BSP_Printf("bank1_page_count: %d\r\n", bank1_page_count);
    BSP_Printf("start_page_num: %d\r\n", start_page_num);
    BSP_Printf("end_page_num: %d\r\n", end_page_num);
    BSP_Printf("page_count: %d\r\n", page_count);

    if ((start_page_num + page_count) > bank1_page_count)
    {
        is_bank2 = 1;
        
        if (start_page_num > bank1_page_count)
            page_count_temp = 0xFF - start_page_num + 1;
        else
            page_count_temp = bank1_page_count - start_page_num;
    }
    
    BSP_Printf("is_bank2: %d\r\n", is_bank2);
    BSP_Printf("bank1 page count: %d\r\n", page_count_temp);
        
    /* 擦除 bank1 的 page */
    erase_init.Banks     = FLASH_BANK_1;
    erase_init.NbPages   = page_count_temp;
    erase_init.Page      = start_page_num;
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
    status = HAL_FLASHEx_Erase(&erase_init, &page_error);

    /* 擦除跨过 bank2 的 page */
    if (is_bank2 && status == HAL_OK)
    {
        page_count -= page_count_temp;
        printf("bank2 page count: %d\r\n", page_count);
        
        erase_init.Banks     = FLASH_BANK_2;
        erase_init.NbPages   = page_count;
        erase_init.Page      = 256;
        erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
        status = HAL_FLASHEx_Erase(&erase_init, &page_error);
    }

    HAL_FLASH_Lock();
    
    if (status != HAL_OK)
        return -2;

    return size;
}


const struct fal_flash_dev stm32_onchip_flash = 
{
    .name      = FAL_ONCHIP_FLASH_DEV_NAME,
    .addr      = FLASH_BASE,
    .len       = ONCHIP_FLASH_SIZE,
    .blk_size  = FLASH_PAGE_SIZE,
    
    .ops.init  = NULL,
    .ops.read  = read,
    .ops.write = write,
    .ops.erase = erase,
};


