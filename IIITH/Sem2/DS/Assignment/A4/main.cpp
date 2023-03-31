#include<bits/stdc++.h>
using namespace std;
map<string,std::vector<string> >hashMap;
void checkFile(ifstream &ifs)
{
	if(!ifs.is_open())
	{
		cout << "Input file doesn't exist";
		exit(0);
	}
}
int main(int argc, char** argv)
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	if(argc < 4)
	{
		cout << "Usage: ./a.out inputR inputS outputFile";
		return 0;
	}
	
	ifstream input; 
	input.open(argv[1]);
	checkFile(input);	

	string X, Y, Z;
	int pos;
	while (getline (input, Y)) 
	{
		while ((pos = Y.find(" ")) != std::string::npos) 
		{
			X =  Y.substr(0, pos);
			Y.erase(0, pos + 1);
		}
		hashMap[Y].push_back(X);
	}
	
	input.close();
	input.open(argv[2]);	
	checkFile(input);	

	ofstream output;
	output.open(argv[3]);
	
	while (getline (input, Z)) 
	{
		while ((pos = Z.find(" ")) != std::string::npos) 
		{
			Y =  Z.substr(0, pos);
			Z.erase(0, pos + 1);
		}
		for (auto x:hashMap[Y])
		{
			output << x <<" "<< Y <<" "<< Z <<"\n";
		}
	}
	return 0;
}