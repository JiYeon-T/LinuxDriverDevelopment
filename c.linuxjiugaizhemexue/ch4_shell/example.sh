#!/bin/bash
#shell example


#脚本格式参考: /etc/bash.bashrc


#eay shell script test
#pwd
#ls -al

#get user enter:
#echo "当前脚本名称 $0"
#echo "所有参数 $*"
#echo "参数个数 $#"
#echo "剩余参数 $1 $2 $3"
#echo "上一条命令的执行结果 $?"

#mkcdrom.sh
#DIR="/media/cdrom"
#DIR="/dev"
#if [ ! -e $DIR ]; then
#  mkdir -vp $DIR
#else
#  echo "$DIR exit"
#fi

#checkhost.sh
#check host online or not
#ping -c 3 -i 1 -W 3 $1 &> /dev/null #结果不输出
#if [ $? -eq 0 ]; then
#  echo "Host $1 is onlien"
#else
#  echo "Host $1 is not online"
#fi

#check score
#read -p "Enter score:" GRADE
#if  [ $GRADE -le 100 ] && [ $GRADE -ge 85 ] ; then
#  echo "Excellent"
#elif [ $GRADE -lt 85 ] && [ $GRADE -ge 60 ]; then
#  echo "Not bad"
#else
#  echo "Bad."
#fi

#通过列表文件创建用户
#read -p "Enter user password:" PASSWD
#for UNAME in `cat users.txt`; do #执行 shell 命令
#  id $UNAME &> /dev/null
#  if [ $? -eq 0 ]; then
#    echo "${UNAME} already exist"
#  else
#    #useradd $UNAME &> /dev/null
#    useradd $UNAME
#    echo "$PASSWD" | passwd &UNAME --stdin $UNAME
#    if [ $? -eq 0 ]; then
#      echo "$UNAME create success"
#    else
#      echo "$UNAME create failed reason:$?" #TODO:get reason string function
#    fi
#  fi
#done

#checkout hose online
#HLIST=$(cat ipaddrs.txt) #执行 shell 命令
#for IP in $HLIST; do
#  ping -c 3 -i 0.2 -W 3 $IP &> /dev/null
#  if [ $? -eq 0 ]; then
#    echo "$IP online"
#  else
#    echo "$IP not online"
#  fi
#done


#guess.sh
#PRICE=$(expr $RANDOM % 10)
#TIMES=0
#echo "You guess priceis:$PRICE"
#while true; do
#  read -p "Please enter" INT
#  let TIMES++
#  if [ $INT -eq $PRICE ] ; then
#    echo "Right"
#    exit 0
#  elif [ $INT -gt $PRICE ] ; then
#    echo "Too large"
#  else
#    echo "too small"
#  fi
#  echo "time:$TIMES"
#done

#while test2
CNT=0
while [ $CNT -lt 10 ]; do
  read -p "$CNT enter"
  let CNT++
done

#checkkeys.sh
#read -p "Enter a character" KEY
#case "$KEY" in
#[a-z]|[A-Z])
#  echo "character"
#;;
#[0-9])
#  echo "number"
#;;
#*)
#  echo "other character"
#esac













