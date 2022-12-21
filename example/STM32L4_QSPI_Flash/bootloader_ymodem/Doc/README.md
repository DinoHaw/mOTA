### ˵��
�������� bootloader ������ STM32L475VET6 оƬ��ʹ�� YModem-1K Э��� UART ��Ϊ MCU ͬ�ⲿͨѶ�ķ�ʽ��

### ʵ�ֵĹ���
ͨ�� UART1 ���� YModem Э��������й̼������ء��洢�����ܺ͸��£� UART3 �����ӡ��־���������� download ������ factory �����滮�� SPI Flash �У� bootloader ͨ�� QSPI �ӿ�����ص� SPI Flash ���̼���д��ָ���ĵ�ַ�С� SPI Flash �ͺ�Ϊ W25Q128JV ��

### ��Ҫʹ�õ�����
1.  GPIO ������ LED ����˸��ָʾ bootloader �Ƿ��������У�
2.  Timer ������ִ�� LED ����˸���������ݵĳ�ʱ���ȣ�
3.  UART �����ⲿ�������ݵ��շ���
4.  QSPI �� SPI Flash �����ӿڣ�

### GPIO ����
| GPIO | ����     |
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