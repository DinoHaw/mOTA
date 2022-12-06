/**
 * \file            firmware_manage.c
 * \brief           firmware package manager
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
 * 2022-12-07     Dino         修复 STM32L4 写入 flash 的最小单位问题
 */


/* Includes ------------------------------------------------------------------*/
#include "firmware_manage.h"


/* Private variables ---------------------------------------------------------*/
static uint8_t  _fw_start_write_flag;                           /* 固件开始写入的标志位 */
static uint16_t _update_progress;                               /* 固件更新的进度， 10000 制 */
static uint16_t _update_progress_step_num;                      /* 固件更新进度的步进单位 */
static uint16_t _storage_data_size;                             /* 固件包写入时记录暂存的固件分包大小，单位 byte */
static uint32_t _write_part_addr;                               /* 固件包写入时记录写入 flash 的相对地址 */
static uint16_t _write_last_pkg_size;                           /* 固件包写入时最后一个分包的大小，单位 byte */
static uint8_t  _fw_first_bytes[ONCHIP_FLASH_ONCE_WRITE_BYTE];  /* 固件包的前几个字节 */
static uint8_t  _fpk_min_handle_buff[FPK_LEAST_HANDLE_BYTE];    /* fpk 固件最小处理单位的缓存区，多次使用以降低系统资源开销 */
static struct FPK_HEAD  _fpk_head;                              /* 用于存放 fpk 固件包头 */
#if (ENABLE_DECRYPT)
static struct AES_ctx  _aes_ctx;                                /* AES 对象 */
#endif
#if (ENABLE_SPI_FLASH == 0)
static struct BSP_FLASH _flash_app_part;                        /* APP 分区 */
    #if (USING_PART_PROJECT > ONE_PART_PROJECT)
    static struct BSP_FLASH _flash_download_part;               /* download 分区 */
        #if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
        static struct BSP_FLASH _flash_factory_part;            /* factory 分区 */
        #endif
    #endif
#endif


/* Extern function prototypes ------------------------------------------------*/
extern void Firmware_OperateCallback(uint16_t progress);


/* Private function prototypes -----------------------------------------------*/
#if (ENABLE_SPI_FLASH)
static FM_ERR_CODE  _Write_FirmwareSubPackage( const struct fal_partition *part, 
                                               uint8_t  *data, 
                                               uint16_t pkg_size, 
                                               uint8_t  decrypt,
                                               FM_FIRMWARE_WRITE_DIR  write_dir);
#else
static FM_ERR_CODE  _Write_FirmwareSubPackage( const struct BSP_FLASH *part, 
                                               uint8_t  *data, 
                                               uint16_t pkg_size, 
                                               uint8_t  decrypt,
                                               FM_FIRMWARE_WRITE_DIR  write_dir);
#endif
static void _Reset_Write(void);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  初始化接口
 * @note   
 * @retval None
 */
void FM_Init(void)
{
    /* TODO: 后续考虑用硬件 CRC ，提供配置选项，允许在软件 CRC 和硬件 CRC 之间选择 */
    CRC32_Init();
#if (ENABLE_SPI_FLASH)
    /* FAL 初始化 */
    fal_init();
#else
    /* 内部 flash 分区初始化 */
    BSP_Flash_Init(&_flash_app_part, APP_PART_NAME, APP_ADDRESS, APP_PART_SIZE);
    #if (USING_PART_PROJECT > ONE_PART_PROJECT)
    BSP_Flash_Init(&_flash_download_part, DOWNLOAD_PART_NAME, DOWNLOAD_ADDRESS, DOWNLOAD_PART_SIZE);
        #if (USING_PART_PROJECT > DOUBLE_PART_PROJECT)
        BSP_Flash_Init(&_flash_factory_part, FACTORY_PART_NAME, FACTORY_ADDRESS, FACTORY_PART_SIZE);
        #endif
    #endif
    
    BSP_Printf("app addr: 0x%.8X\r\n", _flash_app_part.addr);
    #if (USING_PART_PROJECT > ONE_PART_PROJECT)
    BSP_Printf("download addr: 0x%.8X\r\n", _flash_download_part.addr);
        #if (USING_PART_PROJECT > DOUBLE_PART_PROJECT)
        BSP_Printf("factory addr: 0x%.8X\r\n", _flash_factory_part.addr);
        #endif
    #endif
#endif
#if (ENABLE_DECRYPT)
    AES_init_ctx_iv(&_aes_ctx, (uint8_t *)AES256_KEY, (uint8_t *)AES256_IV);
#endif
}


/**
 * @brief  固件包是否有加密
 * @note   调用前需确保 _fpk_head 已经读入了数据
 * @retval 0: 未加密。1: 有加密
 */
inline uint8_t FM_IsEncrypt(void)
{
    /* 读取加密选项 */
    if (_fpk_head.config[1] == 0x01)
        return 1;
    return 0;
}


