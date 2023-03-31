import socket
import sys
import threading
import encryption
IP = socket.gethostbyname(socket.gethostname())
class Client:
    
    def __init__(self, server_ip=IP, server_port=18000):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((server_ip, server_port))
        print("server connected...")


    def senddata(self):
        while True:
            inp = input()
            if not inp:
                print("Enter Valid Input...")
                continue       	
            e = encryption.Encryption()
            inp = e.encrypt(inp)
            self.client_socket.send(inp.encode())

    def run(self):
        th1 = threading.Thread(target=self.senddata)
        th1.daemon = True
        th1.start()

        while True:
            data = self.client_socket.recv(4096)
            if not data:
                break
            d = encryption.Encryption()
            data = d.decrypt(data.decode())
            if(data[1]):
                print("Data is non-corrupted..")
                print(data[0].strip())
            else:
                print("Data is corrupted...")
                print(data[0])


if(len(sys.argv)==1):
    client = Client()
    client.run()
elif(len(sys.argv)==3):
    client = Client(sys.argv[1],int(sys.argv[2]))
    client.run()
else:
    print("Invalid arguments, exiting....")
    sys.exit(1)

