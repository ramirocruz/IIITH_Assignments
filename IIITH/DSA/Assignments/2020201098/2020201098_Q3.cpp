#include<bits/stdc++.h>
using namespace std;

template <class T>
class Deque{

int fptr,bptr;
int s,cap;
T *head;
public:
Deque()
{
    s=0;
    cap=128;
    head=new T[128];
    fptr=bptr=-1;
}
Deque(int n,T data)
{   
    cap = max(2*n,128);
    head= new T[cap];
    fptr = 0;bptr = n-1;
    for(int i=0;i<n;i++)
    {
      head[i] = data;
    }  

    s=n;
    
}

void push_back(T data)
{
   if(bptr == -1)
   {
       fptr=0;
       bptr=0;
       head[bptr]=data;
   }
   else
   {
       if((bptr+1)%cap != fptr)
       {
           head[(bptr+1)%cap]=data;
           bptr = (bptr+1)%cap;
       } 
       else
       {
           T *temp=new T[2*cap];
              int k=0;  
           for(int i=0;i<s;i++)
           temp[k++]=(*this)[i];

            fptr=0;bptr=k-1;
            cap*=2;
            delete[] head;
            head = temp;
            temp=NULL;
            delete temp;
            bptr++;
            head[bptr]=data;
            
       }
   }
    
    
    s++;

}
void push_front(T data)
{
  if(fptr==-1)
  {
      fptr=0;bptr=0;
      head[fptr]=data;
  }
  else
  {
      if((fptr-1+cap)%cap != bptr)
      {
          fptr=(fptr-1+cap)%cap;
          head[fptr]=data;
          
      }
      else
      {
           T *temp=new T[2*cap];
            
            int k=0;  
            temp[k++]=data;            
           for(int i=0;i<s;i++)
           temp[k++]=(*this)[i];

            fptr=0;bptr=k-1;
            cap*=2;
            delete[] head;
            head = temp;
            temp=NULL;
            delete temp;           

       }
  }
    s++;

}
void pop_front()
{
    if(s)
    {
     fptr = (fptr+1)%cap;
     s--;
     if(s==0)
    { fptr=-1;
     bptr=-1;}
    }
}
void pop_back()
{
    if(s)
{  
    bptr = (bptr-1+cap)%cap;
    s--;
    if(s==0)
    {bptr=-1;
    fptr =-1;
    }
}
}

T front()
{ 
    
  return head[fptr];
}
T back()
{
    
    return head[bptr];
}
int size()
{
    return s;
}
bool empty()
{
    if(s==0)
    return true;
    return false;
}
void clear()
{   
 
    delete[] head;
    head = new T[20];
    s=0;
    cap=128;
    fptr=bptr=-1;
   
  
}
void resize(int n,T data)
{   if(n==0)
    clear();
    else if(s==0)
    {
    if(n>cap)
    {
       delete[] head;
       cap = 2*n;
       head= new T[cap];
    }     
    fptr = 0;bptr = n-1;
    for(int i=0;i<n;i++)
    {
      head[i] = data;
    }  

    s=n;
    }
    else if(n<s)
    {
        bptr = (fptr + n-1)%cap;
        s=n;
    }
    else if(n <= cap)
    {
        for(int i=1;i<(n-s+1);i++)
        {   
            head[(i+bptr)%cap]=data;
        }
        bptr = (fptr + n-1)%cap;
        s=n;
    }
    else
    {

       T * temp = new T[2*n];
           int k=0;      
           for(int i=0;i<s;i++)
           temp[k++]=(*this)[i];
            fptr=0;bptr=k-1;
            cap=2*n;
            delete[] head;
            head = temp;
            temp=NULL;
            delete temp; 

        for(int i=1;i<(n - s+1);i++)
        {   
            head[(i+bptr)%cap]=data;
        }
        bptr = n-1;
        s=n;
        

    }

}
T& operator [](int index)
{
    if(index >= s)
    {
        cout<<"Out of bounds deque....";
        exit(1);
    }
    
    return head[(fptr+index)%cap];   
    

}
void display()
{
   
    for(int i=0;i<s;i++)
    {   
        cout<<(*this)[i]<<' ';
    }
    cout<<'\n';
}

};





int main()
{ 
// freopen("output.txt","w",stdout);
Deque<string> *arr =NULL;
    int Q,c,n;
    string x;
    cin>>Q;
    while (Q--){      
 
 cin>>c;

 switch (c)
 {

 case 1: cin>>x;
         arr->push_front(x);  
         arr->display();

         break;
 case 2: arr->pop_front();
         arr->display();
         break;
 
 case 3: cin>>x;
         arr->push_back(x);
         arr->display();
         break;
 
 case 4: arr->pop_back();
         arr->display();
         break;
 
 case 5: if(arr !=NULL)
         delete arr;
         arr = new Deque<string>();
         break;

 case 6: cin>>n>>x;
         if(arr !=NULL)
         delete arr;
         arr = new Deque<string>(n,x);
         arr->display();
         break;

 case 7: cout<<arr->front()<<endl;
         break;

 case 8: cout<<arr->back()<<endl;
         break;
 
 case 9: if(arr->empty())
          cout<<"true\n";
         else
          cout<<"false\n";
         break;

 case 10: cout<<arr->size()<<endl;
          break;

 case 11: cin>>n>>x;
          arr->resize(n,x);
          arr->display();
          break;

 case 12: arr->clear();
          break;
 
 case 13: cin>>n;
          cout<<(*arr)[n]<<endl;
          break;

 case 14: arr->display();
         break;

 default:
     break;
 }
    }


delete arr;
return 0;
}