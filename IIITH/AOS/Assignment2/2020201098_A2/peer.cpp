#include<bits/stdc++.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<openssl/sha.h>
#include<pthread.h>
#include<random>
#include<sys/stat.h>


using namespace std;
#define MAX_LARGE_BUFFER_SIZE (2098200)
#define __512KB (524288)
#define SERVER_BUFFER (100)
unsigned short PEER_PORT;
string PEER_IP;
bool isloggedin = false;
struct FileDetails
{
    vector<bool> chunks;
    string path;
};
struct ChunksDetails
{
    string ip;
    string port;
    bool check;
    int num;
    ChunksDetails()
    {
        check  = false;
    }

};

unordered_map<int,unordered_map<string,FileDetails>> FilesChunkVector;
unordered_map<int,unordered_map<string,vector<ChunksDetails>>> Downloaded_chunks;
vector<string> parse(string &input);
void onend(string msg)
{
    cout<<endl<<msg<<endl;
    exit(1);
}
void onend(string msg,int socket_id)
{
    close(socket_id);
    cout<<endl<<msg<<endl;
    exit(0);
}

int  pathresolver(string &path)
{   string rootname="/";
	string cur=get_current_dir_name();

	if(path[0]=='~')
	{   
		
		if(rootname[rootname.size()-1]=='/')
		path=rootname+path.substr(2);
		else
		{
			path = rootname+path.substr(1);
		}
		

		
	}
	else if(path[0]=='/')
	{
		

	  if(path.size() > rootname.size())
	  {
		  if(path.substr(0,rootname.size()) == rootname)
		  return 0;
	  }
      return -1;
		
	}
	else
	path = cur+'/'+path;

char *resolvedpath = new char[4096];
realpath(path.c_str(),resolvedpath);
path = resolvedpath;	
delete [] resolvedpath;
resolvedpath=NULL;
return 0;
}
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
string getFileHash(string &fname)
{
    
    unsigned char sha_hash[20];
    unsigned char sha_hash_result[40];
    SHA_CTX shactx;
    FILE *fd = fopen(fname.c_str(),"rb");
    SHA1_Init(&shactx);
    char buff[1024];
    int stats;
    while( (stats = fread(buff,1,1024,fd) ) > 0    )
    {
    SHA1_Update(&shactx,buff,stats);
    }
    SHA1_Final(sha_hash,&shactx);


    for(int i=0; i < 20;i++)
    {
        sprintf((char *)&(sha_hash_result[i*2]), "%02x",sha_hash[i]);
    }

    fclose(fd);
     
    return (char *)sha_hash_result;
    
}
string getFileChunksHash(string &fname,int chunksize=__512KB)
{
    unsigned char sha_hash[20];
    unsigned char sha_hash_result[40];
    SHA_CTX shactx;
    FILE *fd = fopen(fname.c_str(),"rb");
    SHA1_Init(&shactx);
    char buff[chunksize];
    int stats;
    string hash="";
    while( (stats = fread(buff,1,chunksize,fd) ) > 0    )
    {
    SHA1_Init(&shactx);
    SHA1_Update(&shactx,buff,stats);    
    SHA1_Final(sha_hash,&shactx);


    for(int i=0; i < 20;i++)
    {
        sprintf((char *)&(sha_hash_result[i*2]), "%02x",sha_hash[i]);
    }
    hash += (char *) sha_hash_result ;

    }

    fclose(fd);
    return hash;
   
}


