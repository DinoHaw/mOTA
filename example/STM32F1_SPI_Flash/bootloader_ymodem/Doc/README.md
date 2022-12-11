### ˵��
�������� bootloader ������ STM32F103ZET6 оƬ��ʹ�� YModem-1K Э��� UART ��Ϊ MCU ͬ�ⲿͨѶ�ķ�ʽ��

### ʵ�ֵĹ���
ͨ�� UART1 ���� YModem Э��������й̼������ء��洢�����ܺ͸��£� download ������ factory ������ͨ�� `user_config.h` ���÷�����Ƭ�� flash �� SPI flash ��

### ��Ҫʹ�õ�����
1.  GPIO ������ LED ����˸��ָʾ bootloader �Ƿ��������У�
2.  Timer ������ִ�� LED ����˸���������ݵĳ�ʱ���ȣ�
3.  UART �����ⲿ�������ݵ��շ���
4.  SPI ������ SPI flash ��

### GPIO ����
| GPIO | ����      |
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

### ע������
��ʹ�� perf_counter ��������Ҫ��������Ϊ gnu99
- AC5: Options for Target -> C/C++ -> ��ѡ GNU extensions
- AC6: Options for Target -> C/C++ (AC6) -> Language C ѡ�� gnu99