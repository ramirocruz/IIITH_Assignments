#include<bits/stdc++.h>
using namespace std;
#define MAX (2000011)
int freq[MAX];

void BuildStree(vector<int>&ar,vector<int>&stree,int start,int end,int node)
{
    if(start == end)
    {
        stree[node]=ar[start];
        return ;
    }
    int mid = (start+end)/2,left = 2*node+1;
    int right = left+1;


    BuildStree(ar,stree,start,mid,left);
    BuildStree(ar,stree,mid+1,end,right);


    stree[node] = min(stree[left],stree[right]);
}

void UpdateStree(vector<int>&ar,vector<int>&stree,int start,int end,int node,int key,int val)
{

    if(start == end)
    {
        ar[key]=val;
        stree[node]=val;
        return;
    }
    int mid=(start+end)/2;
    int left = 2*node+1;
    int right=left+1;
    if(key > mid)
    {
        UpdateStree(ar,stree,mid+1,end,right,key,val);
    }
    else
    {
        UpdateStree(ar,stree,start,mid,left,key,val);
    }
    
    stree[node] = min(stree[left],stree[right]);
}
int Query(vector<int>&stree,int start,int end,int node,int l,int r)
{
    if(l>end || r < start)
    {
        return INT_MAX;
    }
    if(l<= start && r>=end)
    {
        return stree[node];
    }

    int mid=(start+end)/2;
    int left = 2*node+1;
    int right = left+1;
    int ans1 = Query(stree,start,mid,left,l,r);
    int ans2 = Query(stree,mid+1,end,right,l,r);

    return min(ans1,ans2);
}


void BFS(vector<int>&stree)
{
   queue<int>q;
   q.push(0);
   q.push(-1);
   while(!q.empty())
   {
       while(q.front()!=-1)
       {
       int node = q.front();
       q.pop();
       int left=2*node+1;
       int right =left+1;
       if(left<stree.size())
       q.push(left);
       if(right<stree.size())
       q.push(right);

       cout<<stree[node]<<" ";
       }
       q.push(-1);
       q.pop();
       if(q.front()==-1)
       q.pop();
       
       cout<<endl;

   }

}

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
    int maxn=257;
    if(n > maxn)
        maxn=n;
    for(int i=0;i<maxn;i++)
        freq[i]=0;
    for(int i=0;i<n;i++)
        freq[ar[i].sort_pos.second+1]++;
    

    for(int i=1;i<maxn;i++)
    {
        freq[i]+=freq[i-1];
    }

    for(int i=n-1;i>-1;i--)
    {
        tempSA[freq[ar[i].sort_pos.second+1]-1]=ar[i];
        freq[ar[i].sort_pos.second+1]--;
    }
    for(int i=0;i<n;i++)
    {
        ar[i]=tempSA[i];
    }

    for(int i=0;i<maxn;i++)
        freq[i]=0;
    for(int i=0;i<n;i++)
        freq[ar[i].sort_pos.first+1]++;
    for(int i=1;i<maxn;i++)
    {
        freq[i]+=freq[i-1];
    }
    for(int i=n-1;i>-1;i--)
    {
        tempSA[freq[ar[i].sort_pos.first+1]-1]=ar[i];
        freq[ar[i].sort_pos.first+1]--;

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
        temp[i].sort_pos.second=-1;
        else
        temp[i].sort_pos.second=str[i+1];        
    }       
    
    
    // counting_sort(temp);
    sort(temp.begin(),temp.end(),comp);
    
    vector<int>SAinv(n);
 
    
    for(int l=2;l<n;l*=2)
    {
            int sort_order =0;
            int prev_pos = temp[0].sort_pos.first;
            temp[0].sort_pos.first=sort_order;
            SAinv[temp[0].index]=sort_order;

        for(int i=1;i<n;i++)
           {
             if(!(prev_pos == temp[i].sort_pos.first && temp[i-1].sort_pos.second == temp[i].sort_pos.second))
                sort_order++;

                prev_pos = temp[i].sort_pos.first;
                temp[i].sort_pos.first = sort_order;
                SAinv[temp[i].index]=i;
           }
        for(int i=0;i<n;i++)
        {
            
            if(temp[i].index + l >= n)
            temp[i].sort_pos.second=-1;
            else
            temp[i].sort_pos.second= temp[SAinv[temp[i].index + l]].sort_pos.first;            
        }
        
      
        // counting_sort(temp);
        sort(temp.begin(),temp.end(),comp);
    }
    
    
    for(int i=0;i<n;i++)
    {
        SA[i]=temp[i].index;
    }



}

