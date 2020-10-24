#include<bits/stdc++.h>
using namespace std;
template<class T>
struct Node
{
    T key;
    int height;
    int count;
    int netcount;
    Node *left,*right;
    Node(T k)
    {
        key=k;
        height=0;
        count=1;
        netcount=1;
        left=NULL;
        right=NULL;
    }

};
 template<class T>
  T&  close( T &lower ,  T &data,  T &higher)
    {
        if((higher - data) < (data - lower))
        return higher;
        return lower;

    }

template<class T>
struct Less
{
    bool operator () (const T &a , const T &b)
    {
        return a<b;
    }

};
template<class T>
struct Equal
{
    bool operator () (const T &a , const T &b)
    {
        return a==b;
    }

};
template<class T,class Comparator = Less<T>,class Comparator2 = Equal<T>>
class AVLTree
{
     Node<T> *root;
     Comparator comp;
     Comparator2 eql;
     int size_val;
     

    int height_util(Node<T>*p)
    {
        if(p==NULL)
        return -1;
        return (1 + max(height_util(p->left),height_util(p->right)));
    }
    int get_balance_util(Node<T>*p)
    {
        if(p==NULL)
        return 0;
        return (height_util(p->left)-height_util(p->right));
    }
    int get_netcount(Node<T>*p)
    {
        if(p==NULL)
        return 0;
        return p->netcount;
    }

    int get_count(Node<T>*p)
    {
        if(p==NULL)
        return 0;
        return get_count(p->left) + get_count(p->right) + p->count;
    }
    
    Node<T>* get_common_ancestor_util(Node<T>*p,T low,T high)
    {
        if(p==NULL)
        return p;
        if(comp(low,p->key) && comp(high,p->key))
        {
            return get_common_ancestor_util(p->left,low,high);
        }
        else if(comp(p->key,low) && comp(p->key,high))
        {
            return get_common_ancestor_util(p->right,low,high);
        }
        else
        {
            return p;
        }
    }

    Node<T>* right_rotation_util(Node<T>* p)
    {
       auto temp = p->left;       
       p->left = temp->right;
       temp->right=p;
       p->height = 1 + max(height_util(p->left),height_util(p->right));
       temp->height= 1 + max(height_util(temp->left),height_util(temp->right));
       return temp;
    }
      Node<T>* left_rotation_util(Node<T>* p)
    {
        auto temp=p->right;
        p->right=temp->left;
        temp->left=p;
        p->height = 1 + max(height_util(p->left),height_util(p->right));
        temp->height= 1 + max(height_util(temp->left),height_util(temp->right));
        return temp;
    }

