import requests
import os
import json

class RPC:
    home = "http://localhost:5050/"
    config = None
    def __init__(self):
        self.config = self.get_data_from_file()

    def update(self):
        res = requests.get(self.home+"list")
        if(res.ok):
            self.config = res.json()
            self.push_data_to_file()
            print("Update Successful..")
            return True
        print("Update Failed..")
        return False

    def get_function_list(self):
        return self.config["function"]

    def get_data_from_file(self):
        with open('config.json','r') as f:
            data = json.loads(f.read())
        return data

    def push_data_to_file(self):
        with open('config.json','w') as f:
            f.write(json.dumps(self.config))
        
    def fcall(self,params):
        
        res = requests.post(self.home+"api",json=params)
        if(res.ok):
            ans = res.json()
            if(ans["status"] != "ok"):
                return ans["status"]
            else:
                return ans["value"]
        return "Network Error"
