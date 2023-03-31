from flask import *
from inspect import signature
app = Flask(__name__)
@app.route('/',methods = ['GET'])
def index():
    return "Hello World!"

sensor_req = None
sensor_res = None
temp = []

def fetch(id):
    global sensor_res
    global sensor_req
    sensor_req = id
    while(sensor_res == None):
        pass
    temp = sensor_res
    sensor_res = None
    sensor_req = None
    return {"id":id,"status":temp["status"],"val":temp["sensor"]}


class Functions:
    def get_sensor_data(self,id:str)-> dict:
        return fetch(id)



method_list = [func for func in dir(Functions) if callable(getattr(Functions, func))and not func.startswith("__")] 
for method in method_list:
    temp.append("{}".format(method) + str(signature(getattr(Functions,method))))

@app.route('/list',methods=['GET'])
def list():
    return jsonify(temp)

def stub(params):
    res = "ok"
    try:        
        val = getattr(Functions,params[0])(*params)
    except TypeError:
        val = None
        res = "Uneven Arguments"
    except AttributeError:
        val = None
        res = "Function Not Present"
    except ZeroDivisionError:
        val = None
        res = "Zero Division Error"
    return {"status":res,"value":val}


@app.route('/api',methods = ['POST'])
def api():
    content = request.json
    return stub(content)

@app.route('/req',methods = ["GET"])
def req_method():
    global sensor_req
    if(sensor_req!=None):
        temp = sensor_req
        sensor_req = None
        return {"id":temp}
    return Response(status=404)



@app.route('/res',methods = ['POST'])
def res_method():
    global sensor_req
    global sensor_res
    sensor_req = None
    content = request.json
    sensor_res = content
    return Response(status=200)

if(__name__ == "__main__"):
    app.run(port="5050",debug=False)
    