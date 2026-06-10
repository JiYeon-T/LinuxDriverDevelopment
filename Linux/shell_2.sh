#!/bin/bash

# test1
#DIR="/home/qz/Desktop/cdrom"	# 創建變量不能加空格
#if [ ! -e $DIR ]		# 如果不存在這個文件夾
#then
#echo "$DIR does not exist, create it."
#mkdir $DIR
#fi


#test2
#ping -c 3 -i 0.2 -W 3 $1 &> /home/qz/Desktop/Linux/log/log.txt
#if [ $? -eq 0 ]
#then 
#echo "Host $1 is On-line."
#else
#echo "Host $1 in not on-line."
#fi

# test3
#read -p "Enter a score(0-100):" GRADE
#if [ $GRADE -ge 85 ] && [ $GRADE -le 100 ]	# $變量
#then
#echo "Excellent."
#elif [ $GRADE -ge 70 ] && [ $GRADE -le 84 ]
#then
#echo "Good."
#else
#echo "Bad."
#fi

# test4: for circle
#read -p "Enter the users's password : " PASSWD
#for UNAME in `cat users.txt`
#do
#id $UNAME &> /dev/null		# id ???
#if [ $? -eq 0 ]
#then
#echo "Already Exist."
#else
#useradd $UNAME &> /dev/null
#echo "PASSWD" | passwd --stdin $UNAME &> /dev/null
#if [ $? -eq 0 ]
#then 
#echo "$UNAME, create success"
#else
#echo "$UNAME, create failure."
#fi
#fi
#done

# test5: 通過腳本檢查主機是否在線
#HLIST=$(cat ./ipaddrs.txt)
#for IP in $HLIST
#do
#ping -c 3 -i 0.2 -W 3 $IP &> /dev/null
#if [ $? -eq 0 ]
#then
#echo "HOST $IP is online."
#else
#echo "HOST $IP is not online."
#fi
#done 

# test6: while circle

# expr命令:取出結果
# SHELL中不可以隨便添加空格
#PRICE=$(expr $RANDOM % 1000)	# 隨機給這個變量復制 
#echo "PRICE=$PRICE"
#TIMES=0
#echo "商品價格在0-999之間,猜猜看是多少？"
#while true
#do
#read -p "請輸入你猜的數字:" NUM
#let TIMES++
#if [ $NUM -eq $PRICE ]
#then
#echo "恭喜，猜對了，實際價格是 : $PRICE"
#echo "猜了$TIMES次"
#exit 0
#elif [ $NUM -gt $PRICE ]
#then
#echo "大了"
#else
#echo "小了"
#fi
#done

# test6: SHELL中的條件選擇語句
# SHELL中不能隨便加括號
read -p "輸入一個東西 : " KEY
case "$KEY" in 
[a-z]|[A-Z])
echo "輸入的是字母."
;;
[0-9])
echo "輸入的是數字."
;;
[])
echo "輸入的是符號"
;;
*)	#默認執行的情況
echo "輸入的是其他字符"
esac

