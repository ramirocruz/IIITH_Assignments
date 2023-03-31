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
		  return;
	  }
      onend("In pathresolver.....Invalid Path...");
		
	}
	else
	path = cur+'/'+path;

char *resolvedpath = new char[4096];
realpath(path.c_str(),resolvedpath);
path = resolvedpath;	
delete [] resolvedpath;
resolvedpath=NULL;
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

bool remove_cmd(vector<string>&source)
{
static string output="";

for(auto src:source)
   {  output+= " --1-- "+src;
	  pathresolver(src); 	
	  output+=" --2-- "+src;
	  struct stat file_info;
	  if(stat(src.c_str(),&file_info)==-1)
	  {
		  onend("Remove_cmd...Error in reading file stat"+src+" ..... "+output);
	  }
	  if(S_ISDIR(file_info.st_mode))
	  {
		  vector<string> lists;
		  createfilesbuffer(src,lists);		 
		  if(remove_cmd(lists)==0)
		  return false;
          rmdir(src.c_str());
	  }
	  else
	  {
	       remove(src.c_str());
	  }	  
	
   }
   return true;

}

bool remove_file_util(string &source)
{
    string temp=source;
    pathresolver(source);
	string fname = get_filename(source);
	string parent = source.substr(0,source.size()-fname.size());
	fname=fname.substr(1);
  	if(search_dir(parent,fname,1)==0)
	return false;
	vector<string> src;
	src.push_back(temp);
	return remove_cmd(src);
}
bool remove_dir_util(string &source)
{   string temp=source;
    pathresolver(source);
	string fname = get_filename(source);
	string parent = source.substr(0,source.size()-fname.size());
	fname=fname.substr(1);
  	if(search_dir(parent,fname,0)==0)
	return false;
	vector<string> src;
	src.push_back(temp);
	return remove_cmd(src);
}
bool copy_cmd(vector<string>&source,string &destination)
{   

   for(auto src:source)
   {  
	  pathresolver(src);

	  cout<<src<<endl;
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
		  copy_permissions(src,newdest);
		 if(copy_cmd(lists,newdest)==false)
		 return false;
        
	  }
	  else
	  {
	       ifstream  _src(src, ios::binary);
		   if(_src.fail())
		   {
			   return false;
		   }
           ofstream  _dst(newdest, ios::binary);
	       _dst << _src.rdbuf();
		   copy_permissions(src,newdest);
	  }	  

	
   }
  return true; 
}

bool move_cmd(vector<string>&source,string &destination)
{
   
   if(copy_cmd(source,destination)==0)
   return false;
   return remove_cmd(source);
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



bool createfile_cmd(string &name,string &destination)
{   

	if(search_dir(destination,name,1))
	return false;
	name=destination+'/'+name;
	struct stat file_info;
	if(stat(destination.c_str(),&file_info)==-1)
	{
        onend("Create File..Error in reading file stat");
	}
	ofstream  _dst(name, ios::binary);
    chmod(name.c_str(),file_info.st_mode);
	return true;
}

bool create_dir_cmd(string &name,string &destination)
{

	if(search_dir(destination,name,0))
	return false;
	
	struct stat file_info;
	if(stat(destination.c_str(),&file_info)==-1)
	{
        onend("Create Dir..Error in reading file stat");
	}
    name=destination+'/'+name;
	if(mkdir(name.c_str(),file_info.st_mode)==-1)
	  return false;
	chmod(name.c_str(),file_info.st_mode);
	return true;

}