/**
 * @brief  检测某个分区是否为空
 * @note   FM_ERR_OK: 分区数据空
 * @param[in]  part_name: 分区名
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_IsEmpty(const char *part_name)
{
    int read_len = 0;
    uint16_t i = 0;
    uint16_t need_read_size = FPK_LEAST_HANDLE_BYTE;
    uint32_t *p_data = (uint32_t *)_fpk_min_handle_buff;
    uint32_t read_posit = 0;
    
    ASSERT(part_name != NULL);
    
#if (ENABLE_SPI_FLASH)     
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
#else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
#endif
    
    for (read_posit = 0; read_posit < part->len; )
    {
        if ((part->len - read_posit) < FPK_LEAST_HANDLE_BYTE)
            need_read_size = part->len - read_posit;
        
    #if (ENABLE_SPI_FLASH) 
        read_len = fal_partition_read(part, read_posit, _fpk_min_handle_buff, need_read_size);
    #else
        read_len = BSP_Flash_Read(part, read_posit, _fpk_min_handle_buff, need_read_size);
    #endif
        if (read_len < 0)
        {
            BSP_Printf("%s: read error (%d).\r\n", __func__, __LINE__);
            return FM_ERR_READ_IS_EMPTY_ERR;
        }
        
        for (i = 0; i < (FPK_LEAST_HANDLE_BYTE / sizeof(p_data)); i++)
        {
            if (p_data[i] != 0xFFFFFFFF)
            {
                BSP_Printf("%s: %s part no empty\r\n", __func__, part_name);
                return FM_ERR_FLASH_NO_EMPTY;
            }
        }
        read_posit += read_len;
    }
    
    BSP_Printf("%s: %s part empty\r\n", __func__, part_name);
    return FM_ERR_OK;
}


/**
 * @brief  获取固件包的固件版本
 * @note   调用前需确保 _fpk_head 已经读入了数据
 * @retval 固件新版本
 */
inline char * FM_GetNewFirmwareVersion(void)
{
    return _fpk_head.fw_new_ver;
}


/**
 * @brief  获取源固件的 CRC32 值
 * @note   调用前需确保 _fpk_head 已经读入了数据
 * @retval 源固件的 CRC32 值
 */
inline uint32_t FM_GetRawCRC32(void)
{
    return _fpk_head.raw_crc;
}


/**
 * @brief  暂存固件包头
 * @note   会同时校验固件包头
 * @param[in]  part_name: 分区名
 * @param[in]  data: 数据
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_StorageFirmwareHead(const char *part_name, uint8_t *data)
{
    uint32_t head_crc = 0;
    uint8_t  *p_fpk_head = (uint8_t *)&_fpk_head;

    ASSERT(part_name != NULL);
    ASSERT(data != NULL);
    
    _Reset_Write();
    memcpy(p_fpk_head, data, FPK_HEAD_SIZE);

#if (ENABLE_DECRYPT == 0)
    /* 若固件包加密，检查是否有解密组件 */
    if (FM_IsEncrypt())
    {
        BSP_Printf("%s: no decrypt component\r\n", __func__);
        return FM_ERR_NO_DECRYPT_COMPONENT;
    }
#endif

#if (ENABLE_SPI_FLASH) 
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
#else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(part_name);
#endif
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    BSP_Printf("%s: %s part\r\n", __func__, part_name);

    if (strncmp(_fpk_head.name, "fpk", sizeof("fpk")) != 0)
        return FM_ERR_FAULT_FIRMWARE;
    
    if ((_fpk_head.pkg_size > part->len)
    ||  (_fpk_head.raw_size > part->len))
        return FM_ERR_FIRMWARE_OVERSIZE;

    /* 校验固件包头数据的正确性 */
    head_crc = CRC32_Calc(p_fpk_head, FPK_HEAD_SIZE - 4);
    if (head_crc != _fpk_head.head_crc)
    {
        BSP_Printf("%s: head crc verify failed. (%.8X - %.8X)\r\n", __func__, _fpk_head.head_crc, head_crc);
        return FM_ERR_FIRMWARE_HEAD_VERIFY_ERR;
    }

    BSP_Printf("name: %s\r\n", _fpk_head.name);
    BSP_Printf("config: %d %d %d %d\r\n", _fpk_head.config[0], _fpk_head.config[1], _fpk_head.config[2], _fpk_head.config[3]);
    BSP_Printf("fw_old_ver: V%d.%d.%d.%d\r\n", _fpk_head.fw_old_ver[0], _fpk_head.fw_old_ver[1], _fpk_head.fw_old_ver[2], _fpk_head.fw_old_ver[3]);
    BSP_Printf("fw_new_ver: V%d.%d.%d.%d\r\n", _fpk_head.fw_new_ver[0], _fpk_head.fw_new_ver[1], _fpk_head.fw_new_ver[2], _fpk_head.fw_new_ver[3]);
    BSP_Printf("user_string: %s\r\n",_fpk_head.user_string);
    BSP_Printf("part_name: %s\r\n", _fpk_head.part_name);
    BSP_Printf("raw_size: %d byte\r\n", _fpk_head.raw_size);
    BSP_Printf("pkg_size: %d byte\r\n", _fpk_head.pkg_size);
    BSP_Printf("timestamp: %d\r\n", _fpk_head.timestamp);
    BSP_Printf("raw_crc: %.8X\r\n", _fpk_head.raw_crc);
    BSP_Printf("pkg_crc: %.8X\r\n", _fpk_head.pkg_crc);
    BSP_Printf("head_crc: %.8X\r\n", _fpk_head.head_crc);
    
    /* 计算固件更新进度的最小单位，降低过程计算量，无更新进度需求可删除 */
    _update_progress_step_num  = _fpk_head.pkg_size / FPK_LEAST_HANDLE_BYTE;
    _update_progress_step_num += _fpk_head.pkg_size % FPK_LEAST_HANDLE_BYTE;
    _update_progress_step_num  = 10000 / _update_progress_step_num;
    BSP_Printf("%s: progress unit: %d\r\n", __func__, _update_progress_step_num);

    return FM_ERR_OK;
}


