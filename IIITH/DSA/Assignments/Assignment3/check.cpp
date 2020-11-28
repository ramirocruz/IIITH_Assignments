#include<bits/stdc++.h>
using namespace std;

void numbers(char* file)
{
  long long count = 0, ele;
  FILE *f = fopen(file, "r");
  while(1)
  {
    if(fscanf(f, "%lld,", &ele) != 1)
    {
      break;
    }
    count++;
  }
  fclose(f);
  cout<<string(file)<<" : "<<count<<endl;
}

bool sorted(char* file)
{
  bool flag = true;
  long long ele, prev, i;
  i=1;
  FILE *f = fopen(file, "r");
  fscanf(f, "%lld,", &prev);
  while (true)
  {
    if (fscanf(f, "%lld,", &ele) != 1)
    {
      break;
    }
    if (prev > ele)
    {
      flag = false;
      break;
    }
    i++;
    prev = ele;
  }
  cout << i << endl;
  fclose(f);
  return flag;
}

int main(int argc, char* argv[])
{
	numbers(argv[1]);
	numbers(argv[2]);
	cout << sorted(argv[2]) << endl;
	return(0);
}