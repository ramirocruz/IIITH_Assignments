str = input().strip()
count = [0 for item in range(26)]
for char in str:
    if(char.isalpha()):
        count[ord(char.lower())-97] += 1
flag=True
for el in count:
    if(el == 0):
        flag=False
        break

if(flag):
    print("YES")
else:
    print("NO")
