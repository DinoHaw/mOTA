# mOTA

### 一、简介
本开源工程是一款专为 32 位 MCU 开发的 OTA 组件，组件包含了 bootloader 、固件打包器、固件发送器三部分。 

mOTA 中的 m 可意为 mini 、 micro 、 MCU ( Microcontroller Unit )，而 OTA ( Over-the-Air Technology )，即空中下载技术。根据维基百科的定义， OTA 是一种为设备分发新软件、配置，乃至更新加密密钥（为例如移动电话、数字视频转换盒或安全语音通信设备——加密的双向无线电）的方法。 OTA 的一项重要特征是，一个中心位置可以向所有用户发送更新，其不能拒绝、破坏或改变该更新，并且该更新为立即应用到频道上的每个人。用户有可能“拒绝” OTA 更新，但频道管理者也可以将其踢出频道。由此可得出 OTA 技术几个主要的特性：
1.  一个中心可向多个设备分发更新资料（固件）；
2.  更新资料一旦发送便不可被更改；
3.  设备可以拒绝更新；
4.  中心可以排除指定的设备，使其不会接收到更新资料。

本工程仅实现 OTA 更新资料的部分技术，即上文列出的 OTA 技术几个主要的特性，而不关心中心分发资料中间采用何种传输技术。（本工程的 example 使用 UART 作为传输媒介）


### 二、实现的功能
MCU 设备上的 OTA 升级可理解为 IAP (In Application Programming) 技术， MCU 通过外设接口（如 UART 、 IIC 、 SPI 、 CAN 、 USB 等接口），连接具备联网能力的模块、器件、设备（以下统称上位机）。上位机从服务器上拉取固件包，再将固件包以约定的通讯协议，经由通讯接口发送至 MCU ，由 MCU 负责固件的解析、解密、存储、更新等操作，以完成设备固件更新的功能。需要注意的是， example 提供的示例不基于文件系统，而是通过对 Flash 划分为不同的功能区域完成固件的更新。

 **本组件实现了以下功能：** 

1.  **固件包完整性检查：** 自动检测固件 CRC 值，保证固件数据的可靠性。
2.  **固件加密：** 支持 AES256 加密算法，提高固件的安全性。
3.  **APP 完整性检查：** 支持 APP 运行前进行完整性检查，以确认运行的固件无数据缺陷。
4.  **断电保护：** 当固件更新过程中（含下载、解密、更新等过程），任何一个环节断电，设备再次上电时，依然能确保有可用的固件。（需配置为至少双分区）
5.  **固件水印检查：** 可检测固件包是否携带了特殊的水印，确认非第三方或非匹配的固件包。
6.  **固件自动更新：** 当 download 或 factory 分区有可用的固件，且 APP 分区为空或 APP 分区不是最新版本的固件，可配置为自动开始更新。
7.  **恢复出厂设置：** factory 分区存放稳定版的固件，当设备需要恢复出厂设置时，该固件会被更新至 APP 分区。
8.  **功能可裁剪：** 本组件通过功能裁剪可实现单分区、双分区、三分区的方案切换、是否配置解密组件、是否自动更新 APP 、是否检查 APP 完整性、 _是否使用 SPI Flash （待实现）_ 。


 **未实现的功能：** 
1.  支持将固件存放至 SPI Flash 的功能。


### 三、 bootloader 架构
#### （一）软件框架

#### （二）文件功能描述
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


### 四、 bootloader 的设计思路
整个 bootloader 设计思路的内容较多，本设计思路也以 PDF 文档的形式提供，详见[《bootloader程序设计思路》](https://gitee.com/DinoHaw/mOTA/blob/master/document/bootloader%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF.pdf)。
![bootloader的程序设计思路（YModem）](image/bootloader%E7%9A%84%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF%EF%BC%88YModem%EF%BC%89.png)

### 五、固件更新流程
根据配置的分区方案不同
#### （一）单分区方案
#### （二）多分区方案
#### （三）恢复出厂固件


### 六、固件的检测与处理机制
写出固件的检测与处理机制，是为了方便理解代码逻辑，此部分也以 PDF 文档的形式提供，详见[《bootloader程序设计思路》](https://gitee.com/DinoHaw/mOTA/blob/master/document/bootloader%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF.pdf)

### 七、所需的工具
1.  [Firmware_Packager(YModem)](https://gitee.com/DinoHaw/mOTA/tools/Firmware_Packager(YModem)) 此工具是必选项，负责打包 bin 固件，并为 bin 固件添加一个 96 byte 的表头，最终生成为 fpk(Firmware Package) 固件包。关于 96 byte 表头的具体内容，详见[《fpk固件包表头信息》](https://gitee.com/DinoHaw/mOTA/blob/master/document/fpk%E5%9B%BA%E4%BB%B6%E5%8C%85%E8%A1%A8%E5%A4%B4%E4%BF%A1%E6%81%AF.pdf)。
2.  [YModem_Sender](https://gitee.com/DinoHaw/mOTA/tools/YModem_Sender) 本工程的 example 采用广泛使用且公开的 YModem-1K 通讯协议，因此也提供了一个基于 YModem 协议的发送器，运行平台是 windows 。

### 八、移植说明
由于写教程工作量较大，本开源工程暂不提供移植说明文档，代码分层设计，具备一定的移植性，有经验的工程师看 example 中的示例代码基本都能自行移植到别的芯片平台。这里仅做几点说明。
1.  bootloader 部分的核心代码都在 `source` 目录下，是移植的必需文件。
2.  


### 九、一些问题的解答
1.  为什么不使用 RTOS ？

2.  为什么要将 bootloader 设计在 flash 的首地址？

3.  为什么不使用标志位作为固件需要更新的标志？

4.  为什么要设计成单分区、双分区和三分区？

5.  什么是 fpk ？

6.  我可以不用固件打包工具（Firmware_Packager），直接用 bin 文件进行更新吗？
> 目前，


### 十、引用的第三方库

本开源工程使用了或将使用以下的第三方库，感谢提供的优秀的代码库（排名不分先后）。

1.   **[fal](https://github.com/RT-Thread/rt-thread/tree/master/components/fal)**   (Flash Abstraction Layer) ，RT-Thread 团队的开发的库，是对 Flash 及基于 Flash 的分区进行管理、操作的抽象库。
2.   **[SFUD](https://github.com/armink/SFUD)**  (Serial Flash Universal Driver) 一款使用 JEDEC SFDP 标准的串行 (SPI) Flash 通用驱动库。
3.   **[crc-lib-c](https://github.com/whik/crc-lib-c)**  为本工程的 CRC32 验算提供了基础。
4.   **[tinyAES](https://github.com/kokke/tiny-AES-c)**  这是一个用 C 编写的 AES 、 ECB 、 CTR 和 CBC 加密算法的小型可移植的库。
5.   **[SEGGER RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/)**  SEGGER's Real Time Transfer (RTT) is the proven technology for system monitoring and interactive user I/O in embedded applications. It combines the advantages of SWO and semihosting at very high performance.


### 参与贡献
1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

 :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3:  :tw-1f1e8-1f1f3: 
