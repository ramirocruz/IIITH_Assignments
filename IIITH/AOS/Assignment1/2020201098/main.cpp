#include<bits/stdc++.h>
#include<pwd.h>
#include<grp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <dirent.h>
#include "commands.h"
using namespace std;
winsize terminal;
termios original,raw_one,raw_timeout;
unsigned int FILE_POINTER,FILES_LEN,TERMINAL_LEN,CURSOR,SCROLL_POINTS=0,pagesize=10,SCR=10;
string rootname;
vector<vector<string>> files_list;
stack<string> backstack,forwardstack;

void onend(string);
void onend(void);
void onend(int);
void Raw_off(void);
void Raw_on(void);
void clear(void);
void display(void);
void cursor(void);
void getTerminalHeight(int);

bool comp(const vector<string>&a,const vector<string>&b)
{
     return a[5] < b[5];
}

/******************************************************
*******************************************************
 Get Cursor Position
*******************************************************
*******************************************************/
int get_cursor_pos(int &len) {
 string str;
 len=0;
 char c=0;
 write(1, "\033[6n", 4);

 for( int i = 0; c != 'R'; i++ )
 {
    if(read(STDIN_FILENO,&c,1)==-1)
    {
        return 1;
    }
   str+=c;
 }
if(str.size()<2)
return 1;

for(int i=2;str[i]!=';';i++)
{
    len = len*10 + (str[i] - '0');
}
return 0;
}


/******************************************************
*******************************************************
 Get Terminal height function
*******************************************************
*******************************************************/

void getTerminalHeight(int sig)
{
 clear();
 if((ioctl(STDOUT_FILENO,TIOCGWINSZ,&terminal))== -1)
 {
     printf("Error......\n");
     exit(1);
 }

 TERMINAL_LEN = terminal.ws_row;
 SCR= (TERMINAL_LEN -2 > 1)?TERMINAL_LEN -2 : 1;
 SCROLL_POINTS=0;
 if(sig==SIGWINCH)
  {
   display();
     
  }
 

}
/******************************************************
*******************************************************
     Clear Screen Function
*******************************************************
*******************************************************/

 void clear()
{

//    printf("\x1b[2J\x1b[1;1H");
   printf("\033c");

// system("clear");
}

/******************************************************
*******************************************************
    Raw Mode off
*******************************************************
*******************************************************/
void Raw_off()
{
    tcflush(STDIN_FILENO, TCIFLUSH);
    if((tcsetattr(STDIN_FILENO,TCSANOW,&original)) ==-1)
    {
        cout<<"Error in Raw off\n";
        exit(1);
    }
}

/******************************************************
*******************************************************
    Function to run just before termination:
    Called by Ctrl + C signal
*******************************************************
*******************************************************/

void onend(int n)
{
    clear();
    Raw_off();
       
    exit(1);

}

/******************************************************
*******************************************************
    Exit function with Type of error input
*******************************************************
*******************************************************/

void onend(string error)
{
    clear();
    Raw_off();
    cout<<"Error:...is..."<<error<<endl;   
    exit(1);

}

/******************************************************
*******************************************************
    Called before end
*******************************************************
*******************************************************/
void onend(void)
{
 
    Raw_off();  

}



/******************************************************
*******************************************************
    Save the initial details of Termios
*******************************************************
*******************************************************/

void Termios_init()
{
    
    
    if( (tcgetattr(STDIN_FILENO,&original)) == -1)
     {
         cout<<"Error.....\n";
         exit(1);
     }
     bzero(&raw_one, sizeof(raw_one));
     raw_one = original;
     raw_one.c_lflag &= ~(ICANON | ECHO);

     bzero(&raw_timeout, sizeof(raw_timeout));
     raw_timeout = original;
     raw_timeout.c_lflag &= ~(ICANON | ECHO);
     raw_timeout.c_cc[VMIN]=0;
     raw_timeout.c_cc[VTIME]=1;
    
}


/******************************************************
*******************************************************
    Raw Mode On
*******************************************************
*******************************************************/

void Raw_on()
{

    tcflush(STDIN_FILENO, TCIFLUSH);
    if((tcsetattr(STDIN_FILENO,TCSANOW,&raw_one)) == -1)
    {
        onend("raw on");
    }
}





