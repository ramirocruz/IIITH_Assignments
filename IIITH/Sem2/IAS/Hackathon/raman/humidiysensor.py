import flask
import random
import datetime
import threading
class HumiditySensor():
    def __init__(self,id,ip,port):
        self.ip = ip
        self.port = port
        self.sprinkler = False
    
    def getHumidity(self):
        if(self.sprinkler):
            return random.uniform(30,50)
        return random.uniform(0,30)
    
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
            if(content["func"] == "getHumidity"):
                return {"data" : self.getHumidity()}

            if(content["func"] == "setSprinkler"):
                self.setSprinkler()
                return {"data":None}

            return {None}
        
        app.run(host=self.ip,port=self.port,debug=False)

h = HumiditySensor(1,"0.0.0.0",18000)
h.run()


