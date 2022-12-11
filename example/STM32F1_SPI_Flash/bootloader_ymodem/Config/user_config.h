/**
 * \file            user_config.h
 * \brief           user configuration of the application
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
 * Version:         v1.0.3
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-23     Dino         the first version
 * 2022-12-04     Dino         1. 增加一个记录版本的机制，可选写在 APP 分区
 *                             2. 增加长按按键恢复出厂固件的选项
 *                             3. 将 flash 的擦除粒度配置移至 user_config.h 
 *                             4. 增加是否判断固件包超过分区大小的选项
 * 2022-12-07     Dino         增加 ONCHIP_FLASH_ONCE_WRITE_BYTE 配置项
 * 2022-12-08     Dino         1. 增加固件包可放置在 SPI flash 的功能
 *                             2. 增加 ENABLE_FACTORY_UPDATE_TO_APP 配置项
 */

/**
 * OTA组件的开放配置选项
 */
#include "app_config.h"

/**
 * 【选择分区方案】
 * 解释: 
 *    APP:      可运行的固件区域
 *    download: 用于更新固件时的固件临时存放区域
 *    factory:  用于存放可在紧急情况下恢复固件使用的区域
 *    ONE_PART_PROJECT:     单分区方案（ APP ）
 *    DOUBLE_PART_PROJECT:  双分区方案（ APP + download ）
 *    TRIPLE_PART_PROJECT:  三分区方案（ APP + download + factory ）
 * 选项: 
 *    ONE_PART_PROJECT      或 0
 *    DOUBLE_PART_PROJECT   或 1
 *    TRIPLE_PART_PROJECT   或 2
 */
#define USING_PART_PROJECT                  TRIPLE_PART_PROJECT


/**
 * 【flash相关配置项】
 * 说明: 
 *    配置各个分区的大小
 * 注意事项: 
 *    ！！！片内 Flash 需进行页对齐， 分区首地址必须是 Flash 的 每个独立 page 或 sector 的首地址，否则固件无法运行！！！
 *    ！！！放置在 SPI Flash 的分区至少需要最小擦写粒度的整数倍为单位进行对齐，建议以 sector 的整数倍为单位进行对齐！！！
 */
#define ONCHIP_FLASH_SIZE                   ((uint32_t)(512 * 1024))    /* 片上 flash 容量，单位: byte */
#define BOOTLOADER_SIZE                     ((uint32_t)(32 * 1024))     /* 预留给 bootloader 的空间，单位: byte（最小需要大于本工程编译后的大小） */
#define APP_PART_SIZE                       ((uint32_t)(32 * 1024))     /* 预留给 APP 分区的空间，单位: byte（注意页对齐） */
#define DOWNLOAD_PART_SIZE                  (APP_PART_SIZE)             /* 预留给 download 分区的空间，单位: byte（注意页对齐，不使用时，写0） */
#define FACTORY_PART_SIZE                   (APP_PART_SIZE)             /* 预留给 factory 分区的空间，单位: byte（注意页对齐，不使用时，写0） */


/**
 * 【选择是否启用解密组件】
 * 说明: 
 *    若固件包有加密，则必须启用。若固件包无加密，可按需选择是否启用
 *    * AES256_KEY 必须等于 32 字节， AES256_IV 必须等于 16 字节
 * 选项: 
 *    0: 不启用
 *    1: 启用
 */
#define ENABLE_DECRYPT                      1
    #if (ENABLE_DECRYPT)
    #define AES256_KEY                      "0123456789ABCDEF0123456789ABCDEF"  /* 必须等于 32 字节 */
    #define AES256_IV                       "0123456789ABCDEF"                  /* 必须等于 16 字节 */
    #endif


/**
 * 【片内 flash 一次写入的最小字节数】
 * 说明：
 *    每个 MCU 内部 flash 单次可以写入的字节数有所不同，因此提供本选项用于配置，单位是 byte
 *    该值的大小由内部 flash 的操作接口决定，默认是 4 byte ，不能随意配置，需要清楚本选项意味着什么
 */
#define ONCHIP_FLASH_ONCE_WRITE_BYTE        4


