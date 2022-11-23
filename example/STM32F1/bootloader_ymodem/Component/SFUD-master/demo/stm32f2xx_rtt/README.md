# stm32f2xx RT-Thread Demo

---

## 1�����

�� Demo ʹ�� RT-Thread �����Ŷӵ� [ART-WiFi](http://www.rt-thread.org/product/5.html) �����壬����� `STM32F205RG` ������������� `W25Q64` SPI Flash��������һ· SPI ���ߣ����Ի��������һ· SPI Flash����ͼ��ʾ
![RTT_ART_WiFi](http://git.oschina.net/Armink/SFUD/raw/master/docs/zh/images/RTT_ART_WiFi.jpg)

> ע�⣺���û�� ART_WiFi �����壬ֻҪ���������ƣ�ͬ������ʹ�ô� Demo ��

### 1.1��ʹ�÷���

- Flash �豸�� RT-Thread SPI �豸 `spi10` ���ӵ�Ϊ���ص� `W25Q64` ��`spi30` SPI �豸���ӵ������Լ���ӵ� `W25Q128`��
- SFUD ��ʼ������ `app/src/app_task.c` �� `sys_init_thread` �߳�����ɶԴ��� Flash �ĳ�ʼ����
- �����նˣ���ʼ����ɺ���Խ������е��ն��뿪�������ӣ�����1�������ն����������������ɲ��Թ��̡�

#### 1.1.1 Flash ��������

```
msh >sf
Usage:
sf probe [spi_device]           - probe and init SPI flash by given 'spi_device'
sf read addr size               - read 'size' bytes starting at 'addr'
sf write addr data1 ... dataN   - write some bytes 'data' to flash starting at 'addr'
sf erase addr size              - erase 'size' bytes starting at 'addr'
sf status [<volatile> <status>] - read or write '1:volatile|0:non-volatile' 'status'
sf bench                        - full chip benchmark. DANGER: It will erase full chip!
```

���磺

- 1��̽�Ⲣѡ�� SPI �豸����Ϊ `spi10` ������ Flash �豸���в�����������������ѡ��󣬽������Ĳ���������Դ� Flash �豸��

```
msh >sf probe spi10
[SFUD]Find a Winbond flash chip. Size is 8388608 bytes.
[SFUD]sf_cmd flash device is initialize success.
8 MB sf_cmd is current selected device.
```

- 2����ȡ�ӵ�ַ 0 ��ʼ������ 64 �ֽ����ݣ�������������

```
msh >sf read 0 64
Read the W25Q64 flash data success. Start from 0x00000000, size is 64. The data is:
Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
[00000000] 54 00 00 00 90 8F E3 A7 69 61 70 5F 6E 65 65 64 
[00000010] 5F 63 6F 70 79 5F 61 70 70 3D 30 00 69 61 70 5F 
[00000020] 63 6F 70 79 5F 61 70 70 5F 73 69 7A 65 3D 30 00 
[00000030] 73 74 6F 70 5F 69 6E 5F 62 6F 6F 74 6C 6F 61 64 
```

- 3��д���ݴӵ�ַ 10 ��ʼ������ 5 �ֽ����ݣ�������������

```
msh >sf write 10 1 2 3 4 5
Write the W25Q64 flash data success. Start from 0x0000000A, size is 5.
Write data: 1 2 3 4 5 .
```

- 4�������ӵ�ַ 0 ��ʼ������ 8192 �ֽ����ݣ�������������

```
msh >sf erase 0 8192
Erase the W25Q64 flash data success. Start from 0x00000000, size is 8192
```

- 5����ȡ/�޸� Flash ״̬�Ĵ���

��ȡ Flash ״̬�Ĵ�����ǰ״̬

```
msh >sf status
The W25Q64 flash status register current value is 0x00.
```

�޸� Flash ��ǰ״̬Ϊ `0x1C` ,����Ϊ `����ʧ�����綪ʧ��` ��
```
msh >sf status 1 28
Write the W25Q64 flash status register to 0x1C success.
```

- 6������ Flash ȫƬ�����ܣ����������£�

```
msh >sf bench yes
Erasing the W25Q64 8388608 bytes data, waiting...
Erase benchmark success, total time: 20.591S.
Writing the W25Q64 8388608 bytes data, waiting...
Write benchmark success, total time: 32.768S.
Reading the W25Q64 8388608 bytes data, waiting...
Read benchmark success, total time: 16.129S.
```

�������ܲ��Խ������ [`/docs/zh/benchmark.txt`](https://github.com/armink/SFUD/blob/master/docs/zh/benchmark.txt)

## 2���ļ����У�˵��

`RVMDK` ��ΪKeil�����ļ������ڼ��룩

`EWARM` ��ΪIAR�����ļ�

## 3�����÷���

�� `rtconfig.h` ����������������Ϣ��ÿ�����õĹ������ [`rt-thread/components/drivers/spi/sfud/inc/sfud_cfg.h`](https://github.com/RT-Thread/rt-thread/blob/master/components/drivers/spi/sfud/inc/sfud_cfg.h)

``` C
#define RT_USING_SFUD                      //��ѡ
#define RT_DEBUG_SFUD                  1   //��ѡ
#define RT_SFUD_USING_SFDP                 //��ѡ
#define RT_SFUD_USING_FLASH_INFO_TABLE     //��ѡ
```