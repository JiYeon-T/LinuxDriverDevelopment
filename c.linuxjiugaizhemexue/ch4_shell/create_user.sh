#!/bin/bash


#通过列表文件创建用户
read -p "Enter user password:" PASSWD
for UNAME in `cat users.txt`; do #执行 shell 命令
  id $UNAME &> /dev/null
  if [ $? -eq 0 ]; then
    echo "${UNAME} already exist"
  else
    #useradd $UNAME &> /dev/null
    useradd $UNAME
    echo "$PASSWD" | passwd &UNAME --stdin $UNAME
    if [ $? -eq 0 ]; then
      echo "$UNAME create success"
    else
      echo "$UNAME create failed reason:$?" #TODO:get reason string function
    fi
  fi
done

