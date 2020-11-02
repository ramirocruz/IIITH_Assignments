#include<bits/stdc++.h>
using namespace std;
#define MAX (1000000)
int freq[MAX];
struct Suffix
{
  int index;
  pair<int,int> sort_pos;
};

vector<Suffix>tempSA(MAX);
bool comp(Suffix &a,Suffix &b)
{
    if(a.sort_pos.first == b.sort_pos.first)
    return a.sort_pos.second < b.sort_pos.second;
    
    return a.sort_pos.first < b.sort_pos.first;
}
void display(vector<Suffix>&ar)
{
    cout<<"--------------####------------------\n";
    for(auto &x:ar)
    {
        cout<<x.sort_pos.first<<" -- "<<x.sort_pos.second<<endl;
    }
    cout<<"--------------$$$$------------------\n";
}
void counting_sort(vector<Suffix>&ar)
{

    int n=ar.size();
    int maxn=256;
    if(n > maxn)
        maxn=n;
    for(int i=0;i<maxn;i++)
        freq[i]=0;
    for(int i=0;i<n;i++)
        freq[ar[i].sort_pos.second]++;
    

    for(int i=1;i<maxn;i++)
    {
        freq[i]+=freq[i-1];
    }

    for(int i=n-1;i>-1;i--)
    {
        tempSA[freq[ar[i].sort_pos.second]-1]=ar[i];
        freq[ar[i].sort_pos.second]--;
    }
    for(int i=0;i<n;i++)
    {
        ar[i]=tempSA[i];
    }

    for(int i=0;i<maxn;i++)
        freq[i]=0;
    for(int i=0;i<n;i++)
        freq[ar[i].sort_pos.first]++;
    for(int i=1;i<maxn;i++)
    {
        freq[i]+=freq[i-1];
    }
    for(int i=n-1;i>-1;i--)
    {
        tempSA[freq[ar[i].sort_pos.first]-1]=ar[i];
        freq[ar[i].sort_pos.first]--;

    }
    for(int i=0;i<n;i++)
    {
        ar[i]=tempSA[i];
    }

}

void create_suffix_array(string &str,vector<int>&SA)
{
    int n=str.size();
    vector<Suffix>temp(n);
    for(int i=0;i<n;i++)
    {
        temp[i].index=i;
        temp[i].sort_pos.first=str[i];
        if(i+1 >=n)
        temp[i].sort_pos.second=0;
        else
        temp[i].sort_pos.second=str[i+1];        
    }       
    
    sort(temp.begin(),temp.end(),comp);
    counting_sort(temp);
    
    vector<int> &new_pos=SA;      
 
    
    for(int l=2;l<n;l*=2)
    {
            int sort_order =0;
            int prev_pos = temp[0].sort_pos.first;
            temp[0].sort_pos.first=sort_order;
            new_pos[temp[0].index]=sort_order;

        for(int i=1;i<n;i++)
           {
             if(!(prev_pos == temp[i].sort_pos.first && temp[i-1].sort_pos.second == temp[i].sort_pos.second))
                sort_order++;

                prev_pos = temp[i].sort_pos.first;
                temp[i].sort_pos.first = sort_order;
                new_pos[temp[i].index]=i;
           }
        for(int i=0;i<n;i++)
        {
            
            if(temp[i].index + l >= n)
            temp[i].sort_pos.second=0;
            else
            temp[i].sort_pos.second= temp[new_pos[temp[i].index + l]].sort_pos.first;            
        }
        
        sort(temp.begin(),temp.end(),comp);
        counting_sort(temp);
    }
    
    
    for(int i=0;i<n;i++)
    {
        new_pos[i]=temp[i].index;
    }


}

int main()
{
string s="Dca1ca";
string _s=s;
vector<int>SA(_s.size());
create_suffix_array(_s,SA);

int res;
for(int i=0;i<_s.size();i++)
{
    if(SA[i]<s.size())
    {
        res=SA[i];
        break;
    }
}
cout<<_s<<endl;
for(int i=0;i<_s.size();i++)
{
    cout<<i<<" -- "<<SA[i]<<" -- "<<_s.substr(SA[i])<<endl;
}
for(int i=0;i<s.size();i++)
cout<<s[(i+res)%s.size()];
cout<<endl;
cout<<res<<endl;
return 0;
}