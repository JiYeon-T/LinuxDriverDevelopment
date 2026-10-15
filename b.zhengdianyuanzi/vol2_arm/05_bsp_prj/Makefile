#linux
CROSS_COMPILE 	?= arm-linux-gnueabihf-
#X86 GCC
# CROSS_COMPILE 	:=
TARGET		  	?= bsp

CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

INCDIRS 		:= 	. \
					imx6ul \
				   	bsp/clk \
				   	bsp/led \
				   	bsp/delay \
					bsp/beep


				   			   
SRCDIRS			:= 	project \
				   	bsp/clk \
				   	bsp/delay \
					bsp/led \
					bsp/beep

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

$(TARGET).bin : $(OBJS)
	$(LD) -Timx6ul.lds -o $(TARGET).elf $^ -Map=$(TARGET).map
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) $< -Wall -nostdlib -c -O2 $(INCLUDE) -o $@

$(COBJS) : obj/%.o : %.c
	$(CC) $< -Wall -nostdlib -c -O2 $(INCLUDE) -o $@


.PHONY: clean
	
clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(OBJS) $(TARGET).map
#imxdownload生成的文件也一并删掉
	rm -rf load.imx

#烧录命令:
#chmod u+x imxdownload
#imxdownload bsp.bin /dev/sdb   //烧写到SDK卡/dev/sdb中,不能烧写到/dev/sda或/dev/sda1设备中

