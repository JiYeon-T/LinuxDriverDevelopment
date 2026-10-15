# ARM 裸机篇

学而不死则wang,思而不学则dai.




# TODO:

深入学习Linux 操作：《鸟哥的 Linux 私房菜》

- 单步调试一下 FreeRTOS，看下线程调度以及上下文切换等









# 交叉编译环境安装

- linaro 公司开发的交叉编译器

```shell
mv gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf.tar  /usr/local/arm  # 交叉编译器安装位置
tar -xvf gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf.tar 
sudo vi /etc/profile  # 添加环境变量
export PATH=$PATH:/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin  # 修改后要重启电脑,因为 /etc/profile 仅在开机时候加载

```

NOTE: `source /etc/profile` 或 `. /etc/profile` 仅可以使当前终端生效, 其他终端无效。





# Cortex A7 架构

**《正点原子嵌入式Linux驱动开发指南》ch6**

**《ARM Cortex-A(arm V7) 编程手册》**

**《ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition.pdf》**



## 工作模式

9 种工作模式 = 1 user 模式（用户模式） + 8 个特权模式

- 寄存器：

16 个32 位的通用寄存器

低寄存器组: R0~R7: 

高寄存器 :R8~R12,   R13(SP), R14(LR), R15(PC)

CPSR: 当前程序状态寄存器， 

SPSR: 备份程序状态寄存器， 当特定的异常中断发生时， SPSR 用来保存当前的 CPSR，当异常退出以后可以用 SPSR 中保存的值来恢复 CPSR

**在不同的运行模式下，有些寄存器是公用的，有些是独有的。例如：R0~R7 9 个模式公用的; FIQ 模式有自己的 R8_fiq~R12_fiq 等**



## 内存分布





## GNU ARM 汇编

GNU 汇编的所有伪指令都是以 . 开头的，例如: .set, .section, .global, .equ, .word

- GNU ARM 汇编与 Keil  汇编定义常量的方式

汇编中的常量定义类似于C语言语言中的宏，在使用的时候展开，不占用空间

|           |   定义常量方式   |
| :-------: | :--------------: |
|  GNU ARM  | .equ NAME, VALUE |
| Keil 汇编 |  NAME EQU VALUE  |
|           |                  |

例：

```assembly
	.text
	.global _start

	/*定义常量*/
	.equ LED_PIN, 5			@定义 LED引脚号
	.equ GPIO_BASE, 0x40021000 @ 定义 GPIO 基地址
	.equ MAX_COUNT, 1000		@定义最大计数值

_start:
	/*使用定义的常量*/
	LDR R0, =GPIO_BASE		@加载基地址
	MOV R1, #LED_PIN		@使用引脚号常量
	MOV R2, #MAX_COUNT 		@使用计数常量	
```



- 注释方式

|           |                注释方式                 |
| :-------: | :-------------------------------------: |
|    GNU    | @这是注释 <br />或<br /> /* 这是注释 */ |
| Keil 汇编 |                ;这是注释                |
|           |                                         |



- push stack & pop stack

```asm
#栈地址是向下增长的
PUSH {R0~R3, R12} 	@将R0~R3,R12压栈
PUSH {LR}			@LR压栈


POP {LR}			@先恢复 LR
POP {R0~R3, R12}	@再恢复 R0~R3
```

另一种写法：

```shell
STMFD SP!, {R0~R3, R12}  #入栈
STMFD SP!, {LR}

LDMFD SP!, {LR}  # 出栈
LDMFD SP!, {R0~R3, R12}
```

根据 ATPCS 规则，ARM 使用的 FD （Full Deceding 满递减）类型的堆栈，SP 指向最后一个入栈的数值，堆栈是由高地址向低地址增长的，也就是前面说的向下增长的堆栈，因此常用的命令是 STMFD 和 LDMFD. STM 和 LDM 的指令寄存器列表中编号最小的对应低地址，编号大的对应高地址（也就是有多个寄存器时，高地址先入栈？？？）。





**ATPCS 规则**

# TODO:



# IMX6ULL

CortexA7 架构



## 开发板使用注意事项

《正点原子嵌入式Linux驱动开发指南》ch5 I.MX6ULL ALPHA 开发板平台介绍 - 5.6



## 代码烧录地址

