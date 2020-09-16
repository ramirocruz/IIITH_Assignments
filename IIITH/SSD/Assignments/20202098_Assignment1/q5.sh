#! /bin/bash
read str
str=`echo ${str,,}`
str2=`echo $str|rev`
if [[ $str == $str2 ]]
then 
echo Yes
else
echo No
fi

