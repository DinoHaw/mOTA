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
 * Version:         v1.0.1
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-23     Dino         the first version
 * 2022-12-08     Dino         ���ӹ̼����ɷ����� SPI flash �Ĺ���
 */

/* Includes ------------------------------------------------------------------*/
#include "bsp_flash.h"
#include "fal_onchip_flash.h"


#if (IS_ENABLE_SPI_FLASH == 0)
/* Private variables ---------------------------------------------------------*/
static struct BSP_FLASH *_part_head;


/* Private function prototypes -----------------------------------------------*/
static void _Flash_Add(struct BSP_FLASH *part);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  �ڲ� flash ��ʼ��
 * @note   
 * @param[in]  part: flash ��������
 * @param[in]  name: flash ��������
 * @param[in]  addr: flash ������ʼ��ַ
 * @param[in]  size: flash ������С����λ byte
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
 * @brief  ��ȡ�ڲ� flash
 * @note   
 * @param[in]   part: flash ��������
 * @param[in]   relative_addr: ��ȡ����Ե�ַ
 * @param[out]  buff: ��ȡ���ŵ����ݳ�
 * @param[in]   size: ��Ҫ��ȡ�Ĵ�С����λ byte
 * @retval -1: ʧ�ܡ� -2: ָ��Ϊ�ա��� 0 ֵ: �Ѷ�ȡ�����ݳ��ȣ���λ byte
 */
int BSP_Flash_Read(const struct BSP_FLASH *part, uint32_t relative_addr, uint8_t *buff, uint32_t size)
{
    ASSERT(part != NULL);
    
    return read(part->addr + relative_addr, buff, size);
}


/**
 * @brief  д�ڲ� flash
 * @note   ����� buff ������ flash ��Сд�� byte ��������������ᷢ���������
 * @param[in]  part: flash ��������
 * @param[in]  relative_addr: д�����Ե�ַ
 * @param[in]  buff: Ҫд�������
 * @param[in]  size: Ҫд������ݴ�С����λ byte
 * @retval -1: ʧ�ܡ� -2: ָ��Ϊ�ա��� 0 ֵ: ��д������ݳ��ȣ���λ byte
 */
inline int BSP_Flash_Write(const struct BSP_FLASH *part, uint32_t relative_addr, const uint8_t *buff, uint32_t size)
{
    ASSERT(part != NULL);
    
    return write(part->addr + relative_addr, buff, size);
}


/**
 * @brief  �����ڲ� flash
 * @note   
 * @param[in]  part: flash ��������
 * @param[in]  relative_addr: �����������ʼ��ַ
 * @param[in]  size: Ҫ�����Ĵ�С����λ: byte
 * @retval -1: ʧ�ܡ� -2: ָ��Ϊ�ա��� 0 ֵ: �Ѳ��������ݳ��ȣ���λ: byte
 */
inline int BSP_Flash_Erase(const struct BSP_FLASH *part, uint32_t relative_addr, uint32_t size)
{
    ASSERT(part != NULL);
    
    return erase(part->addr + relative_addr, size);
}


/**
 * @brief  ͨ�� flash ��������ȡ flash ��������
 * @note   
 * @param[in]  part_name: flash ������
 * @retval flash ��������
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
 * @brief  �� flash ����������ӽ���������
 * @note   
 * @param[in]  part: flash ��������
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



