#! /bin/bash
ps -au | tr -s " " " " | cut -d " " -f2 | tail -n+2 | sort -n > pid.txt
read lines
temp=`cat pid.txt | wc -l`
if (( $lines > temp ))
then 
  lines=$temp
fi

cat pid.txt | head -$lines
