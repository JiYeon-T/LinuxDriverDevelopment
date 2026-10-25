#!/bin/bash


#check score
read -p "Enter score:" GRADE
echo "get input socre:$GRADE"
if  [ $GRADE -le 100 ] && [ $GRADE -ge 85 ] ; then
  echo "Excellent"
elif [ $GRADE -lt 85 ] && [ $GRADE -ge 60 ]; then
  echo "Not bad"
else
  echo "Bad."
fi