/******************************************************
*******************************************************
 Fill up the vector with file list details
*******************************************************
*******************************************************/
void createdirbuffer()
{
SCROLL_POINTS=0;
CURSOR=1;
files_list.clear();
DIR *dir;
dirent *current_read;
char path[]=".";
dir = opendir(path);
if(dir == NULL)
{
  onend("No directory Found");
}
struct stat filedetail ;
string cdir = get_current_dir_name();

while((current_read=readdir(dir)) != NULL)
{
   string par=current_read->d_name;
  if( par== ".." && cdir == rootname)
    continue;


  vector<string>temp; 
 
  
      if(stat(current_read->d_name,&filedetail)==-1)
      {
          onend("Failed to get stats from file");
      }
      
      
      struct passwd *tpuname = getpwuid(filedetail.st_uid);
      struct group *tpgname = getgrgid(filedetail.st_gid);
       
      string perm;
      perm+=( (S_ISDIR(filedetail.st_mode)) ? "d" : "-");
      perm+=( (filedetail.st_mode & S_IRUSR) ? "r" : "-");
      perm+=( (filedetail.st_mode & S_IWUSR) ? "w" : "-");
      perm+=( (filedetail.st_mode & S_IXUSR) ? "x" : "-");
      perm+=( (filedetail.st_mode & S_IRGRP) ? "r" : "-");
      perm+=( (filedetail.st_mode & S_IWGRP) ? "w" : "-");
      perm+=( (filedetail.st_mode & S_IXGRP) ? "x" : "-");
      perm+=( (filedetail.st_mode & S_IROTH) ? "r" : "-");
      perm+=( (filedetail.st_mode & S_IWOTH) ? "w" : "-");
      perm+=( (filedetail.st_mode & S_IXOTH) ? "x" : "-");
      temp.push_back(perm);
      temp.push_back(tpuname->pw_name);
      temp.push_back(tpgname->gr_name);    
      temp.push_back(to_string(filedetail.st_size));
      string t=ctime(&(filedetail.st_mtim.tv_sec));
      t.pop_back();
      temp.push_back(t);
      temp.push_back(current_read->d_name);
      
  
  
  files_list.push_back(temp);  
 
}
closedir(dir);
sort(files_list.begin(),files_list.end(),comp);

FILES_LEN = files_list.size();
CURSOR=1;
}


/******************************************************
*******************************************************
   Display Function using vector of files details
*******************************************************
*******************************************************/
void display()
{
 clear();
 
 pagesize=SCR;
 if(pagesize+SCROLL_POINTS > FILES_LEN)
 {
     pagesize = FILES_LEN - SCROLL_POINTS;
 }
 
 CURSOR=pagesize+1;
 for(int i=SCROLL_POINTS; i < SCROLL_POINTS+pagesize;i++)
 {
     int j=0;
    cout<<i+1<<'/'<<FILES_LEN<<'\t';
    for(auto &el:files_list[i])
    {
        cout<<el<<'\t';
    }
     cout<<endl;
 }
 
 return;
}

/******************************************************
*******************************************************
    Backmotion
*******************************************************
*******************************************************/

void backmotion()
{
 if(backstack.empty())
  return;
 forwardstack.push(get_current_dir_name());
 char ar[backstack.top().size()+1];
 strcpy(ar,backstack.top().c_str());
 
 backstack.pop();
 chdir(ar);
 createdirbuffer();
 display();
}

/******************************************************
*******************************************************
    Forward Motion
*******************************************************
*******************************************************/

void forwardmotion()
{
 if(forwardstack.empty())
  return;
 backstack.push(get_current_dir_name());
 char ar[forwardstack.top().size()+1];
 strcpy(ar,forwardstack.top().c_str());
 
 forwardstack.pop();
 chdir(ar);
 createdirbuffer();
 display();
}

/******************************************************
*******************************************************
    GOTO Anywhere
*******************************************************
*******************************************************/

bool goto_cmd(string &path)
{
    pathresolver(path);
    string temp = get_current_dir_name();
    if(temp == path)
    return true;
    if(path.size()<rootname.size())
    return false;   
    
    if(chdir(path.c_str())==-1)
    return false;

    backstack.push(temp);
    if(forwardstack.size())
    {
    stack<string>s;
    forwardstack.swap(s);
    }
    createdirbuffer();
    display();
    return true;
}



