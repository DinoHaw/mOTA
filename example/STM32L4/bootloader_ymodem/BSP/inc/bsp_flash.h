/**
 * \file            bsp_flash.h
 * \brief           onchip flash driver
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

#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__

#include "bsp_common.h"


struct BSP_FLASH
{
    char name[MAX_NAME_LEN];

    uint32_t addr;
    uint32_t len;
    
    struct BSP_FLASH *next;
};

void                BSP_Flash_Init      (struct BSP_FLASH *part, const char *name, uint32_t addr, uint32_t size);
int                 BSP_Flash_Read      (const struct BSP_FLASH *part, uint32_t relative_addr, uint8_t *buff, uint32_t size);
int                 BSP_Flash_Write     (const struct BSP_FLASH *part, uint32_t relative_addr, const uint8_t *buff, uint32_t size);
int                 BSP_Flash_Erase     (const struct BSP_FLASH *part, uint32_t relative_addr, uint32_t size);
struct BSP_FLASH *  BSP_Flash_GetHandle (const char *part_name);

#endif

