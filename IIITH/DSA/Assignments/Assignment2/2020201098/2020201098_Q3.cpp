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

template<class T,class D,class Comparator = Less<T>>
class Map
{   
    public:
    struct Node<T,D> *root;
    Comparator comp;
    int size_val;
    int height_util(Node<T,D>*p)
    {
        if(p==NULL)
        return -1;
        return (1 + max(height_util(p->left),height_util(p->right)));
    }
    void insert_util(Node<T,D>* &p,T k, D d)
    {
               
        if(p==NULL)
        {
        Node<T,D> *temp= new Node<T,D>(k,d);
        p=temp;
        size_val++;
        return;
        }
        if(comp(k,p->key))
        {
           insert_util(p->left,k,d);
        }
        else if(comp(p->key,k))
        {
           insert_util(p->right,k,d);
        }   

        p->height=1 + max(height_util(p->left),height_util(p->right));     

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
            return true;
        }
        
        
    }

    void inorder_util(Node<T,D>*p)
    {
         if(p==NULL)
         return;
         inorder_util(p->left);
         cout<<p->key<<" "<<p->value<<endl;
         inorder_util(p->right);
    }

    
    Map()
    {
        root=NULL;
        size_val=0;
    }    
    void insert(T k,D d)
    {
        insert_util(root,k,d);
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
  

};




int main()
{
ios_base::sync_with_stdio(false);
cin.tie(NULL);

Map<int,int> test;

test.insert(2,4);
test.insert(0,6);
test.insert(1,5);
test.inorder();
cout<<test.size()<<endl;
cout<<test.find(3)<<endl<<test.find(1)<<endl;
return 0;
}