vector<char *>  PeerConnect_util(string &port_no,string &ip,vector<string> &cmd)
{   

    char IP_ADDR[50];
    strcpy(IP_ADDR,ip.c_str());
    unsigned short port = stoi(port_no);
    int client_socket=-1;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
      onend("server socket failed....");
    } 
    struct sockaddr_in  client_socket_address;
    
 
    bzero(&client_socket_address,sizeof(client_socket_address));
    client_socket_address.sin_family=AF_INET;
    client_socket_address.sin_addr.s_addr=inet_addr(IP_ADDR);
    client_socket_address.sin_port = htons(port);
    
    struct sockaddr_in server_socket_address;
    int addsize = sizeof(server_socket_address);
     char *recieving_buffer = new  char[MAX_LARGE_BUFFER_SIZE];
    char *sending_buffer = new char[MAX_LARGE_BUFFER_SIZE];
   
   if(connect(client_socket,(struct sockaddr *)&client_socket_address,sizeof(client_socket_address)) < 0)
   {
       onend("Client p2p Connect Error....");
   }
   
 
   int bytesread;
   string input;
   
   if(cmd[0] == "get_chunk_vector")
   {
       input+= cmd[0] + ' ' + cmd[1] + ' ' + cmd[2];
       strcpy(sending_buffer,input.c_str());
        cout<<sending_buffer<<endl;
       if(send(client_socket,sending_buffer,input.size(),0) < 0)
       {
           onend("dead");
       }
          cout<<"hereagain..\n";
        bytesread = read(client_socket,recieving_buffer,MAX_LARGE_BUFFER_SIZE);
        if(bytesread < 1)
        {
            onend("dead2");
        }
        recieving_buffer[bytesread];
        vector<char *>res;
        res.push_back(recieving_buffer);
       return res;        

   }
   else if(cmd[0] == "get_chunks")
   {

       input+= cmd[0] + ' ' + cmd[1] + ' ' + cmd[2] + ' ' + cmd[3];
       strcpy(sending_buffer,input.c_str());
        cout<<"in peer connect - getchunks\n";
       if(send(client_socket,sending_buffer,input.size(),0) < 0)
       {
           onend("dead");
       }
          input = "";
          cout<<"heresd\n";
        
        bytesread = read(client_socket,recieving_buffer,1024);
        if(bytesread < 1)
        {
            onend("dead3");
        }
        recieving_buffer[bytesread]=0;
        cout<<recieving_buffer<<" 2nd\n";
        int len = atoi(recieving_buffer);
        cout<<len<<" 2nd--len\n";
        bytesread =0;
        send(client_socket,"ok",3,0);
        int pos =0;
        bytesread = recv(client_socket,recieving_buffer,MAX_LARGE_BUFFER_SIZE,0);
        cout<<bytesread<<" ++++000++++"<<cmd[3]<<endl;
        pos = bytesread;
        while(bytesread > 0)        
            {
                cout<<bytesread<<" --- 0000----"<<cmd[3]<<endl;                
                
                bytesread = read(client_socket,recieving_buffer+pos,MAX_LARGE_BUFFER_SIZE);
                pos += bytesread;
            }
            
        
        cout<<"hereds\n";
        if(bytesread < 0)
        {
            onend("dead2");
        }
        recieving_buffer[len]=0;
        cout<<input.size()<<" 1st\n";
        char charlen[1024];
        strcpy(charlen,to_string(len).c_str());

        
        
        
        vector<char *>res;
        res.push_back(recieving_buffer);
        res.push_back(charlen);

       return res;           

   }


     delete[] sending_buffer;
    delete[] recieving_buffer;
   close(client_socket);
    char nullchar[] = "NULL";
    return {nullchar};
}

