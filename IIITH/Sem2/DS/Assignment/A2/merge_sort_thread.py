import csv
import heapq
import os
import sys
import threading
table = {}
def get_meta():
    with open("metadata.txt","r") as f:
        data = csv.reader(f)    
        table["cols"]={}
        table["ord"]=[]
        i=0
        sum=0
        for row in data:
            table["cols"][row[0]] = i
            i+=1
            table["ord"].append(int(row[1]))
            sum+= int(row[1])
        table["sum"] = sum


class Chunk:
    def __init__(self,data):
        self.data = data
    def __lt__(self, value):
        if(table["mode"]=="asc"):
            for col in table["req"]:
                pos = table["cols"][col]
                if(self.data[pos] < value.data[pos]):
                    return True
                elif(self.data[pos] > value.data[pos]):
                    return False
            return True
        else:
            for col in table["req"]:
                pos = table["cols"][col]
                if(self.data[pos] > value.data[pos]):
                    return True
                elif(self.data[pos] < value.data[pos]):
                    return False
            return False

def check():
    try:
        f = open(table["input_file"],"r")
    except FileNotFoundError:
        return 1
    table["total"] = os.path.getsize(table["input_file"])
    with open(table["input_file"],"r") as f:
        f.readline()
        table["row_len"] = f.tell()
    table["rows"] = table["total"]//table["row_len"]
    table["effective_mem"] = table["memlimit"]//table["threads"]
    table["chunk_len"] = min(table["effective_mem"]//table["row_len"],table["rows"])
    table["chunks"] = table["rows"]//table["chunk_len"]
    table["thread_len"] = table["chunks"]//table["threads"]
    if(table["chunks"]*table["row_len"] > table["memlimit"]):
        return 2
    if(table["chunks"]>1022):
        return 3
    print(table)
    return 0

def get_data(n):
    f = open(table["input_file"],"r")
    f.seek(table["row_len"]*table["chunk_len"]*n,os.SEEK_SET)
    size = 0
    temp_chunk = []
    while True:
        if(size == table["chunk_len"]):
            break
        x = f.readline()
        if not x:
            break
        temp_chunk.append(Chunk([]))
        size+=1
        pos = 0
        for length in table["ord"]:
            temp_chunk[-1].data.append(x[pos:pos+length])
            pos+=length+2
    f.close()
    temp_chunk.sort()
    with open("temp_chunk{}".format(n),"w") as fw:            
        for row in temp_chunk:
            for i in range(len(table["ord"])):
                if(i < len(table["ord"])-1):
                    fw.write(row.data[i]+"  ")
                else:
                    fw.write(row.data[i]+"\n")

def thread_part(n):
    i=0
    while (i < table["thread_len"]):
        if(n*table["thread_len"] + i >= table["chunks"]):
            break
        get_data(n*table["thread_len"] + i)
        i+=1

def phase1(cmd):
    table["input_file"] = cmd[0]
    table["output_file"] = cmd[1]
    table["memlimit"] = int(cmd[2])*1048576
    table["threads"] = int(cmd[3])
    table["mode"] = cmd[4].lower()
    table["req"] = cmd[5:]
    stats = check()
    if(stats==1):
        return 1
    if(stats==2):
        return 2
    if(stats==3):
        return 3
    threadlist = []
    for i in range(table["threads"]):
        threadlist.append(threading.Thread(target=thread_part,args=[i]))
        threadlist[i].start()
    
    for i in range(table["threads"]):
        threadlist[i].join()

    return 0


def phase2():
    subfiles = []
    heap = []
    try:
        for l in range(table["chunks"]):
            subfiles.append(open("temp_chunk{}".format(l),"r"))
            heap.append(Chunk([]))
            pos=0
            x = subfiles[l].readline()
            for length in table["ord"]:
                heap[-1].data.append(x[pos:pos+length])
                pos+=length+2
            heap[-1].data.append(l)
    except OSError:
        for l in range(len(subfiles)):
            subfiles[l].close()
            os.remove("temp_chunk{}".format(l))
        return 1   
    heapq.heapify(heap)
    op = open(table["output_file"],"w")

    while (len(heap)):
        x  = heapq.heappop(heap)
        n = len(table["ord"])
        for i in range(n):
            if(i < n-1):
                op.write(x.data[i]+"  ")
            else:
                op.write(x.data[i]+"\n")

        temp = subfiles[x.data[-1]].readline()
        if not temp:
            continue
        temp_row=[]
        pos=0
        for length in table["ord"]:
            temp_row.append(temp[pos:pos+length])
            pos+=length+2 
        temp_row.append(x.data[-1])
        heapq.heappush(heap,Chunk(temp_row))
    op.close()
    for l in range(len(subfiles)):
        subfiles[l].close()
        os.remove("temp_chunk{}".format(l))
    return 0


if(len(sys.argv) < 6):
    print("Invalid arguments...")
    sys.exit(1)
get_meta()
stat = phase1(sys.argv[1:])
if(stat==0):
    if(phase2()==0):
        print("Successful...")
    else:
        print("Too many chunks....")
elif(stat==1):
    print("File Not Present")
elif(stat==2):
    print("Not Enough Memory")
else:
    print("Too many chunks...")


        