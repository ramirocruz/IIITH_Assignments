#! /bin/bash
read str
str=`echo $str | tr -s '()' ' ' | tr -s ' ' ' ' | awk '{$1=$1;print}'`
echo "($str)"
