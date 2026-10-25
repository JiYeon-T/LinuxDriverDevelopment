#!/bin/bash


#check host online
#if [ -e ]

HLIST=$(cat ipaddrs.txt) #执行 shell 命令

for IP in $HLIST; do
  ping -c 3 -i 0.2 -W 3 $IP &> /dev/null
  if [ $? -eq 0 ]; then
    echo "$IP online"
  else
    echo "$IP not online"
  fi
done

