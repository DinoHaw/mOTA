/**
  ******************************************************************************
  * @file    fal_stm32_flash.h 
  * @author  Dinor
  * @version 1.0.0
  * @date    2019-06-17
  * @brief   This file belongs to the driver layer of FAL, which is managed and 
  *          called by FAL to implement flash erase and write.
  * @logs    Change Logs:
  * Date           Author       Notes
  * 2019-06-17     Dinor        initial implementation
  ******************************************************************************
  */

#ifndef __FAL_STM32_FLASH_H__
#define __FAL_STM32_FLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_config.h"

/* Exported macro ------------------------------------------------------------*/
//#define FAL_PRINTF                  BSP_Printf

/*****************************Define for standard library********************************/
#ifndef USE_HAL_DRIVER
/** @defgroup FLASH_Type_Erase FLASH Erase Type
  * @{
  */
#define FLASH_TYPEERASE_PAGES       ((uint32_t)0x00)  /*!<Pages erase only*/
#define FLASH_TYPEERASE_MASSERASE   ((uint32_t)0x01)  /*!<Flash mass erase activation*/
#endif

#ifdef __cplusplus
}
#endif

#endif  /* __FAL_STM32_FLASH_H__ */

