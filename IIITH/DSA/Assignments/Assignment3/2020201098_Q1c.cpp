#include<bits/stdc++.h>
using namespace std;
#define MAX (1000000)
int freq[MAX];

struct Suffix
{
  int index;
  pair<int,int> sort_pos;
};

bool comp(Suffix &a,Suffix &b)
{
    if(a.sort_pos.first == b.sort_pos.first)
    return a.sort_pos.second < b.sort_pos.second;
    
    return a.sort_pos.first < b.sort_pos.first;
}

vector<Suffix>tempSA(MAX);
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
    // counting_sort(temp);
    
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
        // counting_sort(temp);
    }
    
    
    for(int i=0;i<n;i++)
    {
        new_pos[i]=temp[i].index;
    }


}

void calculate_LCP(vector<int>&SA,string &str,vector<int>&LCP)
{
    LCP[0]=0;
    int n=SA.size();
    for (int i=1;i<n;i++)
    {
        int len=0;
        for(int j=SA[i],k=SA[i-1];j<n && k<n && (str[j]==str[k]);j++,k++)
        {
            len++;
        }
        LCP[i]=len;
    }
}
int main()
{
    string str;
    cin>>str;

    string s = str+'$'+string(str.rbegin(),str.rend());
    int n =s.size();
    vector<int>LCP(n),SA(n);
    create_suffix_array(s,SA);
    calculate_LCP(SA,s,LCP);
    

    for(int i=0;i<n;i++)
    {
        cout<<i<<" -- "<<LCP[i]<<" -- "<<s.substr(SA[i])<<endl;
    }
    
    int pos=0;
    int maxlen=0;
    for(int i =1;i<n;i++)
    {
        if(LCP[i] > maxlen)
        {
            if(SA[i-1] < n/2 && SA[i] > n/2 || SA[i-1] > n/2 && SA[i] <n/2)
            {
                if(n - (SA[i-1]+SA[i]) == LCP[i])
                {
                    maxlen=LCP[i];
                    pos=SA[i];
                }
            }
        }
    }

    cout<<s.substr(pos,maxlen)<<endl;
    
    return 0;
}