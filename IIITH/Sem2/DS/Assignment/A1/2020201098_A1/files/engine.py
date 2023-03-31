import sqlparse
import re
import csv
from functools import cmp_to_key
from math import inf
database = {}
def fill_db_meta(meta):
    with open(meta,"r") as f:
        metadata = f.readlines()
    lines = len(metadata)
    i=0
    while(i<lines):
        if "<begin_table>" in metadata[i]:
            i=i+1
            table = {}
            database[metadata[i].strip()] = table
            table["cols"] = {}
            table["cols_order"] = []
            i=i+1
            while True:
                if "<end_table>" in metadata[i].strip():
                    break
                table["cols"][metadata[i].strip()]=[]
                table["cols_order"].append(metadata[i].strip())
                i=i+1
            
        i=i+1

aggrs = {"sum":0,"avg":1,"max":2,"min":3,"count":4}
def fill_db_data():
    for table in database.keys():
        with open(table+".csv","r") as f:
            data = csv.reader(f)
            for row in data:
                for i in range(len(row)):
                    database[table]["cols"][database[table]["cols_order"][i]].append(int(row[i]))

def recur(tables,newrow,newtable):
    if(len(tables)==0):
        newtable.append(newrow)
        return
    curtable = tables[0]
    temprow=newrow[:]
    for j in range(len(curtable["cols"][curtable["cols_order"][0]])):
        for i in range(len(curtable["cols_order"])):
            newrow.append(curtable["cols"][curtable["cols_order"][i]][j])
        recur(tables[1:],newrow,newtable)
        newrow=temprow[:]

def join_tables(tablelist):
    newtable={}
    newtable["cols"] = {}
    newtable["rows"] = []
    tables = []
    for table in tablelist:
        tables.append(database[table])
    i=0
    for table in tables:
        for col in range(len(table["cols_order"])):
            newtable["cols"][table["cols_order"][col]] = i
            newtable["cols"][i] = table["cols_order"][col]
            i+=1
    recur(tables,[],newtable["rows"])
    return newtable

def f_count(newtable,cmd):
        return len(newtable["rows"])
    
def f_sum(newtable,cmd):
    sums = 0
    pos = newtable["cols"][cmd.strip()]
    for row in newtable["rows"]:
            sums+=row[pos]
    return sums

def f_avg(newtable,cmd):
    sums = f_sum(newtable,cmd)
    n = len(newtable["rows"])
    avg = sums/n
    return avg

def f_max(newtable,cmd):
    maxs = -inf
    pos = newtable["cols"][cmd.strip()]
    for row in newtable["rows"]:
            maxs = max(maxs,row[pos])
    return maxs

def f_min(newtable,cmd):
    mins = inf
    pos = newtable["cols"][cmd.strip()]
    for row in newtable["rows"]:
            mins = min(mins,row[pos])
    return mins

def aggr(newtable,params):
    cmd = [i.strip() for i in re.split("\(|\)",params) if i.strip()!=""]
    res = 0
    if(cmd[0]=="count"):
        res= f_count(newtable,cmd[1])
    elif(cmd[0]=="sum"):
        res= f_sum(newtable,cmd[1])
    elif(cmd[0]=="avg"):
        res= f_avg(newtable,cmd[1])
    elif(cmd[0]=="max"):
        res= f_max(newtable,cmd[1])
    elif(cmd[0]=="min"):
        res= f_min(newtable,cmd[1])
    return res

def group_by_parse(group_by_part,newtable):
    cmd = group_by_part.split(",")
    pos = [newtable["cols"][i.strip()] for i in cmd]

def order_by_parse(order_by_part,newtable):
    cmd = order_by_part.split(",")
    cmd = [i.split() for i in cmd]
    print(cmd)
    def comp(val1,val2):
        for i in cmd:
            if(len(i)==1):
                pos = newtable["cols"][i[0]]
                if(val1[pos] < val2[pos]):
                    return -1
                elif(val1[pos] > val2[pos]):
                    return 1
            else:
                pos = newtable["cols"][i[0]]
                if "desc" in i:
                    if(val1[pos] > val2[pos]):
                        return -1
                    elif(val1[pos] < val2[pos]):
                        return 1
                else:
                    if(val1[pos] < val2[pos]):
                        return -1
                    elif(val1[pos] > val2[pos]):
                        return 1
        return 0
    newtable["rows"].sort(key=cmp_to_key(comp))

