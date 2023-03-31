import socket
import threading
import sys
import encryption

class Server:
    IP = socket.gethostbyname(socket.gethostname())    

    def __init__(self):
        self.server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)

    def senddata(self,client_socket,client_address):
        while True:
            data = input()
            if not data:
                break
            e = encryption.Encryption()
            data = e.encrypt(data)
            client_socket.send(data.encode())

    def handle_connection(self,client_socket,client_address):
        while True:
            data=client_socket.recv(4096)
            if not data:
                print("client disconnected....")
                client_socket.close()
                return
            d = encryption.Encryption()
            data=d.decrypt(data.decode())
            if(data[1]):
                print("Data is non-corrupted..")
                print(data[0].strip())
            else:
                print("Data is corrupted...")
                print(data[0])

    def run(self,PORT=18000):
        self.server_socket.bind((self.IP,PORT))
        self.server_socket.listen(20)
        print("Listening at {}".format(self.server_socket.getsockname()))
        while True:
            client_socket,client_address = self.server_socket.accept()
            print(client_address[0],client_address[1],' connected')
            th=threading.Thread(target=self.handle_connection,args=(client_socket,client_address))
            th.daemon = True
            th.start()
            th1=threading.Thread(target=self.senddata,args=(client_socket,client_address))
            th1.daemon = True
            th1.start()

if __name__ == "__main__":
    server = Server()
    server.run()
        
