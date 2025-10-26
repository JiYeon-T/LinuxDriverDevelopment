****

# TODO:

写一个关闭笔记本 touchpad 的脚本,放到开机文件里/手动执行

- openssl

- tcpdump 抓包看下， FTP/TFTP 是明文传输, ssh 是加密传输





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
apt-key --help # apt-key - APT key management utility
apt-key add
apt-key exportall | more # display all key
apt-key adv --help # advanced operation
```

- 解决 apt-get update GPG 秘钥问题导致失败问题

```shell
#W: GPG error: http://packages.ros.org/ros/ubuntu xenial InRelease: The following signatures were invalid: KEYEXPIRED 1622248854
#W: The repository 'http://packages.ros.org/ros/ubuntu xenial InRelease' is not signed.
#N: Data from such a repository can't be authenticated and is therefore potentially dangerous to use.
#N: See apt-secure(8) manpage for repository creation and user configuration details.

sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80  --refresh-keys
sudo apt-get update
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

- man - manual

```shell
man man
#命令使用提示
Usage:
 mount [-lhV] # 默认参数
 mount -a [options] # [] - 可选参数
 mount [options] [--source] <source> | [--target] <directory> # <> - 必选参数
 mount [options] <source> <directory>
 mount <operation> <mountpoint> [<target>]
```



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
id -u # -u user
id -n
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

- ​       lsof - list open files

列出打开该文件的进程信息

```shell
lsof --help
lsof /dev/sda1
```

- scp — secure copy (remote file copy program)

TODO:

```shell

```

- strace - trace system calls and signals， 可以查看这条 shell 命令都执行了哪些系统调用

```shell
strace ls
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

- tee - tee - read from standard input and write to standard output and files

```shell
tee --help
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

/dev/null - 类似于回收站/垃圾桶

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

- db_load - Load data from standard input

// TODO:

明文信息不安全，因此需要使用 hash 算法将明文信息文件转换成数据库文件，并降低数据库文件的权限

```shell
apt-get install db-util

root@ubuntu:/srv#  db_
db_archive     db_deadlock    db_hotbackup   db_log_verify  db_recover     db_stat        db_verify
db_checkpoint  db_dump        db_load        db_printlog    db_replicate   db_upgrade 

db_load -T -t hash -f vuser.list vuser.db # 生成 hash 加密文件， vuser.db

qz@ubuntu:/tmp$  file vuser.db 
vuser.db: Berkeley DB (Hash, version 9, native byte-order)
 
```





---

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

| 硬件设备                                                     | 文件名称                                          |
| ------------------------------------------------------------ | ------------------------------------------------- |
| IDE 设备                                                     | /dev/hd[a-d]                                      |
| SCSI/STAT/U盘/硬盘设备<br />a~p 代表 16块不同的硬盘,默认从 a 开始分配<br />硬盘的分区编号也有讲究:<br />主分区或者扩展分区从 1 开始, 到 4 结束<br />逻辑分区从编号 5 开始 | /dev/sd[a-p]<br />sd - storage device，　存储设备 |
|                                                              | /dev/fd[0-1]                                      |
|                                                              | /dev/lp[0-15]                                     |
|                                                              | /dev/cdrom                                        |
|                                                              | /dev/mouse                                        |
|                                                              | /dev/st0 或 /dev/ht0                              |
| TODO: 这都是什么设备                                         | /dev/vcs                                          |
|                                                              | /dev/tty[0-100]                                   |
|                                                              | /dev/ttyS[0-100]                                  |

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

/dev/sda5 表示什么：

1. /dev 表示硬件设备所在的目录
2. ａ 表示系统中同类接口中第一个被识别到的设备
3. 5 表示这个设备是一个逻辑分区

- 查看系统当前所有分区,  /proc/partitions

```shell
qz@ubuntu:~/Desktop/Typora-linux-x64$  cat /proc/partitions 
major minor  #blocks  name

   8        0   41943040 sda
   8        1   33555456 sda1
   8        2          1 sda2
   8        5    8385536 sda5
   8       16   20971520 sdb
   8       32   20971520 sdc
   8       48   20971520 sdd
   8       64   20971520 sde
  11        0    1048575 sr0
   9      127    4190208 md127
```

- blkid - locate/print block device attributes 检查设备的 UUID 或分区类型

```shell
sudo blkid /dev/sdb
qz@ubuntu:~/Desktop/Typora-linux-x64$  sudo blkid /dev/sdc
/dev/sdc: UUID="3b03fdfb-4527-447e-01c1-715946fb860f" UUID_SUB="67b5eec2-07f3-759b-0eaa-b29234afb140" LABEL="ubuntu:0" TYPE="linux_raid_member" # 虽然其他方法查不到挂载信息，但是这里可以看到是 linux raid member

# fdisk
qz@ubuntu:~/Desktop/Typora-linux-x64$  sudo fdisk -l /dev/sdc
Disk /dev/sdc: 20 GiB, 21474836480 bytes, 41943040 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
```



###### 挂载硬件设备

分区 -》 格式化 -》 挂载

**挂载——当用户需要使用硬盘/分区中的数据时，需要先将其与一个已存在的目录建立关联，这个关联的动作就是挂载**

- mount

```shell
mount --help
#  -a, --all               mount all filesystems mentioned in fstab， 自动检查 /etc/fstab 中有无疏漏被挂载的设备文件，如果有则自动进行挂载
# -t, --types <list>      limit the set of filesystem types， 一般不需要使用 -t 参数来指定文件系统类型，linux 系统会自行判断

mount /dev/sdb2 /backup # 将 /dev/sdb2 挂载到 /backup 目录，将硬件设备与目录进行关联
umount /dev/sdb2 # 取消挂载，意味着不再使用硬件资源

mount -lhV # 查看所有的文件系统挂载信息
mount -lhV | grep /dev/sdb # 查看硬盘 /dev/sdb 是否被挂载，如果已经挂载情况下可以尝试 umount /挂载目录取消挂载，再尝试能否使用
```

- umount - 取消挂载

TODO:

 为什么取消挂载后，挂载的目录仍然可以访问？？

```shell
umount /dev/sdb1
```

- /etc/fstab - 如果想让硬件设备和目录永久的自动关联，就必须把挂在信息按照指定的格式写入到 /etc/fstab 中

"设备文件  挂载目录  格式类型  权限选项  是否备份  是否自检"

字段说明：

| 字段     | 意义                                                         |
| -------- | ------------------------------------------------------------ |
| 设备文件 | 一般为设备的路径+设备名称，也可以写唯一的识别码(UUID)        |
| 挂载目录 | 指定要挂载到的目录，需在挂载前创建好                         |
| 格式类型 | 指定文件系统的格式，如：Ext3, Ext4, XFS, SWAP, iso9660(此为光盘设备) |
| 权限选项 | 若设置为 defaults, 则默认权限为 rw, suid, dev, exec, auto, nouser, async |
| 是否备份 | 1-开机后使用 dnmp 进行磁盘备份                               |
| 是否自检 | 1-开机后进行磁盘自检                                         |

例如：

```shell
cat /etc/fstab
# /etc/fstab: static file system information.
#
# Use 'blkid' to print the universally unique identifier for a
# device; this may be used with UUID= as a more robust way to name devices
# that works even if disks are added and removed. See fstab(5).
#
# <file system> <mount point>   <type>  <options>       <dump>  <pass>
# / was on /dev/sda1 during installation
UUID=790b89af-da4a-4a0a-8770-b7ba723d8732 /               ext4    errors=remount-ro 0       1
# swap was on /dev/sda5 during installation
UUID=835a0819-4fb4-46b0-9258-7576b18d16bc none            swap    sw              0       0
/dev/fd0        /media/floppy0  auto    rw,user,noauto,exec,utf8 0       0
```

- 虚拟机添加硬盘设备

使用虚拟机添加 “新硬盘” 后还是按照 “分区 -》 格式化 -》 挂载” 的操作使用磁盘

- fdisk - 磁盘/硬盘分区管理（添加，删除，转换等操作）

```shell
fdisk /dev/sdb
# fdisk 是一个交互式的命令, 交互命令作用：
m - 查看全部可用的参数
n - 添加新的分区
d - 删除某个分区的信息
l - 列出所有可用的分区类型
t - 改变某个分区的类型
p - 查看分区信息
w - 保存并退出
q - 不保存直接退出
```

- file - 查看文件类型

```shell
file --help
file /dev/sdb1
#qz@ubuntu:/dev$  file /dev/sdb1 
#/dev/sdb1: block special (8/17)
```

- partprobe - inform the OS of partition table changes

如果 fdisks 命令创建后分区没有生效(没有 /dev/sdb1 文件)的情况下，可以使用这个命令两次。如果仍然不行，重启电脑

```shell
partprobe
```

- mkfd 格式化文件系统

```shell
# 打印支持的文件系统类型
qz@ubuntu:/dev$  mkfs
mkfs          mkfs.cramfs   mkfs.ext3     mkfs.ext4dev  mkfs.minix    mkfs.ntfs     
mkfs.bfs      mkfs.ext2     mkfs.ext4     mkfs.fat      mkfs.msdos    mkfs.vfat

mkfs.xfs /dev/sdb1 # 格式化 XFS 文件系统
# sudo apt install xfsprogs # 安装格式化 XFS 文件系统需要的工具

mkdir /newFS
mount /dev/sdb1 /newFS # 挂载硬盘到 /newFS 目录
```

- df -  (disk fiesystem) report file system disk space usage

```shell
df --help
df -h # 查看挂载状态和硬盘使用量信息

# TODO:
# 为什么本地测试后没有 /dev/sdb1 硬盘信息？ 但是挂载的文件目录是可以使用的
qz@ubuntu:/newFS$  df -h
Filesystem      Size  Used Avail Use% Mounted on
udev            3.9G     0  3.9G   0% /dev
tmpfs           796M  9.4M  787M   2% /run
/dev/sda1        32G   15G   16G  50% /      # 根目录挂载的是 /dev/sda1, 查看 /etc/fstab 文件，开机后自动挂载的
tmpfs           3.9G   19M  3.9G   1% /dev/shm
tmpfs           5.0M  4.0K  5.0M   1% /run/lock
tmpfs           3.9G     0  3.9G   0% /sys/fs/cgroup
tmpfs           796M   76K  796M   1% /run/user/1000
```

- du - estimate file space usage

```shell
du -sh /newFS
# -h, --human-readable
#  -s, --summarize
```



**如果要设置开机后自动挂载，就要将 /dev/sdb1 的挂载信息添加到 /etc/fstab 中**

```shell
# /etc/fstab: static file system information.
#
# Use 'blkid' to print the universally unique identifier for a
# device; this may be used with UUID= as a more robust way to name devices
# that works even if disks are added and removed. See fstab(5).
#
# <file system> <mount point>   <type>  <options>       <dump>  <pass>
# / was on /dev/sda1 during installation
UUID=790b89af-da4a-4a0a-8770-b7ba723d8732 /               ext4    errors=remount-ro 0       1
# swap was on /dev/sda5 during installation
UUID=835a0819-4fb4-46b0-9258-7576b18d16bc none            swap    sw              0       0
/dev/fd0        /media/floppy0  auto    rw,user,noauto,exec,utf8 0       0
# 增加新挂载的硬盘文件
/def/sfb1       /newFS          xfs     defaults        0       0 
```

- 添加交换分区

交换分区 ： 将硬盘中划分一定的空间，用于保存内存中的数据，以便有限的内存可以给更活跃的程序使用。本质是为了解决物理内存不够的问题。

生产中：交换分区一般为物理内存大小的 1.5-2 倍

```shell
# 虚拟机默认没有交换分区
qz@ubuntu:/newFS$  free 
              total        used        free      shared  buff/cache   available
Mem:        8144672     1427196     5503424       48872     1214052     6342076
Swap:             0           0           0
```

这里进行创建交换分区实验：

分区 -》 格式化 -》 挂载

```shell
#1. 创建分区
fdisk /dev/sdb
# 创建 SWAP 分区
```

- mkswap - 创建交换分区, 专用的格式化命令

```shell
qz@ubuntu:/newFS$  free -m
              total        used        free      shared  buff/cache   available
Mem:           7953        1409        5358          44        1185        6180
Swap:             0           0           0

mkswap /dev/sdb2
swapon /dev/sdb2 # 将 swap 分区正式挂载到系统中

qz@ubuntu:/newFS$  free -h
              total        used        free      shared  buff/cache   available
Mem:           7.8G        1.4G        5.2G         44M        1.2G        6.0G
Swap:           17G          0B         17G
```

为了能够让交换分区重启后仍然生效，修改 /etc/fstab

```shell
#交换分区
/dev/sdb2       swap            swap    defaults        0       0
```

###### 磁盘容量配额 

Linux 是一个多用户，多任务的操作系统， 对某一个用户所能使用的最大磁盘容量进行限制

- quota - display disk usage and limits

```shell
sudo apt-get install quota
```

- xfs_quota - 针对 xfs 文件系统来管理磁盘容量配额服务而设计的命令

```shell
man xfs_quota
xfs_quota --help
# -c 以参数的形式设置要执行的命令
# -x 专家模式，让运维人员能够对 quota 服务进行更多复杂的配置，
# 例如：使用 xfs_quota 设置用户 tom 对 /boot 目录的磁盘容量配额
# 具体限制：硬盘使用量的软限制 3MB 以及硬限制 6MB
# 创建文件数量的软限制 3 个以及硬限制 6 个
xfs_quota -x -c 'limit bsoft=3m bhard=6m isoft=3 ihard=6 tom' /boot
# TODO:
# xfs_quota: cannot setup path for mount /newFS/: No such device or address
xfs_quota -x -c report /boot
```

切换到用户 tom 后测试磁盘容量配额是否生效

```shell
su - tom
dd if=/dev/zero of=/boot/tom bs=5M count=1 # 超过软限制，但仍然能用
dd if=/dev/zero of=/boot/tom bs=8M count=1 # 超过硬限制，报错，无法使用
```

**TODO:**

- edquota - 用户编辑用户的 quota 配额限制

edquota 会调用 vi/vim 编辑器来让 root 管理员修改要限制的细节

```shell
-u 表示针对哪个用户进行限制
-g 表示针对哪个用户组进行限制
# 修改 tom 的硬限制， 5MB->8MB
edquota -u tom
Filesystem  blocks  soft hard inodes  soft   hard
/dev/sda	6144 	3072 8192	1 		3		6

su - tom
# 分别测试写入 8M 以及 10M 文件， TODO:
```

###### 软/硬链接方式

硬链接：可以将它理解为一个指向原始文件 inode 的指针，系统不为它分配单独的 inode 和文件。所以硬链接与原始文件实际上是同一个文件，只是名字不同。我们每添加一个硬链接，该节点的 inode 连接数就为 +1, 而且只有当 inode 的连接数为 0 时，该文件才会真正删除。换言之，即使删除原文件，链接文件仍然存在且可以正常访问。由于技术的局限性，硬链接不能跨分区对目录文件进行链接。

软链接(符号链接)：仅仅包含所链接文件的路径名，因此可以跨文件系统进行链接。原始文件删除后，链接文件也失效，与 windows 中的快捷方式类似。

- ln - make links between files