/**
 * 【选择 bootloader 检测是否需要固件更新的方案】
 * 说明：
 *    因设备上电时受限运行 bootloader ，因此 bootloader 需要有一个获取是否需要进行固件更新的方法
 * 解释：
 *    USING_HOST_CMD_UPDATE:     通过主机指令控制进入固件更新模式，此方式会等待一段时间，以确认是否需要进行固件更新，
 *                               等待时间通过 WAIT_HOST_DATA_MAX_TIME 进行配置
 *    USING_APP_SET_FLAG_UPDATE: 通过在 APP 中设置标志位， bootloader 启动时会通过读取该标志位以判断是要进行固件更新
 *                               亦或是无须固件更新
 * 选项：
 *    USING_HOST_CMD_UPDATE      或 0
 *    USING_APP_SET_FLAG_UPDATE  或 1
 */
#define USING_IS_NEED_UPDATE_PROJECT        USING_APP_SET_FLAG_UPDATE
    #if (USING_IS_NEED_UPDATE_PROJECT == USING_HOST_CMD_UPDATE)
    #define WAIT_HOST_DATA_MAX_TIME         (3 * 1000)      /* 设置等待主机数据的最大等待时间，单位 ms */
    #else
    #define WAIT_HOST_DATA_MAX_TIME         (60 * 1000)     /* 设置等待主机数据的最大等待时间，单位 ms
                                                             * 超过这个时间没有收到主机的指令，意味着主机可能掉线，
                                                             * 此时 bootloader 会尝试跳转至 APP
                                                             * 若设为 0 ，则表示就算主机掉线，也不尝试跳转至 APP
                                                             */
    #define FIRMWARE_UPDATE_VAR_ADDR        0x20000000      /* 固件更新标志位的存放地址（不建议修改，一定要和 APP 一致） */
    #define FIRMWARE_UPDATE_MAGIC_WORD      0xA5A5A5A5      /* 固件需要更新的特殊标记（不建议修改，一定要和 APP 一致） */
    #define FIRMWARE_RECOVERY_MAGIC_WORD    0x5A5A5A5A      /* 需要恢复出厂固件的特殊标记（不建议修改，一定要和 APP 一致） */
    #define BOOTLOADER_RESET_MAGIC_WORD     0xAAAAAAAA      /* bootloader 复位的特殊标记（不建议修改，一定要和 APP 一致） */
    #endif


/**
 * 【选择是否使用按键恢复出厂固件的选项】
 * 说明: 
 *    1. 使用本选项的前提是三分区方案，本选项能起效的前提是正确配置了按键且 factory 分区有可用的固件
 *    2. 选择使用按键恢复出厂固件时，需要配置按键的引脚，如使用的按键和本案例不同，则需要自己配置和初始化 GPIO
 *    3. 本选项仅设备在运行 bootloader 时，可通过按键恢复出厂固件，若设备运行着 APP ，则本选项是无法起效的，因此需要 APP 也同步配置
 *    4. 本选项和通过指令恢复出厂固件的方式不冲突，可以同时使用，也可以不启用本选项
 *    5. 当无 factory 或 factory 无可用固件时，强行恢复出厂固件，将会触发 FACTORY_NO_FIRMWARE_SOLUTION 选项
 * 解释：
 *    ENABLE_FACTORY_FIRMWARE_BUTTON: 选择是否启用长按按键恢复出厂固件
 *    FACTORY_FIRMWARE_BUTTON_PRESS:  按键按下时的电平逻辑
 *    FACTORY_FIRMWARE_BUTTON_TIME:   按键长按的持续时间，单位 ms
 * 选项：
 *    ENABLE_FACTORY_FIRMWARE_BUTTON 选项: 
 *        0: 不启用
 *        1: 启用
 *    FACTORY_FIRMWARE_BUTTON_PRESS 选项: 
 *        KEY_PRESS_LOW:  表示按下时为低电平
 *        KEY_PRESS_HIGH: 表示按下时为高电平
 *    FACTORY_FIRMWARE_BUTTON_TIME 选项: 
 *        按键长按的持续时间，单位 ms ，不能大于 65535
 */ 
