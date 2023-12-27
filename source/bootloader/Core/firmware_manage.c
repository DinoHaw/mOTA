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
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2022-11-23     Dino         the first version
 * v1.1     2022-12-07     Dino         修复 STM32L4 写入 flash 的最小单位问题
 * v1.2     2022-12-10     Dino         增加对 SPI flash 的支持
 * v1.3     2023-05-04     Dino         修复 AES 库已被其它函数使用而导致固件包解密错误的问题
 * v1.4     2023-12-10     Dino         1. 合并 utils.c 到 firmware_manage.c 中
 *                                      2. 将固件头数据的值和掩码修改为可配置宏
 *                                      3. 修复写固件版本时没有进行 flash 写对齐的问题
 */


/* Includes ------------------------------------------------------------------*/
#include "firmware_manage.h"

#if (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH &&    \
     SPI_FLASH_ERASE_GRANULARITY > FPK_LEAST_HANDLE_BYTE)
#error "SPI flash erase granularity oversize than _fpk_min_handle_buff array"
#endif

#if (DOWNLOAD_PART_LOCATION == STORE_IN_ONCHIP_FLASH &&    \
     ONCHIP_FLASH_ERASE_GRANULARITY > FPK_LEAST_HANDLE_BYTE)
#error "onchip flash erase granularity oversize than _fpk_min_handle_buff array"
#endif

#if (IS_ENABLE_SPI_FLASH)
    #define FLASH_OBJECT        fal_partition
    #define GET_FLASH_OBJECT    fal_partition_find
    #define FLASH_PART_READ     fal_partition_read
    #define FLASH_PART_WRITE    fal_partition_write
    #define FLASH_PART_ERASE    fal_partition_erase
#else
    #define FLASH_OBJECT        BSP_FLASH
    #define GET_FLASH_OBJECT    BSP_Flash_GetHandle
    #define FLASH_PART_READ     BSP_Flash_Read
    #define FLASH_PART_WRITE    BSP_Flash_Write
    #define FLASH_PART_ERASE    BSP_Flash_Erase
#endif


/* Private variables ---------------------------------------------------------*/
static bool     _is_start_write;                                /* 固件开始写入的标志位 */
static uint16_t _update_progress;                               /* 固件更新的进度， 10000 制 */
static uint16_t _update_progress_step_num;                      /* 固件更新进度的步进单位 */
static uint16_t _storage_data_size;                             /* 固件包写入时记录暂存的固件分包大小，单位 byte */
static uint32_t _write_part_addr;                               /* 固件包写入时记录写入 flash 的相对地址 */
static uint16_t _write_last_pkg_size;                           /* 固件包写入时最后一个分包的大小，单位 byte */
static uint32_t _crc_tab[256];                                  /* CRC 计算表 */
static uint8_t  _fw_first_bytes[ONCHIP_FLASH_ONCE_WRITE_BYTE];  /* 固件包的前几个字节 */
static uint8_t  _fpk_min_handle_buff[FPK_LEAST_HANDLE_BYTE];    /* fpk 固件最小处理单位的缓存区，多次使用以降低系统资源开销 */
static struct FPK_HEAD  _fpk_head;                              /* 用于存放 fpk 固件包头 */
#if (ENABLE_DECRYPT)
static struct AES_ctx  _aes_ctx;                                /* AES 对象 */
#endif
#if (IS_ENABLE_SPI_FLASH == 0)
static struct BSP_FLASH _flash_app_part;                        /* APP 分区 */
    #if (USING_PART_PROJECT > ONE_PART_PROJECT)
    static struct BSP_FLASH _flash_download_part;               /* download 分区 */
        #if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
        static struct BSP_FLASH _flash_factory_part;            /* factory 分区 */
        #endif
    #endif
#endif
#if (USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP)
/* APP 版本的记录地址，确保该地址在 flash 上是写对齐的 */
static const uint32_t _app_ver_addr = APP_PART_SIZE - ONCHIP_FLASH_ONCE_WRITE_BYTE; 
#endif


/* Extern function prototypes ------------------------------------------------*/
extern void Firmware_OperateCallback(uint16_t progress);