```shell
# -s 创建符号链接(软链接)，如果不带 -s 参数，则默认创建硬链接
# -f 强制创建文件或目录的链接
# -i 覆盖前先询问
# -v 显示创建链接的过程

# 创建后 ll 可以看到链接个数增加了，删除 raw.txt 后 hard_link.txt 仍然啊可以访问， 链接数 -1
ln -P raw.txt hard_link.txt # 参数缺省值就是 -P，所以也可以不写
# 创建软链接，删除 raw.txt 后， soft_link.txt 无法访问
Ln -s raw.txt soft_link.txt # 软链接

qz@ubuntu:/newFS$  ll
total 12
drwxr-xrwx  2 root root   65 10月 16 08:05 ./
drwxr-xr-x 24 root root 4096 10月 15 17:42 ../
-rw-rw-r--  2 qz   qz     20 10月 16 08:01 hard2.txt
-rw-rw-r--  2 qz   qz     20 10月 16 08:01 hard_link.txt
lrwxrwxrwx  1 qz   qz      7 10月 16 08:01 soft_link.txt -> raw.txt
```



#### ch7. 使用 RAID 与 LVM 磁盘阵列技术

###### RAID

Redundant Array of Independent Disks. 独立冗余磁盘阵列

- RAID0

- RAID1

- RAID5
- RAID10



- mdadm - manage MD devices aka Linux Software RAID, 磁盘阵列管理

服务配置文件 `/etc/mdadm/mdadm.conf`

```shell
sudo apt-get install mdadm
# -a 检测设备名称， -a yes 表示自动创建设备文件
# -n 指定设备数量， -n 4 代表使用 4 快硬盘来部署 RAID 磁盘阵列， 最后加上 4 块硬盘设备的名称
# -l 制定 RAID 级别，-l 10 参数掉表 RAID10 方案
# -C 创建一个 RAID 阵列卡， 参数为设备名称(阵列卡名称)，例如:/dev/md0
# -v 显示过程
# -f 模拟设备损坏
# -r 删除设备
# -Q 查看摘要信息
# -D 查看详细信息
# -S 停止 RAID 磁盘阵列
# -x 备份盘数量， -x 1 一个备份盘
```

例如：

```shell
# step1:创建 /dev/md0 RAID 分区
mdadm -Cv /dev/md0 -a yes -n 4 -l 10 /dev/sdb1 /dev/sdc /dev/sdd /dev/sde 
# step2:格式化文件系统：将制作好的 RAID 磁盘阵列格式化为 ext4 格式
mkfs.ext4 /dev/md0
# step3:挂载文件系统
mkdir /RAID
mount /dev/md0 /RAID
# 设置开机自动挂载 /etc/fstab
# RAID
/dev/md0	/RAID 	ext4	defaults 	0	0

df -h
qz@ubuntu:/$  df -h
Filesystem      Size  Used Avail Use% Mounted on
udev            3.9G     0  3.9G   0% /dev
tmpfs           796M  9.4M  786M   2% /run
/dev/sda1        32G   15G   16G  50% /
tmpfs           3.9G   22M  3.9G   1% /dev/shm
tmpfs           5.0M  4.0K  5.0M   1% /run/lock
tmpfs           3.9G     0  3.9G   0% /sys/fs/cgroup
tmpfs           796M   76K  796M   1% /run/user/1000
/dev/md0        3.9G  8.0M  3.7G   1% /RAID

# 查看挂载信息
mdadm -D /dev/md0
qz@ubuntu:/$  sudo mdadm -D /dev/md0 
/dev/md0:
        Version : 1.2
  Creation Time : Thu Oct 16 17:24:12 2025
     Raid Level : raid10
     Array Size : 4190208 (4.00 GiB 4.29 GB)
  Used Dev Size : 2095104 (2046.34 MiB 2145.39 MB)
   Raid Devices : 4
  Total Devices : 4
    Persistence : Superblock is persistent

    Update Time : Thu Oct 16 20:37:33 2025
          State : clean 
 Active Devices : 4
Working Devices : 4
 Failed Devices : 0
  Spare Devices : 0

         Layout : near=2
     Chunk Size : 512K

           Name : ubuntu:0  (local to host ubuntu)
           UUID : 3b03fdfb:4527447e:01c17159:46fb860f
         Events : 17

    Number   Major   Minor   RaidDevice State
       0       8       17        0      active sync set-A   /dev/sdb1
       1       8       32        1      active sync set-B   /dev/sdc
       2       8       48        2      active sync set-A   /dev/sdd
       3       8       64        3      active sync set-B   /dev/sde
```

- 损坏磁盘阵列及修复

```shell
mdadm /dev/md0 -f  /dev/sdb1 # 模拟磁盘损坏
# 当购买了新的硬盘替换后重新挂载即可
umount /RAID
mdadm /dev/md0 -a /dev/sdb
# 查看信息
mdadm -D /dev/md0
```

- 磁盘阵列 + 备份盘

为了避免 RAID1 两块磁盘同时出现问题的情况，使用 RAID5

```shell
# 3个磁盘 + 一个备份盘
mdadm -Cv /dev/md0 -n 3 -l 5 -x 1 /dev/sdb /dev/sdc /dev/sdd /dev/sde
# 格式化文件系统
mkfs.ext4 /dev/md0
# 设置开机自动挂载
echo "/dev/md0 /RAID ext4 defaults 0 0" >> /etc/fstab
mkdir /RAID
mount -a # 从 /etc/fstab 挂载所有磁盘
```

设置 sdb 损坏，会发现备份盘自动顶替上岗

```shell
mdadm /dev/md0 -f /dev/sdb # 设置 sdb 损坏
mdadm -D /dev/md0
```

TODO:

RAID 如何卸载？ 使用过 RAID 的磁盘无法进行其他操作。提示：“can't exclusively open /dev/xxx, mounted filesystem?”



###### LVM

Logical Volume Manager, 逻辑卷管理器

部署好 RAID 磁盘阵列后再修改硬盘分区大小时，就不好修改了——LVM 可以对硬盘资源进行动态调整

创建初衷是为了解决“硬盘设备创建分区后不易修改分区大小的缺陷”

**逻辑卷：在硬盘分区和文件系统之间添加了一个逻辑层，它提供了一个抽象的卷组，可以把多块硬盘进行卷组合并。这样，用户不用关心物理硬盘设备底层的架构和布局，就可以实现对硬盘分区的动态调整**

主要是为了满足动态扩容 / 缩容的需求。

- 扩容
- 精简缩容

部署 LVM 时，需要逐个部署物理卷，卷组和逻辑卷。

| 功能/命令 | 物理卷管理(Phyical Volume) | 卷组管理(Volume Group) | 逻辑卷管理(Logical Volume) |
| --------- | -------------------------- | ---------------------- | -------------------------- |
| 扫描      | pvscan                     | vgscan                 | lvscan                     |
| 建立      | pvcreate                   | vgcreate               | lvcreate                   |
| 显示      | pvdisplay                  | vgdisplay              | lvdisplay                  |
| 删除      | pvremove                   | vgremove               | lvremove                   |
| 扩展      |                            | vgextend               | lvextend                   |
| 缩小      |                            | vgreduce               | lvreduce                   |

例如：对两块硬盘进行卷组合并，将合并后的卷组分割出一个 150MB 的逻辑卷设备，最后进行格式化以及挂载使用

```shell
# step1:新添加的两块磁盘创建物理卷，支持 LVM
pvcreate /dev/sdb /dev/sdc
root@ubuntu:~#  pvcreate /dev/sdb /dev/sdc # 创建 LVM 物理卷
  Physical volume "/dev/sdb" successfully created
  Physical volume "/dev/sdc" successfully created
pvdisplay # 查看物理卷信息
root@ubuntu:~#  blkid /dev/sdb  # 获取磁盘设备信息以及属性
/dev/sdb: UUID="uRPVuc-cGfl-Ej4k-PDyp-IFyN-DhG6-ebNt0U" TYPE="LVM2_member"
# step2: 添加到 storage 卷组中
vgcreate storage /dev/sdb /dev/sdc
vgdisplay # 查看卷组信息
# step3: 切割一个 150MB 的逻辑卷设备
lvcreate -n vo -l 37 storage # -n|--name LogicalVolumeName
lvdisplay
# step4: 将生成的逻辑卷格式化
mkfs.ext4 /dev/storage/vo
# step5: 挂载文件系统
mkdir /linuxprobe
mount /dev/storage/vo /linuxprobe # 使用逻辑卷挂载
df -h # 查看挂载信息，

qz@ubuntu:/linuxprobe$  df -h
Filesystem              Size  Used Avail Use% Mounted on
udev                    3.9G     0  3.9G   0% /dev
tmpfs                   796M  9.4M  787M   2% /run
/dev/sda1                32G   15G   16G  50% /
tmpfs                   3.9G   19M  3.9G   1% /dev/shm
tmpfs                   5.0M  4.0K  5.0M   1% /run/lock
tmpfs                   3.9G     0  3.9G   0% /sys/fs/cgroup
tmpfs                   796M   60K  796M   1% /run/user/1000
/dev/mapper/storage-vo  140M  1.6M  128M   2% /linuxprobe
```

切割逻辑卷有两种计量单位：

1. 以容量为单位， 参数 -L, 例如： -L 150M 表示生成一个 150MB 的逻辑卷
2. 以基本单位的个数为单位，每个基本单元大小 4MB，参数 -l，例如：-l 37 表示生成一个 37*4=148MB 的逻辑卷

- 扩容逻辑卷

**在执行扩容/缩容操作前，必须先取消挂载文件系统**

**扩容操作，先扩容，然后检查文件完整性; 而缩容为了数据安全，需要先检查文件完整性，然后进行缩容操作。**

```shell
umount /linuxprobe # 卸载设备和挂载点的关联
# step1: 逻辑卷扩容至 290MB
lvextend -L 290M /dev/storage/vo
lvdisplay # 查看逻辑卷信息
# step2: 检查硬盘完整性，并重置硬盘容量
e2fsck -f /dev/storage/to
resize2fs /dev/storage/to
# step3:重新挂在硬盘设备并查看挂载状态
mount /dev/storage/vo /linuxprobe
df -h

root@ubuntu:~#  df -h
Filesystem              Size  Used Avail Use% Mounted on
udev                    3.9G     0  3.9G   0% /dev
tmpfs                   796M  9.4M  787M   2% /run
/dev/sda1                32G   15G   16G  50% /
tmpfs                   3.9G   19M  3.9G   1% /dev/shm
tmpfs                   5.0M  4.0K  5.0M   1% /run/lock
tmpfs                   3.9G     0  3.9G   0% /sys/fs/cgroup
tmpfs                   796M   64K  796M   1% /run/user/1000
/dev/mapper/storage-vo  279M  2.1M  259M   1% /linuxprobe
```

- 缩小逻辑卷

```shell
umount /linuxprobe
# step1:检查文件系统的完整性并缩容
e2fsck -f /dev/storage/vo
# step2: 把逻辑卷 /dev/storage/vo 缩容到 120MB
resize2fs /dev/storage/vo 120M
lvreduce -L 120M /dev/storage/vo
lvdispaly
# step3:重新挂载文件系统并查看
mount /dev/storage/vo /linuxprobe
df -h

root@ubuntu:~# df -h
Filesystem              Size  Used Avail Use% Mounted on
udev                    3.9G     0  3.9G   0% /dev
tmpfs                   796M  9.4M  787M   2% /run
/dev/sda1                32G   15G   16G  50% /
tmpfs                   3.9G   19M  3.9G   1% /dev/shm
tmpfs                   5.0M  4.0K  5.0M   1% /run/lock
tmpfs                   3.9G     0  3.9G   0% /sys/fs/cgroup
tmpfs                   796M   68K  796M   1% /run/user/1000
/dev/mapper/storage-vo  113M  1.6M  103M   2% /linuxprobe
```



- e2fsck -  check a Linux ext2/ext3/ext4 file system

```shell
# -f     Force checking even if the file system seems clean.
# 
```

-  resize2fs - ext2/ext3/ext4 file system resizer

```shell
# The resize2fs program will resize ext2, ext3, or ext4 file systems.  It can be used to enlarge or shrink an
# unmounted file system located on device.  If the filesystem is mounted, it can be used to expand  the  size
# of  the  mounted  filesystem,  assuming  the kernel and the file system supports on-line resizing. 
```

- 逻辑卷快照 - 类似与虚拟机还原到某个时间点的功能

如果日后发现数据被该错了，就可以利用之前的快照卷进行还原，LVM 的快照卷有两个特点：

1. 快照卷的容量必须等于逻辑卷的容量
2. 快照卷仅一次有效，一旦执行还原操作后则会自动删除

```shell
vgdisplay # 查看卷组信息
# step1: 生成快照卷
lvcreate -L 120M -s -n SNAP /dev/storage/vo # -s 生成快照卷; -L 指定切割大小; 
root@ubuntu:~#   lvcreate -L 120M -s -n SNAP /dev/storage/vo
  Logical volume "SNAP" created.
# 在逻辑卷中创建一些垃圾文件
dd if=/dev/zero of=/linuxprobe/files count=1 bs=100M
lvdispaly 
  --- Logical volume ---
  LV Path                /dev/storage/SNAP
  LV Name                SNAP
  VG Name                storage
  LV UUID                UxYDHM-SZ7Y-oCR8-bmMk-TmsH-CoKm-7haLuy
  LV Write Access        read/write
  LV Creation host, time ubuntu, 2025-10-17 11:46:24 +0800
  LV snapshot status     active destination for vo
  LV Status              available
  # open                 0
  LV Size                120.00 MiB
  Current LE             30
  COW-table size         120.00 MiB
  COW-table LE           30
  Allocated to snapshot  83.71% # 快照卷占的比例上升了
  Snapshot chunk size    4.00 KiB
  Segments               1
  Allocation             inherit
  Read ahead sectors     auto
  - currently set to     256
  Block device           253:3
#step3: 为了验证快照卷的效果，需要对逻辑卷进行快照还原操作,快照卷会被自动删除（并且前面创建的 100MB垃圾也被删除了）
umount /linuxprobe # 还原前需要先取消挂载
lvconvert --merge /dev/storage/SNAP
#step4: 重新挂载
mount  /dev/storage/vo /linuxprobe
ls -l /linuxprobe
```

- 删除逻辑卷

提前备份好重要的数据信息，依次删除逻辑卷 -> 卷组 -> 物理卷，这个顺序不可以颠倒

```shell
# 首先必须取消挂在
umount /linuxprobe
# step1:删除逻辑卷
lvremove /dev/storage/vo
lvdisplay
# step2:删除 storage 卷组
vgremove storage
vgdispaly
# step3:删除物理卷设备
pvremove /dev/sdb /dev/sdc
pvdispaly
```



#### ch8 iptables 与 firewalld 防火墙

准确说 iptables 与 firewalld 都不是防火墙，他们都是用来定义防火墙管理策略的工具。

iptables 会将配置好的防火墙策略交由内核层面的 netfilter 网络过滤器来处理

firewalld 则会把配置好的防火墙策略交由内核层面的 nftables 包过滤框架来处理;

**Linux 上有多种防火墙管理工具，具体用什么工具按需即可。**



###### iptables

- 策略与规则链

按照拦截位置不同进行分类：

1. 在进行路由选择前处理包，PREROUTING
2. 处理流入的数据包：INPUT
3. 处理流出的数据包：OUTPUT
4. 处理转发的数据包：FORWARD
5. 在进行路由选择后处理包：POSTROUTING

- 处理策略：

ACCEPT：允许流量

REJECT：拒绝流量，但回复

