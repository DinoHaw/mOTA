/**
 * \file            bsp_flash.c
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
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2022-11-23     Dino         the first version
 * v1.1     2022-12-08     Dino         增加固件包可放置在 SPI flash 的功能
 * v1.2     2023-12-10     Dino         将 fal_onchip_flash.h 原型放在本文件声明
 */

/* Includes ------------------------------------------------------------------*/
#include "bsp_flash.h"


#if (IS_ENABLE_SPI_FLASH == 0)
/* Private variables ---------------------------------------------------------*/
static struct BSP_FLASH *_part_head;


/* Extern function prototypes ------------------------------------------------*/
extern int read(long offset, uint8_t *buf, size_t size);
extern int write(long offset, const uint8_t *buf, size_t size);
extern int erase(long offset, size_t size);


/* Private function prototypes -----------------------------------------------*/
static void _Flash_Add(struct BSP_FLASH *part);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  内部 flash 初始化
 * @note   
 * @param[in]  part: flash 分区对象
 * @param[in]  name: flash 分区名称
 * @param[in]  addr: flash 分区起始地址
 * @param[in]  size: flash 分区大小，单位 byte
 * @retval None
 */
void BSP_Flash_Init(struct BSP_FLASH *part, const char *name, uint32_t addr, uint32_t size)
{
    ASSERT(part != NULL);
    
    memcpy(part->name, name, MAX_NAME_LEN);
    part->addr = addr;
    part->len  = size;
    
    _Flash_Add(part);
}


/**
 * @brief  读取内部 flash
 * @note   
 * @param[in]   part: flash 分区对象
 * @param[in]   relative_addr: 读取的相对地址
 * @param[out]  buff: 读取后存放的数据池
 * @param[in]   size: 需要读取的大小，单位 byte
 * @retval -1: 失败。 -2: 指针为空。非 0 值: 已读取的数据长度，单位 byte
 */
int BSP_Flash_Read(const struct BSP_FLASH *part, uint32_t relative_addr, uint8_t *buff, uint32_t size)
{
    ASSERT(part != NULL);
    
    return read(part->addr + relative_addr, buff, size);
}


/**
 * @brief  写内部 flash
 * @note   传入的 buff 至少是 flash 最小写入 byte 的整数倍，否则会发生数组溢出
 * @param[in]  part: flash 分区对象
 * @param[in]  relative_addr: 写入的相对地址
 * @param[in]  buff: 要写入的数据
 * @param[in]  size: 要写入的数据大小，单位 byte
 * @retval -1: 失败。 -2: 指针为空。非 0 值: 已写入的数据长度，单位 byte
 */
inline int BSP_Flash_Write(const struct BSP_FLASH *part, uint32_t relative_addr, const uint8_t *buff, uint32_t size)
{
    ASSERT(part != NULL);
    
    return write(part->addr + relative_addr, buff, size);
}


/**
 * @brief  擦除内部 flash
 * @note   
 * @param[in]  part: flash 分区对象
 * @param[in]  relative_addr: 擦除的相对起始地址
 * @param[in]  size: 要擦除的大小，单位: byte
 * @retval -1: 失败。 -2: 指针为空。非 0 值: 已擦除的数据长度，单位: byte
 */
inline int BSP_Flash_Erase(const struct BSP_FLASH *part, uint32_t relative_addr, uint32_t size)
{
    ASSERT(part != NULL);
    
    return erase(part->addr + relative_addr, size);
}


/**
 * @brief  通过 flash 分区名获取 flash 分区对象
 * @note   
 * @param[in]  part_name: flash 分区名
 * @retval flash 分区对象
 */
struct BSP_FLASH *BSP_Flash_GetHandle(const char *part_name)
{
    struct BSP_FLASH *part;
    
    for (part = _part_head; part != NULL; part = part->next)
    {
        if (strncmp(part->name, part_name, MAX_NAME_LEN) == 0)
            return part;
    }
    
    return NULL;
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  将 flash 分区对象添加进管理链表
 * @note   
 * @param[in]  part: flash 分区对象
 * @retval None
 */
static void _Flash_Add(struct BSP_FLASH *part)
{
    struct BSP_FLASH *target;

    for (target = _part_head; target != NULL; target = target->next)
    {
        if (target == part)
            return;
    }

    part->next = _part_head;
    _part_head  = part;
}

#endif



