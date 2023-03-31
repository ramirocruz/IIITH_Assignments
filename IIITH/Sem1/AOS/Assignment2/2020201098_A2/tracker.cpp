#include<bits/stdc++.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<pthread.h>
#include<arpa/inet.h>
#define SERVER_BUFFER (100)
#define MAX_LARGE_BUFFER_SIZE (2098200)
using namespace std;



struct User{
    bool isloggedIn;
    string u_id,password,ip;
    unsigned short port; 
    User()
    {

    }
    User(string uid,string pass)
    {
        u_id = uid;
        password = pass;
        isloggedIn = false;
    } 
};

struct Group{

    string admin;
    int g_id;
    unordered_set<string> members;
    unordered_set<string> requests;

};
struct FileDetails
{
    string fname;
    string fpath;
    long long size;
    int chunks;
    string filehash;
    string chunkshash;
    unordered_set<string>uname;
};
unordered_map<string,User> User_map;
unordered_map<int,Group>Group_map;
unordered_map<string,unordered_set<int>>Joined_Group;
unordered_map<string,short>uname_to_port_map;
unordered_map<int,unordered_map<string,FileDetails>> FileMap;

vector<string> parse(string &input);

void onend(string msg)
{
    cout<<endl<<msg<<endl;
    exit(0);
}

void onend(string msg,int socket_id)
{
    close(socket_id);
    cout<<endl<<msg<<endl;
    exit(0);
}

// void disconnect(int client_socket,string& uname)
string get_filename(string &path)
{
	for(int i=path.size()-1;i>-1;i--)
	{
		if(path[i]=='/')
		{
			return path.substr(i+1);
		}
	}
	return path.substr(1);
}


bool create_user_cmd(vector<string>&args,string &uname)
{

    if(User_map.find(args[0]) != User_map.end())
        return false;
    
    User temp(args[0],args[1]);
    User_map[args[0]] = temp;
    uname = args[0];

    return true;
}

int login_cmd(vector<string>&args,string &uname,string &ip,unsigned short &port)
{
    
    if(User_map.find(args[0]) == User_map.end())
        return -1;
    if(User_map[args[0]].password != args[1])
        return -2;
    if(User_map.find(uname) !=User_map.end())
        User_map[uname].isloggedIn = false;
    User_map[args[0]].isloggedIn = true;
    uname = args[0];
    User_map[args[0]].ip = ip;
    User_map[args[0]].port = port;

    cout<<ip<<" : "<<port<<" -- "<<uname<<endl;
    return 1;
}

int create_group_cmd(vector<string>&args,string &uname)
{
    int g_id = stoi(args[0]);
    if(Group_map.find(g_id) != Group_map.end())
        return -1;

    
    Group temp_group;
    temp_group.admin = uname;
    temp_group.g_id = g_id;
    Group_map[g_id] = temp_group;
    Joined_Group[uname].insert(g_id);


     return 1;
}

int join_group_cmd(vector<string>&args,string &uname)
{
    int gid =  stoi(args[0]);

    if(Group_map.find(gid) == Group_map.end())
    return -1;

    if(Joined_Group.find(uname)!=Joined_Group.end())
    {
        if(Joined_Group[uname].find(gid) != Joined_Group[uname].end())
            return -2;
    }
    Group_map[gid].requests.insert(uname);

    return 1;
}

int leave_group_cmd(vector<string>&args,string &uname)
{
    int gid=stoi(args[0]);
    if(Group_map.find(gid) == Group_map.end())
    return -1;
    cout<<"test- "<<uname<<endl;
    if(Joined_Group.find(uname)!=Joined_Group.end())
    {
        if(Joined_Group[uname].find(gid) != Joined_Group[uname].end())
            {
                Joined_Group[uname].erase(gid);
                return 1;
            }
    }
    

    return -2;

}

int list_requests_cmd(vector<string>&args,string &uname,string &res)
{
    int gid=stoi(args[0]);
    if(Group_map.find(gid) == Group_map.end())
        return -1;
    auto &rqsts = Group_map[gid].requests;

    for(auto &x:rqsts)
    {
        res+= "* " + x + "\n";
    }
    return 1;
}

int accept_requests_cmd(vector<string>&args,string &uname)
{
    int gid = stoi(args[0]);
    if(Group_map.find(gid) == Group_map.end())
        return -1;
    if(Group_map[gid].requests.find(args[1]) == Group_map[gid].requests.end())
        return -2;
    Group_map[gid].requests.erase(args[1]);
    Group_map[gid].members.insert(args[1]);
    cout<<args[1]<<"  -test\n";
    Joined_Group[args[1]].insert(gid);

    return 1;

}

