









# Linux 文件系统

Linux 是一个多用户共同开发以及使用的开源操作系统，指定类型的文件放到指定位置的潜规则就变得非常重要了。便于后续人员维护。（而不像 windows，想放哪儿就放哪儿）

/bin,  /usr/bin 存放可执行文件(命令)

/sbin, /usr/sbin 系统命令

/root

/home

/boot 存放系统内核和系统启动文件

/mnt 系统引导后挂载文件系统的挂载点

/dev - 设置文件

/etc - 保存系统管理所需的配置文件

/lib - 保存系统程序运行所需的库文件

/lost+found - 一般为空, 系统非正常关机后会保存一些信息

/var - 存储一些不断变化的文件, 如:日志文件 `/var/log`

/usr - 与系统用户直接相关的文件和目录, 包括: /usr/bin, /usr/sbin, /usr/lib, /usr/local 等, local 目录下: /usr/local/bin, /usr/local/sbin 等

/media - 存放 ubuntu 系统自动挂载的设备文件

/proc - 虚拟目录，不实际存储在磁盘上，通常用来保存系统信息和进程信息。与 `sysfs` 哟管

/sys - 系统设备和文件爱你层次结构，并向用户提供详细的内核数据信息

