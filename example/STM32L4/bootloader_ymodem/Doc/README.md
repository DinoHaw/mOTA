### ˵��
�������� bootloader ������ STM32L475VET6 оƬ��ʹ�� YModem-1K Э��� UART ��Ϊ MCU ͬ�ⲿͨѶ�ķ�ʽ��

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
| PC4  | UART3_TX |
| PE7  | R_LED    |