void handle_connection(int client_socket)
{
    char *recieving_buffer = new  char[MAX_LARGE_BUFFER_SIZE];
    char *sending_buffer = new char[MAX_LARGE_BUFFER_SIZE];
    string uname;
 
        cout<<PEER_PORT <<" --- "<<PEER_IP<<endl;
        string input,status;

        int bytesread= read(client_socket,recieving_buffer,MAX_LARGE_BUFFER_SIZE);

            if(bytesread < 1)
            {
                close(client_socket);
                cout<<"Client " <<client_socket<<" logged out....\n";
                return ;
            }

        recieving_buffer[bytesread]=0;
        
        input = recieving_buffer;
        vector<string> cmd = parse(input);
 
        if(cmd[0]=="get_chunk_vector")
        {
                cout<<cmd[0]<<" -- "<<cmd[1]<<" -- "<<cmd[2]<<endl;
                cout<<FilesChunkVector[stoi(cmd[1])][cmd[2]].path<<endl;
                cout<<FilesChunkVector[stoi(cmd[1])][cmd[2]].chunks.size()<<endl;
                string strtemp="";
                for(auto c:FilesChunkVector[stoi(cmd[1])][cmd[2]].chunks)
                {
                    if(c)
                    strtemp+='1';
                    else
                    strtemp+='0';                    
                }
                cout<<strtemp<<endl;
                strcpy(sending_buffer,strtemp.c_str());
                cout<<"sending..\n";
                send(client_socket,sending_buffer,strtemp.size(),0);             
         
        }
        else if (cmd[0] == "get_chunks")
        {
             cout<<"in get chunks\n";
             string fpath = FilesChunkVector[stoi(cmd[1])][cmd[2]].path;
                
             FILE * fd = fopen(fpath.c_str(),"rb");
             int offset = stoi(cmd[3])*__512KB;
             cout<<offset<<" -- "<<cmd[3]<<endl;
             fseek(fd,offset,SEEK_SET);
             char tempar[__512KB];
             char ccc[1024];
             int readval = fread(tempar,1,__512KB,fd);
             cout<<readval<<" readval " << cmd[3]<<endl;
             string temp = to_string(readval);
             strcpy(ccc,temp.c_str());
             send(client_socket,ccc,temp.size(),0);
             int bytes = read(client_socket,recieving_buffer,1023); 
             cout<<bytes<<endl;
             send(client_socket,tempar,readval,0);             
             
         
             fclose(fd);  
 
        }               
         
        
        

    delete[] sending_buffer;
    delete[] recieving_buffer;
    close(client_socket);
   
}

