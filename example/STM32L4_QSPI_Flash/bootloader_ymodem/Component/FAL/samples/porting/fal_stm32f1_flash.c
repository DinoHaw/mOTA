/**
 * \file            fal_stm32f1_flash.c
 * \brief           stm32f1 onchip flash operate interface for FAL liabrary
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


/* Private function prototypes -----------------------------------------------*/
#ifdef USE_STDPERIPH_DRIVER
struct FLASH_ERASE
{
    uint32_t size;          /* The size needed to be erased */
    uint32_t addr;          /* Address that need to be erased */
};

struct FLASH_PAGE
{
    uint8_t size;           /* unit: KByte */
    uint16_t consecut_num;  /* The number of flash page consecutive */
};


/* Private function prototypes -----------------------------------------------*/
static struct FLASH_ERASE _flash_erase;
static struct FLASH_PAGE _flash_page[] = 
{
    {2, 256}
};

/* Quantity of different flash page sizes */
static const uint8_t _flash_diff_size_num = sizeof(_flash_page) / sizeof(struct FLASH_PAGE);


/**
 * @brief  通过 flash 地址获取对应的扇区首地址、扇区序号和扇区大小
 * @note   
 * @param[in]   addr:  flash 地址
 * @param[out]  sec_addr: 所在的扇区首地址
 * @param[out]  sector: 所在的扇区序号
 * @retval 0: 失败。非 0: 所在的扇区大小，单位 byte
 */
static uint8_t _Get_STM32Sector(uint32_t addr, uint32_t *sec_addr, uint32_t *sector)
{
    uint8_t  sec_step = 0;
    uint8_t  size_type_index = 0;
    uint16_t size_type_num_index = 0;
    uint32_t page_addr = FLASH_BASE;

    for (size_type_index = 0; size_type_index < _flash_diff_size_num; size_type_index++)
    {
        for ( size_type_num_index = 0; 
              size_type_num_index < _flash_page[ size_type_index ].consecut_num; 
              size_type_num_index++)
        {
//            FAL_PRINTF("[D] flash page addr: 0x%.8x\r\n", page_addr);
//            FAL_PRINTF("[D] flash page size: %d KB\r\n", _flash_page[ size_type_index ].size);
            page_addr += (_flash_page[ size_type_index ].size * 1024);     /* The address at the end of each page */
            
            if (addr < page_addr)
            {
                if (sec_addr)
                    *sec_addr = page_addr - (_flash_page[ size_type_index ].size * 1024);
                if (sector)
                    *sector = sec_step;
                
                return _flash_page[ size_type_index ].size;
            }
            
            sec_step ++;        /* flash sector step number */
        }
    }
    
    return 0;
}


/**
 * @brief  擦除 flash
 * @note   
 * @param[in]  erase: 擦除对象
 * @param[in]  type: 擦除操作类型
 * @retval HAL Status
 */
static HAL_StatusTypeDef _Flash_Erase(struct FLASH_ERASE *erase, uint8_t type)
{
    uint8_t  size = 0;
    uint32_t sector = 0;
    uint16_t status = FLASH_BUSY;
    uint32_t erased_size = 0;
    uint32_t erase_offset = 0;
    uint32_t cur_erase_addr = 0;

    if (type == FLASH_TYPEERASE_MASSERASE)
    {
        /* Mass erase to be done */
        FLASH_EraseAllPages();
    }
    else
    {
        erase_offset = erase->addr;
        
        FAL_PRINTF("[D] erase_size: %d\r\n", erase->size);
        
        /* it will stop when erased size is greater than setting size */
        while (erased_size < erase->size)
        {
            size = _Get_STM32Sector( erase_offset, &cur_erase_addr, &sector );
            
            FAL_PRINTF("[D] cur_erase_addr: 0x%.8x\r\n", cur_erase_addr);     
            
            status = FLASH_ErasePage( cur_erase_addr );
            
            if (status != FLASH_COMPLETE)
            {
                /* In case of error, stop erase procedure and return the faulty address */
                return HAL_ERROR;
            }
            erased_size += (cur_erase_addr + (size * 1024) - erase_offset);
            
            erase_offset = cur_erase_addr + (size * 1024);  
        }
    }

    if (status == FLASH_COMPLETE)
        return HAL_OK;
    
    return HAL_ERROR;
}


