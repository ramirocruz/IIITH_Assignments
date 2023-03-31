# from pyftpdlib.authorizers import DummyAuthorizer
# from pyftpdlib.handlers import FTPHandler
# from pyftpdlib.servers import FTPServer
# addr = ('127.0.0.1',2021)
# authorizer = DummyAuthorizer()
# home = "/home/zcruz/Documents/IIITH_Assignments/IIITH/Sem2/IAS/Assignment/A4"
# authorizer.add_user("user","password",home,perm="elradfmw")
# authorizer.add_anonymous(home,perm="elradfmw")
# handler = FTPHandler
# handler.authorizer = authorizer
# handler.banner = "pyftpdlib based ftpd ready."
# server = FTPServer(addr,handler)

# server.serve_forever()

import flask
from inspect import signature
app = flask.Flask(__name__)
@app.route('/',methods = ['GET'])
def index():
    return "Hello World!"

# write functions here
class Functions:
    def add(self,a:int,b:int) -> int:
        return int(a) + int(b)
    def sub(self,a:int,b:int) -> int:
        return int(a) - int(b)
    def mult(self,a:int,b:int) -> int:
        return int(a) *int(b)
    def div(self,a:int,b:int) -> int:
        return int(a)/int(b)
    def mod(self,a:int,b:int) -> int:
        return int(a)%int(b)
    def inc(self,a:int) -> int:
        return int(a) + 1
    def avg(self,*params:int) ->int:
        sum=0
        for i in params:
            sum+=int(i)
        return sum/len(params)


temp = []
method_list = [func for func in dir(Functions) if callable(getattr(Functions, func))and not func.startswith("__")] 
for method in method_list:
    temp.append("{}".format(method) + str(signature(getattr(Functions,method))))

@app.route('/list',methods=['GET'])
def list():
    return flask.jsonify(temp)

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
    content = flask.request.json
    return stub(content)

if(__name__ == "__main__"):
    app.run(port="5050",debug=True)