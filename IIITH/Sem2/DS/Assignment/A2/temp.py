length = [3,5,7]
s = input()
ans = []
pos =0
for l in length:
    ans.append(s[pos:pos+l])
    pos+=l+2

print(ans)