DROP：拒绝流量，丢弃数据包，不回复对端任何消息

LOG: 记录日志信息

**防火墙：策略规则是按照从上到下的顺序匹配的，因此一定要把允许动作放到拒绝动作前面，否则所有的流量都会被拒绝掉，从而导致任何主机都无法访问我们的服务**

- iptables/ip6tables — administration tool for IPv4/IPv6 packet filtering and NAT

iptables 可以根据源地址，目的地址，传输协议，服务类型等信息进行匹配

```shell
-P 设置默认策略
-F 清空规则链
-L 查看规则链
-A 在规则链末尾加入新规则
-I num # 在规则链的头部加入新规则
-D 删除某一条规则
-s 匹配来源地址 IP/MASK,加 ! 表示逻辑非，除了这个 IP 外的所有 IP
-d 匹配目标地址
-i 网卡名称，# 匹配从这块网卡流入的数据
-o 网卡名称 # 匹配从这个网卡流出的数据
-p 匹配协议，如：TCP，UDP, ICMP
--dport num # 匹配目标端口号
--sport num # 匹配源端口号

iptables -L # list
iptables -F # clear
iptables -P INPUT DROP # 设置输入默认策略为 DROP，当把默认策略修改为 堵时，就需要设置允许的条件了

root@ubuntu:~#   iptables -L
Chain INPUT (policy DROP) # policy DROP
target     prot opt source               destination      

iptables -I INPUT -p icmp -j ACCEPT # 向 ICMP 链中添加允许 ICMP 流入的策略

iptables -D INPUT 1 # 删除规则链
iptables -P INPUT ACCEPT # 设置默认策略为允许
iptables -I INPUT -s 192.168.10.0/24 -p tcp --dport 22 -j ACCEPT # 将 INPUT 规则设置为仅允许指定网段访问本机的 22 端口，拒绝来自其他所有主机的流量
iptables -A INPUT -p tcp --dport 22 -j REJECT # 拒绝来自其他所有主机的流量
```

ssh 测试：

```shell
ssh 192.168.10.10 # 服务器 ip, 输入对方主机 root 管理员密码后既可以登录
```

添加其他规则：

```shell
iptables -I INPUT -p tcp --dport 12345 -j REJECT # 向 INPUT 规则链中添加拒绝所有人访问 12345 端口的策略
iptables -I INPUT -p udp --dport 12345 -j REJECT
iptables -I INPUT -p tcp -s 192.168.10.5 --dport 80 -j REJECT # INPUT 中拒绝 192.168.10.5 访问 web 服务
iptables -A INPUT -p tcp --dport 1000:1024 -j REJECT # 拒绝所有主机访问本机 1000-1024 端口
```

**iptables 命令配置的防火墙规则默认会在系统下一次重启时失效，如果想让配置的防火墙规则永久生效，还要执行保存命令**

```shell
service iptables save # NOTE:报错 unrecognized service
# 配置文件保存目录:ubuntu/Debian /etc/iptables.rules; Redhat: /etc/sysconfig/iptables
# sudo apt-get install iptables-persistent
iptables-save # 仅仅是打印到 stdout ???
# 保存：
sudo iptables-save > /etc/iptables/rules.v4
sudo ip6tables-save > /etc/iptables/rules.v6 # ipv6
# 从配置文件恢复防火墙
iptables-restore < /etc/iptables/rules.v4
```



###### firewalld

RHEL7 系统中还集成了其他防火墙管理工具，firewalld 服务就是默认的防火墙管理工具，他支持 CLI(命令行界面) 和 GUI (图形用户界面) 两种方式。

**firewalld 定义了一系列防火墙策略，通过区域进行区分。切换上网环境时，避免用户进行频繁操作，只需要切换区域即可。**

区域名称以及策略规则：

| 区域     | 默认策略规则                                                 |
| -------- | ------------------------------------------------------------ |
| trusted  | 允许所有的数据包                                             |
| home     | 拒绝流入的流量，除非与流出流量有关<br />而如果流量与 ssh, mdns, ipp-client, amba-client, dhcpv6-client 服务有关，则允许流量 |
| internal | 等同于 home 区域                                             |
| work     | 拒绝流入的流量，除非与流出流量有关<br />而如果流量与 ssh, ipp-client, amba-client, dhcpv6-client 服务有关，则允许流量 |
| public   | 拒绝流入的流量，除非与流出流量有关<br />而如果流量与 ssh, dhcpv6-client, ipp-client 有关，则允许流量 |
| external | 拒绝流入的流量，除非与流出流量有关<br />而如果流量与 ssh 有关，则允许流量 |
| dmz      | 拒绝流入的流量，除非与流出流量有关<br />而如果流量与 ssh, dhcpv6-client, ipp-client 有关，则允许流量 |
| block    | 拒绝流入的流量，除非与流出流量有关<br />                     |
| drop     | 拒绝流入的流量，除非与流出流量有关                           |

- firewall-cmd - firewalld 的防火墙配置管理工具的 CLI(命令行界面)版本

```shell
sudo apt-get install firewalld
# 参数说明：
--get-default-zone # 查询默认的区域名称
--set-default-zone=<区域名称> # 设置默认的区域，使其永久生效
--get-zones # 显示可用的区域
--get-services # 显示预定义的服务
--get-active-zones # 显示当前正在使用的区域与网卡名名称
--add-source= # 将源自此 IP 或子网的流量导向指定的区域
--remove-source= # 不再将源自此 IP 的流量导向指定的区域
--add-interface=<网卡名称> # 将源自该网卡的流量都导向某个指定的区域
--change-interface=<网卡名称> # 将某个网卡与区域进行关联
--list-all # 显示当前区域的网卡配置参数，资源，端口以及服务等信息
--list-all-zones # 显示所有区域的网卡配置参数，资源，端口以及服务等信息
--add-service=<服务名> # 设置默认区域允许该服务的流量
--remove-service=<服务名> # 设置默认区域不再允许该服务的流量
--add-port=<端口/协议号> # 设置默认区域允许该端口的流量
--remove-port=<端口/协议号> # 设置默认区域不再允许该端口的流量
--reload # 重新加载，即让永久生效的规则立即生效
--panic-on # 开启紧急状况模式
--panic-off # 关机紧急状况模式
```

**与 linux 系统中其他的防火墙策略配置工具一样，使用 firewalld 配置的防火墙策略默认为 runtime 模式，即当前生效模式，重启后会失效。**

如果需要立即生效，则需要加上 `--permanent` 参数，使用永久模式（只有在系统重启之后才会自动生效，如果需要立即生效，需要`firewall-cmd --reload` 重新加载一下）。

```shell
firewall-cmd --get-default-zone # 获取默认区域
firewall-cmd --get-zone-of-interface=ens33 # 获取网卡在 firewalld 服务中的区域
firewall-cmd --permanent --zone=external --change-interface=ens33 # 修改 firewalld 服务中网卡的默认区域为 external, --permanent 重启后生效(永久生效模式)
firewall-cmd --get-zone-of-interface=ens33 # public
firewall-cmd --permanent --get-zone-of-interface=ens33 # external
# 启动/关闭防火墙的紧急模式，阻断一切网络链接，（远程控制时慎用）
firewall-cmd --panic-on
firewall-cmd --panic-off
# 查询 public 区域是否允许 SSH 和 HTTPS 的流量
firewall-cmd --zone=public --query-service=ssh # yes
firewall-cmd --zone=public --query-service=https # no
# 把 firewalld 服务中请求 HTTPS 协议的流量设置为永久允许，并立即生效
firewall-cmd --zone=public --add-service=https # 立即生效模式
firewall-cmd --permanent --zone=public --add-service=https # 永久模式
firewall-cmd --realod # 立即生效
# 把 firewalld 服务中请求 HTTPS 协议的流量设置为永久允许，并立即生效
firewall-cmd --permanent --zone=public --remove-service=http
firewall-cmd --reload
# 把在 firewalld 服务中访问 8080 和 8081 端口的流量策略设置为允许，
firewall-cmd --zone=public --add-port=8080-8081/tcp
firewall-cmd --zone=public --list-ports
# 把访问本机 888 端口的流量转发到 22 端口，并长期有效
firewall-cmd --permanent --zone=public --add-forward-port=port=888:proto=tcp:toport=22:toaddr=192.168.10.10
firewall-cmd --reload
# firewalld 中的富规则表示更细致，拒绝 182.168.10.0/24 网段的所有用户访问 SSH 服务（22端口）
firewall-cmd --permanent --zone=public --add-rich-rule="rule family="ipv4" source address="192.168.10.0/24" service name="ssh" reject"
firewall-cmd --reload
```



- firewall-config - 图形管理工具

```shell
sudo apt-get install firewall-config

```

- NAT(Network Address Transformation)

一种为了解决 IP 地址匮乏而设计的技术，可以使得多个内网中的用户通过同一个 IP 访问 internet. 该技术使用十分广泛，比如：通过路由器访问外网，

**使用 firewalld 可以很方便的实现 NAT 功能，使用 Iptables 却不好实现。**



- firewalld 放行所有流量, TODO:

```shell
sudo firewall-cmd --permanent --zone=public --add-source=0.0.0.0/0 --add-masquerade
sudo firewall-cmd --permanent --zone=public --change-interface=eth0 --add-source=0.0.0.0/0 --add-masquerade
sudo firewall-cmd --permanent --zone=public --add-rich-rule='rule family="ipv4" source address="0.0.0.0/0" accept'
sudo firewall-cmd --reload
```

这些命令会设置firewalld允许所有IPv4流量。

用于测试，暂时关闭虚拟机上的防火墙

```shell
systemctl stop firewalld # 停止服务
systemctl disable firewalld # 开机不启动
```





- Q&A

1. 运行 apache httpd 服务程序后，虚拟机本机通过 `localhost:80` 可以打开默认主页 index.html，但是实体机(其他主机) 通过`172.20.10.2:80`无法打开该网页（可以 ping 通， 且主机 SSH 服务正常(对方可以通过 SSH 连接)），最终定位是防火墙问题，firewalld 仅打开了 https 的流量，没有打开 http. 解决：

```shell
firewall-cmd --permanent --add-service=http # 打开 http 的流量
firewall-cmd --reload
```







###### TCP wrappers

iptable & firewalld 都是基于 TCP/IP 协议的流量过滤工具，而 TCP Wrappers 则是能允许或禁止 Linux 提供服务的防火墙，从而在更好层面保护了 Linux 系统的安全运行。

匹配规则：`/etc/hosts.allow` 然后 `/etc/hosts.deny` 如果都没有匹配到，则默认放行

- 参数说明

| 客户端类型                          | 例                         | 满足事例的客户端列表                     |
| ----------------------------------- | -------------------------- | ---------------------------------------- |
| 单一主机                            | 192.168.10.10              | IP 地址为 192.168.10.10 的主机           |
| 指定网段                            | 192.168.10.                | IP 地址为 192.168.10.0/24 网段的所有主机 |
| 指定网段<br />IP地址 + 子网掩码格式 | 192.168.10.0/255.255.255.0 | IP 地址为 192.168.10.0/24 网段的所有主机 |
| 指定 DNS 后缀                       | .linuxprobe.com            | 所有 DNS 后缀为 .linuxprobe.com 的主机   |
| 指定主机名称                        | www.linuxprobe.com         | 主机名称为 www.linuxprobe.com 的主机     |
| 指定所有客户端                      | ALL                        | 所有主机全部包括在内                     |



- 禁止访问 ssh 服务	

```shell
# /etc/hosts.deny
sshd:*
```

- 允许 192.168.10.0/24 网段的主机访问 ssh 服务

```shell
# /etc/hosts.allow
sshd:192.168.10.
```



#### ch9 使用 ssh 服务管理远程主机

###### 配置网络服务



- nmtui - nmtui - Text User Interface for controlling NetworkManager, CLI 界面

配置网络参数：

```shell
nmtui
```

编辑网络配置文件，设置网卡为开机自启动：

CentOS 为：/etc/sysconfig/network-scripts/ifcfg-eno1677736

Ubuntu 为：/etc/network/interfaces

```shell
ONBOOT=yes
# 修改配置文件后，重启相应的服务生效	
systemctl restart networking # CentOS:systemctl restart network
#或
systemctl restart network-manager.service
```



- NetworkManager - 动态管理网络配置的守护进程，能够让网络设备保持链接状态，可使用 `nmcli`命令进行管理

RHEL 和 CentOS 系统默认使用 NetworkManager 来提供网络服务。

TODO:

Networkmanager 也使用 Dbus 实现进程间通信。详细学习该模块



- nmcli - command-line tool for controlling NetworkManager

```shell
nmcli --help
nmcli -s connection # 查看网络状态
```

- 创建网络会话 - 类似与 firewalld 中的区域功能，不同的网络会话可以设置静态 IP 或者通过 DHCP 获取 IP 之类的

例如：通过 `nmcli` 创建会话，

```shell
# company 在公司使用静态 IP，
nmcli connection add con-name company ifname ens33 autoconnect no type ethernet ip4 192.168.175.10/24 gw4 192.168.0.1
# home:在家中使用 DHCP 分配 IP，
nmcli connection add con-name house type ethernet ifname ens33
nmcli connection show # 显示所有的会话类型
```

**nmcli 配置过的网络会话是永久生效的，当切换不同的场景后，仅需要切换网络会话即可**

```shell
root@ubuntu:/#  nmcli connection up company 
Connection successfully activated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/1)
root@ubuntu:/#  nmcli connection up house 
Connection successfully activated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/2)
root@ubuntu:/#   nmcli connection up 'Wired connection 1'
Connection successfully activated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/3)
# 删除 connection
nmcli connection delete house
```



###### 绑定两块网卡

TODO:

1. ubuntu 上没有 /etc/sysconfig/network-scripts 配置文件

https://www.cnblogs.com/zhoutuo/p/18609981

2. 搞了两块网卡后 nmcli 异常，会话一直处于 connecting 状态， 

```shell
qz@ubuntu:~$  nmcli -p general 
==============================================================
                    NetworkManager status
==============================================================
STATE       CONNECTIVITY  WIFI-HW  WIFI     WWAN-HW  WWAN    
--------------------------------------------------------------
connecting  none          enabled  enabled  enabled  enabled 
# nmcli connection up "Wifi "
```

3. 修改会话信息后可以 Up 起来但是仍然无法上网

```shell
qz@ubuntu:~$  nmcli connection show
NAME     UUID                                  TYPE            DEVICE 
ens33    d76ce20f-6ebe-42d9-abe4-11fb1bb86b2f  802-3-ethernet  ens33  
company  4b3d9878-197b-41bb-b337-f80381d3accb  802-3-ethernet  -
```





###### 远程控制服务 — SSH

Secure Shell

远程管理 Linux 目前的首选方式，此前使用 FTP/telenet (使用明文传输，不安全)。

登录方式：

1. 基于口令验证-账户，密码;
2. 基于密钥的验证-需要在本地生成密钥对，将公钥上传至服务器

- SSH 服务的配置文件：`/etc/ssh/sshd_config`

```shell
man ssh_config

ssh(1) obtains configuration data from the following sources in the following
 order:

       1.   command-line options
       2.   user's configuration file (~/.ssh/config)
       3.   system-wide configuration file (/etc/ssh/ssh_config)
```

- 关闭使用 root 登录 ssh 的权限，可以降低被黑客破解密码的几率

