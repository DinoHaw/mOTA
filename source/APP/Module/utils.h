#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>

#define CRC32_POLYNOMIAL        0x04C11DB7

void        CRC32_Init      (void);
uint32_t    CRC32_Calc      (uint8_t *buf, uint32_t len);
uint32_t    CRC32_StepCalc  (uint32_t crc, uint8_t *buf, uint32_t len);


#endif