/**
 * @brief  校验已放置在分区的固件包的包体数据的正确性
 * @note   一般要先校验包头，注意各分区包体的偏移地址有区别
 * @param[in]  part_name: 分区名称
 * @param[in]  crc32: 需进行比对的 CRC32 校验值
 * @param[in]  is_auto_fill: 是否自动填充固件的首地址数据
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_VerifyFirmware(const char *part_name, uint32_t crc32, uint8_t is_auto_fill)
{
    int      read_len = 0;
    uint8_t  app_part_flag = 0;
    uint32_t pkg_size = 0;
    uint32_t body_crc = 0xFFFFFFFF;
    uint32_t read_posit = 0;
    uint32_t read_posit_temp = 0;
    uint16_t need_read_size = FPK_LEAST_HANDLE_BYTE;
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
    uint8_t  first_flag = 0;
#endif
    
    ASSERT(part_name != NULL);

#if (ENABLE_DECRYPT == 0)
    /* 若固件包加密，检查是否有解密组件 */
    if (FM_IsEncrypt())
    {
        BSP_Printf("%s: no decrypt component\r\n", __func__);
        return FM_ERR_NO_DECRYPT_COMPONENT;
    }
#endif

    if (strncmp(part_name, APP_PART_NAME, MAX_NAME_LEN) == 0)
        app_part_flag = 1;

#if (ENABLE_SPI_FLASH)     
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
#else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
#endif

    BSP_Printf("fpk size: %d byte\r\n", FPK_HEAD_SIZE);
    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t *p = (uint8_t *)&_fpk_head;
        for (uint8_t j = 0; j < FPK_HEAD_SIZE / 6; j++)
            BSP_Printf("%.2X ", p[(i * (FPK_HEAD_SIZE / 6)) + j]);
        BSP_Printf("\r\n");
    }
    BSP_Printf("%s: part name %s\r\n", __func__, part_name);
    
    if (app_part_flag)
        pkg_size = _fpk_head.raw_size;
    else
        pkg_size = _fpk_head.pkg_size;
    BSP_Printf("pkg_size %d\r\n", pkg_size);
    
    /* 校验固件包体的数据正确性 */
    for (; read_posit < pkg_size; )
    {
        /* 剩余的数据数小于最小处理单位时，按剩余字节数处理 */
        if ((pkg_size - read_posit) < FPK_LEAST_HANDLE_BYTE)
            need_read_size = pkg_size - read_posit;
        
        /* 非 APP 分区，需要偏移包头的地址才是包体 */
        if (app_part_flag)
            read_posit_temp = read_posit;
        else
            read_posit_temp = read_posit + FPK_HEAD_SIZE;
        
    #if (ENABLE_SPI_FLASH) 
        read_len = fal_partition_read(part, read_posit_temp, &_fpk_min_handle_buff[0], need_read_size);
    #else
        read_len = BSP_Flash_Read(part, read_posit_temp, &_fpk_min_handle_buff[0], need_read_size);
    #endif
        if (read_len < 0)
        {
            BSP_Printf("%s: read error (%d).\r\n", __func__, __LINE__);
            return FM_ERR_VERIFY_READ_ERR;
        }

    #if (USING_PART_PROJECT > ONE_PART_PROJECT)
        if (is_auto_fill)
        {
            if (first_flag == 0)
            {
                first_flag = 1;
                BSP_Printf("_fw_first_bytes: ");
                for (uint8_t i = 0; i < ONCHIP_FLASH_ONCE_WRITE_BYTE; i++)
                {
                    _fpk_min_handle_buff[i] = _fw_first_bytes[i];
                    BSP_Printf("%.2X ", _fw_first_bytes[i]);
                }
                BSP_Printf("\r\n");
            }
        }
    #endif

        body_crc = CRC32_StepCalc(body_crc, &_fpk_min_handle_buff[0], read_len);
        read_posit += read_len;
    }
    body_crc ^= 0xFFFFFFFF;
    
    if (body_crc != crc32)
    {
        BSP_Printf("%s: body crc verify failed. (%.8X - %.8X)\r\n", __func__, crc32, body_crc);
        if (app_part_flag)
            return FM_ERR_RAW_BODY_VERIFY_ERR;
        else
            return FM_ERR_PKG_BODY_VERIFY_ERR;
    }
    
    return FM_ERR_OK;
}


