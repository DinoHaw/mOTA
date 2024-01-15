#ifndef BSP_FLASH_DRV_H
#define BSP_FLASH_DRV_H

#include "gd32l23x.h"

/*
 FLASH_SPI_CS         | PA4   | GPIO | 软件控制CS
 ---------------------|-------|------|-----------------------
 FLASH_SPI_CLK        | PA5   | SPI  |
 ---------------------|-------|------|-----------------------
 FLASH_SPI_MISO       | PA6   | SPI  |
 ---------------------|-------|------|-----------------------
 FLASH_SPI_MOSI       | PA7   | SPI  | SPI0
*/

#define WRITE            0x02     /* write to memory instruction */
#define QUADWRITE        0x32     /* quad write to memory instruction */
#define WRSR             0x01     /* write status register instruction */
#define WREN             0x06     /* write enable instruction */

#define READ             0x03     /* read from memory instruction */
#define QUADREAD         0x6B     /* read from memory instruction */
#define RDSR             0x05     /* read status register instruction */
#define RDID             0x9F     /* read identification */
#define SE               0x20     /* sector erase instruction */
#define BE               0xC7     /* bulk erase instruction */

#define WTSR             0x05     /* write status register instruction */

#define WIP_FLAG         0x01     /* write in progress(wip) flag */
#define DUMMY_BYTE       0xA5     /*  */


#define FLASH_ID        0xC84015

#define FLASH_SPIx                          SPI0
#define FLASH_SPI_CLK                       RCU_SPI0 

#define FLASH_SPI_CS_PORT                   GPIOD
#define FLASH_SPI_CS_PIN                    GPIO_PIN_2
#define FLASH_SPI_CS_CLK                    RCU_GPIOD

#define FLASH_SPI_CLK_PORT                  GPIOB
#define FLASH_SPI_CLK_PIN                   GPIO_PIN_3
#define FLASH_SPI_CLK_CLK                   RCU_GPIOB

#define FLASH_SPI_MISO_PORT                 GPIOB
#define FLASH_SPI_MISO_PIN                  GPIO_PIN_4
#define FLASH_SPI_MISO_CLK                  RCU_GPIOB

#define FLASH_SPI_MOSI_PORT                 GPIOB
#define FLASH_SPI_MOSI_PIN                  GPIO_PIN_5
#define FLASH_SPI_MOSI_CLK                  RCU_GPIOB

#define  SPI0_FLASH_PAGE_SIZE               0x100
#define  SPI0_FLASH_CS_LOW()                gpio_bit_reset(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)
#define  SPI0_FLASH_CS_HIGH()               gpio_bit_set(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)

/* SPI FLASH 硬件初始化*/
void bsp_flash_hardware_init(void);

/*通过SPI接口发送一个字节，并返回从SPI总线接收的字节*/
uint8_t bsp_spi_flash_send_byte(uint8_t byte);
/*向Flash写入多个字节*/
void bsp_spi_flash_page_write(const uint8_t* pbuffer, uint16_t num_byte_to_write);
/*将数据块写入闪存*/
void bsp_spi_flash_buffer_read(uint8_t *pbuffer, uint16_t num_byte_to_read);


#endif /* BSP_FLASH_DRV_H */