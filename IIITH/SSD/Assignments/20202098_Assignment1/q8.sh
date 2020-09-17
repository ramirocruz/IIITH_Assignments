#! /bin/bash

function multicheck()
{
local str=$1
IFS=','
#echo "$str"
read -ra nums <<< "$str"
#echo ${nums[0]}
for (( i=0; i < ${#nums} -1; i++ ))
do
   if (( ${nums[$i]} > ${nums[$i+1]} ))
     then
      return 0
   fi
done
return 1
     
}

function rangecheck()
{
local str=$1
IFS='-'
read -ra num <<< "$str"
#echo "${num[0]} and ${num[1]}"
if (( ${num[0]} <= ${num[1]} ))
then
return 1
else
return 0
fi

}

function wordcheck()
{
local str=$1
local words="@yearly @weekly @daily @monthly @reboot @hourly"
echo $words | grep -w -q "$str"
 if (( $? == 0 ))
   then
   return 1
   else
   return 0
 fi
}

function validate(){
local str=$1
local n=$2
#echo "$str  passed string  $n  passed index"
 case $n in
  0 ) 
  
    echo "$str" | grep -E -q '^[0-5]?[0-9]$|^[\*]$|^[\*][\/][1-9][0-9]*$'
        if (( $? == 0 ))
          then 
            return 1
          else
            echo "$str" | grep -E -q '^[0-5]?[0-9]-[0-5]?[0-9]$'
            if (( $? == 0 ))
              then 
               rangecheck "$str"
               return $?
              else
                echo "$str" | grep -E -q '^[0-5]?[0-9](,[0-5]?[0-9])+$'
                  if (( $? == 0 ))
                   then
                    multicheck "$str"
                    return $?
                   else
                    return 0
                  fi
            fi
        fi       
  ;;
  
  1 )  
  
    echo "$str" | grep -E -q '^[0-1]?[0-9]$|^[2][0-3]$|^[\*]$|^[\*][\/][1-9][0-9]*$'
        if (( $? == 0 ))
          then 
            return 1
          else
            echo "$str" | grep -E -q '(^[0-1]?[0-9]|^[2][0-3])-([0-1]?[0-9]|[2][0-3])$'
            if (( $? == 0 ))
              then 
               rangecheck "$str"
               return $?
              else
                echo "$str" | grep -E -q '(^[0-1]?[0-9]|^[2][0-3])(,([0-1]?[0-9]|[2][0-3]))+$'
                  if (( $? == 0 ))
                   then
                    multicheck "$str"
                    return $?
                   else
                    return 0
                  fi
            fi
        fi    
        
  ;;
  2 ) 
  
    echo "$str" | grep -E -q '^[0]?[1-9]$|^[1-2][0-9]$|^[3][0-1]$|^[\*]$|^[\*][\/][1-9][0-9]*$'
        if (( $? == 0 ))
          then 
            return 1
          else
             echo "$str" | grep -E -q '(^[0]?[1-9]|^[1-2][0-9]|^[3][0-1])-([0]?[1-9]|[1-2][0-9]|[3][0-1]])$'
            if (( $? == 0 ))
              then 
               rangecheck "$str"
               return $?
              else
                echo "$str" | grep -E -q '(^[0]?[1-9]|^[1-2][0-9]|^[3][0-1])(,([0]?[1-9]|[1-2][0-9]|[3][0-1]))+$'
                  if (( $? == 0 ))
                   then
                    multicheck "$str"
                    return $?
                   else
                    return 0
                  fi
            fi
        fi   
  
  ;;
  3 ) 
  
     echo "$str" | grep -E -q '^[0]?[1-9]$|^[1][0-2]$|^[\*]$|^[\*][\/][1-9][0-9]*$'
        if (( $? == 0 ))
          then 
            return 1
          else
            echo "$str" | grep -E -q '(^[0]?[1-9]|^[1][0-2])-([0]?[1-9]|[1][0-2])$'
            if (( $? == 0 ))
              then 
               rangecheck "$str"
               return $?
              else
                echo "$str" | grep -E -q '(^[0]?[1-9]|^[1][0-2])(,([0]?[1-9]|[1][0-2]))+$'
                  if (( $? == 0 ))
                   then
                    multicheck "$str"
                    return $?
                   else
                    return 0
                  fi
            fi
        fi   
  
  
  ;;
  4 ) 
     
      echo "$str" | grep -E -q '^[0-6]$|^[\*]$|^[\*][\/][1-9][0-9]*$'
        if (( $? == 0 ))
          then 
            return 1
          else
            echo "$str" | grep -E -q '^[0-6]-[0-6]$'
            if (( $? == 0 ))
              then 
               rangecheck "$str"
               return $?
              else
                echo "$str" | grep -E -q '^[0-6](,[0-6])+$'
                  if (( $? == 0 ))
                   then
                    multicheck "$str"
                    return $?
                   else
                    return 0
                  fi
            fi
        fi   
  
  
  ;;
  5 ) 
     
     echo "$str" | grep -E -q '^[\/][^\/]+([\/][^\/]+)*$'
        if (( $? == 0 ))
          then 
            return 1
          else         
            compgen -c | grep -w -q $str
            if (( $? == 0 ))
             then
              return 1
             else
              return 0
            fi

        fi     
  
  ;;
  * ) 
  
  return 0
  
  ;;
 esac
  

}

str=`cat $1| sed -e 's/,\s\+/,/g'`
#echo "${str}"
IFS=' '
read -ra ar <<< "$str"

k=${#ar[@]}
#echo $k
 flag=0
if (( k != 6 ))
  then 
     if (( k == 2 ))
       then 
        wordcheck "${ar[0]}"
         if (( $?==1 ))
           then 
             validate "${ar[1]}" 5
               if (( $?==1 ))
                 then 
                 echo Yes
                 else
                 echo No
               fi
            else
            echo No
         fi
       else
       echo No
     fi
  else
    for i in ${!ar[@]}
     do
       validate "${ar[$i]}" $i
       if (( $?==1 ))
          then 
            continue
          else 
            echo No
            flag=1
            break
       fi
     done
     if (( flag==0 ))
      then 
        echo Yes
     fi

fi