int list_groups_cmd(string &res)
{
    for (auto &G:Group_map)
    {
        res += "* "+to_string(G.second.g_id) + "\t" +G.second.admin+ "\n";
    }
    return 1;

}
int upload_file_cmd(vector<string>&args,string &uname,int client_socket)
{
    int gid = stoi(args[1]);
    string file_name = get_filename(args[0]);
    if(Joined_Group.find(uname) == Joined_Group.end()  ||  Joined_Group[uname].find(gid) == Joined_Group[uname].end())
        return -1;

    if(FileMap.find(gid) != FileMap.end()  && FileMap[gid].find(file_name) != FileMap[gid].end())
        return -2;
    char rec_buffer[MAX_LARGE_BUFFER_SIZE];
    send(client_socket,"uploading",10,0);
    int byteread = read(client_socket,rec_buffer,sizeof(rec_buffer));
    if(byteread < 1)
    {
         if(User_map.find(uname) != User_map.end())
          {
                    User_map[uname].isloggedIn = false;
           
          }
         return -3;
    }
    rec_buffer[byteread]=0;
    string tempstr = rec_buffer;
    vector<string> ar = parse(tempstr);
    if(ar.size() != 6)
        return -4;
    FileDetails fd;
    fd.fname = ar[0];
    fd.fpath = ar[1];
    fd.size = stoi(ar[2]);
    fd.chunks = stoi(ar[3]);
    fd.filehash = ar[4];
    fd.chunkshash = ar[5];
    fd.uname.insert(uname);
    FileMap[gid][fd.fname] = fd;
    
    cout<<fd.filehash<<"       "<<fd.chunkshash<<endl;

    return 1;


}
int list_files_cmd(vector<string>&args,string &uname,string &response)
{
    int gid = stoi(args[0]);
    if(FileMap.find(gid) == FileMap.end())
        return -1;
    response+= "FileName\tSize\tChunks\n";
    for(auto &c:FileMap[gid])
    {
        auto &fd = c.second;
        response += fd.fname +'\t'+ to_string(fd.size)+'\t'+to_string(fd.chunks)+'\n';
    }
    return 1;
}
int download_file_cmd(vector<string>&args,string &uname,int client_socket,string &ip,unsigned short port)
{
    int gid = stoi(args[0]);
    string fname = args[1];

    if(Joined_Group.find(uname) == Joined_Group.end()  ||  Joined_Group[uname].find(gid) == Joined_Group[uname].end())
        return -1;

    if(FileMap.find(gid) == FileMap.end()  || FileMap[gid].find(fname) == FileMap[gid].end())
        return -2;
    
    string unames="";
    
    auto &Unames = FileMap[gid][fname].uname;
    int seedlen=0;
    for(auto &x:Unames)
    {
        if(x == uname)
            continue;
        unames += User_map[x].ip +' '+to_string(User_map[x].port)+' ';
        
    }
    unames += FileMap[gid][fname].chunkshash + ' ' +to_string(FileMap[gid][fname].size) ;
    char send_buf[unames.size()];
    strcpy(send_buf,unames.c_str());
    if( send(client_socket,send_buf,unames.size(),0) < 0 )
    return -5;

    char rec_buf[512];
    int byteread = read(client_socket,rec_buf,sizeof(rec_buf));
    if(byteread < 1)
        return -5;
    rec_buf[byteread]=0;
    string res = rec_buf;
    if(res!="done")
    {
        send(client_socket,"failed",6,0);
        return -5;
    }

    strcpy(send_buf,FileMap[gid][fname].filehash.c_str());  
    if(send(client_socket,send_buf,strlen(send_buf),0) < 0)
        return -5;

    byteread = read(client_socket,rec_buf,sizeof(rec_buf));
    if(byteread < 1)
        return -5;
    rec_buf[byteread]=0;
    res = rec_buf;
    if(res!="correct")
    {
        send(client_socket,"failed",6,0);
        return -4;
    }


 
  FileMap[gid][fname].uname.insert(uname);

  

    
    return 1;
}
int stop_share_cmd(vector<string>&args,string &uname)
{
    int gid = stoi(args[0]);

    if(Joined_Group.find(uname) == Joined_Group.end()  ||  Joined_Group[uname].find(gid) == Joined_Group[uname].end())
        return -1;

    if(FileMap.find(gid) == FileMap.end()  || FileMap[gid].find(args[1]) == FileMap[gid].end())
        return -2;

    if(FileMap[gid][args[1]].uname.find(uname) == FileMap[gid][args[1]].uname.end())
        return -3;

    FileMap[gid][args[1]].uname.erase(uname);
    if(FileMap[gid][args[1]].uname.size() == 0)
        FileMap[gid].erase(args[1]);

    return 1;    
}

