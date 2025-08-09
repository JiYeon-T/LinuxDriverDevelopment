

# y 时等效与:obj-y 直接将该源文件编译进内核
# m 时 编译生成模块 rtc-s3c.ko
# n 时,不编译该源文件, obj-y / obj-m 以外的所有所有目标都不会被编译
obj-$(CONFIG_RTC_DRV_S3C)	+= rtc-s3c.o



# Linux 内核 makefile, 内核的编译系统 kbuild
# (1) 目标定义:
obj-y += foo.o
# 最常见的方法是根据 .config 决定是否参与编译:
obj-$(CONFIG_ISDN)	+= isdn.o
obj-$(CONFIG_iSDN_PPP_BSDCOMP) 	+= isdn_bsdcomp.o

# (2) 多文件模块的定义:
# EXT2 文件系统为例:
obj-$(CONFIG_EXT2_FS) += ext2.o
ext2-y := balloc.o dir.o file.o fsync.o ialloc.o inode.o
			ioctl.o namei.o super.o symlink.o
ext2-$(CONFIG_EXT2_FS_XATTR)	+= xattr.o xattr_user.o xattr_trusted.o
ext2-$(CONFIG_EXT2_FS_POSIX_ACL) += acl.o

# (3) 目录层次的迭代
# CONFIG_EXT2_FS 为 y/m 时, kbuild 会将 ext2 目录列入向下迭代的目标中
obj-$(CONFIG_EXT2_FS) 	+= ext2/
