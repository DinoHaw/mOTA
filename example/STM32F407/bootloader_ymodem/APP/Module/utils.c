/* Includes ------------------------------------------------------------------*/
#include "utils.h"


/* Private variables ---------------------------------------------------------*/
static uint32_t _crc_tab[256];


/* Private function prototypes -----------------------------------------------*/
static uint32_t _Bit_Reverse        (uint32_t poly, uint32_t width);
static void     _CRC32_Initialize   (uint32_t poly);
static uint32_t _CRC32_Calculate    (uint32_t crc_init, uint8_t *buf, uint32_t len);


/* Exported functions ---------------------------------------------------------*/
void CRC32_Init(void)
{
    _CRC32_Initialize(CRC32_POLYNOMIAL);
}


uint32_t CRC32_Calc(uint8_t *buf, uint32_t len)
{
    uint32_t crc_init = 0xFFFFFFFF;

    crc_init = _CRC32_Calculate(crc_init, buf, len) ^ 0xFFFFFFFF;

    return crc_init;
}


uint32_t CRC32_StepCalc(uint32_t crc, uint8_t *buf, uint32_t len)
{
    return _CRC32_Calculate(crc, buf, len);
}


/* Private functions ---------------------------------------------------------*/
static uint32_t _Bit_Reverse(uint32_t poly, uint32_t width)
{
    uint32_t i;
    uint32_t var = 0;

    for (i = 0; i < width; i++)
    {
        if (poly & 0x00000001)
            var |= 1 << (width - 1 - i);

        poly >>= 1;
    }
    return var;
}


static void _CRC32_Initialize(uint32_t poly)
{
    uint32_t i, j;
    uint32_t c;

    poly = _Bit_Reverse(poly, 32);

    for (i = 0; i < 256; i++)
    {
        c = i;

        for (j = 0; j < 8; j++)
        {
            if (c & 0x00000001)
                c = poly ^ (c >> 1);
            else
                c >>= 1;
        }

        _crc_tab[i] = c;
    }
}


static uint32_t _CRC32_Calculate(uint32_t crc_init, uint8_t *buf, uint32_t len)
{
    uint32_t i;
    uint8_t index;

    for (i = 0; i < len; i++)
    {
        index = (uint8_t)(crc_init ^ buf[i]);
        crc_init = (crc_init >> 8) ^ _crc_tab[index];
    }

    return crc_init;
}