```shell
# vi /etc/ssh/sshd_config
PermitRootLogin no # 不允许通过 root 用户登录
PasswordAuthentication no # 不允许通过密码登录，仅允许通过验证密钥登录

# 修改完配置文件以后需要手动重启 sshd 服务
systemctl restart sshd
systemctl enable sshd # 开机自启动 sshd 服务
```

**远程修改 sshd 配置文件后，重启 sshd 服务，远程的 ssh 绘画竟然不会关闭！！！**

- 口令验证登录

```shell
# 客户端执行
ssh 192.168.213.128
# 输入帐号以及密码即可登录
```

- 安全密钥验证登录

```shell
ssh-keygen # 生成密钥对
ssh-copy-id 192.168.10.10 # 将生成的密钥传送到远程主机
ssh 192.168.10.10 # 登录
```

- scp - secure copy(remote file copy program)

基于 SSH 在网络之间安全传输文件的命令

```shell
scp [-12346BCpqrTv] [-c cipher] [-F ssh_config] [-i identity_file] [-l limit]
    [-o ssh_option] [-P port] [-S program] [[user@]host1:]file1 ...
    [[user@]host2:]file2

-v # 显示详细的进度
-P # 指定远程主机的 scp 端口号
-r # 递归传送，用于传送文件夹
-6 # 使用 ipv6 协议

scp /root/readme.txt 192.168.10.10:/home # 将文件上传到远程主机
scp 192.168.10.20:/etc/redhat-release /root # 将远程主机上的文件下载到本地

# 客户端也在本地主机上测试
scp -v readme.txt qz@127.0.0.1:/home/qz
root@ubuntu:/etc/ssh#  scp -v sshd_config qz@127.0.0.1:/home/qz
Executing: program /usr/bin/ssh host 127.0.0.1, user qz, command scp -v -t /home/qz
OpenSSH_7.2p2 Ubuntu-4ubuntu2.10, OpenSSL 1.0.2g  1 Mar 2016 # openssh
debug1: Reading configuration data /etc/ssh/ssh_config
debug1: /etc/ssh/ssh_config line 19: Applying options for *
debug1: Connecting to 127.0.0.1 [127.0.0.1] port 22.
debug1: Connection established.
debug1: permanently_set_uid: 0/0
debug1: key_load_public: No such file or directory
debug1: identity file /root/.ssh/id_rsa type -1
debug1: key_load_public: No such file or directory
debug1: identity file /root/.ssh/id_rsa-cert type -1
debug1: key_load_public: No such file or directory
debug1: identity file /root/.ssh/id_dsa type -1
debug1: key_load_public: No such file or directory
debug1: identity file /root/.ssh/id_dsa-cert type -1
debug1: key_load_public: No such file or directory
debug1: identity file /root/.ssh/id_ecdsa type -1
debug1: key_load_public: No such file or directory
debug1: identity file /root/.ssh/id_ecdsa-cert type -1
debug1: key_load_public: No such file or directory
debug1: identity file /root/.ssh/id_ed25519 type -1
debug1: key_load_public: No such file or directory
debug1: identity file /root/.ssh/id_ed25519-cert type -1
debug1: Enabling compatibility mode for protocol 2.0
debug1: Local version string SSH-2.0-OpenSSH_7.2p2 Ubuntu-4ubuntu2.10
debug1: Remote protocol version 2.0, remote software version OpenSSH_7.2p2 Ubuntu-4ubuntu2.10
debug1: match: OpenSSH_7.2p2 Ubuntu-4ubuntu2.10 pat OpenSSH* compat 0x04000000
debug1: Authenticating to 127.0.0.1:22 as 'qz'
debug1: SSH2_MSG_KEXINIT sent
debug1: SSH2_MSG_KEXINIT received
debug1: kex: algorithm: curve25519-sha256@libssh.org
debug1: kex: host key algorithm: ecdsa-sha2-nistp256
debug1: kex: server->client cipher: chacha20-poly1305@openssh.com MAC: <implicit> compression: none
debug1: kex: client->server cipher: chacha20-poly1305@openssh.com MAC: <implicit> compression: none
debug1: expecting SSH2_MSG_KEX_ECDH_REPLY
debug1: Server host key: ecdsa-sha2-nistp256 SHA256:w5f+e/rfj/2+xLQGvsF4WShB+Ue79S1XeRva0gS3nbE
debug1: Host '127.0.0.1' is known and matches the ECDSA host key.
debug1: Found key in /root/.ssh/known_hosts:4
debug1: rekey after 134217728 blocks
debug1: SSH2_MSG_NEWKEYS sent
debug1: expecting SSH2_MSG_NEWKEYS
debug1: SSH2_MSG_NEWKEYS received
debug1: rekey after 134217728 blocks
debug1: SSH2_MSG_EXT_INFO received
debug1: kex_input_ext_info: server-sig-algs=<rsa-sha2-256,rsa-sha2-512>
debug1: SSH2_MSG_SERVICE_ACCEPT received
debug1: Authentications that can continue: publickey,password
debug1: Next authentication method: publickey
debug1: Trying private key: /root/.ssh/id_rsa
debug1: Trying private key: /root/.ssh/id_dsa
debug1: Trying private key: /root/.ssh/id_ecdsa
debug1: Trying private key: /root/.ssh/id_ed25519
debug1: Next authentication method: password
qz@127.0.0.1's password: 
debug1: Authentication succeeded (password).
Authenticated to 127.0.0.1 ([127.0.0.1]:22).
debug1: channel 0: new [client-session]
debug1: Requesting no-more-sessions@openssh.com
debug1: Entering interactive session.
debug1: pledge: network
debug1: client_input_global_request: rtype hostkeys-00@openssh.com want_reply 0
debug1: Sending environment.
debug1: Sending env LANG = en_US.UTF-8
debug1: Sending command: scp -v -t /home/qz
Sending file modes: C0644 2563 sshd_config
Sink: C0644 2563 sshd_config
sshd_config                                            100% 2563     2.5KB/s   00:00    
debug1: client_input_channel_req: channel 0 rtype exit-status reply 0
debug1: channel 0: free: client-session, nchannels 1
debug1: fd 0 clearing O_NONBLOCK
debug1: fd 1 clearing O_NONBLOCK
Transferred: sent 4440, received 2580 bytes, in 0.6 seconds
Bytes per second: sent 7341.3, received 4265.9
debug1: Exit status 0

```

###### screen - 不间断会话服务, 

screen - screen manager with VT100/ANSI terminal emulation

正常情况下，通过远程进行操作的时候，如果由于网络等原因，远程关闭了，则服务端正在执行的操作也会停止。

screen 是一款能够实现多窗口远程控制的开源服务程序，简单来说就是为了解决网络异常中断或者为了同时控制多个远程终端窗口而设计的。用户可以使用 screen 服务程序在多个会话间自由切换。主要功能：

1. 会话恢复，即便网络中断，也可以让会话随时恢复，确保用户不会失去对远程的控制
2. 多窗口：每个会话都是独立运行的，拥有各自独立的输入
3. 会话共享，可以在其他用户之间共享

默认是没有安装的，通过 apt 安装：

```shell
sudo apt-get install screen
```

通过 yum 本地磁盘安装：

```shell
mkdir -p /media/cdrom
mount /dev/cdrom /media/cdrom
# /etc/yum/repos.d/rhel7.repo, 配置 yum 软件源
baseurl=file:///media/cdrom

yum install screen
```

管理远程会话：

```shell
-d # 将指定的会话进行离线处理
-r # 恢复指定会话
-x # 一次性恢复所有会话
-ls # 显示当前所有会话
-wipe # 删除目前无法使用的会话

screen -S backup # 创建一个名称为 backup 的会话窗口，
screen -ls # 查看所有的会话窗口
exit # 退出会话
# 并不是每次都需要创建一个会话，
# 直接使用 screen 命令执行要运行的命令，这样命令中的一切操作都被记录下来，当命令结束后会话自动结束
screen vim readme.md 
screen -r backup # 恢复会话1
```

- 会话共享功能

```shell
# A 终端创建会话，然后执行操作：
screen -S cpuinfo # 创建 cpuinfo 会话
top # 执行操作
```

B 终端，必须通过 SSH 登录：

```shell
ssh 192.168.0.2
screen -x # 共享会话， B 终端同样可以看到 A 终端的内容
```









#### ch10 使用 Apache 服务部署静态网页

常见的网页服务程序：

IIS : Windows

Nginx:

Apache: CentOS: httpd； Ubuntu/Debian：  apache2

```shell
apt-get install apache2 # apache2
```

**NOTE: httpd 服务有关的配置测试， Ubuntu 和 CentOS 差距较大，如果确认要在 ubuntu 上使用 httpd 服务，这个服务有关的配置还需要再研究一下。功能都有，只不过配置选项的位置和 httpd 有区别**



###### Apache httpd web 服务

安装 apache web 服务

CentOS 通过 yum 安装，需要配置本地软件源; ubuntu /Debian 通过 apt 安装

```shell
apt-get install httpd

root@ubuntu:~# apt-get install httpd
Reading package lists... Done
Building dependency tree       
Reading state information... Done
Package httpd is a virtual package provided by:
  nginx-light 1.10.3-0ubuntu0.16.04.5
  nginx-full 1.10.3-0ubuntu0.16.04.5
  nginx-extras 1.10.3-0ubuntu0.16.04.5
  lighttpd 1.4.35-4ubuntu2.1
  nginx-core 1.10.3-0ubuntu0.16.04.5
  apache2 2.4.18-2ubuntu3.17
  yaws 2.0.2-1
  webfs 1.21+ds1-11
  tntnet 2.2.1-2
  ocsigenserver 2.6-1build2
  mini-httpd 1.23-1
  micro-httpd 20051212-15
  ebhttpd 1:1.0.dfsg.1-4.3
  aolserver4-daemon 4.5.1-18
  aolserver4-core 4.5.1-18
You should explicitly select one to install.
```

安装：

```shell
sudo apt-get install apache2
systemctl start apache2
systemctl status apache2
```

log 目录 `/var/log/apache2/error.log` 

- 运行 apache2 web 服务(httpd)启动失败

```shell
root@ubuntu:~#   systemctl status apache2
● apache2.service - LSB: Apache2 web server
   Loaded: loaded (/etc/init.d/apache2; bad; vendor preset: enabled)
  Drop-In: /lib/systemd/system/apache2.service.d
           └─apache2-systemd.conf
   Active: inactive (dead) since Sun 2025-10-19 07:44:33 CST; 7s ago
     Docs: man:systemd-sysv-generator(8)
  Process: 11300 ExecStop=/etc/init.d/apache2 stop (code=exited, status=0/SUCCESS)
  Process: 11284 ExecStart=/etc/init.d/apache2 start (code=exited, status=0/SUCCESS)

Oct 19 07:44:33 ubuntu apache2[11284]: (98)Address already in use: AH00072: make_sock: could not bind to addres # 无法 bind 该端口
Oct 19 07:44:33 ubuntu apache2[11284]: (98)Address already in use: AH00072: make_sock: could not bind to addres
Oct 19 07:44:33 ubuntu apache2[11284]: no listening sockets available, shutting down
Oct 19 07:44:33 ubuntu apache2[11284]: AH00015: Unable to open logs
Oct 19 07:44:33 ubuntu apache2[11284]: Action 'start' failed.
Oct 19 07:44:33 ubuntu apache2[11284]: The Apache error log may have more information.
Oct 19 07:44:33 ubuntu apache2[11284]:  *
Oct 19 07:44:33 ubuntu apache2[11300]:  * Stopping Apache httpd web server apache2
Oct 19 07:44:33 ubuntu apache2[11300]:  *
Oct 19 07:44:33 ubuntu systemd[1]: Started LSB: Apache2 web server.
```

解决方法：

```shell
# 查看 apache2 服务器配置以及监听端口
cat /etc/apache2/ports.conf
# lsof 查看是否有其他进程绑定该端口，发现被 nginx 占用
lsof -i :80
# 终止 nginx 服务
systemctl stop nginx
# 删除 ngninx 应用
apt remove nginx-common
# 重新启动：
root@ubuntu:~#  systemctl status apache2.service 
● apache2.service - LSB: Apache2 web server
   Loaded: loaded (/etc/init.d/apache2; bad; vendor preset: enabled)
  Drop-In: /lib/systemd/system/apache2.service.d
           └─apache2-systemd.conf
   Active: active (running) since Sun 2025-10-19 07:49:50 CST; 7s ago
     Docs: man:systemd-sysv-generator(8)
  Process: 12072 ExecStart=/etc/init.d/apache2 start (code=exited, status=0/SUCCESS)
   CGroup: /system.slice/apache2.service
           ├─12087 /usr/sbin/apache2 -k start
           ├─12088 /usr/sbin/apache2 -k start
           └─12089 /usr/sbin/apache2 -k start

Oct 19 07:49:49 ubuntu systemd[1]: Starting LSB: Apache2 web server...
Oct 19 07:49:49 ubuntu apache2[12072]:  * Starting Apache httpd web server apache2
Oct 19 07:49:50 ubuntu apache2[12072]: AH00558: apache2: Could not reliably determine the server's fully qualif
Oct 19 07:49:50 ubuntu apache2[12072]:  *
Oct 19 07:49:50 ubuntu systemd[1]: Started LSB: Apache2 web server.
```

此时，打开浏览器： `localhost:80` 已经可以正常打开了.

- 配置服务文件

| 配置文件名称 | 存放位置                                               |
| ------------ | ------------------------------------------------------ |
| 服务目录     | /etc/httpd; /etc/apache2                               |
| 主配置目录   | /etc/httpd/conf/httpd.conf;  /etc/apache2/apache2.conf |
| 网站数据目录 | /var/www/html                                          |
| 访问日志     | /var/log/httpd/access_log; /var/log/apache2/access.log |
| 错误日志     | /var/log/httpd/error_log; /var/log/apache2/error.log   |

**第一个版本位于：/etc/httpd 目录；apache2 的配置文件已经分到了许多 /etc/apache2 子目录文件下了 **

| 参数           | 用途                                                 |
| -------------- | ---------------------------------------------------- |
| ServerRoot     | 服务目录                                             |
| ServerAdmin    | 管理员邮箱                                           |
| User           | 运行服务的用户                                       |
| Group          | 运行服务的用户组                                     |
| ServerName     | 网站服务器的域名                                     |
| DocumentRoot   | 网站数据保存目录                                     |
| Directory      | 网站数据目录的权限                                   |
| Listen         | 监听的端口号                                         |
| DirectoryIndex | 默认的索引页页面, /etc/apache2/mods-enabled/dir.conf |
| ErrorLog       | 错误日志文件                                         |
| CustomLog      | 访问日志文件                                         |
| Timeout        | 网页超时时间，默认300秒                              |

- 网页数据默认保存在 `/etc/www/html` 如果要修改，则要修改 `DocumentRoot` 配置

```shell
# Apache2 /etc/apache2/sites-available/000-default.conf
systemctl restart apache2 # 重启 apache httpd/apache2 服务
systemctl enable apache2 # 开机子启动
```



###### SELinux 安全子系统

对文件资源的访问限制，SELinux 安全上下文确保了文件资源只能被所属的服务程序进行访问。

- 安装 SELinux 组件，默认没有打开

```shell
apt-get install selinux-utils

root@ubuntu:~#  getsebool -a | grep ftp
getsebool:  SELinux is disabled
```

