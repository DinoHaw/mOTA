/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: It is the configure head file for this library.
 * Created on: 2016-04-23
 */

#ifndef _SFUD_CFG_H_
#define _SFUD_CFG_H_

#include "bsp_common.h"

#define SFUD_PRINT              BSP_Printf

#define SFUD_DEBUG_MODE

/* 关闭后只会查询该库在 /sfud/inc/sfud_flash_def.h 中提供的 Flash 信息表。这样虽然会降低软件的适配性，但减少约 2.3kB 的代码量 */
#define SFUD_USING_SFDP

/* 关闭后该库只驱动支持 SFDP 规范的 Flash，也会适当的降低部分代码量。另外 SFUD_USING_SFDP 及 SFUD_USING_FLASH_INFO_TABLE
   这两个宏定义至少定义一种，也可以两种方式都选择。 */
#define SFUD_USING_FLASH_INFO_TABLE

/* 如果产品中存在多个 Flash ，可以添加 Flash 设备表。修改以下的 enum 列表和宏定义 */
enum {
    SFUD_W25Q128_DEVICE_INDEX = 0,
};

#define SFUD_FLASH_DEVICE_TABLE                                                 \
{                                                                               \
   [SFUD_W25Q128_DEVICE_INDEX] = {.name = "W25Q128JV", .spi.name = "SPI2"},     \
}

/* 开启后，SFUD 也将支持使用 QSPI 总线连接的 Flash。 */
//#define SFUD_USING_QSPI

#endif /* _SFUD_CFG_H_ */