/* Private function prototypes -----------------------------------------------*/
static void         _CRC32_Init                 (uint32_t poly);
static uint32_t     _CRC32_Calc                 (uint8_t *buf, uint32_t len);
static uint32_t     _CRC32_StepCalc             (uint32_t crc_init, uint8_t *buf, uint32_t len);
static FM_ERR_CODE  _Write_FirmwareSubPackage   (const struct FLASH_OBJECT *part, 
                                                 uint8_t  *data, 
                                                 uint16_t pkg_size, 
                                                 bool     is_decrypt,
                                                 FM_FIRMWARE_WRITE_DIR  write_dir);
static void         _Reset_Write                (void);


/* Exported functions ---------------------------------------------------------*/
/**
 * @brief  初始化接口
 * @note   
 * @retval None
 */
void FM_Init(void)
{
    _CRC32_Init(CRC32_POLYNOMIAL);
    
#if (IS_ENABLE_SPI_FLASH)
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

    _Reset_Write();
}


/**
 * @brief  固件包是否有加密
 * @note   调用前需确保 _fpk_head 已经读入了数据
 * @retval false: 无加密 | true: 加密
 */
inline bool FM_IsEncrypt(void)
{
    /* 读取加密选项 */
    if (_fpk_head.config[1] == 0x01)
        return true;
    return false;
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
    uint16_t need_read_size = FPK_LEAST_HANDLE_BYTE;
    uint32_t *p_data = (uint32_t *)_fpk_min_handle_buff;
    uint32_t read_posit = 0;
    
    ASSERT(part_name != NULL);
        
    const struct FLASH_OBJECT *part = NULL;
    
    part = GET_FLASH_OBJECT(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: %s not found.\r\n", __func__, part_name);
        return FM_ERR_NO_THIS_PART;
    }
    
    for (read_posit = 0; read_posit < part->len; )
    {
        if ((part->len - read_posit) < FPK_LEAST_HANDLE_BYTE)
            need_read_size = part->len - read_posit;

        read_len = FLASH_PART_READ(part, read_posit, _fpk_min_handle_buff, need_read_size);
        if (read_len < 0)
        {
            BSP_Printf("%s: read error (%d).\r\n", __func__, __LINE__);
            return FM_ERR_READ_IS_EMPTY_ERR;
        }
        
        for (uint16_t i = 0; i < (FPK_LEAST_HANDLE_BYTE / sizeof(p_data)); i++)
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
    const struct FLASH_OBJECT *part = NULL;

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

    part = GET_FLASH_OBJECT(part_name);
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
    head_crc = _CRC32_Calc(p_fpk_head, FPK_HEAD_SIZE - 4);
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
FM_ERR_CODE  FM_VerifyFirmware(const char *part_name, uint32_t crc32, bool is_auto_fill)
{
    int      read_len = 0;
    bool     is_app_part = false;
    uint32_t pkg_size = 0;
    uint32_t body_crc = 0xFFFFFFFF;
    uint32_t read_posit = 0;
    uint32_t read_posit_temp = 0;
    uint16_t need_read_size = FPK_LEAST_HANDLE_BYTE;
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
    bool is_first = false;
#endif
    const struct FLASH_OBJECT *part = NULL;
    
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
        is_app_part = true;
     
    part = GET_FLASH_OBJECT(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }

    BSP_Printf("fpk size: %d byte\r\n", FPK_HEAD_SIZE);
    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t *p = (uint8_t *)&_fpk_head;
        for (uint8_t j = 0; j < FPK_HEAD_SIZE / 6; j++)
            BSP_Printf("%.2X ", p[(i * (FPK_HEAD_SIZE / 6)) + j]);
        BSP_Printf("\r\n");
    }
    BSP_Printf("%s: part name %s\r\n", __func__, part_name);
    
    if (is_app_part)
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
        if (is_app_part)
            read_posit_temp = read_posit;
        else
            read_posit_temp = read_posit + FPK_HEAD_SIZE;

        read_len = FLASH_PART_READ(part, read_posit_temp, &_fpk_min_handle_buff[0], need_read_size);
        if (read_len < 0)
        {
            BSP_Printf("%s: read error (%d).\r\n", __func__, __LINE__);
            return FM_ERR_VERIFY_READ_ERR;
        }

    #if (USING_PART_PROJECT > ONE_PART_PROJECT)
        if (is_auto_fill)
        {
            if (is_first == false)
            {
                is_first = true;
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

        body_crc = _CRC32_StepCalc(body_crc, &_fpk_min_handle_buff[0], read_len);
        read_posit += read_len;
    }
    body_crc ^= 0xFFFFFFFF;
    
    if (body_crc != crc32)
    {
        BSP_Printf("%s: body crc verify failed. (%.8X - %.8X)\r\n", __func__, crc32, body_crc);
        if (is_app_part)
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

    const struct FLASH_OBJECT *part = NULL;
    
    part = GET_FLASH_OBJECT(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found %s part.\r\n", __func__, part_name);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (FLASH_PART_ERASE(part, 0, part->len) < 0)
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
    
    const struct FLASH_OBJECT *part = NULL;
    
    part = GET_FLASH_OBJECT(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }

    if (FLASH_PART_WRITE(part, 0, _fw_first_bytes, ONCHIP_FLASH_ONCE_WRITE_BYTE) < 0)
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

    const struct FLASH_OBJECT *part = NULL;
    
    part = GET_FLASH_OBJECT(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found %s part.\r\n", __func__, part_name);
        return FM_ERR_NO_THIS_PART;
    }
    BSP_Printf("%s: %s part\r\n", __func__, part_name);
    
#if (USING_PART_PROJECT == ONE_PART_PROJECT)
    bool is_decrypt = false;

    /* 读取加密选项 */
    is_decrypt = FM_IsEncrypt();

    return _Write_FirmwareSubPackage(part, data, pkg_size, is_decrypt, FM_DIR_HOST_TO_APP); /* 写入前解密 */
#else
    return _Write_FirmwareSubPackage(part, data, pkg_size, false, FM_DIR_HOST_TO_DOWNLOAD);  /* 写入前不解密 */
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
#if (IS_ENABLE_SPI_FLASH)
    uint32_t *data = NULL;
    const struct FLASH_OBJECT *part = NULL;
#endif
    
    BSP_Printf("0x%.8X address data: 0x%.8X\r\n", addr, value);
    
    if (APP_ADDRESS == addr)
    {
        fw_integrity = (value & FIRMWARE_HEAD_DATA_MASK) == FIRMWARE_HEAD_DATA? FM_ERR_OK : FM_ERR_JUMP_TO_APP_ERR;
    }
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
    #if (DOWNLOAD_PART_LOCATION == STORE_IN_ONCHIP_FLASH && FACTORY_PART_LOCATION == STORE_IN_ONCHIP_FLASH)
    /* download 分区和 factory 分区均在片内 flash */
    else if ((DOWNLOAD_ADDRESS == addr)
    ||       (FACTORY_ADDRESS  == addr))
    {
        if (value == FPK_IDENTIFIER)
            fw_integrity = FM_ERR_OK;
    }
    #elif (DOWNLOAD_PART_LOCATION == STORE_IN_ONCHIP_FLASH && FACTORY_PART_LOCATION == STORE_IN_SPI_FLASH)
    /* download 分区在片内 flash ， factory 分区在 SPI flash */
    else if (DOWNLOAD_ADDRESS == addr)
    {
        if (value == FPK_IDENTIFIER)
            fw_integrity = FM_ERR_OK;
    }
    else if (FACTORY_ADDRESS == addr)
    {
        part = GET_FLASH_OBJECT(FACTORY_PART_NAME);
        if (part == NULL)
        {
            BSP_Printf("%s: not found factory part.\r\n", __func__);
            return FM_ERR_NO_THIS_PART;
        }
        
        if (FLASH_PART_READ(part, 0, (uint8_t *)&_fpk_min_handle_buff[0], 4) < 0)
        {
            BSP_Printf("%s: read error.\r\n", __func__);
            return FM_ERR_READ_FLASH_ERR;
        }

        data = (uint32_t *)_fpk_min_handle_buff;

        if (*data == FPK_IDENTIFIER)
            fw_integrity = FM_ERR_OK;
    }
    #elif (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH && FACTORY_PART_LOCATION == STORE_IN_SPI_FLASH)
    /* download 分区和 factory 分区均在片内 SPI flash */
    else if (DOWNLOAD_ADDRESS == addr)
    {
        part = GET_FLASH_OBJECT(DOWNLOAD_PART_NAME);
        if (part == NULL)
        {
            BSP_Printf("%s: not found download part.\r\n", __func__);
            return FM_ERR_NO_THIS_PART;
        }
        
        if (FLASH_PART_READ(part, 0, (uint8_t *)&_fpk_min_handle_buff[0], 4) < 0)
        {
            BSP_Printf("%s: read error.\r\n", __func__);
            return FM_ERR_READ_FLASH_ERR;
        }

        data = (uint32_t *)_fpk_min_handle_buff;

        if (*data == FPK_IDENTIFIER)
            fw_integrity = FM_ERR_OK;
    }
    else if (FACTORY_ADDRESS == addr)
    {
        part = GET_FLASH_OBJECT(FACTORY_PART_NAME);
        if (part == NULL)
        {
            BSP_Printf("%s: not found factory part.\r\n", __func__);
            return FM_ERR_NO_THIS_PART;
        }
        
        if (FLASH_PART_READ(part, 0, (uint8_t *)&_fpk_min_handle_buff[0], 4) < 0)
        {
            BSP_Printf("%s: read error.\r\n", __func__);
            return FM_ERR_READ_FLASH_ERR;
        }

        data = (uint32_t *)_fpk_min_handle_buff;

        if (*data == FPK_IDENTIFIER)
            fw_integrity = FM_ERR_OK;
    }
    #else
    /* download 分区在 SPI flash ， factory 分区在片内 flash */
    else if (DOWNLOAD_ADDRESS == addr)
    {
        part = GET_FLASH_OBJECT(DOWNLOAD_PART_NAME);
        if (part == NULL)
        {
            BSP_Printf("%s: not found download part.\r\n", __func__);
            return FM_ERR_NO_THIS_PART;
        }
        
        if (FLASH_PART_READ(part, 0, (uint8_t *)&_fpk_min_handle_buff[0], 4) < 0)
        {
            BSP_Printf("%s: read error.\r\n", __func__);
            return FM_ERR_READ_FLASH_ERR;
        }

        data = (uint32_t *)_fpk_min_handle_buff;

        if (*data == FPK_IDENTIFIER)
            fw_integrity = FM_ERR_OK;
    }
    else if (FACTORY_ADDRESS == addr)
    {
        if (value == FPK_IDENTIFIER)
            fw_integrity = FM_ERR_OK;
    }
    #endif
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

    const struct FLASH_OBJECT *part = NULL;
    
    part = GET_FLASH_OBJECT(APP_PART_NAME);
    if (part == NULL)
    {
        BSP_Printf("%s: not found app part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (FLASH_PART_READ(part, _app_ver_addr, (uint8_t *)&_fpk_head.fw_old_ver[0], FPK_VERSION_SIZE) < 0)
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
 * @retval 打包后固件的 CRC32 值
 */
inline uint32_t FM_GetPackageCRC32(void)
{
    return _fpk_head.pkg_crc;
}


/**
 * @brief  将分区内的固件包头读出
 * @note   读出后的数据将放在 _fpk_head 中
 * @param[in]  part_name: 分区名称
 * @retval FM_ERR_CODE
 */
FM_ERR_CODE  FM_ReadFirmwareHead(const char *part_name)
{
    ASSERT(part_name != NULL);

    const struct FLASH_OBJECT *part = NULL;

    _Reset_Write();

    part = GET_FLASH_OBJECT(part_name);
    if (part == NULL)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (FLASH_PART_READ(part, 0, (uint8_t *)&_fpk_head, FPK_HEAD_SIZE) < 0)
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
    ASSERT(from_part_name != NULL);

    int      read_len = 0;
    bool     is_decrypt = false;
    uint32_t read_posit = 0;
    uint32_t write_posit = 0;
    uint32_t need_read_size = FPK_LEAST_HANDLE_BYTE;
    FM_ERR_CODE result = FM_ERR_OK;
    const struct FLASH_OBJECT *app_part = NULL;
    const struct FLASH_OBJECT *firmware_part = NULL;
    
    app_part = GET_FLASH_OBJECT(APP_PART_NAME);
    if (app_part == NULL)
    {
        BSP_Printf("%s: not found APP part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    firmware_part = GET_FLASH_OBJECT(from_part_name);
    if (firmware_part == NULL)
    {
        BSP_Printf("%s: not found %s part.\r\n", __func__, from_part_name);
        return FM_ERR_NO_THIS_PART;
    }

    _Reset_Write();
    BSP_Printf("%s: from %s part write to APP\r\n", __func__, from_part_name);

    /* 读取加密选项 */
    is_decrypt = FM_IsEncrypt();

#if (ENABLE_DECRYPT)
    /* 当有固件包需要刷入 APP 分区时，每次都需要对 AES 进行初始化，存在 AES 库已被其它函数使用的情况 */
    if (is_decrypt)
        AES_init_ctx_iv(&_aes_ctx, (uint8_t *)AES256_KEY, (uint8_t *)AES256_IV);
#endif

    for (write_posit = 0; write_posit < _fpk_head.pkg_size; )
    {
        if ((_fpk_head.pkg_size - read_posit) < FPK_LEAST_HANDLE_BYTE)
            need_read_size = _fpk_head.pkg_size - read_posit;

        read_len = FLASH_PART_READ(firmware_part, (read_posit + FPK_HEAD_SIZE), _fpk_min_handle_buff, need_read_size);
        if (read_len < 0)
        {
            BSP_Printf("%s: read error (%d).\r\n", __func__, __LINE__);
            return FM_ERR_UPDATE_READ_ERR;
        }

        result = _Write_FirmwareSubPackage(app_part, _fpk_min_handle_buff, read_len, is_decrypt, FM_DIR_DOWNLOAD_TO_APP);
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

#if (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH)
    const uint32_t earse_unit = SPI_FLASH_ERASE_GRANULARITY;
#else
    const uint32_t earse_unit = ONCHIP_FLASH_ERASE_GRANULARITY;
#endif

    /* 将 download 分区首地址的数据读出，长度为片内 flash 最小擦除粒度 */
    const struct FLASH_OBJECT *part = NULL;
    
    part = GET_FLASH_OBJECT(part_name);
    if (NULL == part)
    {
        BSP_Printf("%s: not found.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (FLASH_PART_READ(part, 0, &_fpk_min_handle_buff[0], earse_unit) < 0)
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
    if (FLASH_PART_ERASE(part, 0, earse_unit) < 0)
    {
        BSP_Printf("%s: %s part erase failed.\r\n", __func__, part_name);
        return FM_ERR_UPDATE_VER_ERASE_ERR;
    }

    /* 将新的数据写入擦除的区域 */ 
    if (FLASH_PART_WRITE(part, 0, &_fpk_min_handle_buff[0], earse_unit) < 0)
    {
        BSP_Printf("%s: write error (%d).\r\n", __func__, __LINE__);
        return FM_ERR_UPDATE_VER_WRITE_ERR;
    }

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
    const struct FLASH_OBJECT *part = NULL;

    part = GET_FLASH_OBJECT(APP_PART_NAME);
    if (part == NULL)
    {
        BSP_Printf("%s: not found APP part.\r\n", __func__);
        return FM_ERR_NO_THIS_PART;
    }
    
    if (FLASH_PART_READ(part, _app_ver_addr, (uint8_t *)&_fpk_head.fw_old_ver[0], FPK_VERSION_SIZE) < 0)
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

#if (ONCHIP_FLASH_ONCE_WRITE_BYTE > FPK_VERSION_SIZE)
    uint8_t version[ONCHIP_FLASH_ONCE_WRITE_BYTE] = {0};
    memcpy((uint8_t *)&version[0], (uint8_t *)&_fpk_head.fw_new_ver[0], FPK_VERSION_SIZE);
    
    if (FLASH_PART_WRITE(part, _app_ver_addr, (uint8_t *)&version[0], sizeof(version)) < 0)
    {
        BSP_Printf("%s: write error.\r\n", __func__);
        return FM_ERR_WRITE_VER_ERR;
    }
#else
    if (FLASH_PART_WRITE(part, _app_ver_addr, (uint8_t *)&_fpk_head.fw_new_ver[0], FPK_VERSION_SIZE) < 0)
    {
        BSP_Printf("%s: write error.\r\n", __func__);
        return FM_ERR_WRITE_VER_ERR;
    }
#endif

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
    _is_start_write      = false;   /* 固件开始写入的标志位 */
    _update_progress     = 0;       /* 固件更新的进度， 10000 制 */
    _storage_data_size   = 0;       /* 固件包写入时记录暂存的固件分包大小，单位 byte */
    _write_part_addr     = 0;       /* 固件包写入时记录写入 flash 的相对地址 */
    _write_last_pkg_size = 0;       /* 固件包写入时最后一个分包的大小，单位 byte */
}


/**
 * @brief  将固件分包按顺序写入某个分区
 * @note   循环调用本函数，无须指定写入地址，函数内部自行记录已写入的大小
 * @param[in]  part: 分区对象
 * @param[in]  data: 数据
 * @param[in]  pkg_size: 数据大小，单位 byte
 * @param[in]  is_decrypt: false: 不解密直接写入 | true: 解密后写入
 * @retval FM_ERR_CODE
 */
static FM_ERR_CODE  _Write_FirmwareSubPackage( const struct FLASH_OBJECT *part, 
                                               uint8_t  *data, 
                                               uint16_t pkg_size, 
                                               bool     is_decrypt,
                                               FM_FIRMWARE_WRITE_DIR  write_dir)
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
    if (is_decrypt)
        AES_CBC_decrypt_buffer(&_aes_ctx, &fw_4096byte_buff[0], _storage_data_size);
#endif

    /* 保存首地址的几个字节数据，等待最后写入 */
    if (_is_start_write == false)
    {   
        for (uint8_t i = 0; i < ONCHIP_FLASH_ONCE_WRITE_BYTE; i++)
            _fw_first_bytes[i]  = fw_4096byte_buff[i];

        fw_4096byte_buff   += ONCHIP_FLASH_ONCE_WRITE_BYTE;
        _storage_data_size -= ONCHIP_FLASH_ONCE_WRITE_BYTE;
        _write_part_addr    = ONCHIP_FLASH_ONCE_WRITE_BYTE;
    }
     
    if (FLASH_PART_WRITE(part, _write_part_addr, fw_4096byte_buff, _storage_data_size) < 0)
    {
        BSP_Printf("%s: write error (%d).\r\n", __func__, __LINE__);
        _Reset_Write();
        return FM_ERR_WRITE_PART_ERR;
    }

    _write_part_addr  += _storage_data_size;
    _storage_data_size = 0;
    _is_start_write    = true;

    _update_progress += _update_progress_step_num;
    Firmware_OperateCallback(_update_progress);
    
    return FM_ERR_OK;
}


/**
 * @brief  CRC32 计算表初始化
 * @note   
 * @param[in]  poly: 多项式
 * @retval None
 */
static void _CRC32_Init(uint32_t poly)
{
    uint32_t i, j, c;
    uint32_t var = 0;

    for (i = 0; i < 32; i++)
    {
        if (poly & 0x00000001)
            var |= 1 << (32 - 1 - i);

        poly >>= 1;
    }

    for (i = 0; i < 256; i++)
    {
        c = i;
        for (j = 0; j < 8; j++)
        {
            if (c & 0x00000001)
                c = var ^ (c >> 1);
            else
                c >>= 1;
        }
        _crc_tab[i] = c;
    }
}


/**
 * @brief  CRC32 计算
 * @note   
 * @param[in]  buf: 数据源
 * @param[in]  len: 数据大小 byte
 * @retval CRC32 校验值
 */
static uint32_t _CRC32_Calc(uint8_t *buf, uint32_t len)
{
    uint32_t crc_init = 0xFFFFFFFF;

    return (_CRC32_StepCalc(crc_init, buf, len) ^ 0xFFFFFFFF);
}


/**
 * @brief  CRC 逐步计算
 * @note   
 * @param[in]  crc_init: CRC 初始值
 * @param[in]  buf: 数据源
 * @param[in]  len: 数据大小 byte
 * @retval CRC32 校验值
 */
static uint32_t _CRC32_StepCalc(uint32_t crc_init, uint8_t *buf, uint32_t len)
{
    uint8_t index;

    for (uint32_t i = 0; i < len; i++)
    {
        index = (uint8_t)(crc_init ^ buf[i]);
        crc_init = (crc_init >> 8) ^ _crc_tab[index];
    }

    return crc_init;
}