void* as_server(void *)
{

 
    
    int peer_socket=-1;
    struct sockaddr_in  peer_socket_address;
    
    if ((peer_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
      onend("tracker socket failed....");
    } 
    
    bzero(&peer_socket_address,sizeof(peer_socket_address));
    peer_socket_address.sin_family=AF_INET;

    peer_socket_address.sin_addr.s_addr=inet_addr(PEER_IP.c_str());
    peer_socket_address.sin_port = htons(PEER_PORT);
    int optval =0;
    int optlen = sizeof(optval);
    setsockopt(peer_socket,SOL_SOCKET,SO_REUSEPORT,(char *)&optval,optlen);

    if(bind(peer_socket,(struct sockaddr *)(&peer_socket_address),sizeof(peer_socket_address)) < 0)
    {
        onend("Tracker... Unable to Bind");
    }
    if(listen(peer_socket,SERVER_BUFFER) < 0)
    {
        onend("Tracker .... Listening Failed..");
    }
    cout<<"Listening on IP :- "<<PEER_IP<<" and port no.:- "<<PEER_PORT<<endl; 
    vector<thread> Tvect;
       
    while(true)
    {
              

        int client_socket= accept(peer_socket,(struct sockaddr *)NULL,NULL);
        if(client_socket < 0)
        {
            onend("Client accept error...");
        }

        cout<<"connected..:"<<client_socket<<endl;
    
        
        Tvect.push_back(thread(handle_connection,client_socket));   
        
        
    }

   for (auto &th:Tvect)
   {
       if(th.joinable())
       {
           th.join();
       }
   }
    
    
    

 close(peer_socket);

 return NULL;

}

int upload_util(string &filepath,int client_socket,string &response,vector<string>&ans)
{
    FILE *fd = fopen(filepath.c_str(),"rb");
    if(fd == NULL)
    {
        return -1;
    }

    fseek(fd,0LL,(long long)SEEK_END);
    long long size = ftell(fd);
    fclose(fd);
    string filehash = getFileHash(filepath);
    string filechunkshash = getFileChunksHash(filepath);
    int chunks = (size/__512KB) + (bool)(size % __512KB);
    string fpath = filepath;
    pathresolver(fpath);
    string fname = get_filename(fpath);
    ans.push_back(fname);
    ans.push_back(to_string(chunks));
    ans.push_back(fpath);
    string res = fname + ' ' + fpath + ' ' + to_string(size) + ' ' + to_string(chunks) + ' ' + filehash + ' ' + filechunkshash;

    char sending_buffer[res.size()];
    char recieving_buffer[1024];
    strcpy(sending_buffer,res.c_str());
    send(client_socket,sending_buffer,res.size(),0);
    int bytesread = read(client_socket,recieving_buffer,sizeof(recieving_buffer));
    if(bytesread < 1)
    {
      return -2;
    }
    recieving_buffer[bytesread]=0;
    response = recieving_buffer;
    return 1;
}
void get_chunks(ChunksDetails d,vector<string>args)
{
    vector<string> cmd = {"get_chunks",args[0],args[1],to_string(d.num)};
    vector<char *> res = PeerConnect_util(d.port,d.ip,cmd);
   
    // cout<<res[1].size()<<" size-- "<<res[0].size()<<endl;
    cout<<"in get_chunk no "<< d.num<<endl;
    cout<<"hr1\n";
   char buf[__512KB];  
   cout<<"hr2\n";
    
    cout<<"hr3\n";
    int readval = stoi(res[1]);
    cout<<"hr4 : "<<readval;
    string fname = args[2]+"/"+args[1]+to_string(d.num);
    cout<<"hr5\n";
    FILE *fd = fopen(fname.c_str(),"wb");
    cout<<"hr6\n";
    fwrite(res[0],1,readval,fd);
    fclose(fd);
    cout<<"in _ get_chunks 2\n";
   
}
void get_chunk_vector(string ip,unsigned short port,vector<string>args)
{
    vector<string> cmd = {"get_chunk_vector",args[0],args[1]};
    string p = to_string(port);
    string res = PeerConnect_util(p,ip,cmd)[0];
    cout<<"response\n";
    cout<<res<<endl;
    
    for(int i=0;i<res.size();i++)
    {
        if(res[i] == '1')
        {
            ChunksDetails df;
            df.port = p;
            df.ip = ip;
            df.check = true;
            df.num = i;
            Downloaded_chunks[stoi(args[0])][args[1]][i] = df;
        }
    }
    cout<<"now _ here\n";
}
int piecewise_Algo(vector<string>&args,vector<string>&strar,string &chunkhash,long long &size)
{
    int chunks = size/__512KB + (bool) size%__512KB;
    string destination = args[2] + '/' + args[1];
    cout<<"8\n";
   
    cout<<destination<<endl;
    
    struct stat fstat;
    if(stat(destination.c_str(),&fstat) == 0)
        return -1;

    vector<ChunksDetails> tmpar(chunks);
    cout<<" chunk size : "<<chunks<<endl;
    unordered_map<string,vector<ChunksDetails>> tpchunks;
    tpchunks.insert({args[1],tmpar});
    Downloaded_chunks.insert({stoi(args[0]),tpchunks});
    cout<<"9\n";
    // string dummyval(size,0);
    // cout<<"10\n";
    // ofstream newfile(destination.c_str(),ios::out|ios::binary);
    // cout<<"11\n";
    // newfile.write(dummyval.c_str(),size);
    // cout<<"12\n";
    // newfile.close();


    vector<thread> th1;
    cout<<"check point\n";

    for(int i=0;i<strar.size();i+=2)
    {
        string ip = strar[i];
        unsigned short port = stoi(strar[i+1]);
        cout<<"check"<<i<<endl;
        th1.push_back(thread(get_chunk_vector,ip,port,args));
    }
    cout<<"check__\n";
    for(auto &t:th1)
    {
        if(t.joinable())
        t.join();
    }
cout<<"13\n";
    for(int i=0;i<chunks;i++)
    {
        if(Downloaded_chunks[stoi(args[0])][args[1]][i].check == false)
        return -2;
    }
cout<<"14\n";
    vector<thread> th2;
    
    int len = chunks;
    vector<ChunksDetails> &tempar = Downloaded_chunks[stoi(args[0])][args[1]];
    cout<<"15\n";
    random_shuffle(tempar.begin(),tempar.end());
    for(int i=0;i<chunks;i++)
    {
        th2.push_back(thread(get_chunks,tempar[i],args));
    }
    for(auto &t:th2)
    {
        if(t.joinable())
        t.join();
    }
    
    cout<<"16\n";
    FilesChunkVector[stoi(args[0])][args[1]].path = destination;
    vector<bool>temp(chunks,true);
    FilesChunkVector[stoi(args[0])][args[1]].chunks = temp;
    return 1;

}
int merge_files(string destination,int chunks)
{
    FILE *fd = fopen(destination.c_str(),"wb+");
    cout<<"destin : "<<destination<<endl;
    char temp[__512KB];
    FILE * tempfd = NULL;
    for(int i=0;i<chunks;i++)
    {
        string fname=destination+to_string(i);
        tempfd = fopen(fname.c_str(),"rb");
        if(tempfd == NULL)
        {
            onend("File error");
        }
        int readval = fread(temp,1,__512KB,tempfd);
        cout<<i<<" -- "<<fname<<" -- "<<readval<<endl;
        fwrite(temp,1,readval,fd);
        fclose(tempfd);
    }
    fclose(fd);
return 1;
}
int download_util(int client_socket,string &input,vector<string>&args)
{

   char sending_buffer[MAX_LARGE_BUFFER_SIZE],recieving_buffer[MAX_LARGE_BUFFER_SIZE];
   int bytesread; 
   strcpy(sending_buffer,input.c_str());   
   cout<<"4\n";
   send(client_socket,sending_buffer,strlen(sending_buffer),0); 
   cout<<"5\n";
   bytesread = read(client_socket,recieving_buffer,sizeof(recieving_buffer));
   if(bytesread < 1)
   {
      return -5;      
   }
   recieving_buffer[bytesread]=0;
   string str = recieving_buffer;
   if (str == "Not in group..")
   {
       return -1;
   }
   if ( str == "File not present..")
   {
       return -2;
   }
   cout<<"6\n : "<<str<<endl;
   vector<string> strar = parse(str);
   string chunkhash = strar[strar.size()-2];
   for(auto &c:strar)
   cout<<c<<endl;
   cout<<chunkhash<<endl;
   long long size = stoll(strar[strar.size()-1]);
   cout<<size<<endl;
   cout<<strar.size()<<endl;
   strar.pop_back();
   strar.pop_back();
   cout<<"6\n";
   
   int stat = piecewise_Algo(args,strar,chunkhash,size);
   if(stat !=1 )
   {
       send(client_socket,"failed",6,0);
       if(stat == -1)
            return -1;

       return -5;
   }
        

    cout<<"7 comeon\n";
    string destination = args[2] + '/' + args[1];

    int chunks = size/__512KB + (bool) size%__512KB;

    if(merge_files(destination,chunks) != 1)
    {
        send(client_socket,"failed",6,0);
        return -5;
    }
    
    send(client_socket,"done",5,0);
    bytesread = read(client_socket,recieving_buffer,sizeof(recieving_buffer));
    if(bytesread < 1)
        return -5;
    recieving_buffer[bytesread]=0;
    str = recieving_buffer;
    if(str == "Download Failed..")
    {
        send(client_socket,"failed",6,0);
        return -5;
    }

    if(str != getFileHash(destination))
    {
        send(client_socket,"failed",6,0);
        return -5;
    }
    send(client_socket,"correct",8,0);
    
    return 1;

}

void inline send_util(int &client_socket,string &input,char sending_buffer[],char recieving_buffer[])
{   

   int bytesread; 
   strcpy(sending_buffer,input.c_str());   
   send(client_socket,sending_buffer,strlen(sending_buffer),0); 
   bytesread = read(client_socket,recieving_buffer,1024);
   if(bytesread < 1)
   {
      close(client_socket);
      onend("Peer Terminated..");
   }
   recieving_buffer[bytesread]=0;
}
void parsecommand(vector<string>&args,string &input,int client_socket)
{


   string Errstr = "INVALID COMMAND OR ARGUMENTS...";
   if(args.size()==0)
   cout<< Errstr <<endl;
   string cmd=args[0];
   
   args.erase(args.begin()); 
   char recieving_buffer[1024];
   char sending_buffer[1024];
 

  
 if(cmd=="create_user")
 {
   
   send_util(client_socket,input,sending_buffer,recieving_buffer);
   cout<<recieving_buffer<<endl;
  
 }
 else if(cmd=="login")
 {
    send_util(client_socket,input,sending_buffer,recieving_buffer);
    cout<<recieving_buffer<<endl;
    
 }
 else if(cmd=="create_group")
 {  
     send_util(client_socket,input,sending_buffer,recieving_buffer);
     cout<<recieving_buffer<<endl;

     
 }
 else if(cmd=="join_group")
 { 
     send_util(client_socket,input,sending_buffer,recieving_buffer);
      cout<<recieving_buffer<<endl;
     
 }
 else if(cmd=="leave_group")
 {
    send_util(client_socket,input,sending_buffer,recieving_buffer);
     cout<<recieving_buffer<<endl;
    
 }
 else if(cmd=="list_requests")
 {  
      send_util(client_socket,input,sending_buffer,recieving_buffer);
      cout<<recieving_buffer<<endl;
 }
 else if(cmd=="accept_request")
 {
     send_util(client_socket,input,sending_buffer,recieving_buffer);
      cout<<recieving_buffer<<endl;
     
 }
 else if(cmd=="list_groups")
 { 
   send_util(client_socket,input,sending_buffer,recieving_buffer);
     cout<<recieving_buffer<<endl;
    
  
 }
 else if(cmd=="list_files")
 {

    send_util(client_socket,input,sending_buffer,recieving_buffer);
     cout<<recieving_buffer<<endl;
   
    
 }
 else if(cmd == "upload_file")
 {
     if(args.size() != 2)
     {
         cout<<Errstr;
         return;
     }
     if(pathresolver(args[0]) != 0)
        {
            cout<<"Invalid File Path...\n";
            return;
        }
     
     FILE * fd = fopen(args[0].c_str(),"rb");
     if(fd == NULL)
     {
         cout<<"Invalid File...\n";
         return;
     }
     fclose(fd);
     input = cmd + ' '+ args[0] +' '+ args[1];
     send_util(client_socket,input,sending_buffer,recieving_buffer);
     string res = recieving_buffer;
     if(res == "uploading")
     {
         vector<string> ans;
         int stat = upload_util(args[0],client_socket,res,ans);
         if(stat==-1)
         cout<<"File name invalid..\n";
         else if(stat==-2)
         cout<<"Tracker Error..\n";
         else
         {
             if(res == "File Uploaded...")
             {
                 
                 vector<bool>temp(stoi(ans[1]),true);
                 FileDetails fd;
                 fd.chunks  = temp;
                 fd.path = ans[2];
                 FilesChunkVector[stoi(args[1])][ans[0]]=fd;
                 cout<<"Chunks are : - \n";
                 for(auto c:fd.chunks)
                 {
                     cout<<c<<" ";
                 }
                 cout<<endl;
             }
             cout<<res<<endl;
         }
         
     }
     else
     {
         cout<<res<<endl;
     }
     
     
     
 }
 else if(cmd == "download_file")
 {
     cout<<"1\n";
    if(args.size() != 3)
     {
         cout<<Errstr;
         return;
     }
     cout<<"2\n";
     pathresolver(args[2]);
     cout<<"3\n";
     int stat = download_util(client_socket,input,args);
     if(stat == -1)
     cout<< "Not in group.."<<endl;
     else if(stat == -2)
        cout<< "File not present.."<<endl;
     else if(stat == -4)
        cout<< "File Corrupted"<<endl;
     else if(stat == -5)
        cout<< "Download Failed.."<<endl;

     cout<< "File Downloaded.."<<endl;

     
 }
 else if(cmd == "logout")
 {
      send_util(client_socket,input,sending_buffer,recieving_buffer);
      cout<<recieving_buffer<<endl;
      isloggedin = false;
 }
 else if(cmd == "show_downloads")
 {
     send_util(client_socket,input,sending_buffer,recieving_buffer);
     cout<<recieving_buffer<<endl;
 }
 else if(cmd == "stop_share")
 {
     send_util(client_socket,input,sending_buffer,recieving_buffer);
     cout<<recieving_buffer<<endl;
 }
 else
 {
     cout<<Errstr<<endl;
 }
 
 


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

int send_ipport(int client_socket)
{
    char send_buf[512],rec_buf[512];
   
    if(send(client_socket,"ipport",7,0) < 0)
    {
        return -1;
    }
   
    int byteread = read(client_socket,rec_buf,sizeof(rec_buf));
    if(byteread < 1)
    {
        return -1;
    }
   
    string res = PEER_IP+' '+to_string(PEER_PORT);
    strcpy(send_buf,res.c_str());
    if(send(client_socket,send_buf,res.size(),0) < 0)
    {
        return -1;
    }
    
    byteread = read(client_socket,rec_buf,sizeof(rec_buf));
    if(byteread < 1)
    {
        return -1;
    }
   
    return 1;

}

int main(int argc,char * argv[])
{

    FILE * trackinfo;

    string portip;
    if(argc != 3)
    {
        trackinfo = fopen("tracker_info.txt","r");
        portip = "127.0.0.1:38888";
 
    }
    else
    {
        trackinfo = fopen(argv[2],"r");
        portip = argv[1];

    }
    if(trackinfo == NULL)
    {
        onend("No such tracker info file..");
    }
    char IP_ADDR[50];
    unsigned short port;
    vector<string>ip_addrs;
    vector<short>ports;
    for(int i=0;i<2;i++)
    {
        fscanf(trackinfo,"%s",IP_ADDR);
        ip_addrs.push_back(IP_ADDR);
        fscanf(trackinfo,"%hu",&port);
        ports.push_back(port);
    }

    fclose(trackinfo);

    PEER_IP =  portip.substr(0,portip.find(':'));
    PEER_PORT = stoi(portip.substr(portip.find(':')+1));


    int client_socket=-1;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
      onend("server socket failed....");
    } 
    struct sockaddr_in  client_socket_address;
    
    pthread_t asServerTh;
    if(pthread_create(&asServerTh,NULL,as_server,NULL) < 0)
    {
        onend("Error in Run as Server..");
    }
 
    bzero(&client_socket_address,sizeof(client_socket_address));
    client_socket_address.sin_family=AF_INET;
    client_socket_address.sin_addr.s_addr=inet_addr(ip_addrs[0].c_str());
    client_socket_address.sin_port = htons(ports[0]);
    
    char recieving_buffer[1024];
    char sending_buffer[1024];

   if(connect(client_socket,(struct sockaddr *)&client_socket_address,sizeof(client_socket_address)) < 0)
   {
    client_socket_address.sin_addr.s_addr=inet_addr(ip_addrs[1].c_str());
    client_socket_address.sin_port = htons(ports[1]);

    if(connect(client_socket,(struct sockaddr *)&client_socket_address,sizeof(client_socket_address)) < 0)
    {
        onend("connection error ...");
    }

   }
   if(send_ipport(client_socket)!=1)
   {
       onend("Failed to send address to tracker..");
   }
 
   while(true)
   {
  
   int bytesread;
   string input;
   getline(cin,input);
   if(input.size()==0)
    continue;
   
   vector<string> args = parse(input);
   parsecommand(args,input,client_socket);


   }
   close(client_socket);
return 0;
}