对于 IMX6ULL 来说, 链接地址应该指向 RAM 地址, RAM 分为内部 RAM 和外部 RAM(也就是 DDR), 

IM6ULL 的 RAM 地址范围:

内部 RAM: 0x00900000~0x0091FFFF

对于 512MB 的 DDR: 0x80000000~0x9FFFFFFF 

对于 256MB 的 DDR: 0x80000000~0x8FFFFFFF 

**本系列视频，逻辑代码链接地址为 0x87800000 (DDR 中, 而没有使用内部 RAM)，**，要使用 DDR 就必须先初始化，对于 IMX6ULL 来说 bin 文件无法直接运行，必须添加一个头部信息，其中包含了 DDR 的初始化参数。 IMX SOC 内部 boot room 会从 SD 卡，EMMC 等外部存储中读取 DDR 参数信息，然后初始化 DDR，并且将 bin 文件拷贝到指定地址位置。

- 为什么使用 0x87800000 这个地址作为 RAM 运行的开始地址而不是 0x80000000 呢？

代码一致性考虑，后面移植 Uboot 也从该地址开始。



- DDR 与 PSRAM 的区别

PSRAM - 适用与低功耗无联网，小型嵌入式系统，用于扩展 MCU 的运行内存，存放临时zhen缓存，能大幅降低硬件开发难度和 BOM 成本;

DDR - 适用于高性能场景，支撑 Linux 系统运行，高清显示，多媒体编解码，边缘 AI 计算等高吞吐量需求，大容量场景下性价比优势明显;



## 启动方式：

- 通过拨马开关选择 BOOT_MODE[0] & BOOT_MODE[1]，选择从 FUSE 启动，

  | BOOT_MOE[1:0] |                          BOOT 类型                           |
  | :-----------: | :----------------------------------------------------------: |
  |      00       |                        从 EFUSE 启动                         |
  |      01       |                    串行下载, serial / USB                    |
  |      10       | 内部 BOOT 模式（NXP内部 BOOT 代码）<br />  选择从 efuse, SDCard, EMMC, NorFlash, QSPI flash 等设备启动。 |
  |      11       |                             保留                             |

  

- 启动设备的配置 BOOT_CFG[1] [2] [4] 

《核心板原理图》- IM6ULL BOOT CORE 章节

通过这些引脚配置启动设备的参数，例如：falsh 的 page size, 从哪个 SD card 启动(IM6ULL 支持插)等



## 启动头文件

- NXP 出厂就写 Boot Rom 的代码做的事情

1设置内核时钟为 196MHz, 是能 MMU & Cache，是能 L1 Cache, L2 Cache， MMU 为了加速启动

2从外部存储读入 bin 文件（image）

《datasheet》- Program image 

- IVT 和 Boot data 

IVT

Boot data

DCD

《datasheet》- Program image 

- DCD 数据 - Device Configuration Data

《datasheet》-  Device Configuration Data



- 《IM6ULL》 8.4.1 - Device Initialization

内部 ROM: 0x00000000~0x00017FFF， 96KB

内部 RAM: 0x00900000~0x0091FFFF   128KB



- load.imx 头文件中存放的内容：







# DDR - Double Date Rate SDRAM(Radom Access Memory)

电脑内存条的一种。

- DDR 与 PSRAM 的区别

|   对比维度   |              PSRAM              |                        DDR(含 LPDDR)                         |
| :----------: | :-----------------------------: | :----------------------------------------------------------: |
|   刷新机制   |         芯片内置自刷新          |                需外部 DDR 控制器定时强制刷新                 |
|   对外接口   | 异步SRAM, QSPI/xSPI等，引脚极少 |              专用的 DDR 差分总线，引脚数量极多               |
|   工作频率   |     主流 80-200MHz, 上限低      |                主流 3200~6400 MHz, 高频高性能                |
|   带宽能力   |      低带宽，最大 3.2GB/s       |               **超高带宽**，LPDDR5最高 64GB/s                |
|   容量范围   |      小容量，主流 2MB~64MB      |                  **大容量**，单个8GB~128GB                   |
| 硬件设计难度 |  极低，无需等长布线，阻抗匹配   |                  极高，需严格等长，屏蔽设计                  |
|   物料成本   |    中低，小容量场景性价比高     |                    大容量场景成本优势明显                    |
|   应用场景   |     小容量低功耗嵌入式场景      | 可支撑 Linux系统运行，多媒体编解码，<br />边缘AI计算等高性能场景 |



