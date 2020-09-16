#! /bin/bash

function validity() {
local str=$1

if (( ${#str} < 2 ))
   then
     return 0
fi
flag=0
val=0
for (( i=${#str} - 1;i > -1; i-- ))
 do
   if (( flag == 0 ))
     then 
       (( val += ${str:$i:1} ))
     else
       (( temp = ${str:$i:1} ))
       (( temp *=2 ))
       if (( temp > 10 ))
          then 
            (( temp -= 9 ))
       fi
       (( val += temp ))
   fi
   #echo "$flag Flag"   
   (( flag = !flag ))
   #echo "$val Value"

 done

#echo $val
if (( $val%10 ==0 ))
  then
   #echo 11
   return 1
  else
   #echo 00
   return 0
fi

}

read str
str=`echo $str | tr -d ' '`
check=`echo $str | grep -o [^0-9]`

if [[ -z $check ]]
  then
    validity $str
    status=$?
    if (( status )) 
       then
          echo Valid
       else
          echo Invalid
    fi
  else
    echo Invalid
fi