def show_table(newtable):
    newtable["rows"].insert(0,newtable["ord"])
    for row in newtable["rows"]:
        for els in row:
            if(els==None):
                print("",end="\t")
            else:
                print(els,end="\t")
        print("")

def where_parse(where_part,newtable):
    temp = where_part
    where_part = re.split("and|or",where_part)
    where_part = [i.strip() for row in where_part for i in re.split("<=|>=|!=|==|<|>",row) if i.strip() != ""] 
    print(where_part)
    temptable = []
    for row in newtable["rows"]:
        tempstr = temp
        for els in where_part:
            if(els.isnumeric()):
                pass
            else:
                val = row[int(newtable['cols'][els])]
                tempstr = tempstr.replace(els,str(val))
        if(eval(tempstr)):
            temptable.append(row)

    newtable["rows"] = temptable

def make_distinct(newtable):
    b = []
    for row in newtable["rows"]:
        if row not in b:
            b.append(row)
    newtable["rows"] = b

def select_parse(select_part,newtable):
    isdistict = False
    isaggr = False
    if "distinct" in select_part:
        isdistict = True
    col_list = [cols.strip() for temp in re.split("distinct",select_part) for cols in temp.split(",") if (cols.strip() != "")]
    if(len(col_list)==0):
        print("No column selected")
    if(len(col_list)==1 and col_list[0] == "*"):
        newrow = []
        for i in newtable["cols"].keys():
            if(str(i).isnumeric()):
                pass
            else:
                newrow.append(i)
        
        if(isdistict):
            make_distinct(newtable)

        newtable["ord"]=newrow
        ord = {}
        for i in range(len(newrow)):
            ord[newrow[i]] = i
            ord[i] = newrow[i]
        newtable["cols"] = ord
        return
    
    temptable=[]
    for j in col_list:
        temp = re.split("\(|\)",j)[0].strip()
        try:
            aggrs[temp]
            isaggr=True
        except KeyError:
            pass
    if(isaggr):
        temprow=[]
        for j in col_list:
            temp = re.split("\(|\)",j)[0].strip()
            print(temp)
            try:
                aggrs[temp]
                temprow.append(aggr(newtable,j))
            except KeyError:
                try:
                    temprow.append(newtable["row"][0][newtable["cols"][j]])
                except KeyError:
                    temprow.append(None)

        newtable["rows"][:] = [col_list,temprow]
    else:
        for i in newtable["rows"]:
            temprow=[]
            for j in col_list:        
                temprow.append(i[newtable["cols"][j]])
            temptable.append(temprow)
        newtable["rows"] = temptable
    if(isdistict):
        make_distinct(newtable)

    newtable["ord"]=col_list
    ord = {}
    for i in range(len(col_list)):
        ord[col_list[i]] = i
        ord[i] = col_list[i]
    newtable["cols"] = ord

def parse(query):
    query = sqlparse.format(query,keyword_case  = 'lower')
    if ";" not in query:
        print("No ;")
        return
    if "select" not in query or "from" not in query:
        print("No select or from")
        return
    select_part=""
    tables=[]
    from_part=""
    where_part=""
    group_by_part=""
    order_by_part=""
    query =  query.strip(";")
    q = query

    select_part = re.split("select",q)[1].strip()
    from_part = re.split("from",select_part)
    select_part = from_part[0].strip()
    from_part = from_part[1].strip()
    if "where" in from_part:
        where_part = re.split("where",from_part)[1].strip()
        where_part = re.split("group by|order by",where_part)[0].strip()
    from_part = re.split("where|group by|order by",from_part)[0].strip()
    if "order by" in q:
        order_by_part = re.split("order by",q)[1].strip()
    if "group by" in q:
        if order_by_part=="":
            group_by_part = re.split("group by",q)[1].strip()
        else:
            group_by_part = re.split("group by",q)[1]
            group_by_part = re.split("order by",group_by_part)[0].strip()

    tables = [tabs.strip() for tabs in from_part.split(",")]
    newtable = join_tables(tables)
    if(len(newtable["cols"]) == 0):
        print("No such tables")
        return
    if(where_part):
        where_parse(where_part,newtable)
    if(group_by_part):
        group_by_parse(group_by_part,newtable)
    select_parse(select_part,newtable)
    if(order_by_part):
        order_by_parse(order_by_part,newtable)
    
    show_table(newtable)
                

while True:
    fill_db_meta("metadata.txt")
    fill_db_data()
    q = input()
    parse(q)