#if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
#define ENABLE_FACTORY_FIRMWARE_BUTTON      0
#define FACTORY_FIRMWARE_BUTTON_PRESS       KEY_PRESS_LOW
#define FACTORY_FIRMWARE_BUTTON_TIME        3000
#endif


/**
 * 【选择通过 bootloader 将固件包下载至 factory 分区时是否更新至 APP 的选项】
 * 说明: 
 *    通过 bootloader 将固件包下载至 factory 分区时是否自动更新至 APP ，若不启用，则 bootloader 会在固件包下载至 factory 
 *    分区并校验成功后尝试跳转至 APP 分区，若 APP 分区无可用的固件或校验失败，将会根据 FACTORY_NO_FIRMWARE_SOLUTION 选项
 *    执行对应的方案
 * 选项: 
 *    0: 不启用
 *    1: 启用
 */ 
#define ENABLE_FACTORY_UPDATE_TO_APP        0


/**
 * 【选择判断固件包是否超过分区大小的选项】
 * 说明: 
 *    当启用判断时，若有特殊处理，需自行修改代码，位于 app.c 的 APP_Running 函数 的 EXE_FLOW_VERIFY_FIRMWARE_HEAD 流程
 * 选项: 
 *    0: 不启用
 *    1: 启用
 */ 
#define ENABLE_CHECK_FIRMWARE_SIZE          1


/**
 * 【选择自动更新固件的处理方案】
 * 说明: 
 *    1. 在固件更新过程中设备异常断电或重启后，选择是否自动更新已下载好的固件以及自动更新的处理方案
 *    2. 该选项的执行优先级低于上位机更新的方式，这意味着除非上位机超时未发送数据，否则将会优先执行上位机的固件更新
 * 解释: 
 *    
 *    DO_NOT_AUTO_UPDATE:           不需要自动更新，不希望有这种断电恢复固件的机制
 *    ERASE_DOWNLOAD_PART_PROJECT:  更新完成后擦除 download 分区。设备上电时会通过检测 download 分区有无可用固件
 *                                  以判断是否需要自动更新固件
 *                                  * 选择此方案后，将无法选择是否在上电后对 APP 的固件进行安规校验( USING_APP_SAFETY_CHECK_PROJECT )。
 *                                    因为 APP 固件安规校验的数据来源是 download 分区的固件包。
 *    MODIFY_DOWNLOAD_PART_PROJECT: 更新完成后修改 download 分区的固件表头的版本信息。设备上电时会对比 download 分区固件包头记录的新旧版本，
 *                                  若新旧版本不一致，则开始自动更新固件
 *                                  * 此种方式需要修改 download 分区的数据，有以下优劣势：
 *                                    1. 优势：上电时可校验 APP 分区的固件数据正确性和完整性，以提高 APP 固件有损坏或遭篡改时的安全性，甚至
 *                                             可以将固件恢复正常，有效提高系统的安全等级
 *                                    2. 劣势：需要对表头所在的 flash sector 擦除后再重新写入，这意味着每次更新都会擦除同个 sector 两次。
 *                                             倘若 flash 的最小擦除粒度比较大，该方式是无法实现的，因为部分 MCU 的 flash 最小擦除粒度
 *                                             较大，如 STM32 的 F4 系列，其 sector0 是 16 byte ，相对于 VERSION_WRITE_TO_APP 选项，
 *                                             实在得不偿失，除非 sector 较小，如 1 或 2 kByte ，否则推荐选择 VERSION_WRITE_TO_APP                                   
 *    VERSION_WRITE_TO_APP:         更新完成后将新的固件版本写进 APP 分区的尾部，占用 16 byte ，设备上电时会对比 download 分区固件包头
 *                                  记录的版本和 APP 存放的版本，若两个版本不一致，则开始自动更新固件
 *                                  * 此种方式有以下优劣势：
 *                                    1. 优势：上电时可校验 APP 分区的固件数据正确性和完整性，以提高 APP 固件有损坏或遭篡改时的安全性，甚至
 *                                             可以将固件恢复正常，有效提高系统的安全等级
 *                                    2. 劣势：需要占用 APP 分区尾部 16 byte 的空间
 * 选项: 
 *    DO_NOT_AUTO_UPDATE            或 0
 *    ERASE_DOWNLOAD_PART_PROJECT   或 1
 *    MODIFY_DOWNLOAD_PART_PROJECT  或 2
 *    VERSION_WRITE_TO_APP          或 3
 */
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
#define USING_AUTO_UPDATE_PROJECT           VERSION_WRITE_TO_APP
#endif


