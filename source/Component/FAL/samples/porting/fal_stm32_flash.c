/**
  ******************************************************************************
  * @file    fal_stm32_flash.c 
  * @author  Dinor
  * @version 1.0.0
  * @date    2019-06-17
  * @brief   This file belongs to the driver layer of FAL, which is managed and 
  *          called by FAL to implement flash erase and write.
  * @logs    Change Logs:
  * Date           Author       Notes
  * 2019-06-17     Dinor        initial implementation, flash write function not
  *                             compatible with HAL libraries that operate in 
  *                             different ways temporarily.
  ******************************************************************************
  */

#include "fal_stm32_flash.h"
#include <fal.h>
#include "bsp_common.h"

/* Extern function prototypes ------------------------------------------------*/
extern void Firmware_OperateCallback(uint16_t progress);

#ifndef USE_HAL_DRIVER
struct FLASH_Erase
{
    uint32_t size;          // The size needed to be erased
    uint32_t addr;          // Address that need to be erased
};
struct FLASH_Erase  flash_erase;
#endif

#ifdef USE_STDPERIPH_DRIVER
struct FLASH_Page
{
    uint8_t size;           // unit: KByte
    uint16_t consecut_num;  // The number of flash page consecutive
};

struct FLASH_Page flash_page[] = 
{
    {2, 256}
};

// Quantity of different flash page sizes
uint8_t flash_diff_size_num = sizeof(flash_page)/sizeof(struct FLASH_Page);

uint8_t STM32_GetSector(uint32_t addr, uint32_t* sec_addr, uint32_t* sector)
{
    uint8_t  sec_step = 0;
    uint8_t  size_type_index = 0;
    uint16_t size_type_num_index = 0;
    uint32_t page_addr = FLASH_BASE;

    for (size_type_index = 0; size_type_index < flash_diff_size_num; size_type_index++)
    {
        for ( size_type_num_index = 0; 
              size_type_num_index < flash_page[ size_type_index ].consecut_num; 
              size_type_num_index++)
        {
//            FAL_PRINTF("[D] flash page addr: 0x%.8x\r\n", page_addr);
//            FAL_PRINTF("[D] flash page size: %d KB\r\n", flash_page[ size_type_index ].size);
            page_addr += (flash_page[ size_type_index ].size * 1024);     // The address at the end of each page
            
            if (addr < page_addr)
            {
                if (sec_addr)
                    *sec_addr = page_addr - (flash_page[ size_type_index ].size * 1024);
                if (sector)
                    *sector = sec_step;
                
                return flash_page[ size_type_index ].size;
            }
            
            sec_step ++;        // flash sector step number 
        }
    }
    
    return 0;
}
#endif

#ifndef USE_HAL_DRIVER
HAL_StatusTypeDef FLASH_Erase (struct FLASH_Erase* erase, uint8_t type)
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
#if (STM32FXX == __STM32F1)
        FLASH_EraseAllPages();
#elif (STM32FXX == __STM32F4)
        FLASH_EraseAllSectors( VoltageRange_3 );
#endif
    }
    else
    {
        erase_offset = erase->addr;
        
        FAL_PRINTF("[D] erase_size: %d\r\n", erase->size);
        
        /* it will stop when erased size is greater than setting size */
        while (erased_size < erase->size)
        {
            size = STM32_GetSector( erase_offset, &cur_erase_addr, &sector );
            
            FAL_PRINTF("[D] cur_erase_addr: 0x%.8x\r\n", cur_erase_addr);     
            
#if (STM32FXX == __STM32F1)
            status = FLASH_ErasePage( cur_erase_addr );
#elif (STM32FXX == __STM32F4)
            sector *= 8;
            
            if (sector > 11)
                sector += 32;
            
            status = FLASH_EraseSector( sector, VoltageRange_3 );
            FAL_PRINTF("[D] sector: 0x%.2x (status = %d)\r\n", sector, status);
#endif
            
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

HAL_StatusTypeDef HAL_FLASH_Unlock(void)
{
    FLASH_Unlock();
    
    return HAL_OK;
}

HAL_StatusTypeDef HAL_FLASH_Lock(void)
{
    FLASH_Lock();
    
    return HAL_OK;
}
#endif

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
#if (ENABLE_SPI_FLASH)
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
    #if (STM32FXX == __STM32F1)
    FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR );
    #elif (STM32FXX == __STM32F4)
    FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |      \
                     FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_RDERR );
    #endif
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

