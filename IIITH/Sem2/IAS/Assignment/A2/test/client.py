from lib import RPC

rpc = RPC()

for i in rpc.get_function_list():
    print(i)


while(True):
    params = input().split()
    ans = rpc.fcall(params)
    print(ans)