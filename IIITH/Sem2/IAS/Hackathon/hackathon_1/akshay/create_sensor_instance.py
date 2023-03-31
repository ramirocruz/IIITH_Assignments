import os
import sys
import json
import importlib

SENSOR_INSTANCES_CONFIG = "sensor_instance.json"
SENSOR_TYPE_CONFIG = "sensor_types.json"
sensor_filepath = dict()
DB = dict()

def set_sensor_types():
    global sensor_filepath
    with open(SENSOR_TYPE_CONFIG) as f:
        sensor_types = json.load(f)
        
    for i in sensor_types:
        sensor_filepath[(sensor_types[i]["sensor_name"] + "##" + sensor_types[i]["manufacturer"])] = sensor_types[i]["class_code"]

def create_instance():

    with open(SENSOR_INSTANCES_CONFIG) as f:
        sensor_instances = json.load(f)
    
    for i in sensor_instances:
        sensorType = sensor_instances[i]["sensorType"]
        manufacturer = sensor_instances[i]["manufacturer"]
        sensorId = sensor_instances[i]["id"]
        sensorIP = sensor_instances[i]["ip"]
        sensorPort = sensor_instances[i]["port"]

        type_manufacturer = sensorType + "##" + manufacturer

        if type_manufacturer not in sensor_filepath:
            print("Sensor Type with the specified manufacturer not available =>")
            print(sensorType + " " + manufacturer)
            sys.exit()
        
        class_name = os.path.splitext(os.path.basename(sensor_filepath[type_manufacturer]))[0]
        module = importlib.import_module(class_name)    
        obj = getattr(module, class_name)(sensorId,sensorIP,sensorPort)

        for key in sensor_instances[i]["attributes"]:
            if key not in DB:
                DB[key] = dict()
            value = sensor_instances[i]["attributes"][key]
            if value in DB[key]:
                DB[key][value].append(obj)
            else:
                DB[key][value] = [obj]

if __name__ == "__main__":
    set_sensor_types()
    create_instance()
