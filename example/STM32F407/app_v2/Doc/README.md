### 说明
本工程是示例 APP ，并假设固件版本是 v2.0 。工程代码基于 STM32F407ZET6 芯片。

### 实现的功能
1.  开机后从 UART3 打印版本信息
2.  黄绿色 LED 以 500ms 未周期进行闪烁
3.  以 1 秒为周期打印字符串 "[x] app v2.0" (x 是递增的数字 )
4.  当单击 KEY0 时，模拟上位机发送了更新指令，复位进入 bootloader
5.  当长按 KEY1 时（约 3 秒，可配置），模拟需要进行恢复出厂固件的场景，复位进入 bootloader

### 主要使用的外设
1.  GPIO （用于 LED 的闪烁，指示 app 是否正常运行，以及按键的检测）
2.  Timer （用于执行 LED 的闪烁）
3.  UART （打印数据）

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