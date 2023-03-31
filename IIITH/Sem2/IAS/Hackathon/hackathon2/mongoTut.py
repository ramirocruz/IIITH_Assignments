from pymongo import MongoClient

client = MongoClient('localhost', 27017)

# Access database
mydatabase = client['varun']
  
# Access collection of the database
mycollection=mydatabase['user_info']
  
# dictionary to be added in the database
mydict = { "name": "John", "address": "Highway 37" }
  
# inserting the data in the database
x = mycollection.insert_one(mydict)