#! /bin/bash
#./q2_helper.sh

read str
str=`echo $str | grep -o . | sort | tr -d '\n'`
str=`echo $str`
d=`cat commands.txt | grep -w -n $str`
line_no=`echo $d | cut -d ':' -f1`
if [[ -n $d ]]
then
echo Yes
sed -n ${line_no}p data.txt
else
echo No
fi

