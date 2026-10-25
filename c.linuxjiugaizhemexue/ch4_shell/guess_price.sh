#!/bin/bash


#$RANDOM是一个内置的环境变量,用于生成伪随机数
PRICE=$(expr $RANDOM % 10)
TIMES=0
echo "You guess priceis:$PRICE"

while true; do
  read -p "Please enter" INT
  let TIMES++
  if [ $INT -eq $PRICE ] ; then
    echo "Right"
    exit 0
  elif [ $INT -gt $PRICE ] ; then
    echo "Too large"
  else
    echo "too small"
  fi
  #echo -e参数表示对\进行转义
  echo -e "time:$TIMES\n"
done


