objs := start.o main.o

ledc.bin:$(objs)
#链接 .o 文件, 代码运行的起始地址放到了链接脚本中 imx6ul.lds
	arm-linux-gnueabihf-ld -Timx6ul.lds -o ledc.elf -Map=led.map $^
#生成目标文件 ledc.bin
	arm-linux-gnueabihf-objcopy -O binary -S ledc.elf $@
#反汇编生成 .s 文件
	arm-linux-gnueabihf-objdump -D -m arm ledc.elf > ledc.dis

%.o:%.s
#start.s 中没有初始化 C 库，所i这里也没有参与编译
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -O2 -o $@ $<
	
%.o:%.S
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -O2 -o $@ $<
	
%.o:%.c
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -O2 -o $@ $<


clean:
	rm -rf *.o ledc.bin ledc.elf ledc.dis led.map *.imx
	
	
