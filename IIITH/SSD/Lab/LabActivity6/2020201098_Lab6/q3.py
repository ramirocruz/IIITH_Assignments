str_list = input().split(',')
str_list = list(str_list)

count=0
str_list=[str.strip() for str in str_list]
newlist = list(filter(lambda str: str[0].isupper(),str_list))
for el in newlist:
    count+=len(el)

print(count)
print(newlist)