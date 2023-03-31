import flask
import requests
import threading
import time

main_server = "http://localhost:5050"
global sensors
sensors = {}
time_interval = 5
app = flask.Flask(__name__)
@app.route('/',methods = ["POST"])
def index():
    global sensors
    content = flask.request.json
    sensors[str(content["id"])] = content["val"]
    return flask.Response(status=200)

def check_req():
    while True:
        time.sleep(5)
        global sensors
        print(sensors)
        try:
            res = requests.get(main_server+"/req")
            if(res.ok):
                res = res.json()
                print(res)
                print(sensors)
                status = "ok"
                print(status)
                try:
                    sensor_id = str(res["id"])                    
                    requests.post(main_server+"/res",json={"status":status,"sensor":sensors[sensor_id]})
                except KeyError:
                    status = "not valid id"
                    print(status)
                    requests.post(main_server+"/res",json={"status":status,"sensor":None})
                except TypeError:
                    print("TypeError")
                    pass
        except:
            print("##")
        

th = threading.Thread(target=check_req)
th.start()


app.run(port="18000",debug=False)
th.join()