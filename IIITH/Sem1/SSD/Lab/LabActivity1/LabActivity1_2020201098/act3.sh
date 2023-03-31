#! /bin/sh

cal >> out1.txt
date >> out1.txt
cat out1.txt
cat out1.txt|tail -3
sed -n 3,7p out1.txt
sed -n 3,7p out1.txt| wc -l
echo "This day is awesome." >> out2.txt
cat out2.txt| wc -w
echo "I am looking forward to the day." >> out2.txt
cat out2.txt| wc -l
cat out1.txt| cut -d " " -f5
cat out1.txt| cut -d " " -f4-9
cat out1.txt| cut -d " " -f3-
cat out2.txt|cut -d " " -f2,4|tr -s "\n" " "