    Node<T>* insert_util(Node<T>* p,Node<T> *Data)
    {
               
        if(p==NULL)
        {
        size_val++;        
        return Data;
        }

        if(eql(p->key,Data->key))
        {
            (p->count)++; 
            (p->netcount)++;
            size_val++;           
            return p;
        }

        if(comp(Data->key,p->key))
        {
           p->left=insert_util(p->left,Data);
        }
        else if(comp(p->key,Data->key))
        {
           p->right=insert_util(p->right,Data);
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
        p->netcount = get_count(p);
        return p;
      
    }

    bool search_util(Node<T>*p,T k)
    {
        if(p==NULL)
        return false;
        if(eql(k,p->key))
        {
            return true;
        }
        if(comp(k,p->key))
        {
            return search_util(p->left,k);
        }
        else 
        {
            return search_util(p->right,k);
        }      
        
        
    }

    Node<T>* get_util(Node<T>* &p,T k)
    {    
        if(p==NULL)
        {
          return p;
        }
        
        if(eql(k,p->key))
        {
            return p;
        }

        if(comp(k,p->key))
        {
            return get_util(p->left,k);
        }
        else 
        {
            return get_util(p->right,k);
        }
   
       
    }

    Node<T>* lower_bound_util(Node<T>* &p,T k)
    {    
        if(p==NULL)
        {
          return p;
        }
        
        if(eql(k,p->key))
        {
            return p;
        }

        if(comp(p->key,k))
        {

            return lower_bound_util(p->right,k);
        }
        else 
        {
           
            auto temp =lower_bound_util(p->left,k);
            if(temp==NULL)
            return p;
            return temp;
        }
   
       
    }

  Node<T>* upper_bound_util(Node<T>* &p,T k)
    {    
        if(p==NULL)
        {
          return p;
        }
        
        if(eql(k,p->key) || comp(p->key,k))
        {        

            return upper_bound_util(p->right,k);
        }
        else 
        {
            
            auto temp = upper_bound_util(p->left,k);
            if(temp==NULL)
            return p;
            return temp;
        }
   
       
    }

  Node<T>* inverse_lower_bound_util(Node<T>* &p,T k)
    {    
        if(p==NULL)
        {
          return p;
        }

        if(eql(k,p->key))
        {
            return p;
        }

        if(comp(p->key,k))
        {        

            auto temp = inverse_lower_bound_util(p->right,k);
            if(temp==NULL)
            return p;
            return temp;
        }
        else 
        {
            
           return inverse_lower_bound_util(p->left,k);
        }
   
       
    }


    Node<T>* get_Max(Node<T>* &p,Node<T>* &parent)
    {
        if(p->right==NULL)
        return parent;

        return get_Max(p->right,p);

    }

    Node<T>* delete_util(Node<T>* &p,T k,bool &found)
    {
        if(p==NULL)
        {
            
            return p;
        }

        if(comp(k,p->key))
        {
            p->left=delete_util(p->left,k,found);
            
        }
        else if(comp(p->key,k))
        {
            p->right=delete_util(p->right,k,found);
            
        }
        else
        {
            size_val--;
            found = true;
            if(p->count>1)
            {
                (p->count)--;
                (p->netcount)--;
                return p;
            }

            
            if(p->left==NULL && p->right==NULL)
            {
                
                delete p;
                p=NULL;
            }
            else if(p->left==NULL)
            {
             auto &temp = p;
             p=p->right;
             delete temp;                           
            }
            else if(p->right ==NULL)
            {
            auto &temp = p;
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
        p->netcount = get_count(p);
        return p;
      

    }

    int count_range_util(Node<T> *p,T low ,T high)
    {
        if(p==NULL)
        return 0;
        if(comp(p->key,low))
        return count_range_util(p->right,low,high);
        if(comp(high,p->key))
        return count_range_util(p->left,low,high);
        return p->count+count_range_util(p->left,low,high) + count_range_util(p->right,low,high);
    }
   

    void inorder_util(Node<T>*p)
    {
         if(p==NULL)
         return;
         inorder_util(p->left);
         cout<<p->key<<" --- "<<p->count<<" --- "<<p->netcount<<endl;
         inorder_util(p->right);
    }

    Node<T>* rev_inorder_util(Node<T>*p,int k,int &count)
    {    
         
         if(k < 1 || k > size_val || p==NULL)
         return NULL;
       
         auto temp1 =rev_inorder_util(p->right,k,count);
         if(temp1 != NULL)
         return temp1;
        
         count+=p->count;
         
         if(count >= k)
         return p;

         return rev_inorder_util(p->left,k,count);
    }
    
    Node<T> * largest_util(Node<T>*p,int k)
    {
        if(p==NULL)
        return p;
        int rcount = get_netcount(p->right);
        if(rcount < k && k <= rcount + p->count)
        return p;
        if(k > rcount + p->count)
        {
            k-= rcount+p->count;
            return largest_util(p->left,k);
        }
        else
        {
            
            return largest_util(p->right,k);
        }
    }

    public:
    AVLTree()
    {
        root=NULL;
        size_val=0;
    }

    void insert(T k)
    {
        Node<T> *temp= new Node<T>(k);
        root=insert_util(root,temp);
        
    }
    void inorder()
    {
        inorder_util(root);
    }
    bool find(T k)
    {
        return search_util(root,k);
    }
    int erase(T k)
    {
        if(root==NULL)
        return -1;
        bool found=false;
        root=delete_util(root,k,found);
        if(found==false)
        return -1;
        
       return 0;
    } 
    int  count(T k)
    {
        auto temp = get_util(root,k);
        if(temp == NULL)
        return -1;
        return temp->count;
    }
    int count_range(T low,T high)
    {
        // return count_range_util(root,low,high);
        auto lower = lower_bound_util(root,low);
        auto higher = inverse_lower_bound_util(root,high);
        if(lower != NULL && higher !=NULL)
        {
          auto total = get_common_ancestor_util(root,low,high);
          return total->netcount - (get_netcount(lower->left) + get_netcount(higher->right)); 
        }
        return 0;
    }
    Node<T>* lower_bound(T k)
    {
        return lower_bound_util(root,k);
    }
    Node<T>* upper_bound(T k)
    {
        return upper_bound_util(root,k);
    }

    T closest(T k,T&(& func)(T&,T&,T&)=close)
    {
        auto lower=inverse_lower_bound_util(root,k);
        auto upper=lower_bound_util(root,k);
        if(lower==NULL && upper==NULL)
        {
            cout<<"Tree Empty...\n";
            exit(1);
        }
        if(lower==NULL)
        return upper->key;
        else if(upper==NULL)
        return lower->key;
        return func(lower->key,k,upper->key);
       
    }
    Node<T>* largest(int k)
    {
        // int count=0;
        // return rev_inorder_util(root,k,count);  
        return largest_util(root,k);      
    }
    int size()
    {
        return size_val;
    }

  
  

};


string& test(string& lower,string& data,string& upper)
{
    return lower;
}
int main()
{
ios_base::sync_with_stdio(false);

AVLTree<int> tree;
Node<int>*temp=NULL;
int data,low,high;
int k,mode,Q;
cin>>Q;
while(Q--)
{
    cin>>mode;
    switch (mode)
    {
    
    case 1: cin>>data;
            tree.insert(data);
            break;

    case 2: cin>>data;
            if(tree.erase(data)==-1)
            cout<<"Data not present...\n";
            
            break;

    case 3: cin>>data;
            if(tree.find(data))
            cout<<"Present\n";
            else
            cout<<"Not Present\n";
            break;

    case 4: cin>>data;
            cout<<tree.count(data)<<endl;
            break;

    case 5: cin>>data;
            temp=tree.lower_bound(data);
            if(temp==NULL)
            cout<<"Element Not Present..\n";
            else
            cout<<temp->key<<endl;
            break;

    case 6: cin>>data;
            temp=tree.upper_bound(data);
            if(temp==NULL)
            cout<<"Element Not Present..\n";
            else
            cout<<temp->key<<endl;
            break;

    case 7: cin>>data;
            cout<<tree.closest(data)<<endl;
            break;

    case 8: cin>>k;
            temp=tree.largest(k);
            if(temp==NULL)
            cout<<"Invalid Argument..\n";
            else
            cout<<temp->key<<endl;
            break;

    case 9: cin>>low>>high;
            cout<<tree.count_range(low,high)<<endl;
            break;
    
    case 10: cout<<"................\n"; 
             tree.inorder();
             cout<<".................\n";
             break;

    case 11: cout<<tree.size()<<endl;
             break;
    
    default: cout<<"Enter valid mode..\n";
             break;
    }
}




return 0;
}