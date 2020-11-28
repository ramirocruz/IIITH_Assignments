#include<bits/stdc++.h>
using namespace std;


template<class T,class D>
struct Node
{
    T key;
    D value;
    int height;
    Node *left,*right;
    Node(T k,D v)
    {
        key=k;
        value =v;
        left=NULL;
        right=NULL;
        height=0;
    }

};

template<class T>
struct Less {
bool operator() (const T& a, const T& b)
    {
        return a<b;
    }
};
template<class T>
struct Equal {
bool operator() (const T& a, const T& b)
    {
        return a==b;
    }
};


template<class T,class D,class Comparator = Less<T>,class Comparator2 = Equal<T>>
class Map
{   
    
    struct Node<T,D> *root;
    Comparator comp;
    Comparator2 eql;
   
    int size_val;
    int height_util(Node<T,D>*p)
    {
        if(p==NULL)
        return -1;
        return (1 + max(height_util(p->left),height_util(p->right)));
    }
    int get_balance_util(Node<T,D>*p)
    {
        if(p==NULL)
        return 0;
        return (height_util(p->left)-height_util(p->right));
    }
    Node<T,D>* right_rotation_util(Node<T,D>* &p)
    {
       auto temp = p->left;       
       p->left = temp->right;
       temp->right=p;
       p->height = 1 + max(height_util(p->left),height_util(p->right));
       temp->height= 1 + max(height_util(temp->left),height_util(temp->right));
       return temp;
    }
      Node<T,D>* left_rotation_util(Node<T,D>* &p)
    {
        auto temp=p->right;
        p->right=temp->left;
        temp->left=p;
        p->height = 1 + max(height_util(p->left),height_util(p->right));
        temp->height= 1 + max(height_util(temp->left),height_util(temp->right));
        return temp;
    }
    Node<T,D>* insert_util(Node<T,D>* p,Node<T,D> *Data)
    {
               
        if(p==NULL)
        {
        size_val++;
        return Data;
        }
        if(comp(Data->key,p->key))
        {
           p->left=insert_util(p->left,Data);
        }
        else if(comp(p->key,Data->key))
        {
           p->right=insert_util(p->right,Data);
        }
        else
        {
          if(eql(p->key,Data->key))
           p->value = Data->value;
        }   

        p->height=1 + max(height_util(p->left),height_util(p->right)); 

        int firstbal= get_balance_util(p);
        int leftbal=get_balance_util(p->left);
        int rightbal=get_balance_util(p->right);

        if(firstbal > 1 && leftbal >-1)
        {
            return right_rotation_util(p);
        }
        if(firstbal < -1 && rightbal <1)
        {
            return left_rotation_util(p);
        }
        if(firstbal > 1 && leftbal < 0)
        {
            p->left = left_rotation_util(p->left);
            return right_rotation_util(p);
        }
        if(firstbal < -1 && rightbal > 0)
        {
            p->right = right_rotation_util(p->right);
            return left_rotation_util(p);
        }
        return p;
      
    }
    bool search_util(Node<T,D>*p,T k)
    {
        if(p==NULL)
        return false;
        if(comp(k,p->key))
        {
            return search_util(p->left,k);
        }
        else if(comp(p->key,k))
        {
            return search_util(p->right,k);
        }
        else
        {
            return eql(p->key,k);
        }
        
        
    }

    D& get_util(Node<T,D>* &p,T k)
    {    
        if(p==NULL)
        {
            D dummydata;
            Node<T,D> *temp=new Node<T,D>(k,dummydata);
            root=insert_util(root,temp);
            return (temp->value);
        }
       

        if(comp(k,p->key))
        {
            return get_util(p->left,k);
        }
        else if(comp(p->key,k))
        {
            return get_util(p->right,k);
        }
        else
        {
            return (p->value);
        }
       
    }
    Node<T,D>* get_Max(Node<T,D>* p,Node<T,D>* parent)
    {
        if(p->right==NULL)
        return parent;

        return get_Max(p->right,p);

    }
    Node<T,D>* delete_util(Node<T,D>* &p,T k)
    {
        if(p==NULL)
        return p;

        if(comp(k,p->key))
        {
            p->left=delete_util(p->left,k);
            
        }
        else if(comp(p->key,k))
        {
            p->right=delete_util(p->right,k);
            
        }
        else
        {
            size_val--;
            
            if(p->left==NULL && p->right==NULL)
            {
                
                delete p;
                p=NULL;
            }
            else if(p->left==NULL)
            {
             auto temp = p;
             p=p->right;
             delete temp;                           
            }
            else if(p->right ==NULL)
            {
            auto temp = p;
            p=p->left;
            delete temp;             
            }
            else
            {
                auto pred_parent=get_Max(p->left,p);               
                auto predecessor = pred_parent->left;

                if(pred_parent==p)
                {
                  predecessor->right = pred_parent->right;

                }
                else
                {
                predecessor=pred_parent->right;
                pred_parent->right=predecessor->left;                     
                predecessor->right=p->right;
                predecessor->left=p->left;
                }
                pred_parent=NULL;
               swap(p,predecessor);                     
                                           
              delete predecessor;
              delete pred_parent;               
                               
            }
            
        

        }
       

        if(p==NULL)
        return p;

      p->height=1 + max(height_util(p->left),height_util(p->right)); 

        int firstbal= get_balance_util(p);
        int leftbal=get_balance_util(p->left);
        int rightbal=get_balance_util(p->right);

        if(firstbal > 1 && leftbal >-1)
        {
            return right_rotation_util(p);
        }
        if(firstbal < -1 && rightbal <1)
        {
            return left_rotation_util(p);
        }
        if(firstbal > 1 && leftbal < 0)
        {
            p->left = left_rotation_util(p->left);
            return right_rotation_util(p);
        }
        if(firstbal < -1 && rightbal > 0)
        {
            p->right = right_rotation_util(p->right);
            return left_rotation_util(p);
        }
        return p;
      

    }

    void inorder_util(Node<T,D>*p)
    {
         if(p==NULL)
         return;
         inorder_util(p->left);
         cout<<p->key<<" --- "<<p->value<<endl;
         inorder_util(p->right);
    }
    void clear_util(Node<T,D>*p)
    {
         if(p==NULL)
         return;
         clear_util(p->left);
         clear_util(p->right);
         delete p;
    }

    public:
    Map()
    {
        root=NULL;
        size_val=0;
    }    
    void insert(T k,D d)
    {
        Node<T,D> *temp= new Node<T,D>(k,d);
        root=insert_util(root,temp);
        
    }
    void inorder()
    {
        inorder_util(root);
    }
    int size()
    {
        return size_val;
    }
    bool find(T k)
    {
        return search_util(root,k);
    }
    void erase(T k)
    {
        root=delete_util(root,k);
        
    }
    void clear()
    {
        clear_util(root);
        root=NULL;
        size_val=0;
    }

    D& operator [](T k)
    {
    
      return get_util(root,k);     

    }
  

};




int main()
{
ios_base::sync_with_stdio(false);
cin.tie(NULL);
struct comp{
bool operator ()(const int &a,const int &b)
{   
   
    return to_string(a) < to_string(b);
}
};
// Map<int,int> test;
Map<int, int> mp;
	mp.insert(1, 2);
	mp.insert(2, 2);
	mp.insert(3, 2);
	mp.erase(2);
	mp.inorder();
return 0;
}