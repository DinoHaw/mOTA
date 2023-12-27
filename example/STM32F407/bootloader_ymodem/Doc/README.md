### 说明
本工程是 bootloader ，基于 STM32F407ZET6 芯片，使用 YModem-1K 协议和 UART 作为 MCU 同外部通讯的方式。

### 实现的功能
通过 UART1 接收 YModem 协议包，进行固件的下载、存储、解密和更新， UART3 负责打印日志。

### 主要使用的外设
1.  GPIO （用于 LED 的闪烁，指示 bootloader 是否正常运行）
2.  Timer （用于执行 LED 的闪烁、主机数据的超时检测等）
3.  UART （和外部进行数据的收发）

### GPIO 功能
| GPIO | 功能     |
|------|----------|
| PA9  | UART1_TX |
| PA10 | UART1_RX |
| PA13 | SWDIO    |
| PA14 | SWCLK    |
| PB10 | UART3_TX |
| PE2  | KEY2     |
| PE3  | KEY1     |
| PE4  | KEY0     |
| PF9  | LED0     |
| PF10 | LED1     |