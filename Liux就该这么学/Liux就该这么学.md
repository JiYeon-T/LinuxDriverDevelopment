****

# TODO:

写一个关闭笔记本 touchpad 的脚本,放到开机文件里/手动执行





#### ch2. 系统命令



```shell
Lixu 系统中命令参数有长短格式之分.
```

##### 1. 包管理

- RPM(Redhat Package Manager)

早期 Linux 安装软件包只能源码安装,需要处理不同库之间的依赖关系等,很复杂,繁琐.

RPM 会建立统一的数据库,详细记录软件信息并自动分析依赖关系.		

```shell
rpm --help # 帮助
rpm -ivh filename.rpm # 安装软件
# -i, --install                      install package(s)
rpm -Uvh filename.rpm # 升级软件
# -U, --upgrade=<packagefile>+       upgrade package(s)
rpm -e filename.rpm # 卸载软件
# -e, --erase=<package>+             erase (uninstall) package
rpm -qpi filename.rpm # 查询软件描述信息
# -p, --package                      query/verify a package file
rpm -qpl filename.rpm # 列出软件文件信息
# -l, --list                         list files in package
rpm -qf filename # 查询文件属于哪个 rpm
# -f, --file                         query/verify package(s) owning file
```

- yum( Yellow dog Updater, Modified)