# GPIO

每个 GPIO 至少有两个寄存器， 

XXX_PAD_XXX 设置该引脚的电气特性;

XX_MUX_XXX 设置该引脚复用的功能

具体情况需要参考数据手册。



- GPIO 的内部上下拉与外部上下拉的区别

内部上下拉：

优点：1. 即使外部没人接任何高低电平，该引脚输出也可以保持一个稳定电平;

2. 不需要额外的外部电路

- GPIO 输出的推挽输出与开漏输出区别

NOTE: 自行研究

0



# LED 驱动

## GPIO 操作步骤(方法论)

设置时钟 -> 设置 IO 复用 -> 配置 GPIO 的电气属性 -> 配置 GPIO 的输入/输出

(1) 设置 GPIO 对应的时钟

(2) 设置 IO 的复用功能，使其复用为 GPIO 功能。这里是设置寄存器 IOMUX_SW_MUX_CTL_PAD_XX_XX

(3) 设置 GPIO 的上/下拉，设置速度，等。这里是设置寄存器 IOMUX_SW_PAD_CTL_PAD_XX_XX

(4) 设置 GPIO 的输入/输出电平，使能中断等



## 汇编方式

为什么要用 cortex-A 汇编：

1. 用汇编初始化一些 SOC 外设;
2. 使用汇编初始化 DDR，I.MX6ULL 不需要，新拍你自带的有(内部有 90KB ROM 用于存储 NXP 自己写的初始化代码)
3. 设置 SP 指针，一般指向 DDR，设置好 C 语言运行环境;


```C
int a, b;
a = b; 
```
这段 C 代码对应的汇编语言:
```asm

;假设 a 地址为 0x20, b 地址为 0x30,
LDR R0, =0x30
LDR R1, [R0]	;取出b的值
LDR R0, =0x20	;a的地址
STR R1,[R0]		;将b得知保存到R0即0x20地址指向的内存中
```





## C 语言方式



### 汇编初始化 C 语言运行环境，

STM32 的实现方式(Keil 编译器，汇编语法与 GNU 有点区别)

```asm
;分配栈内存
Stack_Size 		EQU 		0x00000400
				AREA STACK, NOINIT, READWRITE, ALIGN=3 ;ALIGN=2^3=8
Stack_Mem 		SPACE 		Stack_Size
__initial_sp  						; 初始化 SP 指针

;堆内存
Heap_Size 		EQU 		0x00000200
				AREA HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem 	   	SPACE 		Heap_Size
__heap_limit

;代码开始运行位置
Reset_Handler 	PROC
	EXPORT Reset_Handler 		[WEAK]
	IMPORT __main
	IMPORT SystemInit
	LDR R0, =SystemInit 		; 系统硬件初始化
	BLX R0
	LDR R0, =__main 			;C库初始化函数, 内部会调用 main(). 永远不会返回, 所以用 BX
	BL R0
	ENDP
```



NXP IMX6ULL

```asm
# 这里没有初始化 C 库
_start:
	/* 进入SVC模式 */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	/* 将r0寄存器中的低5位清零，也就是cpsr的M0~M4 	*/
	orr r0, r0, #0x13 	/* r0或上0x13,表示使用SVC模式					*/
	msr cpsr, r0		/* 将r0 的数据写入到cpsr_c中 					*/

	ldr sp, =0X80200000	/* 设置栈指针. 分配 2MB 栈空间. DDR 起始地址为 0x80000000，ARM 使用满减栈,高地址为栈底. 0x80000000+0x00200000=0x80200000 */
	b main				/* 跳转到main函数, 永远不返回 */


```



### 链接脚本

链接文件：描述如果很多 .o 文件如何链接成可执行文件，输入文件如何映射到输出文件中，在输出文件中的内存排布
在链接文件里可以指定文件/函数应该存放到哪个代码段里或者指定的地址

**链接脚本由一系列命令组成，每个命令是一个带有参数的关键字或者一个对符号的赋值，可以使用分号分隔命令。文件名之类的字符串可以直接键入，也可以使用通配符 "*"**

最简单的链接脚本可以只包含一个 SECTIONS ，在这一个 SECTIONS 里面来描述输出文件的内存布局。一般编译出来的代码都包含 text, data, bs, rodata 四个段，

