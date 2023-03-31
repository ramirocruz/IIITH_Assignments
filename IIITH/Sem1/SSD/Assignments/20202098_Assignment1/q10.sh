#! /bin/bash
read optr
read num
for (( i=0;i<num;i++ ))
do
read opnd[$i]
done
echo `echo "scale=6;${opnd[@]}" | tr -s ' ' "$optr" | bc | awk '{printf "%.4f", $0}' | sed '/\./ s/\.\{0,1\}0\{1,\}$//' `

