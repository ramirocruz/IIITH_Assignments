import socket
import sys
import threading
from encryption import GenerateKey,Encryption


class Client:
    def __init__(self,server_ip,server_port, sip,sport):
        self.client_socket =socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.client_socket.connect((server_ip,server_port))
        self.sip=sip
        self.sport=sport
        self.server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.text=''
        self.user_name=''
        self.filepath=''
        self.rollnumber=""
    def senddata(self):
        self.client_socket.send((self.sip+':'+str(self.sport)).encode())
        while True:
            inp=input()
            tokens=inp.split()
            if tokens[0]=='send_file':
                self.client_socket.send((tokens[0]+' '+tokens[1]).encode())
                self.filepath=tokens[2]
            elif tokens[0]=='send':
                self.client_socket.send((tokens[0]+' '+tokens[1]).encode())
                self.text=inp.split(' ',2)[2]
            elif tokens[0]=='group_send_file':
                for group in tokens[1].split(","):
                    self.client_socket.send((tokens[0]+' '+group).encode())
                self.filepath=tokens[2]
            elif tokens[0]=='group_send':
                for group in tokens[1].split(","):
                    self.client_socket.send((tokens[0]+' '+group).encode())
                self.text=" ".join(tokens[2:])
            elif tokens[0]=='login':
                self.user_name=tokens[1]
                self.client_socket.send(inp.encode())
            else:
                if tokens[0] == "create_user":
                    self.rollnumber = tokens[3]
                self.client_socket.send(inp.encode())

    def handle_connection(self,client_socket,client_address):
        DH = GenerateKey(self.rollnumber)
        peerhash = int(client_socket.recv(4096).decode())
        client_socket.send(str(DH.hashkey).encode())
        DH.gen_key(peerhash)
        En = Encryption(DH.finalkey)

        data = client_socket.recv(4096)
        if not data:
            client_socket.close()
            return
        data = En.decrypt(data)
        print(data.decode())
        if 'sent a file' in data.decode():
            client_socket.send('ok'.encode())
            data=client_socket.recv(4096)
            filename = En.decrypt(data).decode()
            with open(filename, 'wb') as f:
                print('receiving data...',end="")
                while True:
                    data = client_socket.recv(4096)
                    data = En.decrypt(data)
                    if not data:
                        break
                    f.write(data)
                    client_socket.send('ok'.encode())
                print("done")
        client_socket.close()
       

            

    def server(self):
        self.server_socket.bind((self.sip,self.sport))
        self.server_socket.listen(1)
        while True:
            client_socket,client_address=self.server_socket.accept()
            self.handle_connection(client_socket,client_address)

    def run(self):
        th1 = threading.Thread(target=self.senddata)
        th1.daemon=True
        th1.start()
        th2=threading.Thread(target=self.server)
        th2.daemon=True
        th2.start()

        while True:
            data = self.client_socket.recv(4096)
            if not data:
                break

            if data.decode()[:9]=='send_file':
                ip,port=data.decode()[10:].split(':')
                p2psocket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                p2psocket.connect((ip,int(port)))
                
                DH = GenerateKey(self.rollnumber)        
                p2psocket.send(str(DH.hashkey).encode())
                peerhash = int(p2psocket.recv(4096).decode())
                DH.gen_key(peerhash)
                En = Encryption(DH.finalkey)


                data = En.encrypt((self.user_name+' : sent a file').encode())
                p2psocket.send(data)
                filename = self.filepath.split('/')[-1]
                p2psocket.recv(4096)
                data = En.encrypt(filename.encode())
                p2psocket.send(data)
                try:
                    with open(self.filepath, 'rb') as f:
                        l = f.read(4096)
                        while(l):
                            data = En.encrypt(l)
                            p2psocket.send(data)
                            l = f.read(4096)
                            p2psocket.recv(4096)
                except FileNotFoundError as e:
                    print("ERROR:", e)
                p2psocket.close()

            elif data.decode()[:4].lower()=='send':

                ip,port=data.decode()[5:].split(':')
                p2psocket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                p2psocket.connect((ip,int(port)))
                DH = GenerateKey(self.rollnumber)        
                p2psocket.send(str(DH.hashkey).encode())
                peerhash = int(p2psocket.recv(4096).decode())
                DH.gen_key(peerhash)
                En = Encryption(DH.finalkey)
                data = En.encrypt((self.user_name+' : '+self.text).encode())
                p2psocket.send(data)
                p2psocket.close()


            elif data.decode()[:15]=='group_send_file':
                data=data.decode()[16:]
                members=data.split(';')[:-1]
                groupname=data.split(';')[-1]
                for i in members:
                    ip,port=i.split(':')
                    p2psocket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                    p2psocket.connect((ip,int(port)))

                    DH = GenerateKey(self.rollnumber)        
                    p2psocket.send(str(DH.hashkey).encode())
                    peerhash = int(p2psocket.recv(4096).decode())
                    DH.gen_key(peerhash)
                    En = Encryption(DH.finalkey)

                    data = En.encrypt((self.user_name+' '+groupname+' : sent a file').encode())
                    p2psocket.send(data)
                    filename = self.filepath.split('/')[-1]
                    p2psocket.recv(4096)
                    data = En.encrypt(filename.encode())
                    p2psocket.send(data)
                    try:
                        with open(self.filepath, 'rb') as f:
                            l = f.read(4096)
                            while(l):
                                data = En.encrypt(l)
                                p2psocket.send(data)
                                l = f.read(4096)
                                p2psocket.recv(4096)
                    except FileNotFoundError as e:
                        print("ERROR:", e)
                    p2psocket.close()

            elif data.decode()[:10].lower()=='group_send':
                data=data.decode()[11:]
                members=data.split(';')[:-1]
                groupname=data.split(';')[-1]
                for i in members:
                    ip,port=i.split(':')
                    p2psocket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                    p2psocket.connect((ip,int(port)))

                    DH = GenerateKey(self.rollnumber)        
                    p2psocket.send(str(DH.hashkey).encode())
                    peerhash = int(p2psocket.recv(4096).decode())
                    DH.gen_key(peerhash)
                    En = Encryption(DH.finalkey)

                    data = En.encrypt((self.user_name+' '+groupname+' : '+self.text).encode())
                    p2psocket.send(data)
                    p2psocket.close()
           
            elif data.decode()[:4].lower()=='list_groups':
                print(data.decode()[5:])
            else:
                print(data.decode())

    def print_cmds(self):
            cmds = ["CREATE_USER <username> <password> <rollnumber>", "LOGIN <username> <password>",
                    "SEND <username> <message>", "SEND_FILE <username> <filepath>",
                    "GROUP_SEND <groupname> <message>", "GROUP_SEND_FILE <groupname> <filepath>",
                    "LIST_GROUPS: lists all active chats", "JOIN <groupname>: join or create a group",
                    "CREATE_GROUP <groupname>: create a group"]

            print("****** COMMANDS *******")
            for cmd in cmds:
                print(cmd)

            print("***********************\n")

if __name__ == "__main__":
    client = Client(sys.argv[1],int(sys.argv[2]),sys.argv[3],int(sys.argv[4]))
    client.print_cmds()
    client.run()
