import socket
import threading
import os
import re
from encodings.base64_codec import base64_encode

BUFFER_SIZE = 1024
PORT = 8888
IP = socket.gethostbyname(socket.gethostname())
server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server_socket.bind(("",PORT))

def upload_data(client_socket,filename):
    filepath = os.path.abspath(filename)
    header="HTTP/1.1 200 OK"
    header2="Content-Length: {}".format(os.path.getsize(filepath))
    blank="\r\n\r\n"
    client_socket.send((header+header2+blank).encode())
    with open(filepath,"rb") as f:
        bytes_read = f.read(BUFFER_SIZE)
        while bytes_read:            
            client_socket.send(bytes_read)
            bytes_read = f.read(BUFFER_SIZE)
        # client_socket.close()

def parse(msg):
    # msg = msg.split("GET ")[1]
    return msg

def handle_connection(server_socket,client_socket,address):
    print("{} Client Connected...".format(address))
    stat = True
    while stat:
        msg = client_socket.recv(BUFFER_SIZE)
        if not msg:
            break
        msg = msg.decode()
        url_pattern = re.compile("^GET (.*)[ ].*")
        msg = url_pattern.match(msg).group(1)
        msg = msg[1:]
        print(msg)
        filename = parse(msg)
        print("here1")
        upload_data(client_socket,filename)
        print("here2")
        stat  = False
    client_socket.close()


server_socket.listen()
print("Listening to ip:port = {}:{}".format(IP,PORT))
while True:
    client_socket,address = server_socket.accept()
    print("Connected")
    handle_thread = threading.Thread(target=handle_connection,args=(server_socket,client_socket,address))
    handle_thread.start()
    print("Clients Active : {}".format(threading.active_count()-1))

server_socket.close()