TODO: 找不到这两个命令

- getenforce - 获取 SELinux 的工作状态

```shell
getenforce
```

- semanage - 用户管理 SELinux 的策略

```shell
-l # 查询
-a # add
-m # modify
-d # delete
semanage fcontext -a -t httpd_sys_content_t /home/wwwroot # 向新的网站数据目录中增加一条 SELinux 安全上下文，让这个目录以及里面的所有文件都可以被 httpd 服务程序访问
semanage fcontext -a -t httpd_sys_content_t /home/wwwroot/*
```

- restorecon

如果执行完上述命令后仍然无法访问，还需要执行 `restorecon`命令将设置好的 SELinux 上下文立即生效

```shell
restorecon -Rv /home/wwwroot/
```

###### 个人主页功能

当需要为每个用户建立一个独立的网站时 Apache httpd 服务提供了该功能，而不需要自己实现。

https://www.cnblogs.com/escwq/p/11782912.html

修改配置文件，打开 UserDir 功能：

```shell
# 对 web server 用户来说， ~userid 需奥 711 权限; ~userid/public_html 需要 755 权限
# UserDir disabled
UserDir public_html

# 创建目录，增加权限
su - qz
mkdir public_html
chmod -Rf 755 public_html/
echo "This is $(users)'s homepage" > public_html/index.html

# 重新启动 apache2 web 服务,
# TODO: 主页没有生效, 仍然是 apache2 的 Index.html 界面
systemctl restart apache2

# 对于有 SecLinux 限制的机器，需要接触限制
getsebool -a | grep http
setsebool -P httpd_enable_homedirs=on
```

-  htpasswd - Manage user files for basic authentication

设置需要输入用户名以及密码才可以访问主页



- apache2 的方法和 httpd 已经不一样了，需要使用 `a2ensite` 命令

TODO: apache2 个人主页暂时没找到搭建教程， 和 httpd 稍微有点区别

```shell
```



TODO:

- 博客元主页可以插入前端 CSS 代码 

https://www.cnblogs.com/liuke123/p/10706087.html



###### 虚拟主机功能

可以把一台物理服务器分割成多个“虚拟的服务器”，避免多台服务器的费用。

apache 的虚拟主机功能是服务器基于用户请求的不同 IP 地址，主机域名或端口号，实现提供多个网站同时为外部提供访问服务的技术。



1. 基于 IP 地址：不同 ip 地址;
2. 基于端口号: 相同 ip 不同端口号;
3. 基于域名：相同 ip 以及端口，不同域名;

- 基于 IP 地址（多宿主机）

```shell
# step1: 使用 nmtui 配置网卡，同时配置 3 个 IP
# 192.168.10.10/24
# 192.168.20.10/24
# 192.168.30.10/24

systemctl restart network-manager # 重启网络服务，
# 确保 3 个 ip 的网络都可以正常访问， 
# 首先 3 个 ip 本地都可以 ping 通， 虚拟机可以访问百度
root@ubuntu:/etc/apache2# ifconfig
ens33     Link encap:Ethernet  HWaddr 00:0c:29:28:3e:e6  
          inet addr:192.168.10.10  Bcast:192.168.10.255  Mask:255.255.255.0 # 但是只有一个 ipv4 地址？？/
          inet6 addr: 2408:8478:1b03:6220:aa04:7d7d:a4ae:3f65/64 Scope:Global
          inet6 addr: fe80::afbd:cf89:b6f1:2e79/64 Scope:Link
          inet6 addr: 2408:8478:1b03:6220:d5e6:d663:110c:c339/64 Scope:Global
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:33583 errors:0 dropped:0 overruns:0 frame:0
          TX packets:40766 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:36494327 (36.4 MB)  TX bytes:3859771 (3.8 MB)
# 外部通过这 3 个 IP 都可以访问网络：
mkdir -p /home/wwwroot/10
mkdir -p /home/wwwroot/20
mkdir -p /home/wwwroot/30
echo "IP:192.168.10.10" > /home/wwwroot/10/index.html
echo "IP:192.168.10.20" > /home/wwwroot/20/index.html
echo "IP:192.168.10.30" > /home/wwwroot/30/index.html
```

修改配置文件 `vi /etc/httpd/conf/httpd.conf`， 修改后需要重启 httpd 服务

```xml
<VirtualHost 192.168.10.10>
DocumentRoot /home/wwwroot/10
ServerName www.linuxprobe.com
<Directory>
AllowOverride None
Require all granted
</Directory>
</VirtualHost>

<VirtualHost 192.168.10.20>
DocumentRoot /home/wwwroot/20
ServerName www.linuxprobe.com
<Directory>
AllowOverride None
Require all granted
</Directory>
</VirtualHost>

<VirtualHost 192.168.10.30>
DocumentRoot /home/wwwroot/30
ServerName www.linuxprobe.com
<Directory>
AllowOverride None
Require all granted
</Directory>
</VirtualHost>
```

如果无法通过不同的 ip 访问主页，可能需要增加 SELinux 安全上下文。



- 基于主机域名

当服务器无法为每个网站都分配一个独立 IP 的时候，可以让 Apache 自动识别请求的域名，从而跟你据不同的域名请求来传输不同的内容。`/etc/hosts` 是强制把某个主机域名解析到指定的 ip 地址的配置文件。主要这个文件配置正确，即使网卡参数中没有 DNS 信息也能将域名解析为某个 IP 地址

修改 `/etc/hosts` 配置文件，修改后立即生效

```shell
192.168.10.10 www.linuxprobe.com bbs.linuxprobe.com tech.linuxprobe.com
```

在主页目录下创建保存不同网页数据的文件以及网页

```shell
mkdir -p /home/wwwroot/www
mkdir -p /home/wwwroot/bbs
mkdir -p /home/wwwroot/tech
echo "www.linuxprobe.com" > /home/wwwroot/www/index.html
echo "bbs.linuxprobe.com" > /home/wwwroot/bbs/index.html
echo "tech.linuxprobe.com" > /home/wwwroot/tech/index.html
```

修改 httpd 配置文件 `/etc/httpd/conf/httpd.conf`，然后重启 httpd 服务 `systemctl restart httpd` 才会生效

```shell
<VirtualHost 192.168.10.10>
DocumentRoot /home/wwwroot/www
ServerName www.linuxprobe.com
<Directory "/home/wwwroot/www">
AllowOverride None
Require all granted
</Directory>
</VirtualHost>

<VirtualHost 192.168.10.10>
DocumentRoot /home/wwwroot/bbs
ServerName bbs.linuxprobe.com
<Directory "/home/wwwroot/bbs">
AllowOverride None
Require all granted
</Directory>
</VirtualHost>

<VirtualHost 192.168.10.10>
DocumentRoot /home/wwwroot/tech
ServerName tech.linuxprobe.com
<Directory "/home/wwwroot/tech">
AllowOverride None
Require all granted
</Directory>
</VirtualHost>
```

如果无法通过不同的 ip 访问主页，可能需要增加 SELinux 安全上下文。



- 基于端口号

基于端口号的虚拟主机功能可以让用户通过指定的端口号来访问服务器上的网站资源，一般来说，使用 80, 443, 8080 端口是比较合理的，使用其他端口则会收到 SELinux 的限制。

创建文件夹以保存首页文件

```shell
mkdir -p /home/wwwroot/6111
mkdir -p /home/wwwroot/6222
echo "port:6111" > /home/wwwroot/6111/index.html
echo "port:6222" > /home/wwwroot/6222/index.html
```

修改 httpd 服务配置文件 `vi /etc/httpd/conf/httpd.conf`，然后重启 httpd 服务 `systemctl restart httpd` 才会生效

```xml
# 增加监听端口
Listen 80
Listen 6111
Listen 6222

#虚拟主机
<VirtualHost 192.168.10.10:6111>
DocumentRoot /home/wwwroot/6111
ServerName tech.linuxprobe.com
<Directory "/home/wwwroot/6111">
AllowOverride None
Require all granted
</Directory>
</VirtualHost>

<VirtualHost 192.168.10.10:6222>
DocumentRoot /home/wwwroot/6222
ServerName tech.linuxprobe.com
<Directory "/home/wwwroot/6222">
AllowOverride None
Require all granted
</Directory>
</VirtualHost>
```

如果无法通过不同的 ip 访问主页，可能需要增加 SELinux 安全上下文。

```shell
semanage port -l | grep http
semanage port -a -t http_port_t -p tcp 6111
semanage port -a -t http_port_t -p tcp 6222
```



###### Apache 的访问控制

**Apache 可以通过源主机名，源 IP 地址，或源主机上的浏览器特征等信息对网站上的资源访问进行控制。**

Allow:

Deny:

```shell
Order allow, Deny # 表示允许源主机与访问规则进行匹配，若匹配成功则允许访问，否则拒绝访问请求
```

编辑配置文件 `/etc/httpd/conf/httpd.conf`，然后重启 httpd 服务 `systemctl restart httpd`

```shell
<Directory "/var/www/html/server">
Order allow,deny
Allow from 192.168.10.10 # 仅允许该主机访问
</Directory>
```





#### ch11 使用 vsftpd 服务传输文件

```shell
apt-get install ftp # 安装 FTP 客户端
apt-getinstall vsftpd # FTP 服务

man vsftpd # 查看帮助信息，就是一个 FTP 服务的守护进程

systemctl enable vsftpd # 设置为开机子启动
root@ubuntu:~#  systemctl status vsftpd # 安装完成后查看服务状态
● vsftpd.service - vsftpd FTP server
   Loaded: loaded (/lib/systemd/system/vsftpd.service; enabled; vendor preset: enabled)
   Active: active (running) since Mon 2025-10-20 23:31:56 CST; 3min 2s ago
 Main PID: 4176 (vsftpd)
   CGroup: /system.slice/vsftpd.service
           └─4176 /usr/sbin/vsftpd /etc/vsftpd.conf
```

FTP 服务守护进程名称  vsftpd(Very Secure File Transfer Protocol Daemon)

服务配置文件 `/etc/vsftpd/vsftpd.conf`，

```shell
grep -v "#"  /etc/vsftpd/vsftpd.conf # -v, 反选中，仅显示不包含 # 的行
```

###### FTP 使用

| 内部命令      | 命令说明                                       | 备注 |
| ------------- | ---------------------------------------------- | ---- |
| pwd           | 显示当前目录                                   |      |
| put           | 上传                                           |      |
| prompt        | 关闭交互模式                                   |      |
| newer         | 下载时，检测是不是新文件                       |      |
| mkdir         | 在远端ftp服务器上，建立文件夹                  |      |
| mput          | 上传文件，模糊匹配，                           | 批量 |
| mget          | 下载文件，模糊匹配                             | 批量 |
| mdelete       | 删除文件，模糊匹配                             |      |
| hash          | 显示#表示下载进度                              |      |
| get           | 下载                                           |      |
| delete        | 删除远端ftp服务器上的文件                      |      |
| close         | 在不结束ftp进程的情况下，关闭与ftp服务器的连接 |      |
| cdup          | 上一层目录                                     |      |
| cd            | 切换远端ftp服务器上的目录                      |      |
| ！            | 执行本地主机命令                               |      |
| binary        | 设置文件传输方式为二进制模式                   |      |
| ascii         | 设置文件传输方式为ASCII模式                    |      |
| ls            | 显示服务器上的目录                             |      |
| bye           | 退出ftp命令状态                                |      |
| lcd directory | 改变本地的当前目录为directory                  |      |
| cd directory  | 改变服务器的当前目录为directory                |      |
| quit          | 断开连接并退出ftp服务器                        |      |
| open          | 连接ftp服务器                                  |      |
| put           | 从客户端传送指定文件到服务器                   |      |
| get           | 从服务器下载指定文件到客户端                   |      |

```shell
ftp 192.168.10.10
put abc.txt server_abc.txt # 将 本地的 abc.txt 推送到服务端的 server_abc.txt
get server_abc.xt abc.txt # 从服务端下载到本地
rename server_abc.txt abc.txt # 修改文件名
```





vsftpd 三种工作模式：

1. 匿名开放模式 annomous
2. 本地用户模式 local
3. 虚拟用户模式

参数：

| 参数                                                 | 作用                                                         |
| ---------------------------------------------------- | ------------------------------------------------------------ |
| listen=[YES/NO]                                      | 是否以独立的方式监听服务                                     |
| listen_address=IP                                    | 设置要监听的 IP 地址                                         |
| listen_port=21                                       | 设置 FTP 服务要监听的端口                                    |
| download_enable=[YES/NO]                             | 是否允许下载文件                                             |
| userlist_enable=[YES/NO]<br />userlist_deny=[YES/NO] | 设置用户列表（通过用户列表同意/禁止制定用户 FTP 客户端登录） |
| max_clients=0                                        | 最大客户端连接数，0表示不限制                                |
| max_per_ip=0                                         | 同一 IP 的最大连接数，0表示不限制                            |
| anonymous_enable=[YES/NO]                            | 是否允许匿名用户访问                                         |
| anon_upload_enable=[YES/NO]                          | 是否允许匿名用户上传文件                                     |
| anon_umask=022                                       | 匿名用户上传文件的 umask 值                                  |
| anon_root=/var/ftp                                   | 匿名用户的 FTP 根目录                                        |
| anon_mkdir_write_enable=[YES/NO]                     | 是否允许匿名用户创建目录                                     |
| anon_other_write_enable=[YES/NO]                     | 是否开放匿名用户的其他写入权限（包括重命名，删除等）         |
| anon_max_rate=0                                      | 匿名用户上传的最大速度(Bytes/s)，0表示不限制                 |
| local_enable=[YES/NO]                                | 是否允许本地用户登录 FTP                                     |
| local_umask=022                                      | 本地用户上传文件的 umask 值                                  |
| local_root=/var/ftp                                  | 本地用户的 FTP 根目录                                        |
| chroot_local_user=[YES/NO]                           | 是否将用户权限禁锢在 FTP 目录，以确保安全                    |
| local_max_rate                                       | 本地用户上传的最大速度(Bytes/s)，0表示不限制                 |

###### 匿名开放模式

// TODO: 这个匿名模式总是报各种权限问题。

设置参数 `vi /etc/vsftpd.conf`， 然后重启 vsftpd 服务 `systemctl restart vsftpd`

```shell
anonymous_enable=YES
anon_umask=022
anon_upload_enable=YES
anon_mkdir_write_enable=YES
anon_other_write_enable=YES
```

FTP 客户端登录：

```shell
ftp 192.168.10.10
账户： annonymous
密码：空
```

登录成功，但是创建文件失败，/var/ftp 目录 ftp 用户没有写权限,修改用户权限	

```shell
ls -ld /var/ftp/pub
mkdir -p /var/ftp/pub
chown -Rf ftp /var/ftp/pub # 默认访问的是 /var/ftp 目录， 查看权限，只有 root 才有写入权限，将目录的所有者改为 user ftp
#  ftp 账户在用户中已经存在， ls -l /etc/passwd | grep ftp
# /var/ftp 运行的用户通常是 ftp/vsftpd

# 仍然报错是什么原因???
ftp>  cd pub
550 Failed to change directory.

# /var/ftp 目录加上了 0777 权限，仍然报错：
修改权：确保 chroot 目录的根目录（"/"）不是可写的。你可以通过修改目录权限来实现这一点。例如，如果你使用的是 /var/ftp 作为 chroot 目录，你应该确保 /var/ftp 的父目录 / 不是可写的。
```

