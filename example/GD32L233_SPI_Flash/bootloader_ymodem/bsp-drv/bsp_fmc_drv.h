#ifndef BSP_FMC_DRV_H
#define BSP_FMC_DRV_H

#include "gd32l23x.h"
#include <stdint.h>

#define FMC_PAGE_SIZE           ((uint16_t)0x1000U)     /* 每页 4KB */
#define FMC_WRITE_START_ADDR    ((uint32_t)0x0803E000)  /* 最后一页即 63 页开始 */
#define FMC_WRITE_END_ADDR      ((uint32_t)0x0803FFFF)  /* 最后一页即 63 页结束 */

/* 32 位机总线一次访问 4 个字节地址 */
#define FMC_READ(addrx)         (*(volatile uint32_t *)(uint32_t)(addrx))
    
/* 擦除页 */
void bsp_fmc_erase_page(uint32_t page_addr);
/* 写数据到指定地址 */
void bsp_fmc_program(uint32_t *data, uint32_t addressx);

#endif /* BSP_FMC_DRV */