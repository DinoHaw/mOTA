/**
 * \file            fal_stm32f4_flash.c
 * \brief           stm32f4 onchip flash operate interface for FAL liabrary
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

#include "fal_stm32_flash.h"
#include <fal.h>
#include "bsp_common.h"

/* 是否使能了选项字的 DB1M bit */
#define ENABLE_DB1M_BIT         0

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base @ of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base @ of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base @ of Sector 11, 128 Kbytes */


/* Extern function prototypes ------------------------------------------------*/
extern void Firmware_OperateCallback(uint16_t progress);


/* Private struture ----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static uint32_t _Get_STM32SectorIndex(uint32_t addr);


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
#if (ENABLE_SPI_FLASH)
    uint32_t addr = stm32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif

    FAL_PRINTF("read addr: 0x%.8x\r\n", addr);

    if ((addr + size) > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: read outrange flash size! addr is (0x%p)\n", (void*)(addr + size));
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
    int status = 0;
#if (ENABLE_SPI_FLASH)
    uint32_t addr = stm32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif
    uint32_t end_addr = addr + size;
    
    if (end_addr > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: write outrange flash size! addr is (0x%p)\n", (void*)(addr + size));
        return -1;
    }
    
    if (size < 1)
        return -1;

#if 1
    size_t   data_len = size * sizeof(uint32_t);
    uint32_t *data = (uint32_t *)buf;
    
    HAL_FLASH_Unlock();

    for (uint32_t i = 0; 
         (i < data_len) && (addr <= (end_addr - sizeof(uint32_t))); 
         i++)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
           be done by word */
        if (HAL_FLASH_Program(TYPEPROGRAM_WORD, addr, *(uint32_t *)(data + i)) == HAL_OK)
        {
            /* Check the written value */
            if (*(uint32_t *)addr != *(uint32_t *)(data + i))
            {
                /* Flash content doesn't match SRAM content */
                status = -1;
                break;
            }
            /* Increment FLASH destination address */
            addr += sizeof(uint32_t);
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            status = -2;
            break;
        }
    }
    
    HAL_FLASH_Lock();

    if (status < 0)
        return status;
    
    return size;

#else
    uint8_t ret         = 0;
    uint8_t index       = 0;
    size_t  byte_step   = 0;
    uint32_t write_data = 0;
    uint32_t temp_data  = 0;
    const uint8_t byte_num = sizeof(uint32_t);

    HAL_FLASH_Unlock();
    
    /* Clear pending flags (if any) */
//    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR | \
//                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    
    FAL_PRINTF("write addr: 0x%.8x\r\n", addr);
    for (byte_step = 0; byte_step < size; )
    {
        if ((size - byte_step) < byte_num)         /* The number of bytes less than a WORD/half WORD */
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

        /* write data */
        ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, write_data);
        if (HAL_OK == ret)
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
    HAL_FLASH_Lock();
    
    if (status)
        return status;

    return size;
#endif
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
//    uint32_t progress = 0;
//    uint32_t progress_unit = 0;
    uint32_t page_err = 0;
    HAL_StatusTypeDef  status = HAL_OK;
    FLASH_EraseInitTypeDef  erase_init = {0};
#if (ENABLE_SPI_FLASH)
    uint32_t addr = stm32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif
    uint32_t end_addr = addr + size;

    if (end_addr > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: erase outrange flash size! addr is (0x%p)\n", (void *)end_addr);
        return -1;
    }

    HAL_FLASH_Unlock();
    
    /* Clear pending flags (if any) */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR | \
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    
    uint8_t  sector_count;
    uint32_t sector_head_num;
    uint32_t sector_end_num;
    
    /* 获取起始地址所在的 flash sector */
    sector_head_num = _Get_STM32SectorIndex( addr );
    
    /* 获取结尾地址所在的 flash sector */
    sector_end_num = _Get_STM32SectorIndex( end_addr-1 );
    
    sector_count = sector_end_num - sector_head_num;
    sector_count ++;
    
#if (ENABLE_DB1M_BIT)
    if (sector_head_num > FLASH_SECTOR_7)
        sector_head_num += 4;
#endif
    
    /* 填充数据 */
    erase_init.TypeErase    = TYPEERASE_SECTORS;
    erase_init.Sector       = sector_head_num;
    erase_init.NbSectors    = sector_count;
    erase_init.VoltageRange = VOLTAGE_RANGE_3;
    
    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
       you have to make sure that these data are rewritten before they are accessed during code
       execution. If this cannot be done safely, it is recommended to flush the caches by setting the
       DCRST and ICRST bits in the FLASH_CR register. */
    status = HAL_FLASHEx_Erase(&erase_init, &page_err);

    HAL_FLASH_Lock();
    
    if (status != HAL_OK)
        return -2;

    return size;
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  通过 flash 地址获取对应的扇区序号
 * @note   
 * @param[in]   addr: flash 地址
 * @retval 所在的扇区序号，从 FLASH_SECTOR_0 开始
 */