string parsecommand(vector<string>&args,string &uname,int client_socket,string &ip,unsigned short &port)
{
   string Errstr = "INVALID COMMAND OR ARGUMENTS...";
   if(args.size()==0)
   return Errstr;
   string cmd=args[0];
   
   args.erase(args.begin()); 
  
 if(cmd=="create_user")
 {
   if(args.size() != 2)
   return Errstr;
   if(create_user_cmd(args,uname))
        return "user created..";
    return "user exists or something is wrong...";
 }
 else if(cmd=="login")
 {
     if(args.size() != 2)
     return Errstr;
     int stats = login_cmd(args,uname,ip,port);
     if(stats == 1)
        return "user logged in..";
     if(stats == -1)
        return "user not available..";
     
     
     return "Password do not match..";
 }
 else if(cmd=="create_group")
 {  
     if(args.size()!=1)
     return Errstr;

     int stats = create_group_cmd(args,uname);
     if(stats == -1)
     return "Group already present..";
     
     return "Group Created..";
   
 }
 else if(cmd=="join_group")
 { 
     if(args.size()!=1)
     return Errstr;
     int stats = join_group_cmd(args,uname);
     if(stats == -1)
     return "Invalid Group Id";
     if(stats == -2)
     return "Already in Group..";

     return "Group Joining Request Sent..";
 }
 else if(cmd=="leave_group")
 {
     if(args.size()!=1)
     return Errstr;
     int stats = leave_group_cmd(args,uname);
     if(stats == -1)
        return "Invalid Group id..";
     if(stats == -2)
        return "Not in the Group..";
     return "Group Left..";
 }
 else if(cmd=="list_requests")
 {  
     if(args.size()!=1)
     return Errstr;
     string res;
     int stats = list_requests_cmd(args,uname,res);
     if(stats == -1)
     return "Invalid Group Id..";

     return "List of requests...\n" + res;
 }
 else if(cmd=="accept_request")
 {
     if(args.size() !=2)
     return Errstr;
     int stats = accept_requests_cmd(args,uname);
     if(stats == -1)
     return "Invalid Group Id..";
     if(stats == -2)
     return "No requests from given UserId";
     return "Request accepted..";
 }
 else if(cmd=="list_groups")
 { 
     if(args.size())
     return Errstr;
     string res;
     int stats = list_groups_cmd(res);
     return "List of Groups...\nGroup Id \t Group Admin\n" + res;
  
 }
 else if(cmd=="list_files")
 {
     if(args.size() != 1)
        return Errstr;
     string response;
     int stat = list_files_cmd(args,uname,response);
     if(stat == -1)
        return "No Files..";
     
    return "List of Files...\n"+response;   
    
 }
 else if(cmd == "upload_file")
 {
     if(args.size() != 2)
     return Errstr;
     int stat = upload_file_cmd(args,uname,client_socket);
     if(stat == -1)
        return "Client not in Group..";
     if(stat == -2)
        return "File already present..";
     if(stat == -3)
        return "Client Disconnected..";
     if(stat == -4)
        return "Data Corrupted..";
     return "File Uploaded...";
 }
 else if(cmd == "download_file")
 {
     if(args.size() != 3)
        return Errstr;
    int stat = download_file_cmd(args,uname,client_socket,ip,port);
    if(stat == -1)
        return "Not in group..";
    if(stat == -2)
        return "File not present..";
    if(stat == -4)
        return "File Corrupted";
    if(stat == -5)
        return "Download Failed..";

     return "File Downloaded..";
 }
 else if(cmd == "logout")
 {
     if(args.size())
     return Errstr;

    if(User_map.find(uname) != User_map.end())
    {
        User_map[uname].isloggedIn = false;
    }
    return "Client Logged Out..";
 }
 else if(cmd == "show_downloads")
 {

     return "List of Downloads...";
 }
 else if(cmd == "stop_share")
 {
    if(args.size()!=2)
        return Errstr;
    int stat = stop_share_cmd(args,uname);
    if(stat == -1)
        return "Client Not in Group..";
    if(stat == -2)
        return "File Not Present..";  
    if(stat == -3)
        return "You are not sharing that file..";  

    return "Sharing Stopped...";
 }
 
return "INVALID COMMAND OR ARGUMENTS...";
}


vector<string> parse(string &input)
{
  vector<string> args;
  for(int i=0;i<input.size();i++)
  {
    if(input[i]==' ')
    {
        while(i<input.size() && input[i]==' ')
        i++;
        
    }
    string temp;
    while(i<input.size() && input[i]!=' ')
    {
        temp+=input[i];
        i++;
    }
    i--;
    if(temp.size())
    args.push_back(temp);
  }

 return args;
}