###### 本地用户模式

编辑配置

```shell
anonymous_enable=NO
local_enalbe=YES
write_enable=YES
local_umask=YES
```

###### 虚拟用户模式

三种模式中最安全的一种认证模式。它需要为 FTP 服务单独建立用户数据库文件，**虚拟出用来进行口令验证的账户信息，而这些账户信息在服务器系统中实际上是不存在的，仅供 FTP 程序内部使用**。这样即使黑客破解了账户信息也无法登录服务器，从而有效降低了破坏范围。



配置参数：

```shell
anonymous_enable=NO
local_enable=YES
guest_enable=YES # 开启虚拟用户模式
guest_username=virtual # 指定虚拟用户帐号
pam_service_name=vsftpd.vu # 指定 Pam 文件, 表示登陆 FTP 服务器时是根据 /etc/pam.d/vsftpd 文件进行安全认证的
# 我们现在要做的就是把 vsftpd 主配置文件中原有的 PAM 认证文件 vsftpd 修改为新建的 vsftpd.vu 即可
```

###### PAM

一组安全机制的模块，系统管理员可以轻易的调整服务程序的认证方式，而不必对应用程序进行任何修改。PAM 采取了分层设计（应用程序层，应用接口层，鉴别模块层）。



######  TFTP - 简单文件传输协议

Trivial File Transfer Protocol 是一种基于 UDP 协议在客户端和服务端之间进行简单文件传输的协议。**简单文件传输，甚至不支持遍历目录。提供不复杂，开销不大的文件传输服务**

TFTP 在传输时使用 UDP 协议，69 端口，因此传输过程不像 FTP 那样可靠。但是 TFTP 不需要客户端的权限认证，也就减少了无谓的系统和网络带宽消耗，因此在传输琐碎(trivial)不大的文件时，效率更高。

```shell
apt-get install tftp
apt-get install tftp-server
apt-get install xinetd
```

**TFTP 是使用 xinetd 服务程序来管理的，xinetd 服务可以用来管理多种轻量级的网络服务，而且具有强大的日志功能。**

安装 TFTP 后，需要在 xinetd 服务程序中将其开启，把默认的 disable 修改为 no

xiugai  tftp 配置文件 `vim /etc/xinet.d/tftp` ，修改后重启 xinetd 服务 `systemctl restart xinetd`

```shell
systemctl enable xinetd # 开机启动

firewall-cmd --permanent --add-port=69/udp # 防火墙打开 udp 69 端口
firewall-cmd --realod # 重新加载防火墙

# This is the udp version.
service tftp
{
	socket_type		= dgram
	protocol		= udp
	wait 			= yes
	user 			= root
	server 			=  /usr/sbin/in.tftpd
	server_args 	= --secure /var/lib/tftpboot
	disable 		= no
	per_source		= 11
	cps				= 100 2
	flags			= IPv4
}
```

TFTP 的根目录为：`/var/lib/tftpboot`，

客户端登陆：

```shell
tftp # 192.168.10.10 直接连接
verbose # 打印日志模式
connect 192.168.10.10
status

tftp>  get test.txt # 下载文件
getting from 192.168.10.10:test.txt to test.txt [netascii]
Received 37402 bytes in 0.0 seconds [inf bits/sec]

tftp> put vuser.db vuser.db
putting vuser.db to 192.168.10.10:vuser.db [netascii]
Error code 1: File not found

# TODO: 网上说 put 要使用绝对路径，仍然失败，/var/lib/tftpboot 文件夹的 owner 已经修改为了 o+w 也不行
tftp>  put /tmp/vuser.db /var/lib/tftpboot/vuser.db
putting /tmp/vuser.db to 192.168.10.10:/var/lib/tftpboot/vuser.db [netascii]
Error code 1: File not found

quit # 退出
```



#### ch12 使用 Samba 或 NFS 实现文件共享

- SMB 协议（Server Message Block）

旨在解决局域网内的文件或打印机等资源的共享问题。



###### Samba 

**TODO:**

1. samba 服务安装后 smbd 守护进程正常运行，但是没有 smb 用户导致 `pdbedit` 命令运行异常。
2. 手动创建 smb 用户并且通过 smbpasswd 添加到 smb 用户数据库后，pdbedit 命令仍然无法使用;
3. 手动修改共享目录 `/home/database` 以及 `/var/samba` 目录 owner 为创建的 smb 用户后 `systemctl start smbd` 命令运行失败，smbd 服务启动失败;
4. **后续继续研究一下， 共享目录的实验没做成功。**



samba - Server to provide AD and SMB/CIFS services to clients

基于 SMB 协议的服务程序，可以实现 Linux 与 windows 间共享文件。

```shell
apt-get install samba
apt-get install samba-client # 如果需要 SMB 客户端的话
#配置文件 /etc/samba/smb.conf
systemctl status smbd # 安装后自动启动守护进程, 注意进程名字

man samba

qz@ubuntu:/tmp$  samba
samba             samba_kcc         samba_spnupdate   samba_upgradedns  
samba_dnsupdate   samba-regedit     samba-tool  
```

配置选项：

| [global]       | 参数                                    | 作用                                                       |
| -------------- | --------------------------------------- | ---------------------------------------------------------- |
|                | workgroup=MYGROUP                       | 工作组名称                                                 |
|                | server string = Samba Server Version %v | 服务器介绍信息，参数 %v 为显示 SMB 版本号                  |
|                | log file=/var/log/samba/log.%m          | 日志文件的存放位置与名称， %m 为来访的主机名               |
|                | max log size=50                         | 日志的最大容量为 50KB                                      |
| 安全验证的方式 | security=user                           | 安全验证的方式，总共 4 种                                  |
| 1              | 1. share                                | 来访主机不用验证口令，比较方便，但是安全性差               |
| 2              | 2. user                                 | 需验证来访主机提供的口令后才可以访问，提升了安全性         |
| 3              | 3. server                               | 使用独立的远程主机验证来访主机提供的口令（集中管理账户）   |
| 4              | 4. domain                               | 使用域控制器进行身份验证                                   |
| 用户后台的类型 | passdb backend=tdbsam                   | 定义用户后台的类型，共三种                                 |
| 1              | 1. smbpasswd                            | 使用 smbpasswd 命令为系统用户设置 samba 服务程序的密码     |
| 2              | 2. tdbsam                               | 创建数据库文件兵使用 pdbedit 命令建立 samba 服务程序的用户 |
| 3              | 3. ldapsam                              | 基于 LDAP 服务进行帐号验证                                 |
|                | load printers=yes                       | 设置在 samba 服务启动时是否共享打印机设备                  |
|                | cup options=raw                         | 打印机的选项                                               |
| [homes]        |                                         | 共享有关的参数                                             |
|                | comment=Home Directories                | 描述信息                                                   |
|                | bowseable=no                            | 指定共享信息是否在“网上邻居”中可见                         |
|                | writable=yes                            | 定义是否可以执行写入操作，与 'read only' 相反              |
| [printers]     |                                         | 打印机共享有关的参数                                       |
|                |                                         |                                                            |

配置共享服务

1. 全局配置参数：用于设置整体的资源共享环境，对立面的每一个独立的共享资源都有效
2. 区域配置参数：用于设置单独的共享环境，仅对该资源有效



配置参数：

```shell
[database] # 共享名称为 database
comment=Do Not arbitrarily modify the database file # 注释信息
path=/home/database
public=no # 关闭所有人可见
writable=yes # 允许写入操作

systemctl restart smbd # 重启 smbd 服务
```

step1 创建用于访问共享资源的账户信息，samba 服务默认使用的是用户口令认证模式(user)。只有建立账户数据库后才可以使用口令认证模式。

**samba 服务的数据库文件要求 “账户”必须在当前系统中已经存在，否则日后创建文件时将导致文件的权限属性混乱不堪，由此引发错误。**

- pdbedit - manage the SAM database (Database of Samba Users)

用户管理 SMB 服务程序的账户信息数据库，

```shell
pdbedit --help
-a 用户名 # 建立 samba 账户
-x 用户名 # 删除 samba 账户
-L # list
-u username # 用户名
-Lv # list verbose information
```

- smbpasswd - change a user's SMB password

```shell
-a # This option specifies that the username following should be added to the local smbpasswd file, with the new password typed (type <Enter> for the old password). This option is ignored if the username following already exists in the smbpasswd file and it is treated like a regular change password command. Note that the default passdb backends require the user to already exist in the system password file (usually /etc/passwd), else the request to add the user will fail. This option is only available when running smbpasswd as root.
```

- testparm -  - check an smb.conf configuration file for internal correctness

可以用来测试 smb.conf 中的参数配置是否正确

```shell
```



添加账户信息：

```shell
qz@ubuntu:/home/database$  pdbedit -a -u qz # 添加账户信息报错
new password:
retype new password:
tdbsam_open: Failed to open/create TDB passwd [/var/lib/samba/private/passdb.tdb]
tdbsam_getsampwnam: failed to open /var/lib/samba/private/passdb.tdb!
tdbsam_open: Failed to open/create TDB passwd [/var/lib/samba/private/passdb.tdb]
tdbsam_new_rid: failed to open /var/lib/samba/private/passdb.tdb!
```

Q: 若 smbd 进程显示没有找到 smb 用户，通常由配置文件路径错误或服务未正确启动导致。以下是排查步骤：

A: 没有 smb 用户

```shell
useradd -s /sbin/nologin smb # 
smbpasswd -a smb # 账户&密码:smb
```

- 修改共享目录的访问权限/owner

```shell
chmod -R 777 /home/database
chown -R smb /home/database # smbd 进程通过 smb 账户访问
chown -R smb /var/lib/samba # 修改 samba 文件的的 owner
```



- SELinux 未使能, 如果使能了还需要打开 samba 相关选项

```shell
qz@ubuntu:/home/database$  getsebool
getsebool:  SELinux is disabled
```





- SMB 客户端测试

Windows:

1. 打开IE浏览器输入file://IP/myshare/ 然后输入用户名和密码

2. Win+R: 

```shell
\\192.168.10.10
```

Linux:

```shell
smblcient -L \\192.168.10.10 -U qz
```



###### NFS 

Network File System - 网络文件系统

**觉得 samba 配置麻烦且需要共享文件的主机都是 Linux 系统，则推荐大家部署 NFS 服务来共享文件。NFS 服务可以将远程 Linux 系统上的文件资源挂载到本地主机的目录上。从而使得本地主机基于 TCP/IP 协议，像使用本地主机上的资源那样读写远程 Linux 系统上的共享文件**

```shell
# yum install nfs-utils # CentOS
apt-get install nfs-kernel-server
systemctl status nfs-kernel-server # 查看服务运行状态, nfs-server 一样的
ps -aux | grep nfs # nfsd

root@ubuntu:/#   systemctl status nfs-kernel-server
● nfs-server.service - NFS server and services
   Loaded: loaded (/lib/systemd/system/nfs-server.service; enabled; vendor preset: enabled)
   Active: active (exited) since Thu 2025-10-23 11:09:39 CST; 28s ago
```

两台 Linux 主机一台 NFS 客户端，一台 NFS 服务端

```shell
# step1: 设置客户端以及服务端的 ip 地址：
# 这里设置为同一网段

# step2:在 NFS 服务器上建立用于 NFS 文件共享的目录，并设置足够的权限确保其他人也有写入权限
mkdir /nfsfile
chmod -R 777 /nfsfile

# step3:配置 NFS 服务程序为 /etc/exports
# 允许 192.168.10.0/24 网段的所有主机访问，
/nfsfile 192.168.10.*(rw,sync,root_squash)

# step4:重启 NFS 服务
# 由于在使用 NFS 服务进行文件共享之前，需要使用 RPC(Remote Procedure Call)服务将 NFS 服务器的 IP 地址和端口号等信息发送给客户端。因此在启动 NFS 服务之前，还需要重启并启用 rpcbind 服务程序。
systemctl restart rpcbind
systemctl enable rpcbind
systemctl restart nfs-server # 
```

- exportfs - exportfs - maintain table of exported NFS file systems

```shell
#-a     Export or unexport all directories.
exportfs -ra # 重新加载 NFS 配置参数
systemctl restart nfs-server

# subtree_check 需要指定
# exportfs: /etc/exports [2]: Neither 'subtree_check' or 'no_subtree_check' specified for export "*:/nfsfile".
#  Assuming default behaviour ('no_subtree_check').
#  NOTE: this default has changed since nfs-utils version 1.0.x

```

- 清空防火墙, 这里是 iptables & firewalld

网络文件夹权限说明：

| 参数           | 作用                                                         |
| -------------- | ------------------------------------------------------------ |
| ro             | 只读                                                         |
| rw             | 读写                                                         |
| root_squash    | 当 NFS 客户端以 root 管理员访问时，映射为 NFS 匿名文件       |
| no_root_squash | 当 NFS 客户端以 root 管理晕访问时，映射为 NFS 服务器的 root 管理员 |
| all_squash     | 无论 NFS 客户端使用什么帐号访问，均映射为 NFS 服务器的匿名用户 |
| sync           | 同时将数据写入到内存与硬盘中，保证数据不丢失                 |
| async          | 优先将数据保存到内存，然后再写入硬盘，这样效率更高，但可能会丢失数据 |



---

- NFS 客户端的配置

- showmount -  - show mount information for an NFS server

```shell
-e # 显示 NFS 服务器的共享列表
-a # 显示本机挂在的文件资源情况
-v # 显示版本号

root@ubuntu:/#  showmount  -e
Export list for ubuntu:
/nfsfile 192.168.10.*
```

​	客户端挂在 NFS 目录：

```shell
# 在客户端创建一个挂载文件夹
mkdir /home/qz/nfsfile
mount -t nfs 192.168.10.10:/nfsfile /home/qz/nfsfile
# 如果需要开机自动挂载，则需要修改 /etc/fstab 配置文件
192.168.10.10:/nfsfile /home/qz/nfsfile nfs defaults 0 0
```

- 查看本地磁盘信息

```shell
df -h
```



###### autofs 自动挂载服务

无论是 samba 服务还是 NFS 服务，都需要把挂载信息写入到 `/etc/fstab` 中，在开机的时候自动挂载。但是如果挂载的远程资源太多，会很浪费网络带宽以及硬件资源。如果挂载后长期不使用也会浪费硬件资源。

autofs 是一个自动挂载服务，当检测到用户使用时，才会自动挂载。从而节省了网络资源和服务器的硬件资源。

```shell
apt-get install autofs
systemctl status autofs
ps -aux | grep autofs
# root      10080  0.0  0.0  57100  3332 ?        Ssl  12:19   0:00 /usr/sbin/automount --pid-file /var/run/autofs.pid
```

修改 autofs 的主配置文件 `/etc/auto.master`，按照 “挂载目录，子配置文件” 的格式填写。

1. 挂载目录是挂在位置的的上一级目录, 例如：`挂载到 /media/cdrom`，则写 `/media` 即可;
2. 子配置文件会对这个挂载目录内的配置文件进一步说明，子配置文件没有严格的名字规定，但必须以  `.misc` 结尾;

```shell
# 挂载到 /media/cdrom 目录
/media /etc/iso.misc
```

3. 子配置文件格式：“挂在目录 挂载文件类型以及权限:设备名称”，例如 `/etc/iso.misc`

```shell
iso		-fstype=iso9660,ro,nosuid,nodev:/dev/cdrom
```

