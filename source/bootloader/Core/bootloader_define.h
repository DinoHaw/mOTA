#ifndef __BOOTLOADER_DEFINE_H__
#define __BOOTLOADER_DEFINE_H__


/* bootloader 版本 */
#define BOOT_VERSION_MAIN                   (0x02U) /*!< [15:8] main version */
#define BOOT_VERSION_SUB                    (0x00U) /*!< [7:0] sub version */
#define BOOT_VERSION                        ((BOOT_VERSION_MAIN << 8)    \
                                            |(BOOT_VERSION_SUB))

/* USING_PART_PROJECT */
#define ONE_PART_PROJECT                    0
#define DOUBLE_PART_PROJECT                 1
#define TRIPLE_PART_PROJECT                 2

/* USING_IS_NEED_UPDATE_PROJECT */
#define USING_HOST_CMD_UPDATE               0
#define USING_APP_SET_FLAG_UPDATE           1

/* FACTORY_NO_FIRMWARE_SOLUTION */
#define JUMP_TO_APP                         0
#define WAIT_FOR_NEW_FIRMWARE               1

/* USING_AUTO_UPDATE_PROJECT */
#define DO_NOT_AUTO_UPDATE                  0
#define ERASE_DOWNLOAD_PART_PROJECT         1
#define MODIFY_DOWNLOAD_PART_PROJECT        2
#define VERSION_WRITE_TO_APP                3

/* USING_APP_SAFETY_CHECK_PROJECT */
#define DO_NOT_CHECK                        0
#define CHECK_UNLESS_EMPTY                  1
#define AUTO_UPDATE_APP                     2
#define DO_NOT_DO_ANYTHING                  3

/* DOWNLOAD_PART_LOCATION */
/* FACTORY_PART_LOCATION */
#define STORE_IN_ONCHIP_FLASH               0
#define STORE_IN_SPI_FLASH                  1

#define FIRMWARE_UPDATE_MAGIC_WORD          0xA5A5A5A5      /* 固件需要更新的特殊标记（不建议修改，一定要和 APP 一致） */
#define FIRMWARE_RECOVERY_MAGIC_WORD        0x5A5A5A5A      /* 需要恢复出厂固件的特殊标记（不建议修改，一定要和 APP 一致） */
#define BOOTLOADER_RESET_MAGIC_WORD         0xAAAAAAAA      /* bootloader 复位的特殊标记（不建议修改，一定要和 APP 一致） */

#define APP_PART_NAME                       "app"
#define DOWNLOAD_PART_NAME                  "download"
#define FACTORY_PART_NAME                   "factory"

#define ONCHIP_FLASH_END_ADDRESS            ((uint32_t)(FLASH_BASE + ONCHIP_FLASH_SIZE))            /* 片内 flash 末地址 */
#define APP_ADDRESS                         ((uint32_t)(FLASH_BASE + BOOTLOADER_SIZE))              /* APP 分区起始地址 */
#define DOWNLOAD_ADDRESS                    ((uint32_t)(APP_ADDRESS + APP_PART_SIZE))               /* download 分区起始地址 */
#define FACTORY_ADDRESS                     ((uint32_t)(DOWNLOAD_ADDRESS + DOWNLOAD_PART_SIZE))     /* factory 分区起始地址 */

#endif
