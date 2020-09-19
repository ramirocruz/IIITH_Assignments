#include<bits/stdc++.h>
using namespace std;

void trim(string& s)
    {  
        
        string temp ="";
        for(int i=0;i<s.size();i++)
        {
             if(isdigit(s[i]))
             {
               temp+=s[i];
             }
             else if(s[i]==0)
             {
                 temp+='0';
             }
            
        }
        int i=0;
        int n = temp.size();        
        while((i< n-1) && (temp[i]=='0'))
        i++;
        
        temp=temp.substr(i);
        s= temp;
        

    }
string multiply(string a,string b)
{
   string& op1=a;
   string& op2=b;
   int n1=op1.size();
   int n2=op2.size();
   string ans;
   if(op1=="0" || op2=="0")
   return "0";
   
   ans.resize(n2 + n1);
//    cout<<ans.size()<<endl;
   int rightpos=n2+n1-1;
   for(int i=n2-1;i>-1;i--)
   {
       if(op2[i]=='0')
       {
           rightpos--;
           continue;
       }
       int pos = rightpos,carry =0;
    //    cout<<pos<<endl;
       for(int j=n1-1;j>-1;j--)
       {
           int temp =  (op1[j]-'0')*(op2[i] - '0') + carry + max(0,ans[pos]-'0');
        //    cout<<ans[pos]<<"  pos   "<<carry<<endl;
        //    cout<<temp<<"   ";
           ans[pos] = temp%10 + '0';
           carry = temp/10;
           pos--;
       }
      
       if(carry)
       ans[pos] = carry+'0';
       rightpos--;
        // cout<<endl<<ans<<endl;
   }

   
 
   return ans;
 }
string   add(string a,string b)
{
    int carry =0;
    string &op1 = a;
    string &op2 = b;
    int n1=a.size();
    int n2=b.size();
    if(n2 < n1)
   { swap(op1,op2);
     swap(n1,n2);
   }

    string ans;
    ans.resize(max(n2,n1)+1);
    
    int diff = n2 - n1;
    
        for(int i=n1-1;i>-1;i--)
        {   
            int sum = op1[i]-'0' + op2[i+diff] -'0' + carry;
            ans[i+diff+1] = char(sum%10 +'0');
            carry =  sum/10;
        }
          for(int i=diff-1;i>-1;i--)
        {   
            int sum = op2[i] -'0' + carry;
            ans[i+1] = char(sum%10 +'0');
            carry =  sum/10;
        }
        
        if(carry)
        ans[0]=carry+'0';
        else
        ans = ans.substr(1);
        
    
    return ans;
    
}

string   subtract(string a,string b)
{
    int carry =0;
    string &op1 = a;
    string &op2 = b;
    int n1=a.size();
    int n2=b.size();
    //op1 should be bigger
    if(n2 > n1)  
   { swap(op1,op2);
     swap(n1,n2);
   }else if(n2==n1)
   {
       if(op2 > op1)
       {
           swap(op1,op2);
           
       }
   }

    string ans;
    ans.resize(max(n2,n1));
    
    int diff = n1 - n2;
    
        for(int i=n2-1;i>-1;i--)
        {   
            int sub = op1[i+diff] - op2[i]  - carry;
             carry =0;
             if(sub < 0)
             {
                 sub+=10;
                 carry =1;
             }
             ans[i+diff] = sub + '0';
        }
          for(int i=diff-1;i>-1;i--)
        {   
            int sub = op1[i] -'0' - carry;
            carry =0;
             if(sub < 0)
             {
                 sub+=10;
                 carry =1;
             }
             ans[i]=sub + '0';
        }        
      
        
   
    return ans;
    
}


class BigInt{
    public:
    string data;

    
    unsigned long long size()
    {
        return this->data.size();
    }
    BigInt(){
        this->data="0";
    }
    BigInt(string s)
    {
        trim(s);
        this->data = s;
    }
   void operator = (const BigInt &d)
   {
     
     this->data = d.data;
     
    }
     void operator = (const string &s)
    {      
     this->data = s;   
     trim(this->data);
    }
    
};