例如：要将代码链接到 0x10000000, 数据链接到 0x20000000

```lds
SECTIONS {
	/*定位计数器,后面的文件/段都会以 0x10000000 为起始地址进行链接*/
	. = 0x10000000;
	/*.text 代码段，要链接到 .text 段的所有文件名都需要填入到大括号中,这里使用了通配符*, 表示所有文件的 .text 段都放到这个 .text 段*/
	.text : {*(.text)}
	
	/*重置定位计数器,否则后续的数据段仍然继续按0x10000000为起始地址往下排*/
	. = 0x20000000;
	/*起始地址 4 bytes 对齐*/
	.data ALIGN(4) : {*(.data)}
	
	/*.bss段, __bss_start 和 __bss_end 是符号，其值为定位符".",这两个符号用来保存 .bss 段的开始地址和结束地址
	.bss 段是没有被初始化或初始化为 0 的全局变量,我们需要在代码运行的时候在汇编中手动对他们进行初始化，即可以完成将这段内存清零工作
	.bss 段的起始地址和结束地址符号 __bss_start, __bss_end 是可以在汇编或 .c 文件中被使用的 */
	__bss_start = .;    
	.bss ALIGN(4)  : { *(.bss)  *(COMMON) }    
	__bss_end = .;
}
```







# 启动方式

启动方式通过 BOOT_MODE[1:0] 这两个 PAD 决定：

| BOOT_MODE[1:0] | BOOT 类型      |
| -------------- | -------------- |
| 00             | FUSE 启动      |
| 01             | 串行下载       |
| 10             | 内部 BOOT 模式 |
| 11             | 保留           |

启动设备通过 BOOT_CFG1[7:0]， BOOT_CFG2[7:0],  BOOT_CFG3[7:0] 这 24 个 GPIO 决定，这些 PAD 和 LCD 的 24 根数据线共用LCD_DATA0~LCD_DATA23。

| BOOT 设备选择  | 正式运行后功能         |
| -------------- | ---------------------- |
| BOOT_CFG1[7:0] | LCD1_DATA0~LCD_DATA7   |
| BOOT_CFG2[7:0] | LCD1_DATA8~LCD_DATA15  |
| BOOT_CFG3[7:0] | LCD1_DATA16~LCD_DATA23 |

**这 24 根线和 BOOT_MODE1, BOOT_MODE0 共同组成了 I.MX6U 的启动选择引脚。**

这 24 根引脚只有部分引脚起作用，大部分都直接接地。

开发板上的拨码开关

![BOOT](BOOT.png)

## 镜像文件

IMX6ULL 的内部 ROM 被 NXP 自己使用，所以用户自己的代码必须放到 Nannd/Nor Flash, QSPI Flash, SD Card 等存储设备上，开机后由芯片 BOOT ROM 拷贝到 DDR 中，然后才可以运行。所以不能直接防止编译生成的 bin 文件，要在 bin 文件添加头部。

 镜像文件 = IVT(Image Vector Table) + Boot Data + DCD(Device Configuration Data) + Bin 文件(用户自己编写的 bin)

用户程序的其实地址 `0x87800000`，前面头信息大小 3K

所以 load.imx 的实际其实地址为：0x87800000 - 3072 = 0x877FF400



这三部分数据格式都是固定的，具体意义参考文档：

- IVT

[TAG, Length, Version]



- Boot Data

[start, length, plugin]



- DCD

在执行 BOOT ROM 的时候就对外设进行配置，包括：外设时钟， DDR 等。可以直接操作对应的寄存器

[TAG, Length, Version]

[CMD]

[CMD]



# STM32 风格的驱动

相当于官方 SDK 部分的驱动实现

```C
// 获取 GPIOE_BASE 的地址
GPIOE_BASE = APB2PERIPH_BASE + 0x1800 
    	 = PERIPH_BASE + 0x10000 + 0x1800
    	= 0x40000000 + 0x10000 + 0x1800
    	= 0x40011800

#define GPIOE_BASE 			(APB2PERIPH_BASE + 0x1800)
#define APB2PERIPH_BASE 	(PERIPH_BASE + 0x10000)
#define PERIPH_BASE 		((uint32_t)0x40000000)
    
// 如果要使用的话, 直接通过指针以及定义好的结构体使用
typedef struct {
    __IO uint32_t CRL;
    __IO uint32_t CRH;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t BRR;
    __IO uint32_t LCKR;
} GPIO_TypeDef;

void gpio_set(void)
{
    GPIO_TypeDef *p = (GPIO_TypeDef *)GPIOE_BASE;
    p-> CRL |= 0x01; // bit1 置1
}
```