Yum 是一个在[Fedora](https://baike.baidu.com/item/Fedora/3293972?fromModule=lemma_inlink) 和 RedHat 以及 [CentOS](https://baike.baidu.com/item/CentOS/498948?fromModule=lemma_inlink) 中的 Shell 前端[软件包](https://baike.baidu.com/item/软件包/10508451?fromModule=lemma_inlink)管理器。

尽管 RPM 可以帮助用户查询软件相关的依赖关系,但问题还是要运维人员自己解决,而有些大型软件可能与数十个程序都有依赖,这种情况下依次安装软件包很不方便.

yum 可以根据用户要求分析出所需软件包及其相关的依赖关系,然后自动从服务器下载软件包并安装到系统. yum 软件仓库中的软件包可以是由红帽官方发布的,也可以是第三方的,当然也可以是自己编写的.

TODO: 在CentOS 7.9操作系统上，使用yum info查看软件包信息，包括软件包名称、适用架构、版本号、发行版、软件大小、仓库名称、概要、URL、许可证、描述。

Linux 上好像不用这个?  用的 apt install ??

```shell
yum --help # 帮助
yum repolist all # 列出所有仓库
yum list all # 列出仓库中的所有软件包
yum info pkgname # 查看软件信息, yum info iptables
yum install pkgname
# ubuntu 环境没有配置 yum 仓库
# $ sudo yum install iptables
#There are no enabled repos.
# Run "yum repolist all" to see the repos you have.
# You can enable repos with yum-config-manager --enable <repo>
yum reinstall pkgname # 重新安装
yum update pkgname # 升级
yum remove pkgname # 删除
yum clean all # 清除所有仓库缓存
yum check-update # 检查可更新的软件包
yum groplist # 检查系统中已经安装的软件包组
yum groupinstall pkggroupname # 安装指定的软件包组
yum groupremove pkggroupname # 删除
yum groupinfo pkggroupname # 查询指定软件包组的信息
```

配置 yum 软件仓库:

第一步: 进入 `/etc/yum.repos.d/ ` 修改 yum 软件仓库的配置文件, ubuntu 目录是: `/etc/yum/repos.d`

第二部: 创建 rhel7.repo 新配置文件

```shell
vi rhel7.repo 

[rhel-media]
name=linuxprobe
baseurl=file:///media/cdrom # 提供的方式包括:ftp://, 网页  http://, 本地文件 file://
enabled=1 #设置此源是否可用
gpgcheck=1 #设置此源是否校验文件
gpgkey=file:///media/cdrom/RPM-GPG-KEY_redhat-release  若上参数开启校验,那么请指定公钥文件地址
```

第三步: 按配置参数路径挂载光盘, 并把光盘挂载信息写入到  `/etc/fstab` 目录

```shell
mkdir -p /media/cdrom
mount /dev/cdrom /media/cdrom # 创建挂载点挂载
```

设置开机自动启动:

```shell
vim /etc/fstab
/dev/cdrom   /media/cdrom  iso9660    defaults   0  0
```

第四步:测试

 `yum install httpd -d`



- apt（Advanced Packaging Tool）

APT 是Debian及Ubuntu等Linux发行版的默认软件包管理工具，用于安装、更新、删除和管理软件包。

需root或sudo权限.

```shell
apt --help
apt update # 更新软件包列表
# update - update list of available packages
apt install <package_name> # e.g. sudo apt install curl
# list - list packages based on package names
# search - search in package descriptions
# show - show package details
# install - install packages
# remove - remove packages
# autoremove - Remove automatically all unused packages
# update - update list of available packages
# upgrade - upgrade the system by installing/upgrading packages
# full-upgrade - upgrade the system by removing/installing/upgrading packages
# edit-sources - edit the source information file
```



##### 2. 服务管理

```shell
systemctl --help
```



systemctl 管理服务的启动,停止等

| 作用                         | systemctl 命令<br />RHEL7 系统 | System V init 命令<br />RHEL6 系统 |
| ---------------------------- | ------------------------------ | ---------------------------------- |
| 启动服务                     | systemctl start foo.service    | service foo start                  |
| 重启服务                     | systemctl restart foo.service  | service foo restat                 |
| 停止服务                     | systemctl stop foo.service     | service foo stop                   |
| 重新加载配置文件(不终止服务) | systemctl reload foo.service   | service foo reload                 |
| 查看服务状态                 | systemctl status foo.service   | service foo status                 |

systemctl 管理服务的开机启动等

| 作用                               | systemctl 命令<br />RHEL7 系统           | System V init 命令<br />RHEL6 系统 |
| ---------------------------------- | ---------------------------------------- | ---------------------------------- |
| 开机自动启动-打开                  | systemctl enable foo.service             | chkconfig foo on                   |
| 开机自动启动-关闭                  | systemctl disable foo.service            | chkconfig foo off                  |
| 查看服务是否开机自启动             | systemctl is-enabled foo.service         | chkconfig foo                      |
| 查看各个级别下服务的启动与禁止状态 | systemctl list-unit-files --type=service | chkconfig --list                   |
|                                    |                                          |                                    |



#####  2.  shell 命令

###### 系统命令

- wget

通过 HTTP, HTTPS, FTP 下载文件.支持后台下载,断点续传等很多功能.

```shell
# 下载百度网站的静态资源
wget https://pss.bdstatic.com/static/superman/img/topnav/newzhibo-a6a0831ecd.png
# 查看帮助文档哪个
man wget
# 参数说明:
-b # 后台下载模式
-P # 下载到指定目录
-t # 最大尝试次数
-c # 断点续传
-p # 下载页面所有资源,包括图片视频等
-r # 递归下载
```

- ps

```shell
-a # 显示所有进程(包括其他用户的进程)
-u # 显示用户以及其他详细信息
-x # 显示没有控制终端的进程
# 
ps -aux
ps aux
```

进程状态:

R - 运行态, 进程正在运行或在运行队列中等待

S - 中断,进程正在休眠中

D - 不可中断

Z - 进程已经终止,僵尸态

T - 进程收到停止信号后停止

- top

动态查看系统运行情况

```top

```

- pidof - 查询某个指定服务进程的 pid

```shell
pidof sshd
```

- kill - 杀掉进程

```shell
kill 2222
# killall 杀掉指定名称服务对应的所有进程, 通常复杂的应用程序可能由多个程序实现
pidof httpd
killall httpd
```

- 终端执行的程序后台运行

```shell
./test &
```

- ifconfig - 查看&设置网络

```shell
ifconfig
```

- uname - 查看系统内核以及版本等信息

```shell
uname -a
uname -r
#  -r, --kernel-release     print the kernel release
uname -n # qz
# -n, --nodename           print the network node hostname
uname -s 
# -s, --kernel-name        print the kernel name
```

shell 命令框前面的提示 "qz@qz" 是什么意思

第一个是终端当前登录的用户名称,

第二个是主机名

```shell
qz@qz:~/test$  uname -n
qz
qz@qz:~/test$ 
qz@qz:~/test$  sudo -i root
[sudo] password for qz: 
-bash: root: command not found
qz@qz:~/test$ 
qz@qz:~/test$ sudo -i # 切换到 root
root@qz:~#  sudo --help

```



- uptime - 查看系统负载信息

系统负载越低越好,在生产环境中不要超过 5

```shell
uptime
#FILES
#       /var/run/utmp
#              information about who is currently logged on
#       /proc  process information
```

- free - 查看系统的内存情况

```shell
free --help
free -h
# -h, --human         show human-readable outpu
# 			内存总量		已用量		  可用量	进程共享的内存量	磁盘缓存的内存量	缓存的内存量
              total        used        free      shared 		 buff/cache   	available
Mem:           7.6G        3.8G        325M        1.1G        		3.5G        	2.4G
Swap:          974M        7.6M        967M
```

- who - 查看当前登入主机的用户终端信息

```shell
who --help
who
who -a
```

- last - 查看所有系统的登录记录

由于这些信息是以日志文件的形式保存的,黑客是可以修改的,因此不能以该信息判断没有遭到到黑客入侵

```shell
last
last -a
# -a, --hostlast       display hostnames in the last column
last --help
```

- histroy - 显示历史命令

修改 /etc/profile 文件中的HISTSIZE 变量,可以修改可以保存的最大命令个数

历史命令被保存在 ~/.bash_history (家目录下)文件中

**Linux 系统中以 . 前缀开头的文件均表示隐藏文件, 这些文件大多数为系统服务文件, 可以使用 cat 查看**

```shell
history -c # 清空历史命令
!序号 # 执行第 xxx 调历史命令 cat
cat ~/.bash_history
```

- sosreport - 当 Linux 系统出现异常是,使用该命令来收集系统异常信息

```shell
sudo apt install sosreport
sosreport
```

- passwd

```shell
passwd --help
# 修改密码:
echo "newpassword" | passwd --stdin root # 从标准输入读取,然后设置密码, TODO: passwd 命令没有 --stdin 选项
```

- env - 打印当前所有的环境变量

```shell
env
env | grep bash
```

- chmod - 修改文件/目录权限

权限位: rwxrwxrwx (user, group, other)

```shell
chmod --help
man chmod
chmod u+x test.sh # 为 user 添加 execute 权限
#  Each           MODE           is          of          the          form
# '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'
# -c, --changes          like verbose but report only when a change is made
# -f, --silent, --quiet  suppress most error messages
# -v, --verbose          output a diagnostic for every file processed
# -R, --recursive        change files and directories recursively
```

- chown - 修改文件所有者 / 文件所属组

需要 sudo 权限-

```shell
chown -R system:root test # 修改 test 文件夹的所属组为 system, 所有者为 root
#Examples:
#  chown root /u        Change the owner of /u to "root".
#  chown root:staff /u  Likewise, but also change its group to "staff".
#  chown -hR root /u    Change the owner of /u and subfiles to "root".

# -h, --no-dereference   affect symbolic links instead of any referenced file
# -R, --recursive        operate on files and directories recursively
```



- agettty - Open a terminal and set its mode.

```shell
who -a # 会显示所有终端信息
#           system boot  2025-10-01 13:53
#           run-level 5  2025-10-01 13:53
#LOGIN      tty1         2025-10-05 00:42             10265 id=tty1
#qz       + tty7         2025-10-01 13:54  old         1677 (:0)
#qz       - tty2         2025-10-05 00:42 07:29       10294
#LOGIN      tty6         2025-10-05 00:42             10371 id=tty6
#LOGIN      tty5         2025-10-05 00:42             10388 id=tty5
#LOGIN      tty4         2025-10-05 00:42             10398 id=tty4
#LOGIN      tty3         2025-10-05 00:42             10404 id=tty3

# 查到有终端使用 agettty 命令创建的????
root     10371  0.0  0.0  17676  1812 tty6     Ss+  00:42   0:00 /sbin/agetty --noclear tty6 linux

```

- genromfs - create a romfs image

TODO:

项目上制作编译生成文件的时候会用到这个命令



```shell
man genromfs
mkfs
```

- read - 从命令行读入用户输入

```shell
read -p "Pleaese enter:\n" SCORE # 输入保存到变量 SCORE 中
# -p prompt
# -t timeout
```

- id - print real and effective user and group IDs

```shell
id --help
id -u
# -u user
```

- at - 周期性执行与 cron 类似

at, batch, atq, atrm - queue, examine or delete jobs for later execution

需要先安装, `sudo apt-get install at`,

```shell
at
at 23:30 # 设置执行时间
at -l # 查看所有周期性执行的任务
atrm 序号 # 删除任务

```

- cron - 周期性的执行任务

Linux 系统默认启动的 crond 服务, 专门用于周期性的有计划的执行某一个具体的任务

注意点:1. cron 中的命令都必须添绝对路径, 不知道的可以使用 `whereis` 命令查找

2. cron 配置文件中可以用 # 添加注释;
3. cron 执行周期中的"分"必须指定,不能为空. 星期和日不能同时指定,否则可能冲突
4. 如果有些字段不需要设置,可以使用 * 占位;

```shell
crontab -e # 创建/编辑周期性任务
#	-e	(edit user's crontab)
#	-l	(list user's crontab)
#	-r	(delete user's crontab)
#	-i	(prompt before deleting user's crontab)
# 如果是以管理员身份登录的,可以使用 -u 参数编辑其他用户的任务
```

- whereis - Locate the binary, source, and manual-page files for a command.

```shell
whereis cron
```

- select-editor - 设置本机当前用户的默认编辑器???

修改后所有软件都默认是 vim 打开吗??? 

反正修改后 crontab 是使用 vim 打开了.

```shell
selector-editor # 需要选择数字
```



- pmap - 显示进程的内存分布

```shell
pmap --help
ps -axu 
pmap -x processname
 -p, --show-path             show path in the mapping
 -x, --extended              show details

```





---

###### 文本编辑命令

- cat

```shell
cat -n
# -n, --number             number all output lines
cat --help
```

- more

```shell
more --help
more -p test.cpp
# -p          do not scroll, clean screen and display text
```

- head

```shell
head -n 4 test.cpp
tail -n 4 test.cpp
```

- tr - 替换文本

```shell
cat test.cpp | tr [a-z] [A-Z] # 文本中的内容全部大写输出到终端
tr --help
```

- wc - word count

```shell
wc --help
wc -l test.cpp 
# -l, --lines            print the newline counts
wc -w test.cpp
# -w, --words            print the word counts
wc -c test.cpp
#-c, --bytes            print the byte counts
```

- stat - 查看文件的存储信息和时间等信息

```shell
stat test.cpp
stat -f test.cpp
# -f, --file-system     display file system status instead of file status
stat --help
```

- cut - 按列提取需要的内容

```shell
cut -d: -f1 /etc/password # 显示所有的用户名
# -d 设置间隔符号
# -d, --delimiter=DELIM   use DELIM instead of TAB for field delimiter
# -f 设置需要看得列数
# -f, --fields=LIST       select only these fields;  also print any line
```

- diff - 比较文件差别

```shell
diff --help
diff --brief test.cpp test2.cpp
# -q, --brief                   report only when files differ
diff -c test.cpp test2.cpp # 显示 test.cpp 少/多的东西
diff -c test2.cpp test.cpp
# -c, -C NUM, --context[=NUM]   output NUM (default 3) lines of copied context
```

---

###### 文件目录管理命令

- touch - 创建文件

```shell
touch --help
# -a 仅修改"读取时间"(atime, access time), -a                     change only the access time
# -m 仅修改"修改时间"(mtime, modify time), -m                     change only the modification time
# -d 修改读取时间以及修改时间
#黑客行为:让用户无法察觉文件被修改
#获取文件修改时间,修改文件->修改文件修改时间
```

- mkdir - 创建文件夹

```shell
mkdir --help
# -v, --verbose     print a message for each created directory
# -p, --parents     no error if existing, make parent directories as needed
```

- cp - copy

```shell
# -p 保留源文件的属性, -p                           same as --preserve=mode,ownership,timestamps
# -d 若对象文件为"链接文件",则保留该链接文件的属性, -d                           same as --no-dereference --preserve=links
# -r 递归持续复制,用于目录,-R, -r, --recursive          copy directories recursively
# -i 若目标文件存在则询问,是否覆盖
# -a 相当于 -pdr, -a, --archive                same as -dR --preserve=all
```

- mv -剪切/重命名操作

会删除源文件

```shell
mv x.log y.log # 作用等同于重命名
```

- rm - remove

```shell
rm --help
# -f, --force           ignore nonexistent files and arguments, never prompt
# -r, -R, --recursive   remove directories and their contents recursively
```

- dd - 按照指定大小和个数的数据块来复制文件或转换文件

/dev/zero - 这个文件不占用系统存储空间,却可以提供无穷无尽的数据

```shell
dd --help
# if 输入的文件名称 if=FILE         read from FILE instead of stdin
# of 输出的文件名称 of=FILE         write to FILE instead of stdout
# bs 设置每个'块'的大小 bs=BYTES        read and write up to BYTES bytes at a time
# count 设置要复制的"块"的个数  count=N         copy only N input blocks
dd if=/dev/zero of=560_file count=1 bs=560M # 从 /dev/zero 文件中取出一个大小为 500MB 的数据块
# 例2:使用 dd 命令将光驱设备中的 iso 格式的镜像文件压制出光盘映像文件
# 该功能在 windows 下可能需要使用三方软件,但是在 linux 中可以直接做到
dd if=/dev/cdrom of=RHEL-server-7.0-x86_64-LinuxProbe.com.iso # 
```

- file - 查看 文件类型

```shell
file --help
file /test.cpp
```

---

###### 打包压缩与搜索命令

- tar - 打包/压缩/解压	

```shell
tar -czvf test.tar.gz  /etc # 压缩到 test.tar.gz
tar -xzvf test.tar.gz -C /test2 # 解压到 test2 目录
# -c 创建压缩文件
# -C 指定解压到的目录
# -x 解压缩
# -t 查看压缩包内有哪些文件
# -z 用 Gzip 解压或压缩
# -j 用 bzip2 解压或压缩
# -v 显示解压过程
# -f 目标文件名, 因此 -f 必须放到最后一个
# -p 保留原始的权限与属性
# -P 使用绝对路径来压缩
```

- grep - 文本搜索

```shell
grep --help
# -b 将可执行文件当做文本来搜索
# -c 仅显示匹配条件的条目数
# -i 忽略大小写
# -n 显示行号
# -v 反向选择,仅列出没有关键词的行
# -r, --recursive           like --directories=recurse

# 通过 /etc/password 查找出不允许登录系统的用户
# 登录终端设置为 /sbin/nologin 的用户不允许登录
grep -n /sbin/nologin /etc/password
# 反之,查找允许登录的用户
grep -vn /sbin/nologin /etc/password
```

- find - 查找文件

```shell
find . -name "test*" # 搜索字符串支持正则表达式
find /etc -name "host*" -print
find / -perm -4000 -print # 查找整个系统中搜索权限中包括 SUID 权限的所有文件
# -name 匹配名称,支持正则表达式
# -perm 匹配权限
# -user 匹配所有者
# -group 匹配所在组
find . -name "index*" -m time -1 # 寻找一天以内有人修改的 index 文件 
find /etc -atime +1 # 寻找最好访问时间大于 1 天的文件
find /etc -atime -1 # 寻找最好访问时间小于 1 天的文件
find /etc -atime 1 # 寻找最好访问时间等于 1 天的文件
# -mtime -n/+n/n 匹配修改内容的时间(+n 表示大于x天,指 n 天以前; -n表示小于x天,指n天以内；没有符号:表示等于)
# -atime -n/+n/n 匹配访问文件的时间
# -ctime -n/+n/n 匹配修改文件权限的时间
# --nouser 匹配无所有者的文件
# --nogroup
# -newer f1 !f2 匹配比文件 f1 新但比 f2 旧的文件
# -type b/d/c/p/l/f # 匹配文件类型(块设备/目录/字符设备/管道/链接文件/文本文件)
# -size +n/-n/n 匹配文件大小, (+n 表示大于nKB, -n表示小于nKB)
# -prune 忽略某个目录
# -exec ... {} \; 后面可跟用于进一步处理搜索结果的命令,类似于管道, 其中: {} 表示 find 命令搜索到的每一个文件, 并且命令的结尾必须是 "\;"
```

**根据文件系统层次标准(Filesystem Hierarchy  Standard)协议,Linux 系统中的配置文件会保存到 /etc 目录中**

```shell
find /etc -name "host*"
find /etc -perm -4000 -print # 在整个文件系统中查找包括 SUID 权限的所有文件
```

在 整个文件系统中找出所有归属于 xxx 用户的文件并复制到 /root/findresults 目录

```shell
find / -user xxx -exec cp -a {} /root/findresults \;
find . -type f -exec cp -a {} ./temp \; # 后面好像只能跟一条测试命令
```



- realpath - 打印文件的绝对路径

```
realpath test.txt
```

- 输出系统指定的 shell 解释器

```shell
echo $SHELL
```



###### 网络有关

- tcpdump

```shell
tcpdump -i wlan0 -w test.pcap # 抓包并保存文件
```



- curl - 按照指定的协议从请求内容

如果不指定协议,则默认使用 http

```shell
curl http:// www.baidu.com
curl -L http://www.baidu.com -o index.html # 遵循重定向
# -L, --location      Follow redirects (H)
curl -L -C - http://example.com/testfile -o outputfile
# -C switch是恢复我们文件传输的设备，但还要注意，它后面紧跟一个破折号（-）。
# 这告诉cURL继续文件传输，但是实现这一步，首先要查看已经下载的部分，找到下载的最后一个字节才可以确定从何处可以恢复。
curl -m 60 http:// www.baidu.com
# 设置超时-m, --max-time SECONDS  Maximum time allowed for the transfer
curl --connect-timeout 60 http:// www.baidu.com
# 设置连接超时--connect-timeout SECONDS  Maximum time allowed for connection
curl -u username:password ftp://example.com/readme.txt
# -u, --user USER[:PASSWORD]  Server user and password
curl -x 192.168.1.1:8080 http://example.com
curl -x 192.168.1.1:8080 ftp://example.com/readme.txt
# -x, --proxy [PROTOCOL://]HOST[:PORT]  Use proxy on given port
# 在连接主机之前，很容易直接使用cURL来使用代理。cURL将默认使用HTTP代理，除非您另外指定。
curl --range 0-9999 http://example.com/index.html part1 # 下载 part1
curl --range 10000-99999 http://example.com/index.html part2 # 下载 part2
cat part? > index.html # 合并为 Index.html
# -r, --range RANGE   Retrieve only the bytes within RANGE, 分块下载
cur --cert  path/to/cert.cert:password ftp://example.com
# -E, --cert CERT[:PASSWD]  Client certificate file and password (SSL)
curl -s -O http://example.com # 静默下载
# -s, --silent        Silent mode (don't output anything)
# -O, --remote-name   Write output to a file named as the remote file
curl -I http://www.baidu.com
# -I, --head          Show document info only, 仅获取标题
curl -L -I http://www.baidu.com
#如果将此选项与–L选项结合使用，则cURL将返回其重定向到的每个地址的headers。
curl -H 'Connection: keep-alive' -H 'Accept-Charset: utf-8 ' http://example.com
# 您可以使用-H选项将header传递给cURL。要传递多个header，您只需多次使用-H选项。这是一个例子：
curl -d 'name=geek&location=usa' http://example.com
#-d, --data DATA     HTTP POST data (H), 发布（上传）文件
#POST是网站接受数据的常用方式。例如，当您在线填写表格时，很有可能是使用POST方法从浏览器发送数据。要将数据以这种方式发送到网站，请使用-d选项。
curl -d @filename http://example.com
# --data-raw DATA  HTTP POST data, '@' allowed (H), 要上传文件而不是文本，语法应如下所示：
curl -T myfile.txt ftp://example.com/some/directory/
# -T, --upload-file FILE  Transfer FILE to destination
```

发送电子邮件

```shell
curl smtp://mail.example.com --mail-from me@example.com --mail-rcpt john@domain.com –upload-file email.txt
# 阅读电子邮件:cURL支持IMAP（和IMAPS）和POP3，两者均可用于从邮件服务器检索电子邮件。
curl -u username:password imap://mail.example.com
# 此命令将列出可用的邮箱，但不查看任何特定的邮件。为此，请使用–X选项指定消息的UID。
# -X, --request COMMAND  Specify request command to use
curl -u username:password imap://mal.example.com -X 'UID FETCH 1234'
```

mail.txt # 电子邮件文件需要正确格式化。	

```
 
From: Web Administrator <me@example.com>
 
To: John Doe <john@domain.com>
 
Subject: An example email
 
Date: Sat, 7 Dec 2019 02:10:15
 
 
 
 
John,
 
Hope you have a great weekend.
 
-Admin
```

wget 与 curl 的区别:

wget 仅支持 http 协议.而 curl 支持很多协议, FTP, IMAP, SMTP...

- mali - 发送邮件

```shell
```

- hostname - 获取主机名

```shell
hostname # 获取主机名
hostname --help
vi /etc/hostname # 修改主机名, 可能需要重启生效
```

- 网卡有关操作:

```shell
ifup
ifdown
--help
```

- ping - ping, ping6 - send ICMP ECHO_REQUEST to network hosts

```shell
ping --help
-c # count
-i # interval
-W # timeout
-I # interface
```

- openssh - 可以进行的操作有很多, https 签名等等

TODO:



---

###### 数学运算

- expr - evaluate expressions

https://blog.csdn.net/ChaseAug/article/details/121355468

```shell
expr 1 + 2
expr 1 \* 1000 # 乘法符号需要转义, 如果有结果不符合预期的转义试试
expr 1 / 2 # 整除

a=3 
b=4
echo `expr \($a + 1\)\*\($b+1\)` #输出20，值为(a+1)*(b+1)
```

let - 用于数学计算

https://blog.csdn.net/wzj_110/article/details/105715612

```shell
TIMES=1
let TIMES++ # 不许要加变量符号, $
```







#### ch3 管道运算符

```shell
ls -l /etc | more
```

stdin & stdout & stderr

```shell
cat < in.txt # 文件作为标准输入
ls << over # << 指定分界符,从标准输入读,直到遇到指定的分界符,这里是 "over"

ls -l test.txt > out.txt
cat test.txt >out.txt 2>err.txt # stdout重定向到out.txt, stderr重定向到err.txt,清空文件中原有的内容
cat test2.txt >>out.txt 2>>err.txt # >> 追加写

cat test2.txt > out.txt 2>&1 # 标准错误与标准输出都重定向到文件, 等效与:
cat test2.txt &>out.txt

cat test2.txt >> out.txt 2>&1 # 标准错误与标准输出都重定向到文件(追加), 等效与:
cat test2.txt &>>out.txt
```

###### sudo

- su - 切换用户(Engliash name: super adminstrator)

```shell
su --help

#  -c, --command COMMAND         pass COMMAND to the invoked shell
#  -h, --help                    display this help message and exit
#  -, -l, --login                make the shell a login shell
#  -m, -p,
#  --preserve-environment        do not reset environment variables, and
                                keep the same shell
#  -s, --shell SHELL             use SHELL instead of the default in passwd
```

###### Linux 中的 Login shell 和 nologin shell

参考:# https://blog.csdn.net/lws123253/article/details/89315218

login shell：取得 bash 时需要完整的登陆流程的，就称为 login shell。举例来说，我们登陆 tty1 ~ tty6 时，需要输入用户的账号与密码，此时取得的 bash 就称为“ login shell ”；

non-login shell：取得 bash 接口的方法不需要重复登陆的举动，比如我们登陆 Linux 后， 启动终端Terminal，此时那个终端接口并没有需要再次的输入账号与密码，那个 bash 的环境就称为 non-login shell了。又或者你在原本的 bash 环境下再次使用 bash 这个命令，建立了一个bash子进程，同样的也没有输入账号密码， 那第二个 bash (子程序) 也是 non-login shell 。

**为什么要介绍 login, non-login shell 呢？ 这是因为这两个取得 bash 的情况中，读取的配置文件数据并不一样所致。**

**a. 一般来说，login shell 其实只会读取这两个配置文件：**

/etc/profile：这是系统整体的配置，一般尽量不要修改这个文件.例如:为整个系统设置的环境变量添加在该目录下；

~/.bash_profile 或 ~/.bash_login 或 ~/.profile：属于使用者个人配置，需要修改自己的数据，就写入这些文件！（注意：其实 bash 的 login shell 配置只会读取上面三个文件的其中一个， 而读取的顺序则是依照上面的顺序(~/.bash_profile ->  ~/.bash_login -> ~/.profile), 找到一个存在的文件就停止???）

```shell
# cat ~/.bashrc # linux 用户单独添加环境变量以及设置的命令别名(alias)就可以在该文件中
```



**b. non-login shell则只会读取 ~/.bashrc 这一个文件。**

还有一些比如/etc/man.config、~/.bash_history、~/.bash_logout等文件也会影响bash的操作



login shell与non-login shell的实例区别
我们很多朋友在使用linux的时候肯定会使用到su这个命令，但是很多朋友在使用的时候却常常误用。为什么说是误用？我们接下来就用su这个命令来介绍login shell与non-login shell究竟给我们会带来什么影响。

**这里我们重点强调一下 su 和su -, su 没有带用户名表示切换到 root 用户，但是是以 non-login-shell 的形式登陆；而 su - 则表示以 login-shell 切换 root 用户。**我们看一下，他们有什么区别：

```shell
su --help
# -, -l, --login                make the shell a login shell
# - ：单纯使用 - 如『 su - 』代表使用 login-shell 的变量文件读取方式来登陆系统； 若使用者名称没有加上去，则代表切换为 root 的身份。
# -l ：与 - 类似，但后面需要加欲切换的使用者账号！也是 login-shell 的方式。
# -m ：-m 与 -p 是一样的，表示『使用目前的环境配置，而不读取新使用者的配置文件』
# -c ：仅进行一次命令，所以 -c 后面可以加上命令！

su
env | grep root

su - # 	make the shell a login shell
env | grep root

```

通过比较，我们可以明显发现，以不同的方式登陆后，它们的环境变量存在很大的区别，那么这是什么原因呢，大家应该可以猜到，就是因为他们分别以 non-login-shell 和 login-shell 获取 shell 后，系统读取的配置文件不同导致的。所以为了我们正常的使用，我们应该使用 su -、su - root 或者 su -l root 以 login shell 方式切换至 root 用户，而不是 su 或者 su root 以 non-login shell 方式切换至root用户。

```shell
# 正确:
su-
su - root
su -l root
# 不建议使用/错误用法
su
su root

# 登录后使用 CTRL + D 可以退出(logout)
```

- sudo - 把特定命令的执行权限赋予给指定用户来完成原本只有 root 管理员才可以执行的任务

这样既可以保证普通用户能够完成特定的工作,也可以避免泄漏 root 管理员密码.

主要功能:

1. 限制用户执行指定的命令;
2. 记录用户执行的每一条指令;
3. 配置文件 `/etc/sudoers` 提供集中的用户管理,权限与主机等参数
4. 验证密码后的 5 分钟(默认值) 再次使用无需验证密码

```shell
# sudo - execute a command as another user
sudo -i # change to root
#  -i, --login                 run login shell as the target user; a command may also be specified
#  -, -l, --login                make the shell a login shell
#  -c, --command COMMAND         pass COMMAND to the invoked shell
# -l 列出当前用户可执行的命令
# -u 用户名或 UID, 以指定的用户身份执行命令
# -k 清空密码的有效时间
# -b 在后台执行指定的命令
# -p 更改询问密码的提示语
```

- visudo - 可以修改 sudoers 的配置文件, 也可以直接修改 `/etc/sudoers ` 配置文件

```shell
visudo
```

`/etc/sudoers`  配置文件内容:

谁可以使用     允许使用的主机(以谁的身份)       可执行命令列表

```shell
# User privilege specification
root	ALL=(ALL:ALL) ALL
# 添加
qz ALL=(ALL) /bin/cat # 允许 qz 通过超级用户权限使用 cat
whz ALL=NOPASSWD /bin/ls # 使用该命令不再需要输入密码

# Members of the admin group may gain root privileges
%admin ALL=(ALL) ALL

# Allow members of group sudo to execute any command
%sudo	ALL=(ALL:ALL) ALL

```



 

###### 终端 tty 号

tty：终端设备的统称。

tty一词源于Teletypes，或者teletypewriters，原来指的是电传打字机，是通过串行线用打印机键盘通过阅读和发送信息的东西，后来这东西被键盘与显示器取代，所以现在叫终端比较合适。终端是一种[字符型](https://so.csdn.net/so/search?q=字符型&spm=1001.2101.3001.7020)设备，它有多种类型，通常使用tty来简称各种类型的终端设备。

tty1～6是文本型控制台，

tty7是X Window图形显示管理器。

转载于:https://www.cnblogs.com/EasonJim/p/7189504.html



tty1~7 有什么区别, 可以是

Linux 的 `tty`（虚拟控制台）编号为 1~7，主要用于命令行界面切换，**不同编号对应不同的运行级别和系统模式**：

Linux 运行级别与运行模式

- **tty1（单用户模式）**：运行级别 1，仅允许 root 用户登录，常用于系统维护。 ‌12
- ‌**tty2~6（多用户模式）**‌：运行级别 2~6，支持普通用户登录，默认提供命令行界面。 ‌13
- ‌**tty7（图形界面模式）**‌：运行级别 5 或更高，需安装 X Window 系统并执行 `startx` 命令启动。 ‌23

切换方法:

使用快捷键 ‌**Ctrl+Alt+F1~F6**‌ 可切换不同 tty，‌**F7**‌ 返回图形界面

```shell
who -a
# qz@qz:~$  ls -l /lib/systemd/system/default.target  # 查看系统当前的默认运行级别,这里是多用户的图形界面
# lrwxrwxrwx 1 root root 16 2月   5  2020 /lib/systemd/system/default.target -> graphical.target
# 如果要修改系统默认的运行目标为"多用户,无图形" 模式,可直接用 ln 命令把多用户模式目标文件链接到 /etc/systemd/system 目录
```

systemd 与 sytemV 的区别以及作用

| 作用                                          | systemd 目标名称                     | System V init运行级别 |
| --------------------------------------------- | ------------------------------------ | --------------------- |
| 关机                                          | runlevel0.target, poweroff.target    | 0                     |
| 单用户模式(救援模式)                          | runlevel1.target, rescue.target      | 1                     |
| (多用户的文本界面)等同于级别3**->多用户模式** | runlevel2.target, multi-user.target  | 2                     |
| **多用户的文本界面**                          | runlevel3.target,  multi-user.target | 3                     |
| (多用户的文本界面)等同于级别3**->多用户模式** | runlevel4.target,  multi-user.target | 4                     |
| **多用户的图形界面**                          | runlevel5.target,  graphical.target  | 5                     |
| 重启                                          | runlevel6.target,  reboot.target     | 6                     |
| emergency                                     | emergency.target                     | 紧急 shell            |

target 文件中的内容:

```shell
qz@qz:~$  cat /lib/systemd/system/default.target 

#  This file is part of systemd.
#
#  systemd is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.

[Unit]
Description=Graphical Interface
Documentation=man:systemd.special(7)
Requires=multi-user.target
Wants=display-manager.service
Conflicts=rescue.service rescue.target
After=multi-user.target rescue.service rescue.target display-manager.service
AllowIsolate=yes
```

- 开机后选择进入救援模式的方法:

**救援模式是个好东西,忘记密码/修改系统文件之类的操作,都可以在救援模式下, 没有超级管理员(sudo) 权限的情况下进行**

进入方法

```shell
# 开机进入 Grub 界面后, 按 'e' 进入修改界面
# 'linux' 行末增加 
systemd.unit=secure.target
# F10 开始开机后即进入 secure 模式
```



- 通配符

```shell
# * - 零个或多个字符
# ? - 匹配单个字符
# [0-9] - 匹配 0-9 之间的单个字符
# [abc] - 匹配 abc 中的任意一个字符
ls -l /dev/sda[1-9]
```

- 转移字符

```shell
\ : 使反斜杠后面的一个变量变为单纯的字符串,而不再是个变量对待
'': 单引号转义其中所有的变量为单纯的字符串
"": 保留其中的变量属性,不进行转义处理
``: 把其中的命令执行后结果返回

echo '`ls`' # `ls`
echo `ls` # 输出当前文件夹的文件列表
echo "`ls`"
# echo \`ls` # ERROR
```

**当需要某个命令的输出值时,可以这样**

```shell
echo `uname -a`
echo `xinput` # 输出挤在一起,没有换行
echo "`xinput`" # 添加上双引号,为什么是按照 shell 的格式输出的有换行
echo $PATH # 环境变量 PATH 中使用 : 分隔
```

**shell 中,变量名称一般大写, shell 中不能随便添加括号,否则报错**

```shell
PRICE=5
```

- 定义命令别名

```shell
alias 别名=命令 # 设置别名
unalias 别名   # 取标别名
```







**使用 export 命令可以将名为 LINUX 的一般革量转换成全局变量**

```shell
export os=LINUX
```



#### ch4 shell

- vim

命令模式: 

默认模式

```shell
?字符串 # 从下往上搜索字符串
/字符串 # 从上往下搜索字符串
```

末行模式: 

命令模式通过 : 进入末行模式;  esc 键退出

```shell
:set nu # 显示行号
:set nonu # 不显示行号
```

输入模式: 命令模式通过 a, i, o 键进入输入模式;  esc 键退出

```shell
a - 光标后一个位置
i - 光标前一个位置
o - 光标所在位置另起一行
```



- 周期性执行命令

```shell
at
crond
```



##### shell 脚本

创建的文件默认没有可执行权限,需要增加可执行权限

```shell
chmod u+x example.sh
```

获取用户输入

```shell
$0 # 表示可执行文件的名称,即第 0 个参数
$* # 所有位置的参数值
$# # 总共有几个参数, 不包括 $0
$? # 表示上一条命令的执行结果
$1, $2, $3, ... # 表示第1, 2, 3 个参数
```

判断用户参数

- 文件测试语句

**注意 shell 判断语句的返回值, true 返回 0  返回值:**
**0（真）：如果文件或目录存在。**
**1（假）：如果文件或目录不存在。**

```shell
-d # 测试文件是否目录类型
-e # 测试文件/目录是否存在,
# 使用-e来检查文件或目录的存在是最基本的检查方式，但它不区分是文件还是目录。
# 如果你想分别检查是文件还是目录，可以使用-f（检查文件）和-d（检查目录）选项
#无论是使用[还是[[，-e选项都会返回以下值：

-f # 判断是否为一般文件
-r # 测试当前文件是否有读权限
-w # 测试当前文件是否有写权限
-x # 测试当前文件是否有可执行权限

[ -e /etc/fstab ] # 判断该文件是否存在
echo $? # 获取结果
[ -f /etc/fstab]
```

- 逻辑测试语句

```shell
&&
||
!
[ ! $USER = root ] && echo "user" || echo "root"
```

- 数值比较语句

```shell
-eq # equal
-ne # not equal
-gt # greater than
-lt # less than
=le # less equal
-ge # greter equal
[ 10 -gt 10]
echo $?

# 查看剩余内存
free -m
free -m | grep Mem:
free -m | grep Mem: | awk '{print $4}'
```

- 字符串比较语句

```shell
= # 判断字符串是否相同
!= # 字符串不同
-z # 判断字符串是否为空
[ -z $STRING ]
[ -z $USER ]
```

- 条件控制语句

```shell
if [ -e /etc ]
then
fi

for X in $XLIST
do
done

while X ; do
done

case X in
[a-z]|[A-Z])
  echo "zimu"
;;
*) # default
  echo "None"
