#linux
CROSS_COMPILE 	?= arm-linux-gnueabihf-
#X86 GCC
# CROSS_COMPILE 	:=
TARGET		  	?= bsp
ARCH 			:= ARM


CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

# 包含配置文件
DEFCONFIG 		:= defconfig
CONFIG_FILE 	:= .config

-include $(CONFIG_FILE)

CFLAGS 			:=
CXXFLAGS		:=
LIBPATH 		:=


INCDIRS 		:= 	. \
					imx6ul \

SRCDIRS			:= 	project


ifeq ($(CONFIG_BSP_CLK_ENABLE), y)
SRCDIRS += bsp/clk
INCDIRS += bsp/clk
#TODO:
# CFLAGS += $(call convert-to-cflag, CONFIG_BSP_CLK_ENABLE)
CFLAGS += -DCONFIG_BSP_CLK_ENABLE
endif

ifeq ($(CONFIG_BSP_DELAY_ENABLE), y)
SRCDIRS += bsp/delay
INCDIRS += bsp/delay
CFLAGS += -DCONFIG_BSP_DELAY_ENABLE
endif

ifeq ($(CONFIG_BSP_LED_ENABLE), y)
SRCDIRS += bsp/led
INCDIRS += bsp/led
CFLAGS += -D CONFIG_BSP_LED_ENABLE
endif

ifeq ($(CONFIG_BSP_BEEP_ENABLE), y)
SRCDIRS += bsp/beep
INCDIRS += bsp/beep
CFLAGS += -DCONFIG_BSP_BEEP_ENABLE
endif

ifeq ($(CONFIG_BSP_KEY_ENABLE), y)
SRCDIRS += bsp/key
INCDIRS += bsp/key
CFLAGS += -DCONFIG_BSP_KEY_ENABLE
endif

ifeq ($(CONFIG_BSP_GPIO_ENABLE), y)
SRCDIRS += bsp/gpio
INCDIRS += bsp/gpio
CFLAGS += -DCONFIG_BSP_GPIO_ENABLE
endif

ifeq ($(CONFIG_BSP_INT_ENABLE), y)
SRCDIRS += bsp/int bsp/exit
INCDIRS += bsp/int bsp/exit
CFLAGS += -DCONFIG_BSP_INT_ENABLE
endif

ifeq ($(CONFIG_BSP_EPIT_ENABLE), y)
SRCDIRS += bsp/timer
INCDIRS += bsp/timer
CFLAGS += -DCONFIG_BSP_EPIT_ENABLE
endif

ifeq ($(CONFIG_BSP_UART_ENABLE), y)
SRCDIRS += bsp/uart
INCDIRS += bsp/uart
CFLAGS += -DCONFIG_BSP_UART_ENABLE
# gcc 库文件路径中包含的库文件:
# crtbegin.o  crtbeginS.o  crtbeginT.o  crtend.o  crtendS.o  finclude  include  include-fixed  install-tools  libcaf_single.a  
# libgcc.a  libgcc_eh.a  libgcov.a  libgfortranbegin.a  plugin
#-l链接库;-L指定库文件路径
LIBPATH 		:= -lgcc -L /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4/
#由于uart stdio测试定义了puc,puts 这两个函数, 与内建函数冲突,所以使用 -fno-builtin 表示不使用gcc内建函数,严格使用用户自己的实现
CFLAGS 			+= -fno-builtin
endif

ifeq ($(CONFIG_BSP_LCD_ENABLE), y)
SRCDIRS += bsp/lcd
INCDIRS += bsp/lcd
CFLAGS += -DCONFIG_BSP_LCD_ENABLE
endif

ifeq ($(CONFIG_STDIO_PORT_ENABLE), y)
SRCDIRS += external/stdio/src test
INCDIRS += external/stdio/include test
CFLAGS += -DCONFIG_STDIO_PORT_ENABLE
#移值 stdio 库时 /tmp/cc7wS8oA.s:42: Error: thumb conditional instruction should be in IT block -- `addcs r5,r5,#65536'
#加入以下编译选项解决
CFLAGS += -Wa,-mimplicit-it=thumb
endif

$(warning dirs:$(SRCDIRS))
$(warning cflags:$(CFLAGS))

#使用patsubst(模式替换)函数在每个路径前加 -I，修改后直接作为gcc命令的参数使用(添加头文件搜索路径)
#$(INCLUDE)= -I imx6ul -I bsp/clk -I bsp/led -I bsp/delay
INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

#从所有源文件路径中提取出汇编文件
SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
#从所有源文件路径中提取出 .c 汇编文件
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

#notdir(提取文件名)函数,仅保留变量中的文件名部分,删除路径部分, 例如:SFILENDIR := start.S
SFILENDIR		:= $(notdir  $(SFILES))
CFILENDIR		:= $(notdir  $(CFILES))

#.c/.s文件便以后的.o文件默认是保存在和源文件同目录的
#这里将所有的.o文件都保存到obj目录下
SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
#所有的OBJ文件路径
OBJS			:= $(SOBJS) $(COBJS)
$(warning objs:$(OBJS))
$(warning )
$(warning )

VPATH			:= $(SRCDIRS)

$(TARGET) : $(OBJS)
	$(LD) -Timx6ul.lds -o $(TARGET).elf $^ -Map=$(TARGET).map $(LIBPATH)
	$(OBJCOPY) -O binary -S $(TARGET).elf $(TARGET).bin
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
#创建保存.o文件的obj目录
	@mkdir -p obj
#忽略警告
#$(CC) $< -Wall -nostdlib -c -O2 $(INCLUDE) -o $@
	$(CC) $< -nostdlib -c -O2 $(INCLUDE) -o $@

$(COBJS) : obj/%.o : %.c
	$(CC) $< -nostdlib -c -O2 $(INCLUDE) $(CFLAGS) -o $@

all: $(CONFIG_FILE) $(TARGET)
	@echo "Build complete, Config used:$(CONFIG_FILE)"


#模拟生成.config的规则
#在实际内核中,这一步由 scripts/kconfig/conf 完成
$(CONFIG_FILE): $(DEFCONFIG)
	@echo "Generating .config from $(DEFCONFIG)"
	@cp $(DEFCONFIG) $(CONFIG_FILE)
	@echo "Auto-generated config:"$(CONFIG_FILE)
#加入一些编译时配置
	@echo "CONFIG_ARCH_$(ARCH)=y" >> $(CONFIG_FILE)
	@echo "Config generation done."

#生成.config文件
my_defconfig: $(CONFIG_FILE)

#定义一个辅助函数, 将.config文件中的y/n转换为gcc命令中的-D宏
#如果值为y,则生成-DCONFIG_NAME, 否则为空
#参数:变量名,而非变量的值(不包含$)
define convert-to-cflag
# TODO:
$(warning 1:$(1):$($(1)))
$(if $(filter y,$($(1))),-D$(1))
endef


.PHONY: clean
	
clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(OBJS) $(TARGET).map
#删除obj文件夹
	rm -rf obj
#删除生成的.config配置文件
	rm -rf $(CONFIG_FILE)
#imxdownload生成的文件也一并删掉
	rm -rf load.imx


#烧录命令:
#chmod u+x imxdownload
#imxdownload bsp.bin /dev/sdb   //烧写到SDK卡/dev/sdb中,不能烧写到/dev/sda或/dev/sda1设备中

help:
	@echo "Usage:"
	@echo "all 				: build the project"
	@echo "clean 			: clean build workspace"
	@echo "my_defconfig 	: 生成.config配置文件"