/**
 * @brief  擦除某个分区的固件
 * @note   
 * @param[in]  part_name: 分区名称
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_EraseFirmware(const char *part_name)
{
    ASSERT(part_name != NULL);

#if (ENABLE_SPI_FLASH) 
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found %s part.\r\n", __func__, part_name);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (fal_partition_erase(part, 0, part->len) < 0)
#else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found %s part.\r\n", __func__, part_name);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (BSP_Flash_Erase(part, 0, part->len) < 0)
#endif
    {
        BSP_Printf("%s: %s part erase failed.\r\n", __func__, part_name);
        return FM_ERR_ERASE_PART_ERR;
    }  
    
    return FM_ERR_OK;
}


/**
 * @brief  固件写入分区的最终阶段，将分区首地址的几个字节数据写入 flash
 * @note   程序调用 _Write_FirmwareSubPackage 函数时已暂存进 _fw_first_bytes
 * @param[in]  part_name: 分区名称
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_WriteFirmwareDone(const char *part_name)
{
    ASSERT(part_name != NULL);

#if (ENABLE_SPI_FLASH)     
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }

    if (fal_partition_write(part, 0, _fw_first_bytes, ONCHIP_FLASH_ONCE_WRITE_BYTE) < 0)
#else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }

    if (BSP_Flash_Write(part, 0, _fw_first_bytes, ONCHIP_FLASH_ONCE_WRITE_BYTE) < 0)
#endif
    {
        BSP_Printf("%s: write error (%d).\r\n", __func__, __LINE__);
        return FM_ERR_WRITE_FIRST_ADDR_ERR;
    }
    
    _Reset_Write();
    Firmware_OperateCallback(10000);

    return FM_ERR_OK;
}


/**
 * @brief  将固件分包按顺序写入分区
 * @note   由于固件包头已经写入，这里写入的是固件包体，需要注意在 flash 的偏移位置
 * @param[in]  part_name: 分区名称
 * @param[in]  data: 数据包
 * @param[in]  pkg_size: 数据包大小，单位 byte
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_WriteFirmwareSubPackage(const char *part_name, uint8_t *data, uint16_t pkg_size)
{
    ASSERT(part_name != NULL);
    ASSERT(data != NULL);
    ASSERT(pkg_size != 0);

#if (ENABLE_SPI_FLASH)  
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
#else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(part_name);
#endif
    if (part == NULL)
    {
        BSP_Printf("%s: not found %s part.\r\n", __func__, part_name);
        return FM_ERR_NO_THIS_PART;
    }
    BSP_Printf("%s: %s part\r\n", __func__, part_name);
    
#if (USING_PART_PROJECT == ONE_PART_PROJECT)
    uint8_t decrypt = 0;

    /* 读取加密选项 */
    decrypt = FM_IsEncrypt();

    return _Write_FirmwareSubPackage(part, data, pkg_size, decrypt, FM_DIR_HOST_TO_APP); /* 写入前解密 */
#else
    return _Write_FirmwareSubPackage(part, data, pkg_size, 0, FM_DIR_HOST_TO_DOWNLOAD);  /* 写入前不解密 */
#endif
}


/**
 * @brief  检查固件的完整性
 * @note   通过首地址数据最后写入的机制，判断首地址 4 个字节是否有正确的数据
 * @param[in]  addr: 分区首地址
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_CheckFirmwareIntegrity(uint32_t addr)
{
    uint32_t value = *(volatile uint32_t *)addr;
    FM_ERR_CODE  fw_integrity = FM_ERR_JUMP_TO_APP_ERR;
    
    BSP_Printf("0x%.8X address data: 0x%.8X\r\n", addr, value);
    
    if (APP_ADDRESS == addr)
    {
        fw_integrity = (value & 0x2FF00000) == 0x20000000? FM_ERR_OK : FM_ERR_JUMP_TO_APP_ERR;
    }
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
    else if ((DOWNLOAD_ADDRESS == addr)
    ||       (FACTORY_ADDRESS  == addr))
    {
        if (FPK_IDENTIFIER == value)
            fw_integrity = FM_ERR_OK;
    }
#endif
    
    return fw_integrity;
}


#if (USING_PART_PROJECT > ONE_PART_PROJECT)
/**
 * @brief  判断是否要进行固件自动更新
 * @note   调用前需确保 _fpk_head 已经读入了数据
 * @retval 0: 无须更新。1: 需自动更新
 */
