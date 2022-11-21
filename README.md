![mOTA_logo](image/mOTA_logo_80x80.png)
# mOTA

### 一、简介
&emsp;&emsp;本开源工程是一款专为 32 位 MCU 开发的 OTA 组件，组件包含了 **[bootloader](https://gitee.com/DinoHaw/mOTA/source)** **、固件打包器** **([Firmware_Packager](https://gitee.com/DinoHaw/mOTA/tools/Firmware_Packager(YModem)))** **、固件发送器** 三部分，并提供了一个基于 STM32F103 和 YModem-1K 协议的案例，因此本案例的固件发送器名为 **[YModem_Sender](https://gitee.com/DinoHaw/mOTA/tools/YModem_Sender)** 。

&emsp;&emsp;mOTA 中的 m 可意为 mini 、 micro 、 MCU ( Microcontroller Unit )，而 OTA ( Over-the-Air Technology )，即空中下载技术，根据[维基百科](https://zh.m.wikipedia.org/wiki/%E7%A9%BA%E4%B8%AD%E7%BC%96%E7%A8%8B)的定义， OTA 是一种为设备分发新软件、配置，乃至更新加密密钥（为例如移动电话、数字视频转换盒或安全语音通信设备——加密的双向无线电）的方法。 OTA 的一项重要特征是，一个中心位置可以向所有用户发送更新，其不能拒绝、破坏或改变该更新，并且该更新为立即应用到频道上的每个人。用户有可能“拒绝” OTA 更新，但频道管理者也可以将其踢出频道。由此可得出 OTA 技术几个主要的特性：
1.  一个中心可向多个设备分发更新资料（固件）；
2.  更新资料一旦发送便不可被更改；
3.  设备可以拒绝更新；
4.  中心可以排除指定的设备，使其不会接收到更新资料。

&emsp;&emsp;本工程仅实现 OTA 更新资料的部分技术，即上文列出的 OTA 技术几个主要的特性，而不关心中心分发资料中间采用何种传输技术。（本工程的 example 使用 UART 作为 MCU 和外部的传输媒介）

&emsp; 

---
### 二、实现的功能
&emsp;&emsp;MCU 设备上的 OTA 升级可理解为 IAP (In Application Programming) 技术， MCU 通过外设接口（如 UART 、 IIC 、 SPI 、 CAN 、 USB 等接口），连接具备联网能力的模块、器件、设备（以下统称上位机）。上位机从服务器上拉取固件包，再将固件包以约定的通讯协议，经由通讯接口发送至 MCU ，由 MCU 负责固件的解析、解密、存储、更新等操作，以完成设备固件更新的功能。需要注意的是， `example` 提供的示例不基于文件系统，而是通过对 Flash 划分为不同的功能区域完成固件的更新。

:tw-1f535: **本组件实现了以下功能：** 

1.  **固件包完整性检查：** 自动检测固件 CRC 值，保证固件数据的可靠性。
2.  **固件加密：** 支持 AES256 加密算法，提高固件的安全性。
3.  **APP 完整性检查：** 支持 APP 运行前进行完整性检查，以确认运行的固件无数据缺陷。
4.  **断电保护：** 当固件更新过程中（含下载、解密、更新等过程），任何一个环节断电，设备再次上电时，依然能确保有可用的固件。（需配置为至少双分区）
5.  **固件水印检查：** 可检测固件包是否携带了特殊的水印，确认非第三方或非匹配的固件包。
6.  **固件自动更新：** 当 download 或 factory 分区有可用的固件，且 APP 分区为空或 APP 分区不是最新版本的固件时，可配置为自动开始更新。
7.  **恢复出厂设置：** factory 分区存放稳定版的固件，当设备需要恢复出厂设置时，该固件会被更新至 APP 分区。
8.  **无须 deinit ：** 我们知道，固件更新完毕后从 bootloader 跳转至 APP 前需要对所用的外设进行 deinit ，恢复至上电时的初始状态。本组件的 bootloader 包含了下载器的功能，当使用复杂的外设收取固件包时， deinit 也将变得复杂，甚至很难排除对 APP 的影响。为此，本组件采用了再入 bootloader 的方式，给 APP 提供一个相当于刚上电的外设环境，免去了 deinit 的代码。
9.  **功能可裁剪：** 本组件通过功能裁剪可实现单分区、双分区、三分区的方案切换、是否配置解密组件、是否自动更新 APP 、是否检查 APP 完整性、 _是否使用 SPI Flash （待实现）_ 。    

&emsp;

 :tw-1f534: **暂未实现的功能：** 
1.  支持将固件存放至 SPI Flash 的功能。

&emsp;

---
### 三、 bootloader 架构
#### （一）软件架构
![bootloader软件架构](image/bootloader%E8%BD%AF%E4%BB%B6%E6%9E%B6%E6%9E%84.png)
- 硬件层描述的是运算器件和逻辑器件，如CPU、ADC、TIMER、各类IC等，是所有软件组件的硬件基础，是软件逻辑的最终底层实现。
- 硬件抽象层是位于驱动与硬件电路之间的接口层，将硬件抽象化。它隐藏了特定平台的硬件接口细节，为驱动层提供抽象化的硬件接口，使其具有硬件无关性。
- 驱动层通过调用硬件抽象层的开放接口，实现一定的逻辑功能后封装，提供给上层软件调用。
- 数据传输层负责收发数据，对外开放的是数据发送与接收相关的接口，屏蔽了通讯接口的逻辑代码，使其易于修改为其他类型的通讯接口。
- 协议析构层将调用数据传输层的数据收发接口进行封包发送与收包解析，通过实现用户的自定义协议，完成对数据的构造和解析。
- 应用层负责业务逻辑代码的实现，通过调用其他层封装的接口，完成顶层逻辑功能。

&emsp;

#### （二）文件架构
| 文件                 | 功能描述                                    |
|----------------------|--------------------------------------------|
| main.c               | 由 STM32CubeMX 自动生成，负责外设的初始化 |
| user_config.h        | 用户配置文件，用于裁剪 OTA 组件的功能 |
| app_config.h         | 应用配置文件，配置代码工程的一些运行选项 |
| app.c                | 应用层，负责业务逻辑代码的实现 |
| firmware_manage.c    | 固件的管理接口蹭，提供了固件的所有操作接口 |
| protocol_parser.c    | 协议析构层，实现协议的解包和封包 |
| data_transfer.c      | 数据传输层，对外提供数据发送和接收的接口 |
| data_transfer_port.c | 数据传输层的移植位置，便于修改为其它通讯接口 |
| utils.c              | 工具库，实现了一些需要全局调用的工具性质函数 |
| bsp_config.h         | BSP 层的配置文件 |
| bsp_common.h         | BSP 层的公共头文件 |
| bsp_board.c          | 实现板卡的一些自定义的初始化代码 |
| bsp_uart.c           | 通用的 UART 驱动库 |
| bsp_uart_port.c      | UART 的接口移植文件 |
| bsp_uart_config.h    | UART 的配置文件 |
| bsp_timer.c          | 通用的 timer 驱动库 |
| bsp_flash.c          | flash 的分区操作抽象接口 |
| fal_stm32f1_flash.c  | STM32F1 片内 flash 的写入、读取和擦除的抽象接口 |

&emsp;

---
### 四、 bootloader 的设计思路
&emsp;&emsp;整个 bootloader 设计思路的内容较多，本设计思路也以 PDF 文档的形式提供，详见[《bootloader程序设计思路》](https://gitee.com/DinoHaw/mOTA/blob/master/document/bootloader%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF.pdf)。
![bootloader的程序设计思路（YModem）](image/bootloader%E7%9A%84%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF%EF%BC%88YModem%EF%BC%89.png)

&emsp;

---
### 五、固件更新流程
&emsp;&emsp;根据配置的分区方案不同，固件的更新流程会有些不同，此处仅展示简要的更新流程，更详细的内容，请阅读[《bootloader程序设计思路》](https://gitee.com/DinoHaw/mOTA/blob/master/document/bootloader%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF.pdf)文档和源代码。 
  
&emsp;&emsp;本组件的目的是最大程序的减少 APP 的改动量以实现 OTA 的功能，从下图可知， bootloader 便完成了固件的下载、存放、校验、解密、更新等所有操作， APP 部分所需要做的有以下三件事。
1.  根据 bootloader 占用的大小和 flash 的最小擦除单位，重新设置 APP 的起始位置和中断向量表。
2.  增加触发进入 bootloader 以开始固件更新的方式。（如：接收来自上位机的更新指令）
3.  设置一个更新标志位，且这个标志位在 APP 软复位进入 bootloader 时仍能被读取到。（当固件更新的方式为上位机指令控制时，可以不执行此步骤）

&emsp;&emsp; **一般来说，通知 bootloader 需要进行固件更新的方式有以下两种：** 
1.  采用上位机指令控制的方式，优点是 APP 无须设置更新标志位，即便设备在收到更新指令后断电，也可以照常更新。缺点是设备在上电后， bootloader 需要等待几秒的时间，以确认是否有来自上位机的更新指令，从而决定进入固件更新模式亦或跳转至 APP 。 

2.  APP 在软复位进入 bootloader 之前设置一个特殊的标志位，可以放置在 RAM 或者外部的非易失性存储介质中（如：EEPROM）。此方式的优点是设备上电时 bootloader 无须等待和验证是否有固件更新的指令，通过标志位便可决定是否进入固件更新模式亦或跳转至 APP ，缺点则是 APP 和 bootloader 都要记录标志位所在的地址空间，且该地址空间不能被挪作他用，更不能被意外修改。若使用的是 RAM 作为记录标志位的介质，则还有断电后更新标志信息丢失的问题。

&emsp;&emsp; **综上所述，没有完美的方案，根据实际需求进行选择和取舍即可。** 

> 由于案例采用了 YModem-1K 协议，而本组件开始固件更新的方式是通过上位机发送指令开始的，因此测试时若设备正在运行 APP ，需要有个软复位进入 bootloader 的条件，为了便于展示，案例使用了板卡上的功能按键作为触发条件，模拟上位机向设备发送了更新指令。
   
![固件更新流程图](image/%E5%9B%BA%E4%BB%B6%E6%9B%B4%E6%96%B0%E6%B5%81%E7%A8%8B%E5%9B%BE.png)

&emsp;

---
### 六、固件检测与处理机制
&emsp;&emsp;之所以单独列出固件的检测与处理机制，是为了方便理解代码逻辑，此部分也以 PDF 文档的形式提供，详见[《固件检测与处理机制》](https://gitee.com/DinoHaw/mOTA/blob/master/document/%E5%9B%BA%E4%BB%B6%E6%A3%80%E6%B5%8B%E4%B8%8E%E5%A4%84%E7%90%86%E6%9C%BA%E5%88%B6.pdf)。
![固件检测与处理机制](image/%E5%9B%BA%E4%BB%B6%E6%A3%80%E6%B5%8B%E4%B8%8E%E5%A4%84%E7%90%86%E6%9C%BA%E5%88%B6.png) 

&emsp;

---
### 七、所需的工具
1.  [Firmware_Packager(YModem)](https://gitee.com/DinoHaw/mOTA/tools/Firmware_Packager(YModem)) 此工具是必选项，负责打包 bin 固件，并为 bin 固件添加一个 96 byte 的表头，最终生成为 fpk(Firmware Package) 固件包。关于 96 byte 表头的具体内容，详见[《fpk固件包表头信息》](https://gitee.com/DinoHaw/mOTA/blob/master/document/fpk%E5%9B%BA%E4%BB%B6%E5%8C%85%E8%A1%A8%E5%A4%B4%E4%BF%A1%E6%81%AF.pdf)。由于 YModem-1K 协议的每包的数据大小是 1 Kbyte ，为了便于 bootloader 解包，本工具也将固件表头扩大至了 1 Kbyte ，若自定义的协议支持可变包长，可将表头长度恢复为 96 byte 。
2.  [YModem_Sender](https://gitee.com/DinoHaw/mOTA/tools/YModem_Sender) 本工程的 example 采用广泛使用且公开的 YModem-1K 通讯协议，因此也提供了一个基于 YModem-1K 协议的发送器。由于固件发送器和通讯协议是绑定的，实际使用时，不必绑定此工具，本工程仅为了方便测试而提供。 

> 注：以上的工具是基于 Qt5 开发的，且作为 OTA 组件的一部分，自然也是开源的。运行平台是 windows ，目前仅在 win10 和 win11 上测试过。若需要修改和编译工程，需要自行安装 Qt ，请自行搜索安装教程。

&emsp;

---
### 八、组件占用的空间
&emsp;&emsp;本组件的案例是基于 YModem-1K 协议及 UART 作为 MCU 与外部的数据传输媒介，因此不是仅计算核心代码部分的占用空间情况，而是整个可用工程。此数据才更有参考意义。以下是几种方案配置占用的 flash 和 RAM 的大小。
1.  最大占用：（三分区方案 + 解密组件 + UART 打印日志）  
flash: 19720 ( 19.26 kB )  
RAM: 9976 ( 9.74 kB )

2.  最大占用：（三分区方案 + 解密组件 + SEGGER_RTT 打印日志）  
flash: 17508 ( 17.10 kB )  
RAM: 11952 ( 11.67 kB )

3.  最大占用：（三分区方案 + 解密组件）  
flash: 11304  ( 11.04 kB )  
RAM: 9712  ( 9.48 kB )  

4.  最小占用:（单分区方案）  
flash: 8740 ( 8.5 kB )  
RAM: 9424 ( 9.2 kB )  
 _注：单分区方案无法实现本组件的大部分功能和安全特性，不建议使用，除非 flash 实在受限，否则建议至少使用双分区的方案。_ 

5.  一般占用：（双分区方案）  
flash: 9828 ( 9.6 kB )  
RAM: 9432 ( 9.21 kB )  

6.  一般占用：（双分区方案 + 解密组件）  
flash: 11236 ( 10.97 kB )  
RAM: 9688 ( 9.46 kB )

&emsp;

---
### 九、移植说明
&emsp;&emsp;由于写教程工作量较大，本开源工程暂不提供移植说明文档。代码已分层设计，具备一定的移植性，有经验的工程师看 `example` 中的示例代码基本都能自行移植到别的芯片平台。这里仅做几点说明。
1.  bootloader 部分的核心代码都在 `source` 目录下，是移植的必需文件。
2.  `source/component` 目录下的组件库非移植的必选项，根据功能需要进行裁剪。
3.  因固件包含表头，固件写入的 flash 分区的方式与通讯协议是强相关的。若自定义的协议支持可变长度，那么建议传输第一个分包时就是固件表头的大小（标准表头大小是 96 byte ，本工程因采用 YModem-1K 协议，[固件打包器](https://gitee.com/DinoHaw/mOTA/tools/Firmware_Packager(YModem))将表头扩大到了 1 Kbyte，自行修改即可），从而方便 bootloader 解包。
4.  除开表头部分，固件的每个切包不能超过 4096 byte ，且 4096 除以每个切包大小后必须是整数（如常见的128、256、512、1024、2048等），否则就得修改源码。
5.  单分区方案虽然节省了 flash 空间，但本组件的很多功能和安全特性都无法使用，除非 flash 实在受限，否则建议至少使用双分区的方案。

&emsp;

---
### 十、一些问题的解答
1.  为什么不使用 RTOS ？
> 为了最大程度的减少 bootloader 占用的 flash 空间，体积越小，组件的适用范围就越广。当然，本组件是开源的，想在 bootloader 里增加 RTOS 或者其它代码也是可以的。

&emsp;

2.  为什么要将 bootloader 设计在 flash 的首地址？
> 我们知道， bootloader 的运行环境最理想的情况是未经使用任何外设的。有些设计会将 APP 放置在 flash 首地址， bootloader 放置在其它地址，优点是 APP 无须设置 APP 的起始位置和中断向量表，改动量最少，缺点是这种方式很难做到通用，需要在 bootloader 或 APP 中 deinit 所使用的外设，否则固件更新时可能会出现各式各样的异常。实际上，每个设备每个产品所使用的外设都是不确定的，为了做到通用，本组件选择了 bootloader 设计在 flash 的首地址的方案。

&emsp;

3.  为什么要设计成单分区、双分区和三分区？
> 现实情况是，并非所有设备的 flash 空间都有比较大的富余。有些设备，无法使用多个分区， bootloader + APP 分区已经是极限。而 bootloader 分区方案不同时，其占用的 flash 大小也不同，为了尽可能的减小 bootloader 的体积，而将分区设计成可配置的方式。

&emsp;

4.  什么是 fpk ？
> fpk 是 mOTA 组件的[固件打包器](https://gitee.com/DinoHaw/mOTA/tools/Firmware_Packager(YModem))生成的一种文件，基于 bin 文件，在其头部增加了一个 96 byte 表头后合成的一个新文件，后缀是 `.fpk` 。fpk 取自英文词语 Firmware Package 的缩写，意为固件程序包，本组件统称为固件包，而提及固件时，一般指的是 bin 文件。

&emsp;

5.  我可以不用固件打包器（Firmware_Packager），直接用 bin 文件进行更新吗？
> **目前是不能的。** 本组件提供的功能和安全特性是基于 fpk 表头和多分区的方式实现的，因此需要固件打包器打包固件，生成 fpk 固件包。为了最大程度的使用这些功能和安全特性， bootloader 的更新流程是基于含有表头的固件包开发的，暂时不考虑增加不含表头的更新流程。当然，不排除因为要求的人多了，我就开搞了。:tw-1f60b: 

&emsp;

---
### 十一、引用的第三方库
&emsp;&emsp;本开源工程使用了或将使用以下的第三方库，感谢以下优秀的代码库（排名不分先后）。

1.   **[fal](https://github.com/RT-Thread/rt-thread/tree/master/components/fal)**   (Flash Abstraction Layer) ，RT-Thread 团队的开发的库，是对 Flash 及基于 Flash 的分区进行管理、操作的抽象库。
2.   **[SFUD](https://github.com/armink/SFUD)**  (Serial Flash Universal Driver) 一款使用 JEDEC SFDP 标准的串行 (SPI) Flash 通用驱动库。
3.   **[crc-lib-c](https://github.com/whik/crc-lib-c)**  为本工程的 CRC32 验算提供了基础。
4.   **[tinyAES](https://github.com/kokke/tiny-AES-c)**  这是一个用 C 编写的 AES 、 ECB 、 CTR 和 CBC 加密算法的小型可移植的库。
5.   **[SEGGER RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/)**  SEGGER's Real Time Transfer (RTT) is the proven technology for system monitoring and interactive user I/O in embedded applications. It combines the advantages of SWO and semihosting at very high performance.

&emsp;

---
### 尾巴
开源不易，如果 mOTA 组件对你有用或者帮助到你的话，可以点个 Star 支持下，谢谢。:tw-1f609: 

&emsp;

---
### 参与贡献
1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

 :tw-1f1e8-1f1f3: 