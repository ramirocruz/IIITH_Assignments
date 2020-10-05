#include<dirent.h>
#include<unistd.h>
#include<fstream>
#include<sys/stat.h>
#include<bits/stdc++.h>
using namespace std;

extern string rootname;
extern void onend(string);
void inline pathresolver(string &path)
{
	string cur=get_current_dir_name();
	if(path[0]=='~')
	{   
		if(rootname[rootname.size()-1]=='/')
		path=rootname+path.substr(2);

		
	}
	else if(path[0]=='/')
	{
      if(rootname[rootname.size()-1]=='/')
		path=rootname+path.substr(1);

		
	}
	path = cur+'/'+path;

char *resolvedpath = new char[4096];
realpath(path.c_str(),resolvedpath);
path = resolvedpath;	
delete [] resolvedpath;
}
void copy_permissions(string &src,string &desc)
{
  struct stat file_info;
  if(stat(src.c_str(),&file_info)==-1)
   {
	   onend("Copy Permissions...Reading Stats");
   }
   chown(desc.c_str(),file_info.st_uid,file_info.st_gid);
   chmod(desc.c_str(),file_info.st_mode);
}

bool search_dir(string &path,string &filename,int type)
{

DIR *dir;
dirent *current_read;
dir = opendir(path.c_str());
if(dir == NULL)
{
  
  onend("Error..in..search...no directory found.\n");
}
bool ans=false;
if(type==0)
{
while((current_read=readdir(dir)) != NULL)
{
  string name = current_read->d_name;
  if( current_read->d_type==DT_DIR)
  {
	  if(filename==name)
	  {
		  ans=true;
		  break;
	  }
  }
}
}
else  if(type==1)
{

while((current_read=readdir(dir)) != NULL)
{
  string name = current_read->d_name;
  if( current_read->d_type==DT_REG)
  {
	  if(filename==name)
	  {
		  ans=true;
		  break;
	  }
  }
}
}
else 
{
	while((current_read=readdir(dir)) != NULL)
{
  string name = current_read->d_name;
  
	  if(filename==name)
	  {
		  ans=true;
		  break;
	  }

}

}
closedir(dir);
return ans;	
}

bool search_dir_recursive_cmd(string &path,string &filename)
{


DIR *dir;
dirent *current_read;
dir = opendir(path.c_str());
if(dir == NULL)
{
  
  onend("Error..in..search.recursive no directory found...\n");
}

while((current_read=readdir(dir)) != NULL)
{
  string name = current_read->d_name;
  if(name=="." || name=="..")
  continue;
//   cout<<":::::"<<name<<endl;
 

  if(current_read->d_type == DT_DIR)
  {
	if(name==filename)
	{
	   closedir(dir);
	   return true;
	}
	 string newpath=path+'/'+name;
	  if(search_dir_recursive_cmd(newpath,filename))
	   {
	   closedir(dir);
	   return true;
	   }
  }
  else
  {

	  if(name==filename)
	{
	   closedir(dir);
	   return true;
	}
	else
	{
		int i;
		for( i=name.size()-1;i>-1;i--)
		{
			if(name[i]=='.')
			break;
		}
		if(name.substr(0,i)==filename)
		{
	      closedir(dir);
	      return true;
	    }
	}	
  }
}

closedir(dir);
return false;

}



void createfilesbuffer(string path,vector<string>&lists)
{

DIR *dir;
dirent *current_read;
dir = opendir(path.c_str());
if(dir == NULL)
{
  onend("Createfilesbuffer....No Dir Found..");
}

while((current_read=readdir(dir)) != NULL)
{
   string par=current_read->d_name;
  if( par== ".." || par==".")
    continue;

  lists.push_back(path+'/'+par);  
 
}
closedir(dir);

}
string get_filename(string &path)
{
	for(int i=path.size()-1;i>-1;i--)
	{
		if(path[i]=='/')
		{
			return path.substr(i);
		}
	}
	return path;
}

void remove_cmd(vector<string>&source)
{
string cur=get_current_dir_name();

for(auto &src:source)
   {  
	  pathresolver(src); 
	
	  
	  struct stat file_info;
	  if(stat(src.c_str(),&file_info)==-1)
	  {
		  onend("Remove_cmd...Error in reading file stat");
	  }
	  if(S_ISDIR(file_info.st_mode))
	  {
		  vector<string> lists;
		  createfilesbuffer(src,lists);		 
		  remove_cmd(lists);
          rmdir(src.c_str());
	  }
	  else
	  {
	       remove(src.c_str());
	  }	  
	
   }

}

bool copy_cmd(vector<string>&source,string &destination)
{
   string cur=get_current_dir_name();
   pathresolver(destination);
   if(!search_dir(destination,destination,0))
   return false;
   for(auto &src:source)
   {  
	  pathresolver(src);
	  
	  string newdest=destination+get_filename(src);

	  struct stat file_info;
	  if(stat(src.c_str(),&file_info)==-1)
	  {
		  onend("Copy cmd...Error in reading stat file...");
	  }
	  if(S_ISDIR(file_info.st_mode))
	  {
		  vector<string> lists;
		  createfilesbuffer(src,lists);
		  if(mkdir(newdest.c_str(),file_info.st_mode)==-1)
		   return false;
		 
		 return copy_cmd(lists,newdest);
        
	  }
	  else
	  {
	       ifstream  _src(src, ios::binary);
           ofstream  _dst(newdest, ios::binary);
	       _dst << _src.rdbuf();
		   copy_permissions(src,newdest);
	  }	  

	
   }
  return true; 
}

void move_cmd(vector<string>&source,string &destination)
{
   copy_cmd(source,destination);
   remove_cmd(source);
}

bool rename_cmd(string &from,string &to)
{
   pathresolver(from);
   int x=get_filename(from).size();
   string dest=from.substr(0,from.size()-x+1)+to;

   if(rename(from.c_str(),dest.c_str())==-1)
   return false;
   return true;
}



bool createfile_cmd(string name)
{   
	pathresolver(name);
	string fname=get_filename(name);
	string parent=name.substr(0,name.size() - fname.size());
	if(search_dir(parent,fname,1))
	return false;
	ofstream  _dst(name, ios::binary);
	return true;
}

bool create_dir_cmd(string name)
{
	pathresolver(name);
	string fname=get_filename(name);
	string parent=name.substr(0,name.size() - fname.size());
	if(search_dir(parent,fname,0))
	return false;
	struct stat file_info;
	if(stat(parent.c_str(),&file_info)==-1)
	{
        onend("Creat Dir..Error in reading file stat");
	}

	if(mkdir(name.c_str(),file_info.st_mode)==-1)
	  return false;
	
	return true;

}