/******************************************************
*******************************************************
    Jump to Home Directory
*******************************************************
*******************************************************/

void gotohome()
{
    string temp = get_current_dir_name();
    if(temp == rootname)
    return;
    backstack.push(temp);
    stack<string>s;
    forwardstack.swap(s);
    
    chdir(rootname.c_str());
    createdirbuffer();
    display();
}

/******************************************************
*******************************************************
   Goto Parent Directory
*******************************************************
*******************************************************/
void gotoparent()
{
    string temp = get_current_dir_name(); 
    
    stack<string>s;
    forwardstack.swap(s);
   
    if(files_list[1][5]=="..")
    {
      backstack.push(temp);
      chdir("..");
      createdirbuffer();
      display();
    }
}

/******************************************************
*******************************************************
               Parse Command Function :
*******************************************************
*******************************************************/
string parsecommand(vector<string>&args)
{
   if(args.size()<2)
   return "INVALID COMMAND OR ARGUMENTS...";
   string cmd=args[0];
   string destination=args[args.size()-1];
   pathresolver(destination);
   args.erase(args.begin()); 
  
 if(cmd=="copy")
 {
   args.pop_back();
   if(args.size())
   {
       if(copy_cmd(args,destination))
       return "Copied";
       return "Failed";
   }
   return "Invalid arguments";
 }
 else if(cmd=="move")
 {
   args.pop_back();
   if(args.size())
   {
       if(move_cmd(args,destination))   
       return "Moved";
       return "Failed";
   }
   return "Invalid arguments";
 }
 else if(cmd=="rename")
 { 
   if(args.size()==2)
   {
        if(rename_cmd(args[0],args[1]))
        return "Renamed";
        return "Failed";
   }
   return "Invalid arguments";
 }
 else if(cmd=="create_file")
 {
   if(args.size()==2)
   {
        if(createfile_cmd(args[0],destination))
        return "Success";
        return "Failed";
   }
   else if(args.size()==1)
   {
        destination=get_current_dir_name();
        if(createfile_cmd(args[0],destination))
        return "Success";
        return "Failed";
   }
   
   return "Invalid Arguments";
 }
 else if(cmd=="create_dir")
 {
   if(args.size()==2)
   {
        if(create_dir_cmd(args[0],destination))
        return "Success";
        return "Failed";
   }
   else if(args.size()==1)
   {
        destination = get_current_dir_name();
        if(create_dir_cmd(args[0],destination))
        return "Success";
        return "Failed";
   }
   return "Invalid Arguments";
 }
 else if(cmd=="delete_file")
 {  
   if(args.size()==1)
   {  
        if(remove_file_util(args[0]))
        return "Removed";
        return "Failed";
   }
   return "Invalid Arguments";
 }
 else if(cmd=="delete_dir")
 {
   if(args.size()==1)
   {
        if(remove_dir_util(args[0]))
        return "Removed";
        return "Failed";
   }
   return "Invalid Arguments";
 }
 else if(cmd=="goto")
 { 
   if(args.size()>1)
   return "Invalid Arguments";

   string temp;
   if(goto_cmd(args[0]))
   return "DONE";
   return "INVALID PATH";
 }
 else if(cmd=="search")
 {
    if(args.size()>1)
    return "Invalid Arguments";

    string currentpath=get_current_dir_name();
   if(search_dir_recursive_cmd(currentpath,args[0]))
    return "FOUND";

    return "NOT FOUND";   
    
 }
 
return "INVALID COMMAND OR ARGUMENTS...";
}

/******************************************************
*******************************************************
               Parse String Command:
*******************************************************
*******************************************************/
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

/******************************************************
*******************************************************
            COMMAND MODE  Cursor Movement:
*******************************************************
*******************************************************/