重启 autofs 服务

```shell
systemctl restart autofs
systemctl enable autofs # 将 autofs 加入到系统启动项中
```



#### ch13 使用 BIND 提供域名解析服务

// TODO:

bind9 也需要好好研究一下：

https://blog.51cto.com/yuanbin/108578



BIND - Berkeley Internet Name Domain 服务是全球使用最广泛最安全且高效的域名解析服务程序。

```shell
apt get install bind9
#apt-get install bind-chroot # CentOS
systemctl start bind9 # 开启服务
systemctl enable --now bind9 # 开机自启动
systemctl status bind9
```

**在 Linux 系统中，BIND 服务的名称为 named,**



用到的相关 shell 指令：

- resolvconf - manage nameserver information

resolvconf 是用于管理 /etc/resolv.conf 文件的命令行工具，主要用于动态更新 DNS 配置。

```shell
-u # Just run the update scripts (if updating is enabled).
--enable-updates # Set the flag indicating that resolvconf should run update scripts when invoked in the future with -a, -d or -u.  If a  delayed  update was scheduled then run update scripts.
--disable-updates # Clear the flag.
--updates-are-enabled # Return 0 if the flag is set, otherwise return 1.
systemctl status resolvconf # 还是一个后台运行的服务

root@ubuntu:/tmp# systemctl status resolvconf
● resolvconf.service - Nameserver information manager
   Loaded: loaded (/lib/systemd/system/resolvconf.service; enabled; vendor preset: enabled)
   Active: active (exited) since Thu 2025-10-23 21:37:23 CST; 1h 6min ago
     Docs: man:resolvconf(8)
  Process: 512 ExecStart=/sbin/resolvconf --enable-updates (code=exited, status=0/SUCCESS)
  Process: 497 ExecStartPre=/bin/touch /run/resolvconf/postponed-update (code=exited, status=0/SUCCESS)
  Process: 494 ExecStartPre=/bin/mkdir -p /run/resolvconf/interface (code=exited, status=0/SUCCESS)
 Main PID: 512 (code=exited, status=0/SUCCESS)
   CGroup: /system.slice/resolvconf.service

Oct 23 21:37:22 ubuntu systemd[1]: Starting Nameserver information manager...
Oct 23 21:37:23 ubuntu systemd[1]: Started Nameserver information manager.

resolvconf --enable-updates	# 强制 resolvconf 更新
resolvconf -u
systemcrl restart resolvconf # 重启服务
```



- nslookup - query Internet name servers interactively

一个可以进行域名查询的测试指令

https://cloud.tencent.com/developer/article/2169208

```shell
type可以为一下类型：
 A ：地址记录（直接查询默认类型）
 AAAA ：地址记录
 AFSDB： Andrew文件系统数据库服务器记录
 ATMA ：ATM地址记录
 CNAME： 别名记录
 HINFO： 硬件配置记录，包括CPU、操作系统信息
 ISDN： 域名对应的ISDN号码
 MB： 存放指定邮箱的服务器
 MG： 邮件组记录
 MINFO： 邮件组和邮箱的信息记录
 MR： 改名的邮箱记录
 MX： 邮件服务器记录
 NS： 名字服务器记录
 PTR： 反向记录
 RP： 负责人记录
 RT： 路由穿透记录
 SRV： TCP服务器信息记录
 TXT： 域名对应的文本信息
 X25： 域名对应的X.25地址记录

nslookup -type=A www.baidu.com # 查询 ipv4 地址
nslookup -type=AAAA www.baidu.com # 查询 ipv6 地址
nslookup -type=HINFO www.baidu.com # 查询硬件信息
nslookup -type=CNAME www.baidu.com # 查询别名
```

- rndc - name server control utility

```shell
man rndc
```







- /var/run/named.pid

这个文件用来记录 named 进程的 pid，这个文件是给那些需要向运行的进程们发送信号的程序使用的。`/var/run` 目录下有很多后台程序的 pid 信息。

- 配置文件

主配置文件： `/etc/bind/named.conf`

区域配置文件：`/etc/named.rfc1912.zones`用来保存域名和 IP 地址对应关系所在位置

数据配置文件：`/var/named` 用来保存域名和 ip 真实对应关系

- 服务类型有三种：

1. hint : 根区域
2. master: 主区域， 主服务器
3. slave: 从区域，从服务器

###### 正向解析实验

域名 -> IP

step1:修改 “区域配置文件” CentOS: `/etc/named.rf1912.zones`, ubuntu 这里为：`/etc/named/named.conf.default-zones`

```shell
# 增加 "正向解析参数"
zone "linuxprobe.com" IN{
	# 服务类型
	type master;
	# 域名与 IP 地址解析规则保存的文件位置
	file "linuxprobe.com.zone";
	# 允许哪些客户机动态更新解析信息
	allow-update {none; };
};
# 反向解析参数
#表示 192.168.10.0/24 网段的反向解析区域
zone "10.168.192.arpa" IN{
	type master;
	file "192.168.10.arpa"
}
```

step2:修改数据配置文件 `/var/named/linuxprobe.com.zone` , **bind9 应该有修改，或者是 CentOS 和 ubuntu 的区别？？？ 将 /var/named 目录下的配置文件移动到了 /etc/named 下，名称改为 db.xxx**，这里是：`/etc/named/db.linuxprobe`

```shell
cp -a db.empty db.linuxprobe # 复制时，加上 -a 参数，复制时保留原始文件的所有者，所属组，权限属性等信息，以便让 BIND 服务程序顺利读取
#  模板文件 named.localhost -> db.empty

# 文件示例：
$TTL 86400
#授权信息开始 			# DNS区域的地址	  #域名管理员的邮箱
@       IN      SOA     ns1.example.com. admin.example.com. ( 
                              2023031501   ; Serial #更新序列号
                              3600         ; Refresh # 更新时间
                              1800         ; Retry # 重试延时
                              604800       ; Expire # 失效时间
                              86400        ; Minimum TTL # 无效解析记录的缓存时间
                        )
        IN      NS      ns1.example.com. #域名服务器记录
        IN      NS      ns2.example.com. # 
        IN      A       192.0.2.1 # 地址记录： 
ns1     IN      A       192.0.2.1 # 地址记录：ns1.example.com
ns2     IN      A       192.0.2.2 # 地址记录： ns2.example.com
www     IN      A       192.0.2.100 # 地址记录： www.example.com

# systemctl restart bind9 不会报错了，但是 正向解析实验一直没有生效，已经修改了 DNS server 的地址了呀。。
root@ubuntu:/etc/bind#   cat db.linuxprobe 
; BIND reverse data file for empty rfc1918 zone
;
; DO NOT EDIT THIS FILE - it is used for multiple zones.
; Instead, copy it, edit named.conf, and use that copy.
;
$TTL	86400
@	IN	SOA	linuxprobe.com. root.linuxprobe.com. (
			      1		; Serial
			 604800		; Refresh
			  86400		; Retry
			2419200		; Expire
			  86400 )	; Negative Cache TTL
;
@	IN	NS	www
@	IN	NS 	bss
@	IN	NS 	tech
@	IN	A 	192.168.10.10
www	IN	A	192.168.10.10
bss	IN	A	192.168.10.10
tech	IN	A	192.168.10.10

```

保存配置文件后重启 BIND 服务程序

```shell
# systemctl restart named
systemctl restart bind9
systemctl restart network-manager # 重启网络
```

**一定要把 Linux 系统网卡中的 DNS 地址参数更换成本机 IP 地址，这样就可以使用本机提供哦嗯的 DNS 服务查询了**

```shell
nmtui # 修改 DNS server,原来的 DNS server 为网关地址:172.20.10.1
```

step3: 测试实验结果

```shell
# 以前
root@ubuntu:/etc/bind#   nslookup 
> www.linuxprobe.com
Server:		127.0.1.1
Address:	127.0.1.1#53
Non-authoritative answer:
Name:	www.linuxprobe.com
Address: 8.138.155.96

> bbs.linxprobe.com
Server:		127.0.1.1
Address:	127.0.1.1#53
** server can't find bbs.linxprobe.com: NXDOMAIN

# 使用 bind9 后
# 手动指定 DNS server 为 192.168.10.10 的情况下， DNS 正常，
# 否则仍然使用的是默认的 DNS server (gateway address)进行解析的, 使用 nmtui 工具修改了 DNS server 的 IP 没有生效???
root@ubuntu:/etc/bind#  nslookup 
> server 192.168.10.10
Default server: 192.168.10.10
Address: 192.168.10.10#53
> www.linuxprobe.com # 手动指定 DNS server IP
Server:		192.168.10.10
Address:	192.168.10.10#53

Name:	www.linuxprobe.com
Address: 192.168.10.10
> 
> 
> tech.linuxprobe.com
Server:		192.168.10.10
Address:	192.168.10.10#53

Name:	tech.linuxprobe.com
Address: 192.168.10.10
> bss.linuxprobe.com
Server:		192.168.10.10
Address:	192.168.10.10#53

Name:	bss.linuxprobe.com
Address: 192.168.10.10
```



###### 反向解析实验

IP -> 域名

一般用于：

1. 对某个 IP 上绑定的所有域名进行屏蔽，屏蔽由某个域名发送的垃圾邮件等操作;
2. 针对某个 IP 进行反向解析，进而判断出有多少个网站运行在上面，当购买虚拟主机时，可以通过这个判断有多少个网站运行在上面，进而判断是否有超售问题;

- 配置参数

step1: 编辑 “区域配置文件” `/etc/bind/named.conf.default-zones`，反向解析在配置中需要把域名反写， ip 地址：192.168.10 -》 10.168.192

```shell
#反向解析参数
zone "10.168.192.arpa" IN{
	type master;
	file "/etc/bind/192.168.10.arpa"
}
```

step2:编辑 “数据配置文件” `/etc/bind/192.168.10.arpa` 或 `/etc/bind/db.10.168.192` ubuntu 上这样写。**只需要写主机号，不需要写网络号**

```shell
$TTL    86400
@       IN      SOA     ns1.example.com. admin.example.com. (
                              2023041501 ; Serial
                              3600       ; Refresh
                              1800       ; Retry
                              604800     ; Expire
                              86400      ; Minimum TTL
                        )
        IN      NS      ns1.example.com.
        IN      NS      ns2.example.com.

; PTR records for IP addresses in the 192.168.1.0/24 network
1               IN      PTR     host1.example.com.
2               IN      PTR     host2.example.com.
; Add more PTR records as needed for other IP addresses in your network

#linuxprobe
@       IN      SOA     linuxprobe.com. admin.linuxprobe.com. (
                              2023041501 ; Serial
                              3600       ; Refresh
                              1800       ; Retry
                              604800     ; Expire
                              86400      ; Minimum TTL
                        )
        IN      NS      www.linuxprobe.com.
        IN      NS      tech.linuxprobe.com.
        IN      NS      bss.linuxprobe.com.

; PTR records for IP addresses in the 192.168.1.0/24 network
10               IN      PTR     www.linuxprobe.com.
10               IN      PTR     tech.linuxprobe.com.
10               IN      PTR     bss.linxprobe.com.
```

step3: 测试

```shell
root@ubuntu:/etc/bind#  nslookup 
> server 192.168.10.10
Default server: 192.168.10.10
Address: 192.168.10.10#53
>  192.168.10.10
Server:		192.168.10.10
Address:	192.168.10.10#53

** server can't find 10.10.168.192.in-addr.arpa: NXDOMAIN
> exit

```

###### 部署从服务器

——DNS 服务器的稳定性非常重要

DNS 域名解析服务器中，从服务器可以从主服务器上获取指定的区域数据文件，从而起到备份解析记录与负载平衡的作用，因此通过部署从服务器可以减轻主服务器的负载压力，还可以提升用户的查询效率。

step1: 在主服务器中，允许从服务器的更新请求，

修改配置文件`/etc/named.rfc1912.zones`，重启 dns 服务 `systemctl restart bind9`

```shell
zone "linuxprobe.com" IN {
	type master;
	file "linuxprobe.com.zone";
	allow-update {192.168.10.20; };
};

zone "10.168.192.in-addr.arpa" IN {
	type master;
	file "192.168.10.arpa";
	allow-update {192.168.10.20; };
};
```

step2: 在从服务器中填写主服务器的 IP 地址以及要抓取的区域信息，然后重启服务。**注意此时的服务类型是 slave 而不是 master**

file 参数后面定义的是同步数据配置文件后要保存到的位置，

```shell
zone "linuxprobe.com" IN {
	type slave;
	masters {192.168.10.10; }
	 #从服务器同步数据配置文件后要保存的位置
	file "slaves/linuxprobe.com.zone"
};

zone "10.168.192.in-addr.arpa" IN {
	type slave;
	masters {192.168.10.10; };
	file "slaves/192.168.10.arpa";
};
```

step3: 从服务器启动后即会自动从主服务器同步配置文件到指定目录。在从服务器使用 nslookup 测试

```shell
nslookup # 
www.linuxprobe.com
192.168.10.10
```



###### 安全的加密传输

 互联网中绝大多数(>=95%)的 DNS 服务器都是基于 BIND 域名解析服务搭建的，而 BIND 服务为了提供安全的解析服务，已经对 TSIG(RFC2845) 加密机制提供了支持。TSIG 加密机制保证了 DNS 服务器之间传输域名区域信息的安全性(UDP 53 端口)。

- dnssec-keygen - - DNSSEC key generation tool

```shell
# -a algorithm
# -b keysize
# -n nametype
root@ubuntu:/etc/bind#  dnssec-keygen -a HMAC-MD5 -b 128 -n HOST master-slave
Kmaster-slave.+157+49463

```

配置 DNS 服务的加密传输：

step1: 生成密钥

```shell
dnssec-keygen -a HMAC-MD5 -b 128 -n HOST master-slave
```

step2: 在主服务器中创建密钥验证文件 `/etc/transfer.key`

```shell
key "master-slave" {
	algorithm hmac-md5;
	secret "IdPhCVwbEkaye8AV3f75yw==";
};

chown root:named transfrer.key
chmod 640 transfer.ky
ln transfer.key /etc/transfre.key # 创建链接文件
```

step3: 主服务器开启并加载 bind 服务的密钥验证功能， `/etc/named.conf`

```shell
include "/etc/tranfer.key"
allow-transfer {key master-slave; };
```

step4: 从服务器使其支持密钥验证，配置 DNS 从服务器和主服务器的方法大致相同，都需要在 bin 服务程序的配置文件中创建密钥认证文件. 

```shell
key "master-slave" {
	algorithm hmac-md5;
	secret "IdPhCVwbEkaye8AV3f75yw==";
};

chown root:named transfrer.key
chmod 640 transfer.ky
ln transfer.key /etc/transfre.key # 创建链接文件
```

step5: 从服务器开比密钥验证功能

```shell
include "/etc/tranfer.key"
server 192.168.10.10
{
keys  {key master-slave; };
}
```

step6: DNS 从服务器同步域名区域数据, 删除从服务器 DNS 数据 `/etc/named/slaves/*`，然后测试是否是否自动同步 DNS 数据。

```shell
systemctl restart named
```



###### DNS 缓存服务器

一种不负责域名数据维护的 DNS 服务器. 简单的说，就是把用户经常用到的域名与 IP 地址的解析记录保存到本地，从而提升下次解析的效率。

应用：一般用于经常访问某些固定站点，而且对这些网站的访问速度有较高要求的企业内网中，但实际的应用并不广泛。