static uint32_t _Get_STM32SectorIndex(uint32_t addr)
{
    uint32_t sector = 0;

    if ((addr < ADDR_FLASH_SECTOR_1) && (addr >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((addr < ADDR_FLASH_SECTOR_2) && (addr >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((addr < ADDR_FLASH_SECTOR_3) && (addr >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((addr < ADDR_FLASH_SECTOR_4) && (addr >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((addr < ADDR_FLASH_SECTOR_5) && (addr >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
#if defined(FLASH_SECTOR_5)
    else if ((addr < ADDR_FLASH_SECTOR_6) && (addr >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
#endif
#if defined(FLASH_SECTOR_6)
    else if ((addr < ADDR_FLASH_SECTOR_7) && (addr >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
#endif
#if defined(FLASH_SECTOR_7)
    else if ((addr < ADDR_FLASH_SECTOR_8) && (addr >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_SECTOR_7;
    }
#endif
#if defined(FLASH_SECTOR_8)
    else if ((addr < ADDR_FLASH_SECTOR_9) && (addr >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_SECTOR_8;
    }
#endif    
#if defined(FLASH_SECTOR_9)
    else if ((addr < ADDR_FLASH_SECTOR_10) && (addr >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_SECTOR_9;
    }
#endif
#if defined(FLASH_SECTOR_10)
    else if ((addr < ADDR_FLASH_SECTOR_11) && (addr >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_SECTOR_10;
    }
#endif
#if defined(FLASH_SECTOR_11)
    else if ((addr < ADDR_FLASH_SECTOR_12) && (addr >= ADDR_FLASH_SECTOR_11))
    {
        sector = FLASH_SECTOR_11;
    }
#endif
#if defined(FLASH_SECTOR_12)
    else if ((addr < ADDR_FLASH_SECTOR_13) && (addr >= ADDR_FLASH_SECTOR_12))
    {
        sector = FLASH_SECTOR_12;
    }
#endif
#if defined(FLASH_SECTOR_13)
    else if ((addr < ADDR_FLASH_SECTOR_14) && (addr >= ADDR_FLASH_SECTOR_13))
    {
        sector = FLASH_SECTOR_13;
    }
#endif
#if defined(FLASH_SECTOR_14)
    else if ((addr < ADDR_FLASH_SECTOR_15) && (addr >= ADDR_FLASH_SECTOR_14))
    {
        sector = FLASH_SECTOR_14;
    }
#endif
#if defined(FLASH_SECTOR_15)
    else if ((addr < ADDR_FLASH_SECTOR_16) && (addr >= ADDR_FLASH_SECTOR_15))
    {
        sector = FLASH_SECTOR_15;
    }
#endif
#if defined(FLASH_SECTOR_16)
    else if ((addr < ADDR_FLASH_SECTOR_17) && (addr >= ADDR_FLASH_SECTOR_16))
    {
        sector = FLASH_SECTOR_16;
    }
#endif
#if defined(FLASH_SECTOR_17)
    else if ((addr < ADDR_FLASH_SECTOR_18) && (addr >= ADDR_FLASH_SECTOR_17))
    {
        sector = FLASH_SECTOR_17;
    }
#endif    
#if defined(FLASH_SECTOR_18)
    else if ((addr < ADDR_FLASH_SECTOR_19) && (addr >= ADDR_FLASH_SECTOR_18))
    {
        sector = FLASH_SECTOR_18;
    }
#endif
#if defined(FLASH_SECTOR_19)
    else if ((addr < ADDR_FLASH_SECTOR_20) && (addr >= ADDR_FLASH_SECTOR_19))
    {
        sector = FLASH_SECTOR_19;
    }
#endif    
#if defined(FLASH_SECTOR_20)
    else if ((addr < ADDR_FLASH_SECTOR_21) && (addr >= ADDR_FLASH_SECTOR_20))
    {
        sector = FLASH_SECTOR_20;
    }
#endif
#if defined(FLASH_SECTOR_21)
    else if ((addr < ADDR_FLASH_SECTOR_22) && (addr >= ADDR_FLASH_SECTOR_21))
    {
        sector = FLASH_SECTOR_21;
    }
#endif
#if defined(FLASH_SECTOR_22)
    else if ((addr < ADDR_FLASH_SECTOR_23) && (addr >= ADDR_FLASH_SECTOR_22))
    {
        sector = FLASH_SECTOR_22;
    }
#endif    
#if defined(FLASH_SECTOR_23)
    else /* (addr < FLASH_END_ADDR) && (addr >= ADDR_FLASH_SECTOR_23) */
    {
        sector = FLASH_SECTOR_23;
    }
#endif    

    return sector;
}


const struct fal_flash_dev stm32_onchip_flash = 
{
    .name      = FAL_ONCHIP_FLASH_DEV_NAME,
    .addr      = FLASH_BASE,
    .len       = ONCHIP_FLASH_SIZE,
//    .blk_size  = FLASH_PAGE_SIZE,
    
    .ops.init  = NULL,
    .ops.read  = read,
    .ops.write = write,
    .ops.erase = erase,
};