# NXP IMX6ULL 官方 SDK

代码分层实现了一些 demo

包括：driver, middleware, app 三层，实现了一些 demo

组建层集成了：FreeRTOS, Lwip, USB 协议栈，可以用于学习。



这里仅仅移植了与 GPIO 配置有关的宏定义部分



# BSP 工程管理



## defconfig

`/home/qz/code/LinuxDriverDevelopment/b.zhengdianyuanzi/vol2_arm/05_bsp_prj` 项目配置文件优化，使用 defconfig 方式，在 Makefile  中结合 Config 对需要编译的源文件过滤，避免在源文件(.c) 中通过宏包含整个源文件。

在 Linux 内核或 U-boot 等基于 Kbuild 系统的构建环境中，`defconfig` 不直接被 Makefile include, 标准的流程是：

1. 生成 `.config` 文件：执行 `make xxx_defconfig` ，系统读取 `arch/xxx/configs/xxx_defconfgi` 结和 `Kconfig` 的默认值，生成根目录下的 `.config` 文件
2. 引入 `.config`： 顶层 Makefile 会自动 include `.config`;
3. 使用宏：Makefile 中通过 $(CONFIG_XXX) 变量来判断配置项的值（y, m 或未定义）;

### makefile 中使用 config

1. `defconfig` 文件：仅包含显式设置的配置选项;

```shell
#这是一个实例 defconfig 文件, 只有显式设置的选项才会出现在这里
CONFIG_FEATURE_A=y

#功能B未设置，因此在 .config 中也不会出现 CONFIG_FEATURE_B=y
#如果需要禁用，通常写作 #CONFIG_FEATURE_B is not set, 但在简单 make 中只需不屑即可

#字符串配置
CONFIG_BOARD_NAME="MyCustomBoard"
```

2. `.config` 文件：由工具生成，包含所有配置项的最终状态

3. `Makefile` : 使用 `-include .config` 来加载 config 文件（-include 表示即使 .config 文件不存在也不报错）

- 条件判断：`ifeq ($(CONFIG_XXX), y)`，

- 经典写法: `obj-$(CONFIG_XXX) += file.o`

当 `CONFIG_FEATURE_A=y`,则 `obj-y+= feature_a.o`  

当 `CONFIG_FEATURE_A` 未定义，该行展开为: obj- += feature_a.o, `makefile` 文件会忽略它，从而不编译该文件

```makefile
ARCH ?= arm
DEFCONFIG := configs/my_board_defconfig
CONFIG+FILE := .config

#1.引入.config 文件.
#使用 -include, 而不是 Include, 防止首次编译时 .config 文件不存在导致错误
-include $(CONFIG_FILE)

#2.定义编译目标
TARGET = my_kernel_image

#3.根据CONFIG_决定编译哪些对象
obj-y := main.o
obj-$(CONFIG_FEATURE_A) += feature_a.o
obj-$(CONFIG_FEATURE_B) += feature_b.o

#4.将对象列表转换为实际的文件依赖
#$(VAR:.o=.c) 表示将变量$(VAR)中所有以.o结尾的字符串替换为.c结尾
#OBJ替换为源文件集合??这里应该有问题
OBJS = $(obj-y:.o=.c)

#默认目标, 运行命令启动:make all
all: $(CONFIG_FILE) $(TARGET)
	@echo "Build Complete, Config used:"
	@cat $(CONFIG_FILE)

#模拟生成.config的规则
#在实际内核中,这一步由 scripts/kconfig/conf 完成
$(CONFIG_FILE):$(DEFCONFIG)

#模拟编译步骤
$(TARGET):$(OBJS)
	@echo "Compiling $(TARGET) with objects:$(OBJS)"
	@echo "-CONFIG_FEATURE_A=$(CONFIG_FEATURE_A)"
	@echo "-CONFIG_FEATURE_B=$(CONFIG_FEATURE_B)"
	@touch $(TARGET)
	@echo "Linking complete."

#清理规则
clean:
	rm -f $(CONFIG_FILE) $(TARGET) *.o
	
#仅生成 CONFIG 文件
my_board_defconfig: $(CONFIG_FILE)

#帮助信息
help:
	@echo "Usage:"
	@echo "make my_board_defconfig : Generate .config from defconfig"
	@echo "make 				   : Build the target"
	@echo "make clean			   : clean generated files"
```



