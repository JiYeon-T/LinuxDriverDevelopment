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

INCDIRS 		:= 	. \
					imx6ul \
				   	bsp/clk \
				   	bsp/led \
				   	bsp/delay \
					bsp/beep
				   			   
SRCDIRS			:= 	project


ifeq ($(CONFIG_BSP_CLK_ENABLE), y)
SRCDIRS += bsp/clk
#TODO:
# CFLAGS += $(call convert-to-cflag, CONFIG_BSP_CLK_ENABLE)
CFLAGS += -DCONFIG_BSP_CLK_ENABLE
endif

ifeq ($(CONFIG_BSP_DELAY_ENABLE), y)
SRCDIRS += bsp/delay
# CFLAGS += $(call convert-to-cflag, CONFIG_BSP_DELAY_ENABLE)
CFLAGS += -DCONFIG_BSP_DELAY_ENABLE
endif

ifeq ($(CONFIG_BSP_LED_ENABLE), y)
SRCDIRS += bsp/led
# CFLAGS += $(call convert-to-cflag, CONFIG_BSP_LED_ENABLE)
CFLAGS += -D CONFIG_BSP_LED_ENABLE
endif

ifeq ($(CONFIG_BSP_BEEP_ENABLE), y)
SRCDIRS += bsp/beep
# CFLAGS += $(call convert-to-cflag, CONFIG_BSP_BEEP_ENABLE)
CFLAGS += -DCONFIG_BSP_BEEP_ENABLE
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

VPATH			:= $(SRCDIRS)

$(TARGET) : $(OBJS)
	$(LD) -Timx6ul.lds -o $(TARGET).elf $^ -Map=$(TARGET).map
	$(OBJCOPY) -O binary -S $(TARGET).elf $(TARGET).bin
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
#创建保存.o文件的obj目录
	@mkdir -p obj
	$(CC) $< -Wall -nostdlib -c -O2 $(INCLUDE) -o $@

$(COBJS) : obj/%.o : %.c
	$(CC) $< -Wall -nostdlib -c -O2 $(INCLUDE) $(CFLAGS) -o $@

all: $(CONFIG_FILE) $(TARGET)
	@echo "Build complete, Config used:"
	@echo $(CONFIG_FILE)

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

