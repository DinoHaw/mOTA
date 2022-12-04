### 说明
本工程是 bootloader ，基于 STM32F411RET6 芯片，使用 YModem-1K 协议和 UART 作为 MCU 同外部通讯的方式。

### 实现的功能
通过 UART2 接收 YModem 协议包，进行固件的下载、存储、解密和更新， UART1 负责打印日志。

### 主要使用的外设
1.  GPIO （用于 LED 的闪烁，指示 bootloader 是否正常运行）
2.  Timer （用于执行 LED 的闪烁、主机数据的超时检测等）
3.  UART （和外部进行数据的收发）

### GPIO 功能
| GPIO | 功能     |
|------|----------|
| PA2  | UART2_TX |
| PA3  | UART2_RX |
| PA5  | LED      |
| PA9  | UART1_TX |
| PA10 | UART1_RX |
| PA13 | SWDIO    |
| PA14 | SWCLK    |
| PC13 | BUTTON   |