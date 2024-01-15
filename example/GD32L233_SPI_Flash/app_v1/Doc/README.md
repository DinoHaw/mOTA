### 说明

本工程是示例 APP 。工程代码基于 GD32L233RCT6 芯片。

### 实现的功能

1.  开机后 LED1 亮起
2.  以 500ms 为周期打印字符串 "[x] app v1.0" (x 是递增的数字 )
3.  当单击 Key1 时，模拟上位机发送了更新指令，串口输出 " update firmware " ，复位进入 bootloader
4.  当单击 Key2 时，模拟需要进行恢复出厂固件的场景，串口输出 " firmware recovery " ，复位进入 bootloader

### 主要使用的外设

1.  GPIO （用于 LED 的闪烁，指示 app 是否正常运行，以及按键的检测）
2.  UART （打印数据）

### GPIO 功能

| GPIO | 功能             |
| ---- | ---------------- |
| PA9  | USART0_TX        |
| PA10 | USART0_RX        |
| PC7  | LED1             |
| PC8  | LED2             |
| PC9  | LED3             |
| PC11 | LED4             |
| PC13 | KEY_TAMPER(KEY1) |
| PA0  | KEY_WAKEUP(KEY2) |

