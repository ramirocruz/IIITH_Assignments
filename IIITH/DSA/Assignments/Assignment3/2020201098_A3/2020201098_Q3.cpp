#include<bits/stdc++.h>

using namespace std;


class Heap
{
 int s;
 pair<long long,int>*root;
 void heapify(int node)
 {
     int m = node;
     int left = 2*node+1;
     int right = left+1;
     if(left < s && root[left] < root[m])
     m=left;
     if(right < s && root[right]<root[m])
     m=right;

     if(m!=node)
     {
         swap(root[node],root[m]);
         heapify(m);
     }

 }
 void revheapify(int node)
 {
      int parent=(node-1)/2;
      if(parent > -1 && root[node] < root[parent])
      {
          swap(root[node],root[parent]);
          revheapify(parent);
      }
 }

 public:
 void display()
 {
     for(int i=0;i<s;i++)
     cout<<root[i].first<<" - "<<root[i].second<<" ";
     cout<<endl;
 }
 
 Heap(int size)
 {
     s=0;
     root = new pair<long long,int>[size];
 }

 bool empty()
 {
     if(s)
     return false;
     return true;
 }
 void push(pair<long long,int>data)
 {
     root[s++]=data;
     revheapify(s-1);
 }
 pair<long long,int> top()
 {
     return root[0];
 }
 void pop()
 {
     swap(root[0],root[--s]);
     heapify(0);     
 }
 
~Heap()
 {
     delete [] root;
 }

};

void maxheapify(int node,int n,vector<long long>&ar)
{
    int m = node;
    int left = 2*node + 1;
    int right = left+1;

    if(left < n && ar[left] > ar[m])
     m =left;
    if(right < n && ar[right] > ar[m])
     m=right;
    if(node != m)
    {
        swap(ar[node],ar[m]);
        maxheapify(m,n,ar);
    }
}

void heap_sort(vector<long long>&ar,int n)
{

    for(int i=n/2-1;i>-1;--i)
    {
        maxheapify(i,n,ar);
    }
    for(int i=n-1;i>-1;--i)
    {
        swap(ar[i],ar[0]);
        maxheapify(0,i,ar);
    }
}

int main(int argc, char *argv[])
{


auto start = chrono::high_resolution_clock::now(); 

string inpfile;
string outputfilename;
if(argc != 3)
{
    inpfile="unsorted_file.txt";
    outputfilename="output.txt";
}
else
{
    inpfile=argv[1];
    outputfilename = argv[2];
}

FILE *in = fopen(inpfile.c_str(),"r");
if(in==NULL)
{
    cout<<"Failed..";
    exit(1);
}
int chunk_size=1000000;
int chunks=0;
bool finished = false;
int lastpos;
string outfilename="opfile_";

vector<long long> tempinp(chunk_size);
while(!finished)
{
   
for (int i=0;i<chunk_size;i++)
{
      
    if(fscanf(in,"%lld,",&tempinp[i])==-1)
     {
         finished =true; 
        lastpos=i-1;
         break;
     } 
       lastpos=i;   

}
chunks++;
heap_sort(tempinp,lastpos+1);

FILE * tmp = fopen((outfilename+to_string(chunks)).c_str(),"w");
for(int i=0;i<=lastpos;i++)
fprintf(tmp,"%lld\n",tempinp[i]);
fclose(tmp);
}

fclose(in);

Heap min_heap(chunks);


vector<FILE*> fptrs(chunks);

finished=false;
for(int i=0;i<chunks;i++)
{

    fptrs[i] = fopen((outfilename+to_string(i+1)).c_str(),"r");
    if(fptrs[i]==NULL)
    {
        cout<<"Error in "<<i<<" -- "<<(outfilename+to_string(i+1)).c_str()<<endl;
        exit(1);
    }

}
finished =false;
long long data;
for(int i=0;i<chunks;i++)
{
    fscanf(fptrs[i],"%lld",&data);
    min_heap.push({data,i});
}


FILE *op = fopen(outputfilename.c_str(),"w");

pair<long long,int>temp;
while(!min_heap.empty())
{
    temp = min_heap.top();
    min_heap.pop();
    fprintf(op,"%lld\n",temp.first);
    if(fscanf(fptrs[temp.second],"%lld",&data)==-1)
        continue;
    min_heap.push({data,temp.second});
}


fclose(op);
for(int i=0;i<chunks;i++)
{
    fclose(fptrs[i]);
}
for(int i=0;i<chunks;i++)
{
    remove((outfilename+to_string(i+1)).c_str());
}

  
    auto stop = chrono::high_resolution_clock::now(); 

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);   
    cout << "Time taken by function: "
         << duration.count()/1000000 << " seconds" << endl; 

return 0;
}