uint8_t FM_IsNeedAutoUpdate(void)
{
    BSP_Printf("%s\r\n", __func__);

#if (USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
    #if (ENABLE_SPI_FLASH)  
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(APP_PART_NAME);
    #else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(APP_PART_NAME);
    #endif
    if (part == NULL)
    {
        BSP_Printf("%s: not found app part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (BSP_Flash_Read(part, (APP_PART_SIZE - FPK_VERSION_SIZE), (uint8_t *)&_fpk_head.fw_old_ver[0], FPK_VERSION_SIZE) < 0)
    {
        BSP_Printf("%s: read error.\r\n", __func__);
        return FM_ERR_READ_VER_ERR;
    }
#endif

    BSP_Printf("fw old ver: V%d.%d.%d.%d\r\n", _fpk_head.fw_old_ver[0], _fpk_head.fw_old_ver[1], _fpk_head.fw_old_ver[2], _fpk_head.fw_old_ver[3]);
    BSP_Printf("fw new ver: V%d.%d.%d.%d\r\n", _fpk_head.fw_new_ver[0], _fpk_head.fw_new_ver[1], _fpk_head.fw_new_ver[2], _fpk_head.fw_new_ver[3]);

    if (_fpk_head.fw_old_ver[0] != _fpk_head.fw_new_ver[0]
    ||  _fpk_head.fw_old_ver[1] != _fpk_head.fw_new_ver[1]
    ||  _fpk_head.fw_old_ver[2] != _fpk_head.fw_new_ver[2]
    ||  _fpk_head.fw_old_ver[3] != _fpk_head.fw_new_ver[3])
        return 1;
    else
        return 0;
    
//    if (strncmp(_fpk_head.fw_old_ver, _fpk_head.fw_new_ver, FPK_VERSION_SIZE) == 0)
//        return 0;
//    return 1;
}


/**
 * @brief  获取当前操作的固件包的分区名称
 * @note   调用前需确保 _fpk_head 已经读入了数据
 * @retval 分区名称
 */
inline char * FM_GetPartName(void)
{
    return _fpk_head.part_name;
}


/**
 * @brief  获取旧的固件版本，即 APP 分区正在运行的固件版本
 * @note   调用前需确保 _fpk_head 已经读入了数据
 * @retval 固件旧版本
 */
inline char * FM_GetOldFirmwareVersion(void)
{
    return _fpk_head.fw_old_ver;
}


/**
 * @brief  获取打包后固件的 CRC32 值
 * @note   调用前需确保 _fpk_head 已经读入了数据
 * @retval 打包后固件的CRC32值
 */
inline uint32_t FM_GetPackageCRC32(void)
{
    return _fpk_head.pkg_crc;
}


/**
 * @brief  将固件包头写入 flash
 * @note   
 * @retval FM_ERR_CODE
 */
//FM_ERR_CODE  FM_WriteFirmwareHead(const char *part_name)
//{
//    uint8_t *p_fpk = (uint8_t *)&_fpk_head;
//    
//    _fw_first_bytes[0] = p_fpk[0];
//    _fw_first_bytes[1] = p_fpk[1];
//    _fw_first_bytes[2] = p_fpk[2];
//    _fw_first_bytes[3] = p_fpk[3];
//    
//#if (ENABLE_SPI_FLASH) 
//    const struct fal_partition *part = NULL;

//    part = fal_partition_find(part_name);
//    if (part == NULL)
//    {
//        BSP_Printf("%s: not found.\r\n", __func__);
//        return FM_ERR_NO_THIS_PART;
//    }

//    if (fal_partition_write(part, 4, &p_fpk[4], FPK_HEAD_SIZE - 4) < 0)
//#else
//    const struct BSP_FLASH *part = NULL;
//    
//    part = BSP_Flash_GetHandle(part_name);
//    if (part == NULL)
//    {
//        BSP_Printf("%s: not found.\r\n", __func__);
//        return FM_ERR_NO_THIS_PART;
//    }
//    BSP_Printf("%s: %s part 0x%.8X\r\n", __func__, part_name, part->addr);
//    
//    if (BSP_Flash_Write(part, 4, &p_fpk[4], FPK_HEAD_SIZE - 4) < 0)
//#endif
//    {
//        BSP_Printf("%s: write error (%d).\r\n", __func__, __LINE__);
//        return FM_ERR_WRITE_FLASH_ERR;
//    }
//    
//    return FM_ERR_OK;
//}


/**
 * @brief  将分区内的固件包头读出
 * @note   读出后的数据将放在 _fpk_head 中
 * @param[in]  part_name: 分区名称
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_ReadFirmwareHead(const char *part_name)
{
    ASSERT(part_name != NULL);

    _Reset_Write();
#if (ENABLE_SPI_FLASH)     
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (fal_partition_read(part, 0, (uint8_t *)&_fpk_head, FPK_HEAD_SIZE) < 0)
#else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (BSP_Flash_Read(part, 0, (uint8_t *)&_fpk_head, FPK_HEAD_SIZE) < 0)
#endif
    {
        BSP_Printf("%s: read error.\r\n", __func__);
        return FM_ERR_READ_FIRMWARE_HEAD_ERR;
    }

    return FM_ERR_OK;
}


/**
 * @brief  从某个分区将固件包更新至 APP 分区
 * @note   读取 -> 解密 -> 写入
 * @param[in]  from_part_name: 放置需要更新至 APP 分区的固件包的分区
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_UpdateToAPP(const char *from_part_name)
{
    int      read_len = 0;
    uint8_t  decrypt = 0;
    uint32_t read_posit = 0;
    uint32_t write_posit = 0;
    uint32_t need_read_size = FPK_LEAST_HANDLE_BYTE;
    FM_ERR_CODE result = FM_ERR_OK;
    
    ASSERT(from_part_name != NULL);

#if (ENABLE_SPI_FLASH) 
    const struct fal_partition *app_part = NULL;
    const struct fal_partition *firmware_part = NULL;
    
    app_part = fal_partition_find(APP_PART_NAME);
    if (app_part == NULL)
    {
        BSP_Printf("%s: not found APP part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    firmware_part = fal_partition_find(from_part_name);
#else
    const struct BSP_FLASH *app_part = NULL;
    const struct BSP_FLASH *firmware_part = NULL;
    
    app_part = BSP_Flash_GetHandle(APP_PART_NAME);
    if (app_part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    firmware_part = BSP_Flash_GetHandle(from_part_name);
#endif
    if (firmware_part == NULL)
    {
        BSP_Printf("%s: not found %s part.\r\n", __func__, from_part_name);
        return FM_ERR_NO_THIS_PART;
    }

    _Reset_Write();
    BSP_Printf("%s: from %s part write to APP\r\n", __func__, from_part_name);

    /* 读取加密选项 */
    decrypt = FM_IsEncrypt();

    for (write_posit = 0; write_posit < _fpk_head.pkg_size; )
    {
        if ((_fpk_head.pkg_size - read_posit) < FPK_LEAST_HANDLE_BYTE)
            need_read_size = _fpk_head.pkg_size - read_posit;

    #if (ENABLE_SPI_FLASH) 
        read_len = fal_partition_read(firmware_part, (read_posit + FPK_HEAD_SIZE), _fpk_min_handle_buff, need_read_size);
    #else
        read_len = BSP_Flash_Read(firmware_part, (read_posit + FPK_HEAD_SIZE), _fpk_min_handle_buff, need_read_size);
    #endif
        if (read_len < 0)
        {
            BSP_Printf("%s: read error (%d).\r\n", __func__, __LINE__);
            return FM_ERR_UPDATE_READ_ERR;
        }

        result = _Write_FirmwareSubPackage(app_part, _fpk_min_handle_buff, read_len, decrypt, FM_DIR_DOWNLOAD_TO_APP);
        if (result)
        {
            BSP_Printf("%s: write error (%d).\r\n", __func__, __LINE__);
            return result;
        }

        read_posit  += read_len;
        write_posit += read_len;
    }
    
    return FM_ERR_OK;
}


#if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT)
/**
 * @brief  更新固件包中的版本信息
 * @note   
 * @param[in]  part_name: 分区名称
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_UpdateFirmwareVersion(const char *part_name)
{
    ASSERT(part_name != NULL);

#if (ENABLE_SPI_FLASH && DOWNLOAD_PART_LOCATION == 1)
    #if (SPI_FLASH_ERASE_GRANULARITY > FPK_LEAST_HANDLE_BYTE)
    #error "erase granularity oversize than _fpk_min_handle_buff array"
    #endif
    /* 将 download 分区首地址的数据读出，长度为片内 flash 最小擦除粒度 */
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
    if (NULL == part)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (fal_partition_read(part, 0, &_fpk_min_handle_buff[0], SPI_FLASH_ERASE_GRANULARITY) < 0)
    {
        BSP_Printf("%s: read error.\r\n", __func__);
        return FM_ERR_UPDATE_VER_READ_ERR;
    }

    /* 修改固件包头中旧版本字段的版本信息为新的固件版本 */
    struct FPK_HEAD *p_pkg_head = (struct FPK_HEAD *)&_fpk_min_handle_buff[0];

    p_pkg_head->fw_old_ver[0] = p_pkg_head->fw_new_ver[0];
    p_pkg_head->fw_old_ver[1] = p_pkg_head->fw_new_ver[1];
    p_pkg_head->fw_old_ver[2] = p_pkg_head->fw_new_ver[2];
    p_pkg_head->fw_old_ver[3] = p_pkg_head->fw_new_ver[3];

    /* 将读出数据的区域擦除 */
    if (fal_partition_erase(part, 0, SPI_FLASH_ERASE_GRANULARITY) < 0)
    {
        BSP_Printf("%s: %s part erase failed.\r\n", __func__, part_name);
        return FM_ERR_UPDATE_VER_ERASE_ERR;
    }

    /* 将新的数据写入擦除的区域 */ 
    if (fal_partition_write(part, 0, &_fpk_min_handle_buff[0], SPI_FLASH_ERASE_GRANULARITY) < 0)
    {
        BSP_Printf("%s: write error (%d).\r\n", __func__, __LINE__);
        return FM_ERR_UPDATE_VER_WRITE_ERR;
    }
#else
    #if (ONCHIP_FLASH_ERASE_GRANULARITY > FPK_LEAST_HANDLE_BYTE)
    #error "erase granularity oversize than _fpk_min_handle_buff array"
    #endif
    /* 将 download 分区首地址的数据读出，长度为片内 flash 最小擦除粒度 */
    #if (ENABLE_SPI_FLASH)     
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(part_name);
    if (NULL == part)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (fal_partition_read(part, 0, (uint8_t *)&_fpk_min_handle_buff[0], ONCHIP_FLASH_ERASE_GRANULARITY) < 0)
    #else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    BSP_Printf("%s: part name: %s\r\n", __func__, part_name);
    
    if (BSP_Flash_Read(part, 0, &_fpk_min_handle_buff[0], ONCHIP_FLASH_ERASE_GRANULARITY) < 0)
    #endif
    {
        BSP_Printf("%s: read error.\r\n", __func__);
        return FM_ERR_UPDATE_VER_READ_ERR;
    }

    /* 修改固件包头中旧版本字段的版本信息为新的固件版本 */
    struct FPK_HEAD *p_pkg_head = (struct FPK_HEAD *)&_fpk_min_handle_buff[0];

    p_pkg_head->fw_old_ver[0] = p_pkg_head->fw_new_ver[0];
    p_pkg_head->fw_old_ver[1] = p_pkg_head->fw_new_ver[1];
    p_pkg_head->fw_old_ver[2] = p_pkg_head->fw_new_ver[2];
    p_pkg_head->fw_old_ver[3] = p_pkg_head->fw_new_ver[3];

    /* 将读出数据的区域擦除 */
    #if (ENABLE_SPI_FLASH) 
    if (fal_partition_erase(part, 0, ONCHIP_FLASH_ERASE_GRANULARITY) < 0)
    #else
    if (BSP_Flash_Erase(part, 0, ONCHIP_FLASH_ERASE_GRANULARITY) < 0)
    #endif
    {
        BSP_Printf("%s: %s part erase failed.\r\n", __func__, part_name);
        return FM_ERR_UPDATE_VER_ERASE_ERR;
    }  

    /* 将新的数据写入擦除的区域 */
    #if (ENABLE_SPI_FLASH)     
    if (fal_partition_write(part, 0, &_fpk_min_handle_buff[0], ONCHIP_FLASH_ERASE_GRANULARITY) < 0)
    #else
    if (BSP_Flash_Write(part, 0, &_fpk_min_handle_buff[0], ONCHIP_FLASH_ERASE_GRANULARITY) < 0)
    #endif
    {
        BSP_Printf("%s: write error (%d).\r\n", __func__, __LINE__);
        return FM_ERR_UPDATE_VER_WRITE_ERR;
    }
#endif

    memcpy((uint8_t *)&_fpk_head, &_fpk_min_handle_buff[0], FPK_HEAD_SIZE);
    BSP_Printf("fw old ver: V%d.%d.%d.%d\r\n", p_pkg_head->fw_old_ver[0], p_pkg_head->fw_old_ver[1], p_pkg_head->fw_old_ver[2], p_pkg_head->fw_old_ver[3]);
    BSP_Printf("fw new ver: V%d.%d.%d.%d\r\n", p_pkg_head->fw_new_ver[0], p_pkg_head->fw_new_ver[1], p_pkg_head->fw_new_ver[2], p_pkg_head->fw_new_ver[3]);

    return FM_ERR_OK;
}