/**
  * @brief  Unlock the FLASH control register access
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Unlock(void)
{
    FLASH_Unlock();
    
    return HAL_OK;
}


/**
  * @brief  Locks the FLASH control register access
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Lock(void)
{
    FLASH_Lock();
    
    return HAL_OK;
}
#endif


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
    uint8_t ret = 0;
    size_t  byte_step = 0;
    uint8_t index = 0;
    int8_t  status = 0;
    uint8_t byte_num = sizeof(uint32_t);
#ifdef USE_HAL_DRIVER
    uint32_t write_data = 0, temp_data = 0;
#elif USE_STDPERIPH_DRIVER
    uint32_t write_data = 0, temp_data = 0;
#endif
#if (IS_ENABLE_SPI_FLASH)
    uint32_t addr = stm32_onchip_flash.addr + offset;
#else
    uint32_t addr = offset;
#endif
    
    if ((addr + size) > ONCHIP_FLASH_END_ADDRESS)
    {
        FAL_PRINTF("ERROR: write outrange flash size! addr is (0x%p)\n", (void*)(addr + size));
        return -1;
    }
    
    if (size < 1)
    {
        return -1;
    }

    HAL_FLASH_Unlock();
    
#ifdef USE_HAL_DRIVER
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    byte_num = sizeof(uint32_t);
#elif USE_STDPERIPH_DRIVER
    FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR );
#endif
    
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
#ifdef USE_HAL_DRIVER
        ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, write_data);
        if (HAL_OK == ret)
        {
            /* Check the written value */
            if (*(uint32_t *)addr != write_data)
#elif USE_STDPERIPH_DRIVER
        ret = FLASH_ProgramWord(addr, write_data);
        if (ret == FLASH_COMPLETE)
        {
            /* Check the written value */
            if (*(uint32_t *)addr != write_data)
#endif
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
        
        temp_data = 0;
        write_data = 0;

        addr += byte_num;
    }

__exit:
    HAL_FLASH_Lock();
    
    if (status)
    {
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
#ifdef USE_HAL_DRIVER
    uint32_t PageError = 0;
    HAL_StatusTypeDef status = HAL_OK;
    FLASH_EraseInitTypeDef pEraseInit;
#endif
    uint32_t progress_unit = 0;
    uint32_t progress = 0;
#if (IS_ENABLE_SPI_FLASH)
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
    progress_unit = 10000 * FLASH_PAGE_SIZE / size;
    
#ifdef USE_HAL_DRIVER
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    pEraseInit.TypeErase   = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = addr;
    pEraseInit.Banks       = FLASH_BANK_1;
    pEraseInit.NbPages     = 1;

    for (;
         pEraseInit.PageAddress < end_addr;
         pEraseInit.PageAddress += FLASH_PAGE_SIZE)
    {
        status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
        if (status != HAL_OK)
            break;
        
        progress += progress_unit;
        Firmware_OperateCallback(progress);
    }
#elif USE_STDPERIPH_DRIVER
    FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR );
    
    _flash_erase.addr = addr;
    _flash_erase.size = size;

    if (_Flash_Erase(&_flash_erase, FLASH_TYPEERASE_PAGES) == HAL_OK)
    {
        FAL_PRINTF("stm32f1xx erase finish!\r\n");
    }
    else
    {
        FAL_PRINTF("stm32f1xx erase error!\r\n");
    }
#endif

    HAL_FLASH_Lock();
    
    if (status != HAL_OK)
    {
        return -2;
    }

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