string cmodecursor()
{
    string command;
char c[4]={0};

while (true)
{
  
  read(STDIN_FILENO, &c, 3);


  if(c[0]==10)
  {
    
     return command;
  }
  if(iscntrl(c[0])){
  if(c[0]==127)
  {
      
      if(command.size())
      {command.pop_back();
       char ar[]="\b \b";
       write(STDOUT_FILENO,&ar,3);
      }
  }    
  if(c[0]==27)
  {  
         
     if(c[1]==c[2] && c[1]==0)   
     return "00";
   
  }
  }else
  {
      command+=c[0];
      write(STDOUT_FILENO,&c,1);
  }
  



  

}
}


/******************************************************
*******************************************************
                COMMAND MODE :
*******************************************************
*******************************************************/
void commandmode()
{
  while(true)
  {
  printf("\x1b[%d;1HEnter your Command:\n",TERMINAL_LEN-1);
  char arr[]=">>> ";
  write(STDOUT_FILENO,&arr,sizeof(arr));
  string inp;
  inp=cmodecursor();
   if(inp=="00")
   {
    display();
    
    return;
   }
   if(inp.size()==0)
   continue;
   
   vector<string> args;
   args=parse(inp);
   string res=parsecommand(args);
   createdirbuffer();
   display();
   printf("\x1b[%d;%dH Result:~ ",TERMINAL_LEN-1,25);
   cout<<res<<endl;
  }
}


/******************************************************
*******************************************************
                Cursor Movement:
*******************************************************
*******************************************************/


void cursor()
{  

char c[4]={0};
int c_Y;

while (true)
{
  c[1]=c[2]=0;
  read(STDIN_FILENO,&c,3);
  if(c[0]==10)
  {
     if(CURSOR < pagesize+1)
     return ;
  }

  if(c[0]==27)
  {  
     
     
     if(c[1]==91)
     {
         get_cursor_pos(c_Y);
         switch (c[2])
         {
         case 'A':   if(c_Y>pagesize+1)
                     {
                      char ab[]="\x1b[A";
                      write(STDOUT_FILENO,&ab,sizeof(ab)); 
                     }
                     if(CURSOR > 1)
                     {                            
                         CURSOR--;                    
                      
                      char ab[]="\x1b[A";
                      write(STDOUT_FILENO,&ab,sizeof(ab));                          
                     }
                        
            break;

         case 'B': if(CURSOR < pagesize+1)
                    {

                    CURSOR++;    
                   
                    char ab[]="\x1b[B";
                      write(STDOUT_FILENO,&ab,sizeof(ab)); 

                    }
            break;      
         case 'C': forwardmotion();
            break;

         case 'D': backmotion();
            break;
         
         default:
             break;
         }
         
     }
  }



  if(c[0]=='k' || c[0]=='K')
  {
   if(SCROLL_POINTS>=SCR)
   SCROLL_POINTS-=SCR;
   display();
  }

  if(c[0]=='l'|| c[0]=='L')
  {
    if(SCROLL_POINTS+SCR<=FILES_LEN)
    SCROLL_POINTS+=SCR;   
    display();
  }
    

  if(c[0]=='h' || c[0]=='H')
  {
      gotohome();
  }
   if(c[0]== 127)
  {
      gotoparent();
  }

  if(c[0]==':')
  {
      commandmode();
   
    
  }

}

}


/******************************************************
*******************************************************
                Main Function
*******************************************************
*******************************************************
*******************************************************
                Main Function Begin
*******************************************************
*******************************************************/


int main()
{
    Termios_init();

 printf( "\x1b[8;999;999t" );

signal(SIGINT,onend);
signal(SIGWINCH,getTerminalHeight);
getTerminalHeight(0);
rootname=get_current_dir_name();
Raw_on();
while (true)
{

start:
createdirbuffer();
display();
cursor();
string temp = files_list[SCROLL_POINTS+CURSOR-1][5];


string histdir=get_current_dir_name();
if(chdir(temp.c_str())!=-1)
 {
     backstack.push(histdir);
 }
 else
 {
    int pid = fork();
    
    if (pid == 0) 
    {

       
        execl("/usr/bin/xdg-open", "xdg-open",temp.c_str(), (char *)0);
        Raw_off();
        exit(1);
    }
    
    
   
 
 }
 
}


Raw_off();

return 0;
}




/******************************************************
*******************************************************
                Main Function
*******************************************************
*******************************************************
*******************************************************
                Main Function End
*******************************************************
*******************************************************/