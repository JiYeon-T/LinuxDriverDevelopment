#!/bin/bash

read -p "Enter a character" KEY

case "$KEY" in
[a-z]|[A-Z])
  echo "character"
;;
[0-9])
  echo "number"
;;
*)
  echo "other character"
esac