istream & operator >> (istream &in,  BigInt &d)
{
    in>>d.data;
    return in;
}
  ostream & operator << (ostream &out,  const BigInt &d)
{
    out<<d.data;
    return out;
}


bool operator < (BigInt a,BigInt b)
{
    if(a.size() < b.size())
    return true;
    if(a.size() == b.size())
    return (a.data < b.data);
    return false;
} 
bool operator > (BigInt a,BigInt b)
{
    if(a.size() < b.size())
    return false;
    if(a.size() == b.size())
    return (a.data > b.data);
    return true;
} 
bool operator <= (BigInt a,BigInt b)
{
    if(a.size() < b.size())
    return true;
    if(a.size() == b.size())
    return (a.data <= b.data);
    return false;
} 
bool operator >= (BigInt a,BigInt b)
{
    if(a.size() < b.size())
    return false;
    if(a.size() == b.size())
    return (a.data >= b.data);
    return true;
} 
bool operator == (BigInt a,BigInt b)
{
    if(a.size() < b.size())
    return false;
    if(a.size() == b.size())
    return (a.data == b.data);
    return false;
} 
bool operator == (BigInt a,string b)
{
    if(a.size() < b.size())
    return false;
    if(a.size() == b.size())
    return (a.data == b);
    return false;
} 
bool operator == (string a,BigInt b)
{
    if(a.size() < b.size())
    return false;
    if(a.size() == b.size())
    return (a == b.data);
    return false;
} 
bool operator != (BigInt a,BigInt b)
{
    if(a.size() < b.size())
    return true;
    if(a.size() == b.size())
    return (a.data != b.data);
    return true;
} 


BigInt  operator * (BigInt a,BigInt b){
    
    BigInt temp= multiply(a.data,b.data);
    return temp;
}
BigInt  operator * (BigInt a,string b){
    BigInt temp = multiply(a.data,b);
    return temp;
}
BigInt  operator * (string a,BigInt b){
    BigInt temp = multiply(a,b.data);
    return temp;
}
BigInt&  operator *= (BigInt &a,string b){
    a = multiply(a.data,b);
    return a;
}
BigInt&  operator *= (BigInt &a,BigInt b){
    a = multiply(a.data,b.data);
    return a;
}

BigInt  operator + (BigInt a,BigInt b){
    BigInt temp = add(a.data,b.data);
    return temp;
}
BigInt  operator + (BigInt a,string b){
    BigInt temp = add(a.data,b);
    return temp;
}
BigInt  operator + (string a,BigInt b){
    BigInt temp = add(a,b.data);
    return temp;
}
BigInt&  operator += (BigInt &a,BigInt b){
    a = add(a.data,b.data);
    return a;
}
BigInt&  operator += (BigInt &a,string b){
    a = add(a.data,b);
    return a;
}
BigInt&  operator ++ (BigInt &a,int){
    a= add(a.data,"1");
    return a;
}
BigInt  operator - (BigInt a,BigInt b){
    BigInt temp = subtract(a.data,b.data);
    return temp;
}
BigInt  operator - (BigInt a,string b){
    BigInt temp = subtract(a.data,b);
    return temp;
}
BigInt  operator - (string a,BigInt b){
    BigInt temp = subtract(a,b.data);
    return temp;
}
BigInt&  operator -= (BigInt &a,BigInt b){
    a= subtract(a.data,b.data);
    return a;
}
BigInt&  operator -= (BigInt &a,string b){
    a= subtract(a.data,b);
    return a;
}
BigInt&  operator -- (BigInt &a,int){
    a = subtract(a.data,"1");
    return a;
}


