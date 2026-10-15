led.bin:led.s
#gcc 编译生成 .o 文件
#	arm-linux-gnueabihf-gcc -g -c led.s -o led.o

# 额外生成 map 文件
# -Wl 告诉 gcc 后面的参数要传递给连接器 ld
# -Wa,<options>            Pass comma-separated <options> on to the assembler
# -Wp,<options>            Pass comma-separated <options> on to the preprocessor
# -Wl,<options>            Pass comma-separated <options> on to the linker

# -Wl,-Map=output.map 链接器选项, 制定生成的 map 文件名称
# -Wl,--print-memory-usage 编译结束时在终端打印 flash 和 RAM 的使用摘要
# -Wl,--gc-sections 常配合 -ffunction-sections -fdata-sections 使用, 用于删除未使用的代码段，使 map 文件更准确的反映实际占用的空间
	arm-linux-gnueabihf-gcc -g -c led.s -Wl,-Map=output.map,--print-memory-usage -o led.o 
# TODO: -Wl 为什么无法生成 map 文件

#ld 链接生成 elf 可执行文件，代码运行的起始地址:0X87800000
# -Ttext ADDRESS              Set address of .text section,设置代码段的地址
#arm-linux-gnueabihf-ld -Ttext 0X87800000 led.o -o led.elf
#生成 map 文件
	arm-linux-gnueabihf-ld -Ttext 0X87800000 led.o -o led.elf -Map=led.map

#objcopy elf 转为 bin 文件
# -S --strip-all                   Remove all symbol and relocation information
# -g --strip-debug                 Remove all debugging symbols & sections
	arm-linux-gnueabihf-objcopy -O binary -S -g led.elf led.bin

#objdump 将 elf 反汇编为汇编文件
	arm-linux-gnueabihf-objdump -D led.elf > led.dis


.PHONY: clean

clean:
	rm -rf *.o led.bin led.elf led.dis *.map *.imx
	
	
