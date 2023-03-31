from flask import *
import requests

class RPC:
    home = "http://localhost:8888/"
    def get_function_list(self):
        res = requests.get(self.home+"list")
        return res.json()

    def fcall(self,params):
        res = requests.post(self.home+"api",json=params)
        if(res.ok):
            ans = res.json()
            if(ans["status"] != "ok"):
                return ans["status"]
            else:
                return ans["value"]
        return "Network Error"
