import csv
import heapq
import os
import sys
import math
from itertools import islice

tablelist = []
opfile = []
class Chunk:
    def __init__(self,data,t):
        self.data = data
        self.t = t
    def __lt__(self, value):
        if(self.t == 1):
                pos = 1
                return (self.data[pos] < value.data[pos])

        else:
                pos = 0
                return (self.data[pos] < value.data[pos])
                   
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
    table["chunks"] = math.ceil(table["rows"]/table["blocks"])
       
    return 0

def get_data(table,n):
    f = open(table["input_file"],"r")
    f.seek(table["row_len"]*table["blocks"]*n,os.SEEK_SET)
    size = 0
    temp_chunk = []
    while True:
        if(size == table["blocks"]):
            break
        x = f.readline()
        if not x:
            break
        temp_chunk.append(Chunk(x.split(),table["t"]))
        size+=1

    f.close()
    temp_chunk.sort()
    with open("temp_chunk{}{}".format(table["name"],n),"w") as fw:            
        for row in temp_chunk:
            for i in range(2):
                if(i==0):
                    fw.write(row.data[i]+" ")
                else:
                    fw.write(row.data[i]+"\n")
    del temp_chunk[:]

def make_heap(table):
    subfiles = []
    heap = []
    print("Making Heap")
    try:
        for l in range(table["chunks"]):
            subfiles.append(open("temp_chunk{}{}".format(table["name"],l),"r"))
            x = subfiles[l].readline()
            x = x.split()
            x.append(l)
            heap.append(Chunk(x,table["t"]))
    except OSError:
        for l in range(len(subfiles)):
            subfiles[l].close()
            os.remove("temp_chunk{}{}".format(table["name"],l))
        return 1
    heapq.heapify(heap)
    table["heap"] = heap
    table["sublist"] = subfiles


def OPEN():
    for table in tablelist:
        for i in range(table["chunks"]):
            get_data(table,i)
        make_heap(table)


def getnext(table):
    heap = table["heap"]
    subfiles = table["sublist"]

    if (len(heap) != 0):
        x  = heapq.heappop(heap)
        temp = subfiles[x.data[-1]].readline()
        if temp:
            temp_row=temp.split()
            temp_row.append(x.data[-1])
            heapq.heappush(heap,Chunk(temp_row,table["t"]))
        
        return x.data[:-1]
    else:
        return None

def CLOSE():
    for table in tablelist:
        for l in range(len(table["sublist"])):
            table["sublist"][l].close()
            os.remove("temp_chunk{}{}".format(table["name"],l))


def SORT(table):
    op = open(table["name"]+"op","w")
    data = getnext(table)
    while(data != None):
        op.write(data[0] + " " + data[1] + "\n")
        data = getnext(table)
    op.close()

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
    table1["blocks"] = int(cmd[3])*100    
    table2["blocks"] = int(cmd[3])*100
    stats1 = check(table1)
    stats2 = check(table2)
    if(stats1==1 or stats2 == 1):
        return 1
    if(table1["chunks"]+table2["chunks"] >= (table1["blocks"]//100)**2):
        return 2

    tablelist.append(table1)
    tablelist.append(table2)

    print("Running OPEN function")
    OPEN()
    SORT(table1)
    SORT(table2)
    
    print("Closing Files...")
    CLOSE()

    f1 = open(table1["name"]+"op","r")
    f2 = open(table2["name"]+"op","r")

    f1pos = 0
    f2pos = 0
    M = table1["blocks"]//100
    table1data = f1.readline()
    table2data = list(islice(f2,(M-1)*100))
    opfile = open(table1["name"]+"_"+table2["name"]+"_join.txt","w")
    start = 0
    while(table1data and len(table2data)):
        data1 = table1data.split()
        # print(len(table2data))
        data2 = table2data[start].split()
        if(data1[1] < data2[0]):
            f1pos = f1.tell()
            table1data = f1.readline()
        elif(data1[1] > data2[0]):
            f2pos += table2["row_len"]
            start+=1
            if(start >= len(table2data)):
                f2.seek(f2pos,os.SEEK_SET)
                table2data = list(islice(f2,(M-1)*100))
                start = 0
        else:
            while(data1[1] == data2[0]):
                opfile.write(data1[0]+" "+data1[1] + " " + data2[1]+ "\n")
                start+=1
                if (start >= len(table2data)):
                    f2.seek(f2pos,os.SEEK_SET)
                    table2data = list(islice(f2,(M-1)*100))
                    start = 0
                    break                    
                data2 = table2data[start]
            
            f2.seek(f2pos,os.SEEK_SET)
            table1data = f1.readline()
            table2data = list(islice(f2,(M-1)*100))

    f1.close()
    f2.close()
    os.remove(table1["name"]+"op")
    os.remove(table2["name"]+"op")
    return 0




if(len(sys.argv) != 5):
    print("Invalid arguments...")
    sys.exit(1)
stat = phase1(sys.argv[1:])
if(stat==0):
   print("Success...")
elif(stat==1):
    print("No such files....")
else:
    print("Error....")


        