void calculate_LCP(vector<int>&SA,vector<int>&SAinv,string &str,vector<int>&LCP)
{
    int n= SA.size();
    int k = 0; 
    for(int i=0;i<n;i++)
    {
        SAinv[SA[i]] = i;
    }
    LCP[0]=0;
    for (int i=0; i<n; i++) 
    { 
       
        if (SAinv[i] == n-1) 
        { 
            k = 0; 
            continue; 
        } 

        int j = SA[SAinv[i]+1]; 

        while (i+k<n && j+k<n && str[i+k]==str[j+k]) 
            k++; 
  
        LCP[SAinv[i]+1] = k; 
  
    
        if (k>0) 
            k--; 
    } 


}
int main()
{

    // freopen("input08.txt","r",stdin);
    // freopen("op2.txt","w",stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string str;
    cin>>str;
    
    string s = str+'$'+string(str.rbegin(),str.rend());
    // string s=str;
    int n =s.size();
    // cout<<n<<endl;
    vector<int>LCP(n),SA(n),SAinv(n);
    create_suffix_array(s,SA);
    calculate_LCP(SA,SAinv,s,LCP);
    vector<int>stree(4*n);
    BuildStree(LCP,stree,0,n-1,0);
    

    // for(int i=0;i<n;i++)
    // {
    //     cout<<i<<" -- "<<LCP[i]<<" -- "<<SA[i]<<" -- "<<s.substr(SA[i])<<endl;
    // }
    vector<bool>visited(n,false);
    int pos=0;
    int maxlen=0;
    visited[SAinv[n/2]]=true;
    visited[SAinv[n - n/2]]=true;  
    
    // cout<<"Even:\n";
    for(int i = 0;i<n;i++)
    {
        if(visited[i])
            continue;
        if(n-SA[i] > n-1)
            continue;
        int end = SAinv[n-SA[i]];
        visited[i]=true;
        visited[end] = true;
        
        if(s[SA[i]]!=s[n-SA[i]])
            continue;
        // cout<<i<<" -- "<<SA[i]<<" -- "<<s[SA[i]]<<" -- "<<n-SA[i]<<" -- "<<s[n-SA[i]]<<endl;
        int l = i;
        int r = end;
        if(l > r)
        swap(l,r);
       int curlen = Query(stree,0,n-1,0,l+1,r);
       if(curlen > maxlen)
       {
           maxlen = curlen;
           pos = min(SA[l],SA[r]) - maxlen;
       }
        
    }
    int evenlen = maxlen*2;
    int evenpos = pos;
    maxlen=0;

    // cout<<"Odd\n:";
    std::fill(visited.begin(),visited.end(),false);
    visited[SAinv[n/2]]=true;
    visited[SAinv[n - n/2 + 1]]=true; 
    visited[SAinv[n/2+1]]=true;   

    for(int i = 0;i<n;i++)
    {
        if(visited[i])
            continue;
        if(n-SA[i] + 1 > n-1)
            continue;
        int end = SAinv[n-SA[i]+1];
        visited[i]=true;
        visited[end] = true;
        
        if(s[SA[i]]!=s[n-SA[i]+1])
            continue;
        // cout<<i<<" -- "<<SA[i]<<" -- "<<s[SA[i]]<<" -- "<<n-SA[i]+1<<" -- "<<s[n-SA[i]+1]<<endl;

        int l = i;
        int r = end;
        if(l > r)
        swap(l,r);
       int curlen = Query(stree,0,n-1,0,l+1,r);
       if(curlen > maxlen)
       {
           maxlen = curlen;
           pos = min(SA[l],SA[r]) - maxlen - 1;
       }
        
    }
    
    maxlen = maxlen*2 + 1;

    if(evenlen > maxlen)
    {
        maxlen = evenlen;
        pos = evenpos;
    }

//    cout<<pos<<"--"<<maxlen<<endl;
    cout<<s.substr(pos,maxlen)<<endl;
    
    return 0;
}