/**
 * 【片内 Flash 放置固件包所在 sector 的擦除粒度】
 * 说明: 
 *    USING_AUTO_UPDATE_PROJECT = MODIFY_DOWNLOAD_PART_PROJECT 时，需要给出固件包所在 sector 的擦除粒度，单位是 byte
 */
#if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT)
#define ONCHIP_FLASH_ERASE_GRANULARITY      FLASH_PAGE_SIZE
#endif


/**
 * 【选择是否在上电后对 APP 的固件进行安规校验 及 APP 固件检查有问题时的操作方案】
 * 说明: 
 *    1. USING_AUTO_UPDATE_PROJECT = MODIFY_DOWNLOAD_PART_PROJECT 时，本配置才会起效
 *    2. 部分产品对固件的完整性有安规等级要求，本组件支持 APP 固件的数据完整性检查，通过配置以选择是否启用
 *    3. 当启用时，通过配置 USING_APP_SAFETY_PROJECT 可选择 APP 固件检查有问题时的操作方案
 * 解释: 
 *    DO_NOT_CHECK      : 不校验 APP 固件，即不启用
 *    CHECK_UNLESS_EMPTY: 校验 APP 固件，但无法校验时不校验，确保能运行 APP 而不至于等在 bootloader 中。若 APP 固件校验错误，将会
 *                        自动把可用和正确的固件更新至 APP 
 *                        * 无法进行 APP 固件校验的情况是： download 分区和 factory 分区均无可用固件包
 *    AUTO_UPDATE_APP   : APP 固件校验错误后自动将可用和正确的固件更新至 APP
 *                        * 需要注意的是，当选择了本选项，意味着你十分重视 APP 的数据完整性，也就是说，当 APP 固件校验不通过或
 *                          无法校验时，若 download 分区和 factory 分区均无可用固件包，则会停留在 bootloader 中，不会跳转至 APP ，
 *                          即便 APP 存在固件
 *                        * 需要特别声明的是，有一种情况会导致 APP 无法被执行，那便是通过烧录器将固件烧录进 MCU 的 flash 中，
 *                          因为此时不是通过正常的固件更新程序执行， download 分区和 factory 分区均无可用固件包， APP 固件无法进行
 *                          完整性校验，建议采用正常的固件更新流程，即由 bootloader 处理固件更新，或选择 CHECK_UNLESS_EMPTY 选型
 *                        * 此处的自动更新和 USING_AUTO_UPDATE_PROJECT 的不同，本选项仅在 APP 固件校验不通过时才会
 *                          自动更新，而 USING_AUTO_UPDATE_PROJECT 则是无视本选项进行固件自动更新 
 *    DO_NOT_DO_ANYTHING: APP固件校验错误后不要做任何操作，停在 bootloader 即可，即便 download 分区或 factory 分区有可用的固件包
 *                        * 需要注意的是， DO_NOT_DO_ANYTHING 这个选项并不能阻止 APP 分区为空时且 USING_AUTO_UPDATE_PROJECT 启用了
 *                          自动更新的情况， DO_NOT_DO_ANYTHING 只能阻拦APP分区不为空且校验不通过的情况，要阻止自动更新，需要修改
 *                          上方的 USING_AUTO_UPDATE_PROJECT 选项为 DO_NOT_AUTO_UPDATE
 * 选项:  USING_APP_SAFETY_PROJECT
 *    DO_NOT_CHECK          或 0
 *    CHECK_UNLESS_EMPTY    或 1
 *    AUTO_UPDATE_APP       或 2
 *    DO_NOT_DO_ANYTHING    或 3
 */
#if (USING_PART_PROJECT > ONE_PART_PROJECT &&   \
     (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT || USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP))
#define USING_APP_SAFETY_CHECK_PROJECT      CHECK_UNLESS_EMPTY
#endif