#elif (USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
/**
 * @brief  更新固件包中的版本信息
 * @note   
 * @param[in]  part_name: 分区名称
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_UpdateFirmwareVersion(const char *part_name)
{
#if (ENABLE_SPI_FLASH)  
    const struct fal_partition *part = NULL;
    
    part = fal_partition_find(APP_PART_NAME);
#else
    const struct BSP_FLASH *part = NULL;
    
    part = BSP_Flash_GetHandle(APP_PART_NAME);
#endif
    if (part == NULL)
    {
        BSP_Printf("%s: not found APP part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (BSP_Flash_Read(part, (APP_PART_SIZE - FPK_VERSION_SIZE), (uint8_t *)&_fpk_head.fw_old_ver[0], FPK_VERSION_SIZE) < 0)
    {
        BSP_Printf("%s: read error.\r\n", __func__);
        return FM_ERR_READ_VER_ERR;
    }

    if (_fpk_head.fw_old_ver[0] != 0xFF
    ||  _fpk_head.fw_old_ver[1] != 0xFF
    ||  _fpk_head.fw_old_ver[2] != 0xFF
    ||  _fpk_head.fw_old_ver[3] != 0xFF)
    {
        BSP_Printf("%s: version area no erase.\r\n", __func__);
        return FM_ERR_VER_AREA_NO_ERASE;
    }

    if (BSP_Flash_Write(part, (APP_PART_SIZE - FPK_VERSION_SIZE), (uint8_t *)&_fpk_head.fw_new_ver[0], FPK_VERSION_SIZE) < 0)
    {
        BSP_Printf("%s: write error.\r\n", __func__);
        return FM_ERR_WRITE_VER_ERR;
    }

    return FM_ERR_OK;
}

#endif /* #if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT) */

