### ˵��
�������� bootloader ������ STM32F407ZET6 оƬ��ʹ�� YModem-1K Э��� UART ��Ϊ MCU ͬ�ⲿͨѶ�ķ�ʽ��

### ʵ�ֵĹ���
ͨ�� UART1 ���� YModem Э��������й̼������ء��洢�����ܺ͸��£� UART3 �����ӡ��־��

### ��Ҫʹ�õ�����
1.  GPIO ������ LED ����˸��ָʾ bootloader �Ƿ��������У�
2.  Timer ������ִ�� LED ����˸���������ݵĳ�ʱ���ȣ�
3.  UART �����ⲿ�������ݵ��շ���

### GPIO ����
| GPIO | ����     |
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