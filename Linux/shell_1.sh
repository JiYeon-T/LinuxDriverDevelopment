#! /bin/bash

# bash test
#echo 'this is my first shell test.'
#pwd
#ls -al

#test2參數
#echo "當前腳本文件的名稱：$0"
#echo "總共傳入了:$#個參數，分別是:$*"
#echo "第一個參數:$1, 第二個參數:$2"

# 測試
#[ -f /etc/abc ]
#echo "是否爲文件:$?"

# 邏輯運算
#[ -f /etc/abc ] && echo "/etc EXIST"
#[ -f /etc/abc ] || echo "/etc/abc is NOT exist"
#[ $USER = qz ] && echo "user=qz exist."	# echo $USER
#[ $USER = qz ] && echo "USER=qz" || echo "USER root"

# 整數之間比較大小:-eq, -ne, -gt, -lt, 
#[ 10 -eq 10 ]
#echo $?


# 變量使用
#FreeMem = `free -m | grep Mem: | awk '{print $4}'`	#反引號``:將結果輸出
#echo $FreeMem

# 字符串比較 = ， !=, -z
[ -z $String ]
echo $?		# 打印上一條的結果
[ $LANG != "en.US" ] && echo "NOT en.US"

