#! /bin/bash
compgen -c| sort -u > data.txt
i=0
while read line
do
temp=`echo $line | grep -o . | sort | tr -d '\n'`
ar[$i]=`echo $temp`
(( i++ ))
done < data.txt
printf "%s\n" "${ar[@]}" > commands.txt

