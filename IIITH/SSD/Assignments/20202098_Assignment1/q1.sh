#! /bin/bash
mkdir Assignment1
cd Assignment1
touch lab1.txt lab2.txt lab3.txt lab4.txt lab5.txt
#commands to fill the files
echo "lab1 11111" >> lab1.txt
echo "lab2 1111" >> lab2.txt
echo "lab3 111" >> lab3.txt
echo "lab4 11" >> lab4.txt
echo "lab5 1" >> lab5.txt
#command end
#looping command for rename
#for i in *.txt
#do mv "$i" "${i%.txt}.c"
#done
#rename.ul .txt .c *.txt
#find command for rename
find -name "*.txt" -exec sh -c 'file="{}"; mv -- "$file" "${file%.txt}.c"' \;
ls -lSr | grep -v ^d
#creating text files for displaying
touch tmp1.txt tmp2.txt tmp3.txt
cd
find $(pwd)  -maxdepth 2 -type d,f
cd $OLDPWD
find $(pwd) -type f -name "*.txt" 