#endif /* #if (USING_PART_PROJECT > ONE_PART_PROJECT) */


/* Private functions ---------------------------------------------------------*/
/**
 * @brief  复位写固件的一些记录信息
 * @note   
 * @retval None
 */
static void _Reset_Write(void)
{
    _fw_start_write_flag = 0;    /* 固件开始写入的标志位 */
    _update_progress     = 0;    /* 固件更新的进度， 10000 制 */
    _storage_data_size   = 0;    /* 固件包写入时记录暂存的固件分包大小，单位 byte */
    _write_part_addr     = 0;    /* 固件包写入时记录写入 flash 的相对地址 */
    _write_last_pkg_size = 0;    /* 固件包写入时最后一个分包的大小，单位 byte */
}


/**
 * @brief  将固件分包按顺序写入某个分区
 * @note   循环调用本函数，无须指定写入地址，函数内部自行记录已写入的大小
 * @param[in]  part: 分区对象
 * @param[in]  data: 数据
 * @param[in]  pkg_size: 数据大小，单位 byte
 * @param[in]  decrypt: 0: 固件包无加密。1: 固件包有加密
 * @retval FM_ERR_CODE
 */
#if (ENABLE_SPI_FLASH)
static FM_ERR_CODE  _Write_FirmwareSubPackage( const struct fal_partition *part, 
                                               uint8_t  *data, 
                                               uint16_t pkg_size, 
                                               uint8_t  decrypt,
                                               FM_FIRMWARE_WRITE_DIR  write_dir)
