#ifndef CRC_H
#define CRC_H

#include <stdint.h>

class CRC
{
public:
    CRC();

    static uint8_t  crc4_itu(uint8_t *data, uint16_t length);
    static uint8_t  crc5_epc(uint8_t *data, uint16_t length);
    static uint8_t  crc5_itu(uint8_t *data, uint16_t length);
    static uint8_t  crc5_usb(uint8_t *data, uint16_t length);
    static uint8_t  crc6_itu(uint8_t *data, uint16_t length);
    static uint8_t  crc7_mmc(uint8_t *data, uint16_t length);
    static uint8_t  crc8(uint8_t *data, uint16_t length);
    static uint8_t  crc8_itu(uint8_t *data, uint16_t length);
    static uint8_t  crc8_rohc(uint8_t *data, uint16_t length);
    static uint8_t  crc8_maxim(uint8_t *data, uint16_t length); //DS18B20
    static uint16_t crc16_ibm(uint8_t *data, uint32_t length);
    static uint16_t crc16_maxim(uint8_t *data, uint32_t length);
    static uint16_t crc16_usb(uint8_t *data, uint32_t length);
    static uint16_t crc16_modbus(uint8_t *data, uint32_t length);
    static uint16_t crc16_ccitt(uint8_t *data, uint32_t length);
    static uint16_t crc16_ccitt_false(uint8_t *data, uint32_t length);
    static uint16_t crc16_x25(uint8_t *data, uint32_t length);
    static uint16_t crc16_xmodem(uint8_t *data, uint32_t length);
    static uint16_t crc16_dnp(uint8_t *data, uint32_t length);
    static uint32_t crc32(uint8_t *data, uint32_t length);
    static uint32_t crc32_mpeg_2(uint8_t *data, uint32_t length);
};

#endif // CRC_H