esac

USERS=`cat users.txt` # 效果等效,都是执行 shell 命令,将结果保存到变量 USERS
USERS=$(cat users.txt)
```

- 函数

```shell
function fun {

}
```





#### ch5 用户身份与文件权限

Linux 是一个多用户,多任务的操作系统. 一个 Linux 实体机可以同时让多个人通过虚拟机办公使用.

- users - Output who is currently logged in according to FILE.

- UID - user identifier

管理员 - root , 0

系统用户 1 - 999

普通用户: 1000 -

- GID  - Group Indentifier

基本用户组: 组名称和用户名称相同, 仅该用户一人.

扩展用户组: 后面该用户加入其他用户组,即为扩展用户组.

- useradd - 添加用户

```shell
useradd --help
-d # 指定用户的家目录,默认为: /home/username,  cd ~ 命令即进入用户家目录
-e # 账户的到期时间, YYYY-MM-DD
-u # 用户默认的 UID
-g # 指定一个初始的基本用户组(必须存在)
-G # 指定一个或多个扩展用户组
-N # 不创建与用户同名的基本用户组
-s # 用户默认的 shell 解释器

useradd -d /home/xxy -u 8888 -s /sbin/nologin xxy
id xxy
```

- groupadd - 创建用户组

工作中经常将一组用户添加到同一个组中,便于权限管理

```shell
groupadd --help
groupadd software # 创建 software 组
```

- usermod - 修改用户属性

所有信息都保存在 `/etc/passwd` 文件中,也可以直接修改这个文件实现该功能

```shell
-c # 填写用户账户的备注信息
-d -m # 重新指定用户的家目录并把用户的数据转移过去
-e # 账户的到期时间, YYYY-MM-DD
-g # 变更基本用户组
-G # 变更扩展用户组
-L # 锁定用户,禁止其登录
-U # 解锁用户,允许登录
-s # 变更默认终端
-u # 修改用户的 UID