int erase(long offset, size_t size)
{
#ifdef USE_HAL_DRIVER
    uint32_t NbrOfPages = 0;
    uint32_t PageError = 0;
    HAL_StatusTypeDef status = HAL_OK;
    FLASH_EraseInitTypeDef pEraseInit;
#endif
    uint32_t progress_unit = 0;
    uint32_t progress = 0;
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
    progress_unit = 10000 * FLASH_PAGE_SIZE / size;
    
#ifdef USE_HAL_DRIVER
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    #if (STM32FXX == __STM32F1)
//        NbrOfPages = size / FLASH_PAGE_SIZE;
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
        
    #elif (STM32FXX == __STM32F4)
        uint32_t UserStartSector;
    
        /* Get the sector where start the user flash area */
        STM32_GetSector( addr, NULL, &UserStartSector );
    
        pEraseInit.TypeErase = TYPEERASE_SECTORS;
        pEraseInit.Sector = UserStartSector;
        pEraseInit.NbSectors = 10;
        pEraseInit.VoltageRange = VOLTAGE_RANGE_3;
        status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
    #elif (STM32FXX == __STM32L4)
        NbrOfPages = (FLASH_BASE + ONCHIP_FLASH_SIZE);
        NbrOfPages = (NbrOfPages - addr) / FLASH_PAGE_SIZE;

        if(NbrOfPages > FLASH_PAGE_NBPERBANK)
        {
            pEraseInit.Banks = FLASH_BANK_1;
            pEraseInit.NbPages = NbrOfPages % FLASH_PAGE_NBPERBANK;
            pEraseInit.Page = FLASH_PAGE_NBPERBANK - pEraseInit.NbPages;
            pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
            status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);

            NbrOfPages = FLASH_PAGE_NBPERBANK;
        }

        if(status == HAL_OK)
        {
            pEraseInit.Banks = FLASH_BANK_2;
            pEraseInit.NbPages = NbrOfPages;
            pEraseInit.Page = FLASH_PAGE_NBPERBANK - pEraseInit.NbPages;
            pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
            status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
        }
    #elif (STM32FXX == __STM32F0 || STM32FXX == __STM32F3)
        NbrOfPages = ((APP_PART_ADDR + APP_PART_SIZE) - addr)/FLASH_PAGE_SIZE;

        pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        pEraseInit.PageAddress = addr;
        pEraseInit.NbPages = NbrOfPages;
        status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
    #endif
    
    
#elif USE_STDPERIPH_DRIVER
    #if (STM32FXX == __STM32F1)
    FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR );
    #elif (STM32FXX == __STM32F4)
    FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
                     FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_RDERR );
    #endif
    
    #if (STM32FXX == __STM32F1) || (STM32FXX == __STM32F4)
        flash_erase.addr = addr;
        flash_erase.size = size;
    
        if (FLASH_Erase(&flash_erase, FLASH_TYPEERASE_PAGES) == HAL_OK)
        {
            FAL_PRINTF("stm32f1xx erase finish!\r\n");
        }
        else
        {
            FAL_PRINTF("stm32f1xx erase error!\r\n");
        }
    #endif
#endif

    HAL_FLASH_Lock();
    
    if (status != HAL_OK)
    {
        return -2;
    }

    return size;
}

const struct fal_flash_dev stm32_onchip_flash = { FAL_ONCHIP_FLASH_DEV_NAME, FLASH_BASE, ONCHIP_FLASH_SIZE, FLASH_PAGE_SIZE, {NULL, read, write, erase} };

