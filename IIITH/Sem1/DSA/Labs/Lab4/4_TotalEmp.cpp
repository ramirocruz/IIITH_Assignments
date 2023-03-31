#include <bits/stdc++.h>
using namespace std;
bool comp(const pair<int,int>&a,const pair<int,int>&b)
{
	return a.first < b.first;
}
int ecount[200001]={0};
int main() {
	// your code goes here
	vector<pair<int,int>>pp;
	int data;
	int n;
	cin>>n;
	for(int i=1;i<n;i++)
	{
		cin>>data;
		pp.push_back({data-1,i});
	}
	
	sort(pp.begin(),pp.end());
	int left,right;
	left=n-2;
	auto val=pp[left];
	while(left)
	{
		right = upper_bound(pp.begin(),pp.end(),val,comp) - pp.begin();
		left = lower_bound(pp.begin(),pp.end(),val,comp) - pp.begin();
		// cout<<left<<" "<<right<<endl;
		ecount[pp[left].first]+=right-left;
		for(int i=left;i<=right;i++)
		{
			ecount[pp[left].first]+= ecount[i+1];
		}
		if(left==0)
		break;
		val=pp[left-1];
	}
	
	for(int i=0;i<n;i++)
	cout<<ecount[i]<<' ';
	cout<<endl;
	return 0;
}