import requests

url = "http://0.0.0.0:18000"
while (True):
    funcname = input() 
    res = requests.post(url,json={"func":funcname})
    print(res.json())