string divide(string op1,string op2)
{
    int n1=op1.size();
    int n2=op2.size();
  
    if(op2=="0")
    {
        cout<<"\nError divide by 0.....\n";
        exit(-1);
    }

    if(n1 < n2)
    return "0";
    else if(n1==n2)
    {
        if(op1 < op2)
        return "0";
    }
    string ans,rem;
    int pos = n2-1; 
    string temp = op1.substr(0,n2);
    if(temp<op1)
    {temp+=op1[n2];
    pos=n2;} 

    while (pos < op1.size()) {
 
        int i=1;BigInt tempop(op2);
        for(;i<10;i++)
        {     
              if(tempop > temp)
               break;
             tempop+=op2;
        }
        i--;          
        ans += i + '0';
        tempop-=op2;
        tempop = temp - tempop;
        rem=tempop.data;
        if(rem=="0")
        temp=op1[++pos];
        else
        temp=rem+op1[++pos];
         
    } 
  
   return ans;
}

string modulo(string op1,string op2)
{
    int n1=op1.size();
    int n2=op2.size();
  
    if(op2=="0")
    {
        cout<<"\nError divide by 0.....\n";
        exit(-1);
    }

    if(n1 < n2)
    return op1;
    else if(n1==n2)
    {
        if(op1 < op2)
        return op1;
    }
    string rem;
    int pos = n2-1; 
    string temp = op1.substr(0,n2);
    if(temp<op1)
    {temp+=op1[n2];
    pos=n2;} 

    while (pos < op1.size()) {
 
        int i=1;BigInt tempop(op2);
        for(;i<10;i++)
        {     
              if(tempop > temp)
               break;
             tempop+=op2;
        }

        tempop-=op2;
        tempop = temp - tempop;
        rem=tempop.data;
        if(rem=="0")
        temp=op1[++pos];
        else
        temp=rem+op1[++pos];
         
    } 
  
   return rem;
}

BigInt operator / (BigInt a,BigInt b)
{
    BigInt temp = divide(a.data,b.data);
    return temp;
}
BigInt operator / (BigInt a,string b)
{
    BigInt temp = divide(a.data,b);
    return temp;
}
BigInt operator / (string a,BigInt b)
{
    BigInt temp = divide(a,b.data);
    return temp;
}
BigInt& operator /= (BigInt &a,BigInt b)
{
    a = divide(a.data,b.data);
    return a;
}
BigInt& operator /= (BigInt &a,string b)
{
    a = divide(a.data,b);
    return a;
}
BigInt operator % (BigInt a,BigInt b)
{
    BigInt temp = modulo(a.data,b.data);
    return temp;
}
BigInt operator % (BigInt a,string b)
{
    BigInt temp = modulo(a.data,b);
    return temp;
}
BigInt operator % (string a,BigInt b)
{
    BigInt temp = modulo(a,b.data);
    return temp;
}
BigInt& operator %= (BigInt& a,BigInt b)
{
    a = modulo(a.data,b.data);
    return a;
}
BigInt& operator %= (BigInt& a,string b)
{
    a = modulo(a.data,b);
    return a;
}



BigInt gcd(BigInt a,BigInt b)
{   
    BigInt Z("0");
    if(a == Z)
     return b;
    return gcd(b%a,a);
    
}
BigInt epow(BigInt a,long long p)
{   
    BigInt temp("1");
    if(p==0)
    return temp;
    if(p==1)
    return a;
    if(a=="0" || a=="1")
    return a;

    temp = epow(a,p/2)*epow(a,p/2);
    

    if(p&1)
    {
        temp*=a;
        return temp;
    }
    
    return  temp;
}
BigInt fact(int n)
{
    BigInt ans("1");
    for(int i=2;i<=n;i++)
    ans*= to_string(i);

    return ans;
}


int main()
{
 int Q,mode;
 string num1,num2;
 int num;
 BigInt op1,op2,ans;
 cin>>Q;
 while(Q--)
 {
   
   cin>>mode;
   if(mode == 1)
   {
       
       cin>>num1>>num;
       op1 =num1;
       
       ans = epow(op1,num);
   }
   else if(mode == 2)
   {
       cin>>num1>>num2;
       op1 = num1;
       op2 = num2;
       ans = gcd(op1,op2);
   }
   else{
       cin>>num;
       ans = fact(num);
   }

   cout<<ans<<'\n';
 }
return 0;
}