4. `defconfig` 到 `.config` 的转换：这个实例中，直接复制 `defconfig` 到 `.config`

- 在真实内核中， `make xxx_defconfig` 会调用 `scripts/Kconfig/config` 工具，它会读取 `arch/arm/configs/xxx_defconfig`，并结合所有 `Kconfig` 中的文件默认值，生成一个完整的无冲突的 `.config` 文件

运行测试：

```shell
#执行以下命令会生成 .config 文件 
make xxx_defconfig
```



### 源文件中使用 config

defconfig 生成的 `.config` 文件被 Makefile 中包含后，仅可以在 `Makefile` 中使用。如果要在源文件中使用 makefile 有哪些方法?

1. 使用编译器的 -D 编译标志动态转换，在 Makefile 中读取`CONFIG_XXX=y` 将其转换为 `-DCONFIG_XXX` 传递给 GCC

.config 文件

```makefile
CONFIG_FEATURE_A=y
CONFIG_FEATURE_B=n
CONFIG_MAX_SIZE=1024
```

Makefile 中：

```makefile
#1.包含配置文件
-include .config

#2.定义一个辅助函数, 将.config文件中的y/n转换为gcc命令中的-D宏
#如果值为y,则生成-DCONFIG_NAME, 否则为空
define convert-to-cflag
$(if $(filter y,$($(1))),-D$(1))
endef

#3.处理所有需要导出的配置项
CFLAGS += $(call convert-to-cflag,CONFIG_FEATURE_A)
CFLAGS += $(call convert-to-cflag,CONFIG_FEATURE_B)
#对于带值的配置,直接拼接
ifdef CONFIG_MAX_SIZE
CFLAGS += -DCONFIG_MAX_SiZE=$(CONFIG_MAX_SIZE)
endif

#4.编译规则中使用 CFLAGS
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
```

2. 自动生成头文件 autoconf.h(大型项目标准做法， Linux/Zephyr 使用该方法)。编写一个脚本(shell/python/awk)，解析.config 文件，并生成一个 C 头文件 `include/generated/autoconf.h`, 然后在编译时包含该头文件路径，源文件需要包含该头文件

```shell
#!/bin/bash
OUTPUT_FILE="include/generated/autoconf.h"
mkdir -p include/generated

echo "/*Auto generated file*/. Do not edit.*/" >> $OUTPUT_FILE

while IFS='=' read -r key value; do
	#跳过注释和空行
	[[ $key=`^#.*$ ]] && continue
	[[ -z $key ]] && continue
	
	if [ "$value"="y" ]; then
		echo "#define $key 1" >> $OUTPUT_FILE
	elif [ "$value"="n" ]; then
		#通常n不定义或定义为0
		#echo "#undef $key" >> $OUTPUT_FILE
		:
	else
		#处理字符串或整数
		#如果是字符串,需要加引号
		if [[ $value=`^\".*"$ ]]; then
			echo "#define $key "$value"" >> $OUTPUT_FILE
		else
			echo "#define $key $value" >> $OUTPUT_FILE
		fi
	fi
done < .config
```

Makefile

```makefile
-include .config

#依赖关系：如果.config变了，则重新生成头文件autoconf.h
include/generated/autoconf.h: .config gen_config.sh
	./gen_config.sh

#编译时包含头文件路径
CFLAGS += -Iinclude/generated

#编译规则
%.o: %.c include/generated/autoconf.h
	$(CC) $(CFLAGS) -c $< -o $@
```

源文件:

```c
#include "autoconf.h"

#ifdef CONFIG_FEATURE_A
#include a.h
#endif

#ifdef CONFIG_MAX_SIZE
#define SIZE CONFIG_MAX_SIZE
#else
#define SIZE 255
#endif
```



3. Kconfig - Linux, Zephyr OS 采用





### 项目实践



## 主频

### 操作步骤(方法论)







































































