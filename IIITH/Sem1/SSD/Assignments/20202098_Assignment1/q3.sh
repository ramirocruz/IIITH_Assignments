#! /bin/bash

cat "${HOME}/.bash_history" | tr -s " " " " | cut -d " " -f1 | tac > hist.txt
k=0
while read i
do
 compgen -c | grep -w -q "$i"
    if (( $? == 0 ))
      then 
        ar[$k]="$i"
        (( k++ ))
    fi
if (( k==10 ))
then 
  break
fi
done < hist.txt 

k=0
cat /dev/null > clist.txt
for i in "${ar[@]}"
do
 if [[ -n "$i" ]]
 then
    cat clist.txt | grep -w -q "$i"
      if (( $? == 1 ))
       then 
         echo "$i" >> clist.txt
         (( k++ ))
      fi
 fi
done

k=0
while read i
 do
 str[k]="$i"
 (( k++ ))
 done < clist.txt
 
 cat /dev/null > chist.txt
 
for i in "${str[@]}"
do
 count=` echo "${ar[@]}" | grep -w -o "$i" | wc -l`
 echo "$i $count" >> chist.txt
done

cat chist.txt | sort -nrk 2,2

rm chist.txt clist.txt hist.txt

