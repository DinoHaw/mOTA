### 说明
本工程是 bootloader ，基于 STM32F103ZET6 芯片，使用 YModem-1K 协议和 UART 作为 MCU 同外部通讯的方式。

### 实现的功能
通过 UART1 接收 YModem 协议包，进行固件的下载、存储、解密和更新， download 分区和 factory 分区可通过 `user_config.h` 配置放置在片内 flash 或 SPI flash 。

### 主要使用的外设
1.  GPIO （用于 LED 的闪烁，指示 bootloader 是否正常运行）
2.  Timer （用于执行 LED 的闪烁、主机数据的超时检测等）
3.  UART （和外部进行数据的收发）
4.  SPI （驱动 SPI flash ）

### GPIO 功能
| GPIO | 功能      |
|------|-----------|
| PA9  | UART1_TX  |
| PA10 | UART1_RX  |
| PA13 | SWDIO     |
| PA14 | SWCLK     |
| PB12 | FLASH_CS  |
| PB13 | SPI2_SCK  |
| PB14 | SPI2_MISO |
| PB15 | SPI2_MOSI |
| PE5  | LED1      |

### 注意事项
若使用 perf_counter ，语言需要至少配置为 gnu99
- AC5: Options for Target -> C/C++ -> 勾选 GNU extensions
- AC6: Options for Target -> C/C++ (AC6) -> Language C 选择 gnu99