int rec_addr(int client_socket,string &ip,unsigned short &port)
{
   
    char recieving_buffer[512],send_buf[512];
    int bytesread= read(client_socket,recieving_buffer,sizeof(recieving_buffer));

            if(bytesread < 1)
            {
                return -1;
            }
    
    recieving_buffer[bytesread]=0;        
    string str = recieving_buffer;
    if(str != "ipport")
    return -1;
   
    if(send(client_socket,"ok",3,0) < 0)
    return -1;
    
    bytesread = read(client_socket,recieving_buffer,sizeof(recieving_buffer));
    if(bytesread < 1)
    return -1;
    
    str = recieving_buffer;
    vector<string> temp = parse(str);
    ip = temp[0];
    port = stoi(temp[1]);
   
    if(send(client_socket,"done",5,0) < 0)
        return -1;
    return 1;

}

void handle_connection(int client_socket,sockaddr_in client_socket_address)
{
    char recieving_buffer[1024];
    char sending_buffer[1024];
    string uname;
    unsigned short port ;
    string ip ;
   
    if(rec_addr(client_socket,ip,port) != 1)
    {
        onend("Error receiving address..");
    }
  

     while(true)
        {
           
        string input,status;

         int bytesread= read(client_socket,recieving_buffer,sizeof(recieving_buffer));

            if(bytesread < 1)
            {
                if(User_map.find(uname) != User_map.end())
                {
                    User_map[uname].isloggedIn = false;
                }
                cout<<"Client " <<client_socket<<" logged out....\n";
                break;
            }

        recieving_buffer[bytesread]=0;
        
        input = recieving_buffer;

        cout<<input<<endl;
 
        if(input=="close()")
        {
                
                if(User_map.find(uname) != User_map.end())
                {
                    User_map[uname].isloggedIn = false;
                }
                cout<<"User " <<client_socket<<" logged out....\n";
                break;
        }
        else
        {
            
            vector<string>args;
            args = parse(input);
            status = parsecommand(args,uname,client_socket,ip,port);
            cout<<status<<endl;
        }        
        strcpy(sending_buffer,status.c_str());
        send(client_socket,sending_buffer,strlen(sending_buffer),0);        
         
        
        }


    close(client_socket);
   
}


int main(int argc , char * argv[])
{
    FILE * trackinfo;
    int trackno;

    if(argc != 3)
    {
        trackinfo = fopen("tracker_info.txt","r");
        
        trackno = 1;
    }
    else
    {
        trackinfo = fopen(argv[1],"r");
        trackno = atoi(argv[2]);

    }
    if(trackinfo == NULL)
    {
        onend("No such tracker info file..");
    }
    char IP_ADDR[50];
    short port;

    for(int i=0;i<trackno;i++)
    {
        fscanf(trackinfo,"%s",IP_ADDR);
        fscanf(trackinfo,"%hu",&port);
    }

    fclose(trackinfo);
    

    int tracker_socket=-1;
    struct sockaddr_in  tracker_socket_address,client_socket_address;
    
    if ((tracker_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
      onend("tracker socket failed....");
    } 
    
    bzero(&tracker_socket_address,sizeof(tracker_socket_address));
    tracker_socket_address.sin_family=AF_INET;
    in_addr temp;
    inet_aton(IP_ADDR,&temp);
    tracker_socket_address.sin_addr = temp;
    tracker_socket_address.sin_port = htons(port);
    int optval =0;
    int optlen = sizeof(optval);
    setsockopt(tracker_socket,SOL_SOCKET,SO_REUSEPORT,(char *)&optval,optlen);

    if(bind(tracker_socket,(struct sockaddr *)(&tracker_socket_address),sizeof(tracker_socket_address)) < 0)
    {
        onend("Tracker... Unable to Bind");
    }
    if(listen(tracker_socket,SERVER_BUFFER) < 0)
    {
        onend("Tracker .... Listening Failed..");
    }
    cout<<"Listening on IP :- "<<IP_ADDR<<" and port no.:- "<<port<<endl; 
    vector<thread> Tvect;
    socklen_t  len = sizeof(client_socket_address);
    while(true)
    {
              

        int client_socket= accept(tracker_socket,(struct sockaddr *)&client_socket_address,&len);
        if(client_socket < 0)
        {
            onend("Client accept error...");
        }

        cout<<"connected..:"<<client_socket<<endl;
    

        Tvect.push_back(thread(handle_connection,client_socket,client_socket_address));   
        
    }


   for (auto &th:Tvect)
   {
       if(th.joinable())
       {
           th.join();
       }
   }
    
    
    

 close(tracker_socket);

return 0;
}