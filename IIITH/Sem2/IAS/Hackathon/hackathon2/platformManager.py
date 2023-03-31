from flask import Flask, render_template, request
#from werkzeug import secure_filename
import os
import pymongo
import json
import bson
import base64
from bson.binary import Binary
from pymongo import MongoClient




app = Flask(__name__)

@app.route('/')
def index():
   return render_template('index.html')
	
@app.route('/', methods=['POST'])
def uploade_file():
    uploaded_file = request.files['file']
    if uploaded_file.filename != '':
        uploaded_file.save(uploaded_file.filename)
    #return redirect(url_for('index'))
    return "file uploadeed"

@app.route('/uploadAppZip', methods = ['GET', 'POST'])
def upload_file():
   if request.method == 'POST':
      f=request.files['file']
      print(f.name)
      f = request.files['file']
      f.save("app.zip")
      # validation 
      with open("app.zip","rb") as f:
         encoded = Binary(f.read())
      collection.insert_one({"Application id ": 1,"folder":encoded})
      return 'file uploaded successfully'

@app.route('/uploadDeployConfig', methods = ['GET', 'POST'])
def uploadDeploy_file():
   if request.method == 'POST':
      f = request.files['file']
      f.save('deployConfig.json')
      return 'file uploaded successfully'

@app.route('/uploadSensorType', methods = ['GET', 'POST'])
def uploadSensorType_file():
   if request.method == 'POST':
      f = request.files['file']
      f.save('sensorTypeRegistration.json')
      return 'file uploaded successfully'

@app.route('/uploadSensorInstance', methods = ['GET', 'POST'])
def uploadSensorInstance_file():
   if request.method == 'POST':
      f = request.files['file']
      f.save('sensorInstance.json')
      return 'file uploaded successfully'

@app.route('/getSensorData',methods = ['POST'])
def getdata():
   pass
   #TODO call the sensor manager

@app.route('/setSensorData',methods = ['POST'])
def setdata():
   pass
   #TODO call the action service

if __name__ == '__main__':
   cluster = MongoClient("mongodb+srv://akshay:akshay123@cluster0.bh25q.mongodb.net/myFirstDatabase?retryWrites=true&w=majority")
   db = cluster["AppRepo"]
   collection = db["ID_sourceFolder"]
   app.run(host='0.0.0.0', port=5001,debug=True)