#include<bits/stdc++.h>
using namespace std;
#define MAX_BIT 40
struct Node
{
Node *left,*right;
Node()
{
    left=NULL;
    right=NULL;
}
};

/*
if bit is 0 then left child else right child
*/
void insert(Node* root,long long val)
{
  Node* cur=root;
//   cout<<val<<endl;
  for(int i=MAX_BIT;i>-1;i--)
  {
      bool bit = val&(1LL<<i);
    //   cout<<bit;
      if(bit)
      {
        if(cur->right==NULL)
        cur->right = new Node();
        cur = cur->right;
      }
      else
      {
          if(cur->left ==NULL)
          cur->left = new Node();
          cur = cur->left;
      }
      
  }
//   cout<<endl;
}
Node* init(vector<long long>&ar)
{
     Node *root = new Node();
    for(auto &x:ar)
    {
        insert(root,x);
    }

    return root;
}
long long max_xor(Node* root,long long val)
{
    long long result =0;
    
    for(int i=MAX_BIT;i>-1;i--)
    {
        bool bit = val&(1LL<<i);
        // cout<<bit<<" : ";
        if(bit)
        {
            if(root->left)
            {
                // cout<<result<<" -- ";
                root=root->left;
                result = 2*result + 1;
                // cout<<result<<endl;
            }
            else
            {
                // cout<<result<<" -+- ";
                root=root->right;
                result = 2*result;
                // cout<<result<<endl;
            }
            
        }
        else
        {

            if(root->right)
            {   
                // cout<<result<<" -- ";
                root=root->right;
                result = 2*result + 1;
                // cout<<result<<endl;
            }
            else
            {
                // cout<<result<<" -+- ";
                root=root->left;
                result = 2*result;
                // cout<<result<<endl;
            }
        }
        
    }
 return result;
}

int main()
{
int n,q;
long long val;
cin>>n>>q;
vector<long long>ar(n);
for(int i=0;i<n;i++)
{
    cin>>ar[i];
}
Node *root = init(ar);
while(q--)
{
    cin>>val;
    cout<<max_xor(root,val)<<endl;

}

return 0;
}