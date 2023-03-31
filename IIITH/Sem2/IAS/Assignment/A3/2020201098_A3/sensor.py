import requests
import threading
import secrets
import sys
server_url = "http://localhost:18000"
time_interval = 5
class Sensor:
    def __init__(self,id):
        self.id = id
    def ping(self):
        requests.post(server_url,json={'id':self.id,'val':secrets.randbits(8)})
        threading.Timer(time_interval,self.ping).start()
        

if(len(sys.argv)==2):
    s = Sensor(str(sys.argv[1]))
else:
    s = Sensor(str(secrets.randbits(8)))

print(s.id)
s.ping()