import csv
import heapq
import os
import sys
import math

tablelist = []
opfile = []
class Chunk:
    def __init__(self,data,t):
        self.data = data
        self.t = t
    def __lt__(self, value):
        if(self.t == 1):
                pos = 0
                return (self.data[pos] < value.data[pos])

        else:
                pos = 1
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

def OPEN():
    for table in tablelist:
        for i in range(table["chunks"]):
            get_data(table,i)

def make_heap(table):
    subfiles = []
    heap = []
    print("Making Heap")
    try:
        for l in range(table["chunks"]):
            subfiles.append(open("temp_chunk{}{}".format(table["name"],l),"r"))
            x = subfiles[l].readline()
            heap.append(Chunk([x.split()]))
    except OSError:
        for l in range(len(subfiles)):
            subfiles[l].close()
            os.remove("temp_chunk{}{}".format(table["name"],l))
        return 1
    heapq.heapify(heap)
    table["heap"] = heap
    table["sublist"] = subfiles

def getnext(table):
    heap = table["heap"]
    subfiles = table["sublist"]

    if (len(heap) != 0):
        x  = heapq.heappop(heap)
        temp = subfiles[x.data[-1]].readline()
        if temp:
            temp_row=temp.split()
            temp_row.append(x.data[-1])
            heapq.heappush(heap,Chunk(temp_row))
        
        return x[:-1]
    else:
        return None
def CLOSE():
    for table in tablelist:
        for l in range(len(table["sublist"])):
            table["sublist"][l].close()
            os.remove("temp_chunk{}{}".format(table["name"],l))

    opfile[0].close() 


def phase1(cmd):
    table1 = {}
    table2 = {}

    table1["input_file"] = cmd[0]
    table2["input_file"] = cmd[1]
    table1["name"] = os.path.basename(cmd[0])
    table2["name"] = os.path.basename(cmd[1])
    table1["type"] = 1
    table2["type"] = 2
    table1["mode"] = cmd[2]
    table2["mode"] = cmd[2]
    table1["blocks"] = int(cmd[3])*100    
    table2["blocks"] = int(cmd[3])*100
    stats1 = check(table1)
    stats2 = check(table2)
    if(stats1==1 or stats2 == 1):
        return 1
    if(table1["chunks"]*table2["chunks"] >= table1["blocks"]**2):
        return 2

    tablelist.append(table1)
    tablelist.append(table2)

    print("Running OPEN function")
    OPEN()

    tab1data = getnext(table1)
    tab2data = getnext(table2)
    opfilename = table1["name"] + "_" +table2["name"] + "_join.txt"
    opfile.append(open(opfilename,"w"))
    while(tab1data != None and tab2data != None):
        if(tab1data[1] < tab2data[0]):
            tab1data = getnext(table1)
        elif(tab1data[1] > tab2data[0]):
            tab2data = getnext(table2)
        else:
            opfile[0].write(tab1data[0]+" "+tab1data[1]+" "+tab2data[1]+"\n")
    
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
else:
    print("Error....")


        