/**
 * 【选择是否可以使用 factory 分区的固件包】
 * 说明: 
 *    当启用自动更新或校验 APP 固件完整性时，若 APP 固件不可用，且 download 分区没有可用的固件时，假设有 factory 分区，
 *    且 factory 分区有可用的固件，选择是否将 factory 的固件更新至 APP 中
 * 选项: 
 *    0: 不使用
 *    1: 使用
 */
#if (USING_PART_PROJECT == TRIPLE_PART_PROJECT && \
     (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY || USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP))
#define ENABLE_USE_FACTORY_FIRWARE          1
#endif


/**
 * 【选择当需要恢复出厂固件时，若 factory 分区无固件或固件校验有问题时的解决方案】
 * 解释：
 *    JUMP_TO_APP:           尝试跳转至 APP
 *    WAIT_FOR_NEW_FIRMWARE: 等待上位机发送新的固件包
 * 选项：
 *    JUMP_TO_APP            或 0
 *    WAIT_FOR_NEW_FIRMWARE  或 1
 */
#if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
#define FACTORY_NO_FIRMWARE_SOLUTION        JUMP_TO_APP          
#endif


/**
 * 【选择是否自动纠正固件的分区】
 * 说明: 
 *    该选项是为了修正人为的将分区名写错的情况，是一种能最大程度保证固件更新正常的挽救措施
 *    单分区方案下，无论本功能是否启用，固件包指定的其它分区名都会被修正为 APP 分区，使其可以正常更新
 *    多分区方案下，启用后，固件包指定为 APP 分区时将会被修正为 download 分区，使其可以正常更新
 *    多分区方案下，若不启用，固件包指定为 APP 分区时将会报错，并标记为更新失败
 * 选项: 
 *    0: 不启用
 *    1: 启用
 */
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
#define ENABLE_AUTO_CORRECT_PART             1
#endif


/**
 * 【选择分区的存放位置】
 * 说明: 
 *    当选择启用 SPI Flash 存放固件时，需要指定 download 和 factory 分区的存放位置
 *    本工程使用了 FAL 库，用于管理 flash 的分区。（仅在启用 SPI Flash 时）
 *    更多内容，详见:  https://github.com/RT-Thread-packages/fal
 *    本工程同时还使用了 SFUD 库，因此可自动兼容不同厂家和不同容量的 SPI Flash 
 *    SPI Flash 的底层接口移植文件是 sfud_port.c ，实现内部 API 即可
 *    若使用的 SPI Flash 不支持 SFDP ，则需要修改 sfdu_flash_def.h 文件
 *    修改、移植方式和更多内容，详见:  https://github.com/armink/SFUD
 * 解释: 
 *    STORE_IN_ONCHIP_FLASH : 片内 Flash
 *    STORE_IN_SPI_FLASH    : SPI Flash
 * 注意事项: 
 *    ！！！务必不能强制将 APP 分区设置在 SPI Flash 内。否则，固件将无法运行！！！
 * 选项: 
 *    STORE_IN_ONCHIP_FLASH 或 0
 *    STORE_IN_SPI_FLASH    或 1
 */
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
#define DOWNLOAD_PART_LOCATION              STORE_IN_SPI_FLASH
#if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
#define FACTORY_PART_LOCATION               STORE_IN_SPI_FLASH
#endif

/* 不要修改 IS_ENABLE_SPI_FLASH 的值 */
#if (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH || FACTORY_PART_LOCATION == STORE_IN_SPI_FLASH)
#define IS_ENABLE_SPI_FLASH                 1
#endif
#endif  /* #if (USING_PART_PROJECT > ONE_PART_PROJECT) */


/**
 * 【SPI Flash 放置固件包所在 sector 的擦除粒度】
 * 说明: 
 *    1. 启用了 SPI Flash 时，需要给出固件包所在 sector 的擦除粒度，单位是 byte
 *    2. 若 flash 支持 SFDP 且已经开启 SFUD_USING_SFDP ，则 SPI_FLASH_ERASE_GRANULARITY 不填或填错都无问题，
 *       其会被读到的 SFDP 更新 
 */
#if (IS_ENABLE_SPI_FLASH)
#define SPI_FLASH_ERASE_GRANULARITY         4096
#endif




