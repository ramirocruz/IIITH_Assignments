import socket
import threading
import os
BUFFER_SIZE = 1024
PORT = 18888
IP = socket.gethostbyname(socket.gethostname())
client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
client_socket.connect((IP,PORT))
def parse(msg):
    msg = msg.split("GET ")[1]
    return msg
def download_file(client_socket,filepath):
    with open(filepath,"wb") as f:
        bytes_read = client_socket.recv(BUFFER_SIZE)
        while bytes_read:
            f.write(bytes_read)
            bytes_read = client_socket.recv(BUFFER_SIZE)

            
while True:
    print("Enter data")
    msg = input()        
    msg = parse(msg)
    client_socket.send(msg.encode())
    filepath = os.path.basename(msg)
    print("here")
    download_file(client_socket,filepath)
    print("here2")
client_socket.close()

