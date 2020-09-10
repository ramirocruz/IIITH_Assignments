#! /bin/sh
#currently in root directory
#changing directory using absolute path
cd /usr/local/lib  

#changing back to previous working directory
cd -

#changing directory using relative path
cd usr/src

#going to user home directory
cd

#create a directory in Documents

mkdir Documents/LabActivity1

#entering this directory
cd Documents/LabActivity1

echo $PWD
