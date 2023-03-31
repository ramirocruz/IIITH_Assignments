str_list = input().split(',')
str_list = list(str_list)
count_list=[]
print(str_list)
for i in str_list:
    count_list.append(len(i.strip()))
print(count_list)