usermod -G root linuxprobe # 修改到 root 组
id linuxprobe
usermod -u 8888 linuxprobe # UID
```

- passwd - 修改用户密码,认证信息,到期时间等

root 管理员可以修改其他人的密码

```shell
-l # 锁定用户,禁止其登录
-u # 解锁用户,允许登录
--stdin # 允许用户从标准输入修改密码, echo "newpassword" | passwd --stdin username
-d # 使该用户可用空密码登录
-e # 强制用户在下次登录时修改密码
-S # 显示用户的密码是否被锁定以及密码所采用的加密算法名称

passwd linuxprobe # 修改其他用户的密码(需要 root 权限)
passwd # 修改当前用户的密码
```

- userdel - 删除用户

删除用户时,用户的家目录默认会保存下来,可以使用 -r 参数删除

```shell
-f # 强制删除用户
-r # 同时删除用户以及用户家目录
```



- SUID, SGID, SBIT - 特殊权限位(Special Bit)

文件除了 rwx 以外的特殊权限位, 可以与一般权限位同时使用,以弥补一般权限位不能实现的功能

- SUID - 是一种对二进制程序进行设置的特殊权限位, 可以让二进制执行程序的执行者临时拥有属主的权限(仅对有执行权限的二进制文件有效),

```shell
#例如: passwd 命令
chmod u+s /usr/bin/passwd # 设置 SUID 标志位

