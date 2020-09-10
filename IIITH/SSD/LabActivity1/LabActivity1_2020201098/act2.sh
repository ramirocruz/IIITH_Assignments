#! /bin/bash

grep to -o -i -w hamlet.txt | wc -l

grep is  -i -w -h hamlet.txt

grep bear  -i -w -A 2   hamlet.txt |grep to -i -w  -v

chmod a=--- hamlet.txt
chmod 000 hamlet.txt

chmod 777 hamlet.txt

groups

chown :sudo hamlet.txt


