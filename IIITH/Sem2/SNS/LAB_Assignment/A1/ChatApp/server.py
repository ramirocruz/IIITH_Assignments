import socket
import threading
import sys
from Crypto import Random
class Peer:
    def __init__(self,socket,ip,port):
        self.socket=socket
        self.ip=ip
        self.port=port
        self.isloggedin=False
        self.user_name=''
        self.password=''
        self.rollno=None

class Group:
    def __init__(self,name):
        self.name=name
        self.members=[]
        self.nonce=Random.get_random_bytes(4)

connected_peers={}
groups={}

class Server:
    IP = socket.gethostbyname(socket.gethostname())
    def __init__(self):
        self.server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        self.conn_list = []

    def handle_connection(self,client_socket,client_address,peer_details):
        
        while True:
            data=client_socket.recv(4096)
            if not data:
                break
            data=data.decode()
            tokens=data.split()
            command=tokens[0].lower()
            print(tokens)

            if command=='create_user':
                # print('hello')
                if len(tokens)==4:
                    if tokens[1] in connected_peers:
                        peer_details.socket.send('Username already exists'.encode())
                    else:
                        connected_peers[tokens[1]]=peer_details
                        peer_details.user_name=tokens[1]
                        peer_details.password=tokens[2]
                        peer_details.rollno = tokens[3]
                        peer_details.socket.send('User id created successfully'.encode())
                else:
                    peer_details.socket.send('invalid command'.encode())

            elif command=='login':
                if len(tokens)==3:
                    if tokens[1] in connected_peers:
                        if tokens[2]==connected_peers[tokens[1]].password:
                            peer_details.isloggedin=True
                            peer_details.socket.send('you are now logged in.'.encode())
                        else:
                            peer_details.socket.send('Wrong password'.encode())
                    else:
                        peer_details.socket.send('This user id does not exist'.encode())    
                else:
                    peer_details.socket.send('invalid command'.encode())

            elif command=='send':
                if peer_details.isloggedin:
                    if tokens[1] in connected_peers:
                        if connected_peers[tokens[1]].isloggedin:
                            message='send '+connected_peers[tokens[1]].ip+':'+str(connected_peers[tokens[1]].port)
                            peer_details.socket.send(message.encode())
                        else:
                            peer_details.socket.send((tokens[1]+' is not logged in.').encode())
                    else:
                        peer_details.socket.send(('User '+tokens[1]+' does not exist.').encode())   
                else:                
                    peer_details.socket.send('you need to log in first'.encode())    

            elif command=='send_file':
                if peer_details.isloggedin:
                    if tokens[1] in connected_peers:
                        if connected_peers[tokens[1]].isloggedin:
                            message='send_file '+connected_peers[tokens[1]].ip+':'+str(connected_peers[tokens[1]].port)
                            peer_details.socket.send(message.encode())
                        else:
                            peer_details.socket.send((tokens[1]+' is not logged in.').encode())
                    else:
                        peer_details.socket.send(('User '+tokens[1]+' does not exist.').encode()) 
                else:
                    peer_details.socket.send('you need to log in first'.encode())    
            
            elif command=='group_send_file':
                if peer_details.isloggedin:
                    groupname=tokens[1]
                    if groupname in groups:
                        if peer_details in groups[groupname].members:
                            reply='group_send_file '
                            for i in groups[groupname].members:
                                if i!=peer_details:                      
                                    reply+=(i.ip+':'+str(i.port)+';')
                            reply+=('<'+groupname+'>')
                            peer_details.socket.send(reply.encode())
                        else:
                            peer_details.socket.send('You are not a member of this group'.encode())
                    else:
                        peer_details.socket.send('group name does not exist'.encode())
                else:
                    peer_details.socket.send('you need to log in first'.encode())   
                            

            elif command=='group_send':
                if peer_details.isloggedin:
                    groupname=tokens[1]
                    if groupname in groups:
                        if peer_details in groups[groupname].members:
                            reply='group_send '
                            for i in groups[groupname].members:      
                                if i!=peer_details:                      
                                    reply+=(i.ip+':'+str(i.port)+';')
                            reply+=('<'+groupname+'>')
                            # reply=reply[:-1]
                            peer_details.socket.send(reply.encode())
                        else:
                            peer_details.socket.send('You are not a member of this group'.encode())
                    else:
                        peer_details.socket.send('group name does not exist'.encode())
                else:
                    peer_details.socket.send('you need to log in first'.encode())    

            
            elif command=='create_group':                
                if peer_details.isloggedin:
                    groupname=tokens[1]
                    if groupname in groups:
                        peer_details.socket.send('Groupname already exists'.encode())
                    else:
                        group=Group(groupname)
                        group.members.append(peer_details)
                        groups[groupname]=group
                        peer_details.socket.send('Group created successfully'.encode())
                else:
                    peer_details.socket.send('You need to log in first'.encode())

            elif command=='join':
                if peer_details.isloggedin:
                    groupname=tokens[1]
                    if groupname in groups:
                        groups[groupname].members.append(peer_details)
                        peer_details.socket.send('Group joined successfully'.encode())
                    else:
                        peer_details.socket.send('Groupname does not exist'.encode())
                else:
                    peer_details.socket.send('You need to log in first'.encode())
            
            elif command=='list_groups':
                reply=''
                for i in groups:
                    reply+=(i+' '+str(len(groups[i].members))+'\n')
                reply=reply[:-1]
                peer_details.socket.send(reply.encode())

            if not data:
                print("{}:{} disconnected.....".format(client_address[0],client_address[1]))
                self.conn_list.remove(client_socket)
                client_socket.close()
                break

    def run(self,PORT=18000):
        self.server_socket.bind((self.IP,PORT))
        self.server_socket.listen(20)
        print("Listening at {}".format(self.server_socket.getsockname()))
        while True:
            client_socket,client_address = self.server_socket.accept()
            print(client_address[0],client_address[1],' connected')
            serveratpeerip, seerveratpeerport=client_socket.recv(4096).decode().split(':')
            print(serveratpeerip,seerveratpeerport)
            # self.conn_list.append(client_socket)
            client_details=Peer(client_socket,serveratpeerip,seerveratpeerport)
            # peers.append(client_details)
            th=threading.Thread(target=self.handle_connection,args=(client_socket,client_address,client_details))
            th.daemon = True
            th.start()

if __name__ == "__main__":
    server = Server()
    server.run()
        