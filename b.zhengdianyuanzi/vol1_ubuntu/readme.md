# TODO:
暂时先不安装 vpn, 访问 github 了，vpn 需要至少 buntu18.04, 但是 Linux 驱动的课程使用的是 ubuntu16.04, 升级后怕引入其他问题


# 虚拟机位置

ROS 虚拟机位置：D:\install_location\ubuntu16.04

Linux 驱动开发/调试虚拟机：F:

##  二、VMVare Tools 安装

##  解决全屏后黑边/无法铺满问题

如果切换全屏后四周仍有黑边，是因为未安装适配工具，需按以下步骤配置：

1. ‌安装VMware Tools（核心步骤）‌：

   - 保持虚拟机开机运行，在顶部菜单栏点击「VM→Install VMware Tools」；
   - Windows客户机：打开虚拟光盘，双击`setup64.exe`以管理员安装，完成后重启；
   - Linux（如Ubuntu）客户机：终端执行命令`sudo apt update && sudo apt install open-vm-tools-desktop -y`，安装完成后重启服务`sudo systemctl restart vmtoolsd`。

2. ‌**开启自动分辨率适配**‌：
   关闭虚拟机后，右键点击虚拟机选择「设置→硬件→显示器」，勾选「自动调整客户机分辨率」，建议同时勾选「加速3D图形」，保存后重启虚拟机即可。

3. ‌**手动触发适配**‌：
   虚拟机运行后，点击顶部「查看→自动调整大小→立即适应客户机」，即可强制同步宿主机分辨率，消除黑

## SougouPinYin installation
uninstall Fcitx and install SouGouPinYin

`sudo dpkg -i sougou_xx.pkg`
需要自己去 sougou 官网下载安装包


## sovlve vim dispaly A,B,C,D when use arrow keypad
THe vim function is not complete with Ubuntu 16.06's own package.
```shell
sudo apt-get install vim
sudo apt-get install vim-gtk
```



## 3. Shell commands

```shell
ls
su, sudo su
mv
mkdir
touch
cp
rm
rmdir
mv, move  # move a file to a destination location 
  mv test test1  # rename test to test1
  mv a.c test/  # move a.c to test/
ifconfig  # modify interface's MAC address, IP address etc.
  sudo ifconfig ens33 up  # enable interface
  sudo ifconfig ens3 down  # disable interface
  sudo ifconfig ens33 192.168.175.100  # modify IP address
  sudo ifconfig ens33 reload  # restart interface
reboot  # reboot the computer
poweroff   #
man   # manual page
sync  # synchronize data to the disk
find  # search file
  find . -name a.txt
grep  # search string
du  # disk usage summarize the file size
  du -sh test.txt  # print file total size, include child directory
df  # display filesystem information
  df -h
gedit  # open txt file in edit app
cat  # view file content
ps  # print process information
  ps -aux
top  # print system info dynamically
  top
file  # print file type
```

## Software Installation
```shell
sudo apt-get install git  # install git 

sudo apt-get install mplayer  # a video palyer tool

sudo dpkg -i xxx.deb  # install software with dpkg
```

e.g. 1. install `tree` command with source code

https://developer.aliyun.com/article/1370376


```shell
wget http://mama.indstate.edu/users/ice/tree/src/tree-1.8.0.tgz

tar -zxvf tree-1.8.0.tgz -C /usr/local/src

cd /usr/local/src/tree-1.8.0/

make install
```

2. install google-chrome browser with dpkg

** NOTE: this package can't be used at Ubuntu16.04, Cuz the dependency lib version can't be satifised.**
```shell
sudo wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb

sudo dpkg -i google-chrome-stable_current_amd64.deb 

sudo apt-get -f install  # fix broken
```

## linux system directory
/ - root directory
/bin - executable files
/boot - boot files, /boot/grub-multi OS startup program
/cdrom - CD storage files
/dev - device driver files, 
/etc - configuration files, e.g. configure enviroment variable, /etc/profile
/home - multi user home directory
/lib - library
/lib64 - 
/media - multimedia device, U disk device, SD card etc
/mnt - mount directory
/root - root directory
/opt - optional program files
/proc - virtual process file system, save process information
/run - runnning information
/sbin - user installed binay files, executable files
/sys
/srv - service information
/tmp - temporay files
/var - vaiable files, e.g. log files
/usr - It isn't the user, UNIX Software Resource, save softwares

## relative path and absolute path
. - current directory
.. - parent directory


## disk usage
```shell

df -h
/dev/sdb1       100M   42K  100M   1% /media/qz/B067-03D8

du -h --max-depth=1  # only print the current directory, does not recurisely search
 df -h
```

### mount & umount SD card or U disk
```shell
unmount /media/qz/B067-03D8  # mount directory,

mount /dev/sdb1 /media/qz/B067-03D8  # mount "dist device path" "mount point/directory"
```

- solve incorrectly display Chinese directroy name
```shell
mount -o iocharset=utf8 /dev/sdb1 /media/udisk/  # Linux use UTF-8 code format
```

### create partition
```shell
sudo fdisk /dev/sdb
```

- format file system
```shell
sudo mkfs -t fat /dev/sdb1  # It will show 2 U disks at windows with 1 SD card after create 2 partitions.
sudo mkfs -t fat /dev/sdb2
```

## compress & decompress
7z compress tool.
gzip
bzip2
tar

```shell
tar -vcjf xxx.tar.bz2 xxx  # compress .bz2
tar -vxjf xxx.tar.bz2  # decompress tar.bz2
tar -vczf xxx.tar.gz xxx  # compress .gz
tar -vxzf xxx.tar.gz  # decompress tar.gz
```


rar
zip


## user and user group
/etc/passwd  # user information file
/etc/shadow  # user password file
/etc/group  # user group file 

- add user and group with GUI tools
```shell
sudo apt-get install gnome-system-tools
```

- add user and group with shell commands
adduser  # add suer
finger  # search user
passwd  # modify user's passwd
deluser  # delete user
addgroup  # add group
groups  # display users in group
delgroup  # delete group


## file operation permission
rwx
421

e.g.
-rwxrwxrwx
 u  g  o

- chmod  # modify file operation permission

- chown  # modify file's owner


```shell
sudo chown root hello  # modify file's owner to root
sudo chown .root hello  # modify file's group to root
sudo chown root.root hello  # modify file's owner and group to root
```

## Linux link file
- hard link
same INode number

```shell
ln hello hello2

- symbolic link
different inode number, like a shortcut

ln -s hello hello3

cp -d hello3 hello4  # copy with symbolic
```

## vim



## Linux C program

## cross platform compiler


## make & Makefile

用途：

管理代码构建



相比于手动输 `gcc` 命令编译的优点：

1. 管理很多文件, 不需要每次编译都输入很多文件;
2. 一次编译后, 后续编译不需要每次都重新编译所有文件, 仅重新编译修改文件;



例子：`makefile_test`



## shell

例子：`shell_test`