#else
static FM_ERR_CODE  _Write_FirmwareSubPackage( const struct BSP_FLASH *part, 
                                               uint8_t  *data, 
                                               uint16_t pkg_size, 
                                               uint8_t  decrypt,
                                               FM_FIRMWARE_WRITE_DIR  write_dir)
#endif
{
    uint8_t *fw_4096byte_buff = data;

    /* 主机直接下发的固件分包不满 FPK_LEAST_HANDLE_BYTE 个字节，需要先暂存至满足后再写入 */
    if (write_dir == FM_DIR_HOST_TO_APP)
    {
        fw_4096byte_buff = &_fpk_min_handle_buff[0];

        /* 判断是否是最后一个包 */
        if (_storage_data_size == 0)
        {
            if (_fpk_head.pkg_size - _write_part_addr < FPK_LEAST_HANDLE_BYTE)
            {
                /* 最后一个固件分包的标志 */
                _write_last_pkg_size = _fpk_head.pkg_size - _write_part_addr;
            }
        }
        
        /* 小于最小处理单位时，暂存 */
        if (_storage_data_size < FPK_LEAST_HANDLE_BYTE)
        {
            memcpy(&fw_4096byte_buff[_storage_data_size], data, pkg_size);
            _storage_data_size += pkg_size;
            
            if (_write_last_pkg_size)
            {
                if (_storage_data_size < _write_last_pkg_size)
                    return FM_ERR_OK;
            }   
            else if (_storage_data_size < FPK_LEAST_HANDLE_BYTE)
                return FM_ERR_OK;
        }
    }
    /* 从 download 或 factory 更新至 APP ，因可以直接读取 FPK_LEAST_HANDLE_BYTE 个字节，所以无须暂存 */
    else
        _storage_data_size = pkg_size;

#if (ENABLE_DECRYPT)
    if (decrypt)
        AES_CBC_decrypt_buffer(&_aes_ctx, &fw_4096byte_buff[0], _storage_data_size);
#endif

    /* 保存首地址的几个字节数据，等待最后写入 */
    if (_fw_start_write_flag == 0)
    {   
        for (uint8_t i = 0; i < ONCHIP_FLASH_ONCE_WRITE_BYTE; i++)
            _fw_first_bytes[i]  = fw_4096byte_buff[i];

        fw_4096byte_buff   += ONCHIP_FLASH_ONCE_WRITE_BYTE;
        _storage_data_size -= ONCHIP_FLASH_ONCE_WRITE_BYTE;
        _write_part_addr    = ONCHIP_FLASH_ONCE_WRITE_BYTE;
    }

#if (ENABLE_SPI_FLASH)     
    if (fal_partition_write(part, _write_part_addr, fw_4096byte_buff, _storage_data_size) < 0)
#else
    if (BSP_Flash_Write(part, _write_part_addr, fw_4096byte_buff, _storage_data_size) < 0)
#endif
    {
        BSP_Printf("%s: write error (%d).\r\n", __func__, __LINE__);
        _Reset_Write();
        return FM_ERR_WRITE_PART_ERR;
    }

    _write_part_addr    += _storage_data_size;
    _storage_data_size   = 0;
    _fw_start_write_flag = 1;

    _update_progress += _update_progress_step_num;
    Firmware_OperateCallback(_update_progress);
    
    return FM_ERR_OK;
}



