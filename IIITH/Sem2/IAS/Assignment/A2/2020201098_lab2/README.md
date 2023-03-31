## Lab Assignment 2
* I have made 3 files `client.py`,`server.py`,`lib.py`.
* Run `client.py` and `server.py`.
* `server.py` contains the actual functions. It gives list of all the function parameters types and return types when requested.
* It also has a stub function to handle a particular function request
* `lib.py` is the module to handle the functions calling to the server. It has two methods one to get all the functions details and other is the function calling.
* `client.py` It is the file of the user. First we request all the api calls available on the server side. Then we can call each of them according to our need.
* First we have to import the `lib.py` file in the `client.py` file to use the rpc functions.
