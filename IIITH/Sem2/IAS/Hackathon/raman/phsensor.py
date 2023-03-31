import flask
import random
import datetime
import threading
class PHSensor():
    def __init__(self,id,ip,port):
        self.ip = ip
        self.port = port
        self.sprinkler = False
    
    def getPH(self):
        if(self.sprinkler):
            return random.uniform(7,9)
        return random.uniform(5,7)
    
    def helper(self):
        end_time = datetime.datetime.now() + datetime.timedelta(seconds=10)
        while (datetime.datetime.now() < end_time):
            pass
        self.sprinkler = False

    def setSprinkler(self):
        self.sprinkler = True
        th = threading.Thread(target=self.helper)
        th.start()
    
    def run(self):
        app = flask.Flask(__name__)
        @app.route('/',methods = ["POST"])
        def index():
            content  = flask.request.json
            if(content["func"] == "getPH"):
                return {"data" : self.getPH()}

            if(content["func"] == "setSprinkler"):
                self.setSprinkler()
                return {"data":None}

            return {None}
        
        app.run(host=self.ip,port=self.port,debug=False)

h = PHSensor(1,"0.0.0.0",18000)
h.run()


