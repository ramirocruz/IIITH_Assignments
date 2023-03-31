from flask import *
from inspect import signature
app = Flask(__name__)
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
    def rand(self,a:int) -> int:
        return 7*int(a)


temp = []
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

if(__name__ == "__main__"):
    app.run(port="5050",debug=True)
