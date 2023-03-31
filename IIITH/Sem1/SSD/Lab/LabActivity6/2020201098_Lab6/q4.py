def btod(bits):
    digit=0
    for bit in bits:
        digit = digit*2 + ord(bit)-48
    return digit

str_list = input().split(',')
str_list = list(str_list)
str_list=[str.strip() for str in str_list]
new_list = list(filter(lambda bits:btod(bits)%5==0,str_list))
new_u_list = set(new_list)

print(new_u_list)

