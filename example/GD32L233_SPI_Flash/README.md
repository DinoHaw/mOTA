# 说明

本 demo 工程基于兆易创新的 GD32L233R-EVAL 全功能板。



# 硬件配置说明

1. MCU：GD32L233RCT6
2. Flash：GD25Q16xxxx(2MB)



# 例程使用说明

1. 烧录 bootloader 至 MCU

2. 编译 app1或app2 生成  .bin文件，使用 固件打包器 打包固件

3. 通过 bootloader 管理固件下载与更新

4. 若`#define USING_IS_NEED_UPDATE_PROJECT        USING_APP_SET_FLAG_UPDATE`

   app运行中 **单击按键 KEY_TAMPER** 进行固件更新，**单击按键 KEY_WAKEUP**进行恢复出厂固件。



# 引脚说明

## 1）bootloader与app共同

### 1. 固件包传输

PA2 ---> USART1_TX

PA3 ---> USART1_RX

### 2. 外置Flash

PD2 ---> SPI0_CS

PB3 ---> SPI0_CLK

PB4 ---> SPI0_MISO

PB5 ---> SPI0_MOSI

### 3. 调试信息打印

PA9 ---> USART0_TX

PA10 ---> USART0_RX

> 说明：
>
> 1）此处的USART在开发板 GD32L233R-EVAL 上的物理接口为Mini USB

## 2）app单独

PC7 ---> LED1

PC8 ---> LED2

PC9 ---> LED3

PC11 ---> LED4



PC13 ---> KEY_TAMPER(KEY1)

PA0  ---> KEY_WAKEUP(KEY2)

> 说明：
>
> 1）此处的按键仅做普通按键处理，前缀命名为开发板标识，方便识别



# 特别注意

编译bootloader并烧录，时需在此处将`RAM for Algorithm` 改为`0x00002000`。若不修改，无法烧录。（此处为官方fpk包的BUG，官方 fpk 包默认为 `0x00001000`。）

![image-20240115155625000](C:\Users\wadeR\AppData\Roaming\Typora\typora-user-images\image-20240115155625000.png)