而且缓存服务器是否可以成功解析还与指定的上级 DNS 服务器的允许策略有关，了解即可。

- 配置

修改 BIND 服务的主配置文件 `/etc/named.conf`，增加 forwarders 参数

```shell
# 格式：forwarders {上级DNS服务器地址;};
options {
	forwarders {210.73.64.1; };
};
```

 

###### 分离解析技术

DNS 服务的分离解析功能，可以让处于不同地理位置范围内的读者访问相同的网址，但是从不同的服务器获取数据（不同的地理位置/国家分别架设 DNS 服务器）。

- 配置

step1: DNS 分离解析技术和 DNS 跟服务器功能冲突，需要删除

```shell
#删除：
zone "." IN {
	type hint;
	file "named.ca";
};
```

step2: 修改 “区域配置文件”，实现根据不同地理区域(例如：中国 & 美国)的 IP 分别加载不同的数据配置文件 linuxprobe.com.china & linuxprobe.com.america。当用户访问 linuxprobe.com 时，便会按照不同的数据配置文件找到该区域对应的服务器。

`/etc/named.rf1912.zones`

```shell
# 使用 acl 参数定义变量 china & america
acl "china" {127.71.115.0/24; };
acl "america" {106.185.25.0/24};

view "china" {
	match-clients {"china"; };
	zone "linuxprobe.com" {
		type master;
		file "linuxprobe.com.china"
	};
};

view "america" {
	match-clients {"america"; };
	zone "linuxprobe.com" {
		type master;
		file "linuxprobe.com.america"
	};
};
```

step3: 修改 “数据配置文件”， 通过模板创建 数据配置文件 linuxprobe.com.china & linuxprobe.com.america

`/etc/named/ linuxprobe.com.china `

```shell
@       IN      SOA     linuxprobe.com. root.linuxprobe.com. (
                              2023041501 ; Serial
                              3600       ; Refresh
                              1800       ; Retry
                              604800     ; Expire
                              86400      ; Minimum TTL
                        )
        IN      NS      ns.linuxprobe.com.
ns		IN      A     	122.71.115.10
www		IN		A		122.71.115.15
```

`/etc/named/ linuxprobe.com.america `

```shell
@       IN      SOA     linuxprobe.com. root.linuxprobe.com. (
                              2023041501 ; Serial
                              3600       ; Refresh
                              1800       ; Retry
                              604800     ; Expire
                              86400      ; Minimum TTL
                        )
        IN      NS      ns.linuxprobe.com.
ns		IN      A     	106.85.25.10
www		IN		A		106.85.25.15
```

step4: 测试，将客户主机的 IP 分别修改为： 122.71.15.1 与 106.185.25.，并修改 DNS server 地址为两个不同 ip 的服务主机 ip，使用 `nslookup` 测试

```shell
nslookup
```



#### ch14 使用 DHCP 动态管理主机地址

主要用于自动管理**局域网内主机**的 IP 地址，子网掩码，网关地址，DNS 地址等参数。

**DHCP 的设计初衷就是为了更高效的集中管理局域网中的 IP 地址资源，而且当客户端的租约时间到期后还可以自动回收已分配的 IP 地址，以便交给新加入的客户端。**

```shell
apt-get install dhcpd # CentOS
apt-get install isc-dhcp-server
```

租约：DHCP 客户端能够使用动态分配的 IP 的时间

预约：保证网络中特定的设备（MAC）总是获取到固定的 IP 地址;

- **客户端主机通过 DHCP 获取 IP 流程解析：**

1. DISCOVER: 客户端广播 DHCP 请求
2. OFFER: 服务端回复 DHCP offer，其中包含分配给客户端主机的 IP, gatewat, netmask, DNS, lease time 等信息
3. REQUEST: 客户端广播发送 DHCP request 报文，其中包括 IP, gatewat, netmask, DNS, lease time 等信息
4. ACK: 对应的服务端回复 DHCP ACK，

https://blog.csdn.net/qq_51544942/article/details/124926120



- 配置参数 `/etc/dhcp/dhcpd.conf`

例子：

```shell
#一个标准的配置文件应该包括:全局配置参数，子网网段声明，地址配置选项，以及地址配置参数
ddns-update-style interim; # 全局配置参数
ignore client-updates;

# 子网网段声明
subnet 192.168.10.0 netmask {
	# 地址配置选项
	option routers 192.168.10.1;
	option subnet-mask 255.255.255.0;
	#地址配置参数
	default-lease-time 21600;
	max-lease-time 43200;
}
```

参数说明：

```shell
ddns-update-style [类型] # 定义 DNS 服务动态更新的类型，类型包括：none(不支持动态更新)，interim(互动更新模式),ad(特殊更新模式)
[allow]|[ignore] client-updates # 允许/忽略客户端更新 DNS 记录
default-lease-time [21600] # 默认超时时间
option domain-name-servers [8.8.8.8] # 定义 DNS 服务器地址，8.8.8.8 为谷歌公开的 DNS 服务器地址，稳定性好，硬件设备多
option domain-name ["domain.org"] # 定义 DNS 域名
range # 用于分配的 IP 地址池
option subnet-mask # 定义客户端的子网掩码
option routers # 定义客户端的网关地址
broadcast-address [address] # 定义客户端的广播地址
ntp-server [address] # 定义客户端的网络时间服务器
nis-servers [address] # 定义客户端的 NIS 服务器地址
Hardware [网卡物理地址] # 定义网卡接口的类型与 MAC 地址
server-name [hostname] # 想 DHCP 客户端通知服务端的主机名
fixed-address [address] # 将某个固定的 ip 分配给制定的主机
time-offset [offset] # 制定客户端与格林尼至时间的偏移差
```

需要关闭虚拟机软件自带的 DHCP 服务，为了避免与自己配置的 dhcpd 服务程序发生冲突，应该先关闭：“编辑” -> “虚拟机网络管理”

```shell
ddns-update-style none;
ignore client-updates;
subnet 192.168.10.0 netmask 255.255.255.0 {
	range 192.168.10.50 192.168.10.150;
	option subnet-mask 255.255.255.0;
	option routers 192.168.10.1;
	option domain-name "linuxprobe.com"
	option domain-namer-servers 192.168.10.1;
	default-lease-time 21600;
	max-lease-time 43200;
}

# 重启服务, 
systemctl restart isc-dhcp-server
systemctl enable isc-dhcp-server # 开机自动开启

# TODO: 服务启动失败，提示配置问题
Oct 25 22:15:22 ubuntu sh[7056]: exiting.
Oct 25 22:15:22 ubuntu systemd[1]: isc-dhcp-server.service: Main process exited, code=exited, status=1/FAILURE
Oct 25 22:15:22 ubuntu systemd[1]: isc-dhcp-server.service: Unit entered failed state.
Oct 25 22:15:22 ubuntu systemd[1]: isc-dhcp-server.service: Failed with result 'exit-code'.
root@ubuntu:/etc/dhcp# systemctl status isc-dhcp-server
● isc-dhcp-server.service - ISC DHCP IPv4 server
   Loaded: loaded (/lib/systemd/system/isc-dhcp-server.service; enabled; vendor preset: enabled)
   Active: failed (Result: exit-code) since Sat 2025-10-25 22:15:22 CST; 1min 9s ago
     Docs: man:dhcpd(8)
  Process: 7056 ExecStart=/bin/sh -ec      CONFIG_FILE=/etc/dhcp/dhcpd.conf;      if [ -f /etc/ltsp/dhcpd.conf ]; then CONFIG_FILE=/etc/ltsp/dhcpd.conf; fi;      [ -e /var
 Main PID: 7056 (code=exited, status=1/FAILURE)

Oct 25 22:15:22 ubuntu sh[7056]: Configuration file errors encountered -- exiting
Oct 25 22:15:22 ubuntu sh[7056]: If you think you have received this message due to a bug rather
Oct 25 22:15:22 ubuntu sh[7056]: than a configuration issue please read the section on submitting
Oct 25 22:15:22 ubuntu sh[7056]: bugs on either our web page at www.isc.org or in the README file
Oct 25 22:15:22 ubuntu sh[7056]: before submitting a bug.  These pages explain the proper
Oct 25 22:15:22 ubuntu sh[7056]: process and the information we find helpful for debugging..
Oct 25 22:15:22 ubuntu sh[7056]: exiting.
Oct 25 22:15:22 ubuntu systemd[1]: isc-dhcp-server.service: Main process exited, code=exited, status=1/FAILURE
Oct 25 22:15:22 ubuntu systemd[1]: isc-dhcp-server.service: Unit entered failed state.
Oct 25 22:15:22 ubuntu systemd[1]: isc-dhcp-server.service: Failed with result 'exit-code'.
root@ubuntu:/etc/dhcp#  vi dhcpd.conf

```

##### 分配固定 IP

把 IP 地址和主机网卡的 MAC 地址绑定，既可以实现该主机一直获取到固定 IP 的功能。

配置参数 `/etc/dhcp/dhcpd.conf`

```shell
# 为这一台主机分配固定的 IP
host linuxprobe {
	hardware ethernet 00:0c:29:27:c6:12;
	fixed-address 192.168.10.88;
}
```

然后重启服务即可。

**TODO: 需要使用客户机测试 DHCP 功能，本地只需要保证 DHCP 服务运行正常即可**



#### ch15 使用 Postfix 与 Dovecot 部署邮件服务

电子邮件系统，可以让用户在离线的情况下完成数据的收，发。



常见的邮件协议

- 简单邮件传输协议(SMTP， Simple Mail Transfer Protocol)：用于发送和中转发出的电子邮件，使用 TCP 25 端口
- 邮局协议v3(POP3， Post Office Protocol 3): 用于将电子邮件存储到本地主机，占用服务器的 TCP 110 端口
- 网络消息访问协议（IMCP4，Internet Message Access Protocol 4）:用于在本地主机上访问邮件，使用TCP 110 端口

角色：

- 邮件用户代理(MUA，Mail User Agent)：为用户收，发邮件的服务器
- 邮件投递代理（MDA，Miail Delivery Agent）:用于保存用户邮件的“信箱”服务器，负责将来自 邮件传输代理（MTA, Mail Transfer Agent）的邮件保存到本地的收件箱中;



###### Postfix

基于 SMTP 协议的服务程序，提供发邮件服务功能。IBM 自主研发的免费开源电子邮件服务程序，能够很好的兼容 Sendmail 服务程序，收发能力强于 Sendmail 服务程序。

```shell
apt-get install postfix # RHEL7 默认是安装的
systemctl enable postfix # 开机自启动
systemctl status postfix
```



###### Dovecot

基于 POP3 协议或 IMAP 协议的服务程序，提供收邮件功能。开源服务程序。

```shell
#yum install dovecot

root@ubuntu:/etc/postfix#  apt-get install dovecot-
dovecot-antispam         dovecot-dev              dovecot-ldap             dovecot-managesieved     dovecot-pgsql            dovecot-solr             
dovecot-core             dovecot-gssapi           dovecot-lmtpd            dovecot-metadata-plugin  dovecot-pop3d            dovecot-sqlite           
dovecot-dbg              dovecot-imapd            dovecot-lucene           dovecot-mysql            dovecot-sieve    

sudo apt install dovecot-core dovecot-imapd dovecot-lmtpd dovecot-mysql dovecot-pgsql dovecot-pop3d # 这个命令会安装Dovecot的核心组件以及IMAP、LMTP和POP3服务，同时还会安装MySQL和PostgreSQL的支持模块。
systemctl status dovecot
systemctl enable dovecot
```



- mail -  mail.mailutils - process mail messages

在 Linux 服务端，可以通过 mail 命令管理/查看邮件



```shell
root@ubuntu:/etc/dovecot/conf.d#  mail 
No mail for root

-s	给邮件追加主题
-a	发送邮件附件，多个附件使用多次-a选项即可
-b	指定密件抄送的收信人地址
-c	指定抄送的收信人地址
```





###### Outlook/Foxmail 等客户端软件

供用户使用的邮件客户端软件。



###### 配置邮件服务器

要想检验电子邮件系统的配置效果，需要先部署 BIND 服务程序，为电子邮件服务器和客户端提供 DNS 域名解析服务

- 配置 BIND 域名解析服务

step1：配置服务器主机名称，需要保证服务器主机名称和发信域名一致

```shell
vi /etc/hostname
mail.linuxprobe.com
```

step2: 清空防火墙

```sh
iptables -F
service iptables save
```

step3: 为电子邮件系统提供域名解析，修改“主配置文件”， “区域配置文件”以及“数据配置文件”

```shell
vi /etc/named/named.conf

# 数据配置文件
$TTL 1D
ns		IN		A			192.168.10.10
@		IN 		MX 10		mail.linuxprobe.com
mail	IN		A			192.168.10.10
```

- 配置 Postfix 服务程序，配置文件 `/etc/postfix/main.cf` 

```shell
myhostname # 邮局系统的主机名
mydomain # 邮局系统的域名
myorigin # 从本机发出的邮件的域名名称
inet_interfaces # 监听的网卡接口
mydestination # 可接收邮件的主机名或域名
mynetworks # 设置可转发哪些主机的邮件
relay_domains # 设置可转发哪些网域的邮件·
```

修改：

```shell
myhostname=mail.linuxprobe.com
mydomain=linuxprobe.com
myorigin=$mydomain
inet_interfaces=all
mydestination=$myhostname,$mydomain
```

step2: 创建电子邮件系统的登录帐号，Postfix 与 vsftpd 一样，都可以使用本地系统的账户和密码，因此在本地系统创建常规账户即可。

```shell
useradd boss
echo "linuxprobe" | passwd --stdin boss
systemctl restart postfix
```

- 配置 dovecot 服务程序，`/etc/dovecot/dovecot.conf`

Dovecot 服务程序为了保证电子邮件系统的安全，默认强制用户使用加密方式登录，而由于当前没有加密系统，因此需要添加该参数来允许用户明文登录

```shell
protocols = imap pop3 lmtp
disable_plaintext_auth = no # 允许明文登录
login_trusted_networks=192.168.0.0/24 # 仅允许该网段的用户使用邮箱服务
```

配置邮件格式与存储路径 `/etc/dovecot/config.d.10-mail.conf`

```shell
mail_location=mbox:~/mail:INBOX=/var/mail/%u
```

重启服务

```shell
systemctl restart dovecot
systemctl enable dovecot
```

- 使用 windows outlook 客户端软件测试电子邮件系统

1. 需要修改客户端主机的 DNS 服务器地址

2. outlook 软件默认会通过 SSL 加密协议尝试登录电子邮件服务，失败后点击“下一步” 即可让 Outlook 软件通过非加密的方式验证登录。

###### 设置用户别名邮箱

- aliases - Postfix local alias database format

更新完 /etc/alias 文件后，需要执行`newaliases` 命令，让新的用户别名生效

```shell
abcd:root # 生效后发往 abcd@linuxprobe.com 的邮件 root 可以收到

mail
```





#### ch16 使用 Squid 部署代理缓存服务





#### ch17 使用 iSCSI 服务部署网络存储



#### ch18 使用 MariaDB 数据库管理系统



#### ch19 使用 PXE + Kickstart 无人值守安装服务



#### ch20 使用 LNMP 架构部署动态网站环境



#### 扩展:

TODO:

- awk 脚本

```shell
awk # 也可以用于 shell 字符串操作
```



- sed

  

