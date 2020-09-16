#! /bin/bash
arr=($@) 
val=${arr[0]}
for (( i=1;i<$#;i++ ))
do
 val=`echo $val^${arr[$i]} | bc `
 val=$( echo $val| tr -d " \\" 2>/dev/null )
done
echo $val

