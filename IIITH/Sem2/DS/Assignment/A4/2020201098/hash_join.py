import csv
import heapq
import os
import sys
import math
from itertools import islice
tablelist = []
opfile = []
                   
def check(table):
    try:
        f = open(table["input_file"],"r")
    except FileNotFoundError:
        return 1
    table["total"] = os.path.getsize(table["input_file"])
    with open(table["input_file"],"r") as f:
        f.readline()
        table["row_len"] = f.tell()
    table["rows"] = table["total"]//table["row_len"]
       
    return 0

def hash_data(table):
    f = open(table["input_file"],"r")
    M = (table["blocks"]) - 1

    sublist = []
    for i in range(M):
        sublist.append(open(table["name"]+"_temp{}".format(i),"w"))
    
    x = f.readline()
    while x:
        data = x.split()
        idx = -1
        if(table["t"]==1):
            idx = hash(data[1])%M
        else:
            idx = hash(data[0])%M

        sublist[idx].write(data[0]+ " "+ data[1] + "\n")
        x = f.readline()

    f.close()
    for i in range(M):
        sublist[i].close()


def OPEN():
    for table in tablelist:
        hash_data(table)       




def CLOSE():
    for table in tablelist:
        for l in range(len(table["sublist"])):
            table["sublist"][l].close()
            os.remove("{}_temp{}".format(table["name"],l))

    opfile[0].close() 


def create_file_ptr(table):
    M = (table["blocks"]) - 1
    sublist = []
    for i in range(M):
        sublist.append(open(table["name"]+"_temp{}".format(i),"r"))
    table["sublist"] = sublist

def JOIN(n,table1,table2):
    row1 = os.path.getsize(table1["name"]+"_temp{}".format(n))//table1["row_len"]
    row2 = os.path.getsize(table2["name"]+"_temp{}".format(n))//table2["row_len"]
    outerptr = table1["sublist"][n]
    innerptr = table2["sublist"][n]
    op = opfile[0]
    posX = 1
    posY = 0
    flag = True
    if(row1 > row2):
        outerptr,innerptr = innerptr,outerptr
        posX,posY = posY,posX
        flag = False

    tempdata = outerptr.readlines()
    outerdict = {}
    for row in tempdata:
        idx = row.split()[posX]
        outerdict[idx] = []
    
    for row in tempdata:
        idx = row.split()[posX]
        outerdict[idx].append(row.split())
    
    tempinner = list(islice(innerptr,100))
    while(len(tempinner) > 0):
        for row in tempinner:
            row = row.split()
            idx = row[posY]
            try:
                for data in outerdict[idx]:
                    if(flag):
                        op.write(data[0]+" "+ data[1]+" "+ row[1]+"\n")
                    else:
                        op.write(row[0]+" "+ row[1]+" "+ data[1]+"\n")
            except KeyError:
                continue
        
        tempinner = list(islice(innerptr,100))
    


def check_size(n,table1,table2):
    row1 = os.path.getsize(table1["name"]+"_temp{}".format(n))//table1["row_len"]
    row2 = os.path.getsize(table2["name"]+"_temp{}".format(n))//table2["row_len"]
    if(min(row1,row2) > (table1["blocks"] - 1)*100):
        return False
    return True

def phase1(cmd):
    table1 = {}
    table2 = {}

    table1["input_file"] = cmd[0]
    table2["input_file"] = cmd[1]
    table1["name"] = os.path.basename(cmd[0])
    table2["name"] = os.path.basename(cmd[1])
    table1["t"] = 1
    table2["t"] = 2
    table1["mode"] = cmd[2]
    table2["mode"] = cmd[2]
    table1["blocks"] = int(cmd[3])
    table2["blocks"] = int(cmd[3])
    stats1 = check(table1)
    stats2 = check(table2)
    if(stats1==1 or stats2 == 1):
        return 1


    tablelist.append(table1)
    tablelist.append(table2)

    print("Running OPEN function")
    OPEN()
    print("opened")
    for i in range(table1["blocks"] - 1):
        if not check_size(i,table1,table2):
            return 2
    opfilename = table1["name"] + "_" +table2["name"] + "_join.txt"
    opfile.append(open(opfilename,"w"))
    print("Condition Checked..")
    create_file_ptr(table1)
    create_file_ptr(table2)
    print("File ptr created...")
    for i in range(table1["blocks"] - 1):
        JOIN(i,table1,table2)

        
    print("Closing Files...")
    CLOSE()

    return 0




if(len(sys.argv) != 5):
    print("Invalid arguments...")
    sys.exit(1)
stat = phase1(sys.argv[1:])
if(stat==0):
   print("Success...")
elif(stat==1):
    print("No such files....")
elif(stat==2):
    print("Not enough Memory....")
else:
    print("Error....")


        