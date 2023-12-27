### 说明
本工程是 bootloader ，基于 STM32L475VET6 芯片，使用 YModem-1K 协议和 UART 作为 MCU 同外部通讯的方式。

### 实现的功能
通过 UART1 接收 YModem 协议包，进行固件的下载、存储、解密和更新， UART3 负责打印日志。本案例将 download 分区和 factory 分区规划在 SPI Flash 中， bootloader 通过 QSPI 接口与板载的 SPI Flash 将固件包写入指定的地址中。 SPI Flash 型号为 W25Q128JV 。

### 主要使用的外设
1.  GPIO （用于 LED 的闪烁，指示 bootloader 是否正常运行）
2.  Timer （用于执行 LED 的闪烁、主机数据的超时检测等）
3.  UART （和外部进行数据的收发）
4.  QSPI （ SPI Flash 驱动接口）

### GPIO 功能
| GPIO | 功能     |
|------|----------|
| PA9  | UART1_TX |
| PA10 | UART1_RX |
| PA13 | SWDIO    |
| PA14 | SWCLK    |
| PC4  | UART3_TX |
| PD8  | KEY2     |
| PD9  | KEY1     |
| PD10 | KEY0     |
| PE7  | R_LED    |
| PE8  | G_LED    |
| PE9  | B_LED    |
| PE10 | QSPI_CLK |
| PE11 | QSPI_NCS |
| PE12 | QSPI_IO0 |
| PE13 | QSPI_IO1 |
| PE14 | QSPI_IO2 |
| PE15 | QSPI_IO3 |