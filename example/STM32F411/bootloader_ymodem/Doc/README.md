### ˵��
�������� bootloader ������ STM32F411RET6 оƬ��ʹ�� YModem-1K Э��� UART ��Ϊ MCU ͬ�ⲿͨѶ�ķ�ʽ��

### ʵ�ֵĹ���
ͨ�� UART2 ���� YModem Э��������й̼������ء��洢�����ܺ͸��£� UART1 �����ӡ��־��

### ��Ҫʹ�õ�����
1.  GPIO ������ LED ����˸��ָʾ bootloader �Ƿ��������У�
2.  Timer ������ִ�� LED ����˸���������ݵĳ�ʱ���ȣ�
3.  UART �����ⲿ�������ݵ��շ���

### GPIO ����
| GPIO | ����     |
|------|----------|
| PA2  | UART2_TX |
| PA3  | UART2_RX |
| PA5  | LED      |
| PA9  | UART1_TX |
| PA10 | UART1_RX |
| PA13 | SWDIO    |
| PA14 | SWCLK    |
| PC13 | BUTTON   |