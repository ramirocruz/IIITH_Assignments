#include<bits/stdc++.h>
using namespace std;

template <class T,class D>
struct Node
{
  T key;
  D data;
  Node * next;
  Node()
  {
      next=NULL;
  }
  Node(T k,D d)
  {
      key=k;
      data=d;
      next=NULL;
  }
};
template <class T>
int hash_gen(T k)
{
    return 0;
}
template<class T>
struct Equal
{
bool operator () (const T &a, const T &b)
{
    return (a==b);
}
};
template<class T,class D,class Eql=Equal<T>>
class Unordered_map
{
    Node<T,D> **hash_map;
    int table_size;
    int size_val;
    Eql eql;

    void init_table(Node<T,D> **p,int len)
    {
       for(int i=0;i<len;i++)
       {
           p[i]=NULL;
       }
    }
    bool isloaded()
    {
        double ans = (double)size_val/table_size;
        if(ans > 3)
        return true;
        return false;
    }
    void table_doubling()
    {
        // cout<<"Table doubling initiated.....\n";
        Node<T,D> **temp=new Node<T,D>*[2*table_size];
        init_table(temp,2*table_size);
        for(int i=0;i<table_size;i++)
        {
            temp[i]=hash_map[i];
            hash_map[i]=NULL;
        }
        delete [] hash_map;
        hash_map=temp;
        temp=NULL;
        delete temp;
        table_size*=2;
    }
    public:
    Unordered_map()
    {
        table_size=3;
        hash_map= new Node<T,D> *[table_size];
        init_table(hash_map,table_size);
        size_val=0;
        
    }

    void insert(T k,D d)
    {
        auto temp= new Node<T,D>(k,d);
        int pos=hash_gen(k);
        if(hash_map[pos]==NULL)
        hash_map[pos]=temp;
        else
        {
            auto p=hash_map[pos];
            while(p->next != NULL)
            {
                if(eql(p->key,))
                p=p->next;
            }
            p->next = temp;
        }
        size_val++;
        if(isloaded())
        table_doubling();
    }

  int erase(T k)
   {
       int pos = hash_gen(k);
       if(hash_map[pos]==NULL)
       return -1;
       auto p=hash_map[pos];

       if(eql(p->key,k))
        {
                  hash_map[pos]=p->next;
                  delete p;
                  size_val--;
                  return 0;
        }
       
           auto prev = p;
           p=p->next;


           while(p != NULL)
           {
              if(eql(p->key,k))
              {
                  prev->next=p->next;
                  p->next=NULL;
                  delete p;
                  prev=NULL;
                  delete prev;
                  size_val--;
                  return 0;
              }
              prev=p;
              p=p->next;
           }
           return -1;
       
   } 
   bool find(T k)
   {
       int pos=hash_gen(k);
       auto p=hash_map[pos];
       while(p!=NULL)
       {
           if(eql(p->key,k))
           {
               return true;
           }
           p=p->next;
       }
       return false;
   }
   D& operator [] (T k)
   {
       int pos=hash_gen(k);     
       auto p=hash_map[pos];
       while(p!=NULL)
       {
           if(eql(p->key,k))
           {
               return (p->data);
           }
           p=p->next;
       }
       auto dummyval = new D;
       D &newdata = *dummyval;
       dummyval=NULL;
       delete dummyval;
       insert(k,newdata);
       return (*this)[k];
       

   }
   int size()
   {
       return size_val;
   }
   void show()
   {
       cout<<"...........................\n";
       for(int i=0;i<table_size;i++)
       {
           if(hash_map[i]!=NULL)
           {
               auto p=hash_map[i];
               while(p!=NULL)
               {
                   cout<<p->key<<" "<<p->data<<endl;
                   p=p->next;
               }
           }
       }
       cout<<"............................\n";
   }
};
int main()
{
ios_base::sync_with_stdio(false);

Unordered_map<string,string> test;
string k;
string d;
int mode,Q;
cin>>Q;
while(Q--)
{
    cin>>mode;

    switch(mode)
    {
        case 1: cin>>k>>d;
                test.insert(k,d);
                break;
        case 2: cin>>k;
                if(test.erase(k)==-1)
                cout<<"Key not present..\n";
                break;
        case 3: cin>>k;
                if(test.find(k))
                cout<<"Found\n";
                else
                cout<<"Not Found\n";
                break;
        case 4: cin>>k;
                cout<<test[k]<<endl;
                break;
        case 5: test.show();
                break;
        case 6: cout<<test.size()<<endl;
                break;
        default: cout<<"Enter Valid Mode....\n";
        
    }
}
return 0;
}