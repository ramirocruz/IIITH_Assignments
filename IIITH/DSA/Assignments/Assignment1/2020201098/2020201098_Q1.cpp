#include<bits/stdc++.h>
using namespace std;


template <class T>
class Stack{
  public:
    class Node{
     public:
      T data;
     Node* next;
     };
     Node * topp;
    int size;
    Stack()
    {
        this->topp=NULL;
        this->size=0;
    }
   void push(T data)
   {
      Node * temp = new Node();
       temp->data = data;
       temp->next = this->topp;
       this->topp = temp;
       size++;
   }
   void pop()
   {
       if(!size)
       return;
        Node * temp=topp;
       topp=topp->next;
       delete temp;
       size--;
   }
   T top()
   {
       return ((this->topp)->data);
   }
};

void trim(string& s)
{   string temp;
    bool dig=false;
    for(int i=0;i<s.size();i++)
    {
       if(s[i]==' ')
       continue;

       if(isdigit(s[i]))
       {
           while(i<s.size() && isdigit(s[i]) || s[i]=='.')
           {
               temp+=s[i];
               i++;
           }
           i--;
           temp+='|';
       }
       else
       temp+=s[i];
    }
    s=temp;
}
int prec(char c)
{
    if(c=='*' || c=='/' || c=='%')
    return 2;
    if(c=='+' || c=='-')
    return 1;
    return 0;
}
long double calc(long double a,long double b,char c)
{
    switch (c)
    {
    case '+': return (a+b);
              
    case '-': return (a-b);

    case '*': return (a*b);

    case '/': return (a/b);

    case '%': return (fmod(a,b));

    default: return -1;
        break;
    }
    return -1;
}
long double eval(string& s)
{
     char op;
     long double op1,op2,ans;
    Stack<char> operator_stack;
    Stack<long double> operand_stack;
    operator_stack.push('#');
    for(int i=0;i<s.size();i++)
    {
        // cout<<" Top \n";
        // if(operator_stack.size)
        // cout<<operator_stack.top()<<endl;
        // if(operand_stack.size)
        // cout<<operand_stack.top()<<endl;
        // cout<<" End\n";
        if(s[i]=='(')
        {
            operator_stack.push(s[i]);
            // cout<<"here in (\n";
        }
        else if(s[i]==')')
        { 
            // cout<<"here in )\n";
           
            while(operator_stack.size && operator_stack.top() != '(')
            {
                op  = operator_stack.top();
                operator_stack.pop();
                op2 = operand_stack.top();
                operand_stack.pop();
                op1 = operand_stack.top();
                operand_stack.pop();
                ans = calc(op1,op2,op);
                // cout<<ans<<endl;
                operand_stack.push(ans);
            }
            operator_stack.pop();
            
        }
        else if(s[i]=='+' || s[i]=='-' || s[i] =='*' || s[i] == '/' || s[i] == '%')
        {
            // cout<<"here in operator popping\n";
            while (operator_stack.size && prec(operator_stack.top()) >= prec(s[i]))

            {
                op  = operator_stack.top();
                operator_stack.pop();
                op2 = operand_stack.top();
                operand_stack.pop();
                op1 = operand_stack.top();
                operand_stack.pop();
                ans = calc(op1,op2,op);
                // cout<<ans<<endl;
                operand_stack.push(ans);
            }
            operator_stack.push(s[i]);
        }
        else
        {   string temp="";
        // cout<<"Here in operand input\n";
            while(s[i]!='|')
            {
                temp+=s[i];
                i++;
            }
            ans = stold(temp);
            // cout<<ans<<endl;
            operand_stack.push(ans);
        }
        
        
    }
     while (operator_stack.size>1)
            {
                op  = operator_stack.top();
                operator_stack.pop();
                op2 = operand_stack.top();
                operand_stack.pop();
                op1 = operand_stack.top();
                operand_stack.pop();
                ans = calc(op1,op2,op);
                operand_stack.push(ans);
            }
    operator_stack.pop();
return (operand_stack.top());

}
int main()
{

string s;
getline(cin,s);
trim(s);
// cout<<s<<endl;
long double ans =eval(s);
printf("%0.9Lf\n",ans);
  

return 0;
}