# 用户密码保存在 /etc/shadow 文件中, shadow 文件对其他人是用户的权限是 000,
-rw-r----- 1 root shadow 1660 10月  7 21:04 /etc/shadow
#  但所有用户都可以通过 passwd 命令来修改自己的密码,因为在使用 passwd 命令的时候被加上了 SUID 的特殊权限, 就可以让其他用户临时获得程序所有者(UID)的身份,把更改密码的信息写入到 shadow 文件中
# 查看 passwd 可执行文件的权限: rwx -> rws; 对于原来没有可执行权限的文件 rw- -> rwS
-rwsr-xr-x 1 root root 54256 5月  17  2017 /usr/bin/passwd
```

- SGID

实现的功能:

1. 让执行者临时拥有所属组的权限(对拥有可执行权限的二进制程序设置);

1. 让某个目录中创建的文件自动继承该目录的用户组(仅可以对目录设置);

例1:

早期 `/dev/kmem` 是一个用户存储内核程序要访问数据的字符设备文件, 权限为: 640, 仅 root 和 system 组的成员可以访问,其他用户无法访问
为了获取进程的状态信息, 可以在查看系统进程状态的 ps 命令文件上添加 SGID 权限, 查看 ps 命令文件的属性信息. 这样其他用户执行 ps 命令时,也就临时获取到了 system 组成员的权限, 可以正常访问 `/dev/kmem` 文件访问状态了.

```shell
crw-r--r-- 1 root root 1, 11 10月  6 22:13 /dev/kmsg
-rwxr-sr-x 1 root root 97408 5月  14  2018 /bin/ps
# 本地,新版应该修改了 -rwxr-xr-x 1 root root 97408 5月  14  2018 /bin/ps
chmod -Rf 777 testdir
# drwxrwxrwx 2 qz   qz       4096 10月  8 09:12 testdir
chmod -Rf g+s testdir # 添加 SGID 权限
# drwxrwsrwx 2 qz   qz       4096 10月  8 09:12 testdir
# 在 testdir 文件夹下创建文件, 所属组变更
-rw-rw-r--  1 qz   software      0 10月  8 09:27 abc.txt
```

例2:

如果部分需要一个共享文件夹, 所有人都需要文件夹中的读权限.  就可以为该文件夹设置 SGID 权限,这样文件夹中的任何人创建的任何文件, 所属组中的人都具有读权限.

- SBIT - 当对某个目录设置了 SBIT 后,  用户只能删除自己的文件.

文件的其他人权限部分 x -> t, 如果没有执行权限 -  -> T

```shell
chmod -R o+t testdir
drwxrwxrwt  2 qz   software   4096 10月  8 09:27 testdir/
# 其他人无法删除
whz@qz:/tmp$  rm -rf testdir
rm: cannot remove 'testdir/abc.txt': Operation not permitted
```

**文件能否被删除并不取决于文件自身的权限,而是看当前用户对其所在的目录是否有写入权限**



- 文件的隐藏属性 - 一般权限(rwx)以及特殊权限(SUID, SGID, SBIT) 之外的权限, 被隐藏起来的,用户无法察觉

- lsattr - 查看文件/目录的隐藏属性



- chattr - change file attributes on a Linux file system

```shell
man chattr
+ # 添加属性
- # 移除属性
chattr +a abc.txt # append only
```

参数:

| 参数<br />(+ / -) |                             作用                             |
| :---------------: | :----------------------------------------------------------: |
|         i         | 无法对文件进行修改;<br />若对目录设置了该参数,则无法新建/删除文件,仅可以修改其中的子文件中的内容 |
|         a         |     仅允许补充(追加)内容,无法覆盖/删除内容(append only)      |
|         S         |                文件内容在变更后立刻同步到磁盘                |
|         s         |   彻底从磁盘中删除, 不可恢复(用 0 填充原文件所在硬盘位置)    |
|         A         |            不能修改这个文件的最后访问时间(atime)             |
|         b         |                 不能修改文件或目录的存取时间                 |
|         D         |                     检查压缩文件中的错误                     |
|         d         |               使用 dump 命令时,忽略本文件/目录               |
|         c         |                   默认将文件/目录进行压缩                    |
|         u         |     当删除该文件后亦然保留其在硬盘中的数据,方便日后恢复      |
|         t         |             让文件系统支持尾部合并(tail-merging)             |
|         X         |                 可以直接访问压缩文件中的内容                 |

- 访问控制列表(ACL) - 对特定用户进行限制

不像 rwx 以及 SUID, SGID, SBIT 等权限,对一批/所有用户进行限制

- setfacl - 管理文件/目录的 ACL 规则

```shell
setfacl --help #        setfacl - set file access control lists
# 设置文件夹,需要 -R 参数
#  -R, --recursive         recurse into subdirectories
#  -m, --modify=acl        modify the current ACL(s) of file(s)
setfacl -Rm u:linuxprobe:rwx /root # u:user
setfacl -Rm g:software:rwx /root # 设置用户组的 ACL 权限, g:group
```

如何查看文件是否设置了 ACL 权限, ls -l 命令最后的 . 变成了 +

```shell
sudo setfacl -Rm u:qizhen:rwx shared_dir
# ls -al
drwxrwxrwx+ 2 qz   qz       4096 10月 12 08:43 shared_dir
```

- getfacl - 获取文件控制列表

```shell
qz@qz:/tmp$  getfacl shared_dir/
# file: shared_dir/
# owner: qz
# group: qz
user::rwx
user:qz:rwx
group::rwx
mask::rwx
other::rwx	
```



#### ch6 存储结构与磁盘划分

FHS - 文件系统层次标准



**并不是所有开发者都一定会遵守该标准,需要灵活应变.**

| 目录        | 放置文件内容                                                 |
| ----------- | ------------------------------------------------------------ |
| /boot       | 开机所需文件_内核,开机菜单(grub界面)以及所需配置文件         |
| /dev        | 以文件形式存放任何设备与接口                                 |
| /etc        | 配置文件, 例如: ssh, python, bash... 配置文件                |
| /home       | 用户家目录                                                   |
| /bin        | 存放单用户模式下仍然可以使用的命令                           |
| /lib        | 开机时用到的函数库以及 /bin 与 /sbin 下面的命令要调用的函数  |
| /sbin       | 开机过程中需要的命令                                         |
| /media      | 用于挂载设备文件的目录                                       |
| /opt        | 放置第三方的软件, 例如: ROS, google, jvm                     |
| /root       | 系统管理员的家目录                                           |
| /srv        | 一些网络服务的数据文件目录                                   |
| /tmp        | 任何人均可使用的"共享"临时目录                               |
| **/proc**   | **虚拟文件系统, 例如:系统内核, 进程, 外部设备以及网络状态等** |
| /usr/local  | 用户自行安装的软件, 属于 /usr 目录                           |
| /usr/sbin   | linux 系统开机时会用到的软件,命令,脚本                       |
| /esr/share  | 帮助与说明文档,也可放置共享文件                              |
| /var        | 存放经常变化的文件,如:日志文件                               |
| /lost+found | 当文件系统发生错误时,将一些丢失的文件片段存放在这里          |

- udev 设备管理器

udev 设备管理器会自动把硬件设备管理起来, 

udev 设备管理器的服务会一直以守护进程 (/lib/systemd/systemd-udevd) 的形式存在来监听内核发出的信号来管理 `/dev`目录下的设备文件.Linux 系统中常见的硬件设备的文件名称如表所示:

| 硬件设备                                                     | 文件名称             |
| ------------------------------------------------------------ | -------------------- |
| IDE 设备                                                     | /dev/hd[a-d]         |
| SCSI/STAT/U盘/硬盘设备<br />a~p 代表 16块不同的硬盘,默认从 a 开始分配<br />硬盘的分区编号也有讲究:<br />主分区或者扩展分区从 1 开始, 到 4 结束<br />逻辑分区从编号 5 开始 | /dev/sd[a-p]         |
|                                                              | /dev/fd[0-1]         |
|                                                              | /dev/lp[0-15]        |
|                                                              | /dev/cdrom           |
|                                                              | /dev/mouse           |
|                                                              | /dev/st0 或 /dev/ht0 |
| TODO: 这都是什么设备                                         | /dev/vcs             |
|                                                              | /dev/tty[0-100]      |
|                                                              | /dev/ttyS[0-100]     |

硬盘设备:

```shell
qz@qz:/dev$  ls -l | grep sd
brw-rw----  1 root disk      8,     0 10月 14  2025 sda
brw-rw----  1 root disk      8,     1 10月 14  2025 sda1
brw-rw----  1 root disk      8,     2 10月 14  2025 sda2
brw-rw----  1 root disk      8,     5 10月 14  2025 sda5
brw-rw----  1 root disk      8,     6 10月 14  2025 sda6
```

- 硬盘设备为什么最多有 4 个主分区:

硬盘第一个删除(512 bytes)存储主引导记录(446 bytes) 和分区表信息.

每个分区表大小:16 bytes, 因此剩余空间: 512-446=66bytes, 最多存储 4 个主分区.

如果 4 个主分区无法满足需求, 则需要引入扩展分区, 主分区中的一个分区指向扩展分区(类似与指针), 扩展分区从 :`/dev/sda5` 开始.







#### 扩展:

TODO:

- awk 脚本

```shell
awk # 也可以用于 shell 字符串操作
```



- 还有一个命令,也是专門用于 命令结果判断的

