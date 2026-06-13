#define variables
CC = gcc
objs = main.o calculate.o get_input.o
Q=@

main: $(objs)
	$(Q)echo "compiling"
	$(Q)$(CC) -o main $(objs)

#模式规则以及自动化变量:替换掉重复的.c文件生成.o文件的逻辑
%.o: %.c
	$(Q)$(CC) -c $<

#伪目标:如果不添加该行,clean文件存在的时候,`make clean`不会生效, 因为‘clean’目标已经存在了
.PHONY: clean

clean:
	$(Q)echo "cleaning"
	$(Q)rm -f $(objs)
	$(Q)rm -f main

