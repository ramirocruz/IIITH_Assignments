#include <bits/stdc++.h>
using namespace std;

bool compare(string a, string b)
{
    if (a.size() > b.size())
        return true;
    else if (a.size() == b.size())
    {
        bool flag = 1;
        for (int i = 0; i < a.size(); i++)
        {
            if (a[i] > b[i])
            {
                return true;
                flag = 1;
                break;
            }
            if (a[i] < b[i])
            {
                flag = 0;
                break;
                return false;
            }
        }
        if (flag == 1)
            return true;
        else
            return false;
    }
    else
        return false;
}

string subs(string a, string b)
{
    if (!compare(a, b))
        return "0";
    string ans = "";
    int x = a.length();
    int y = b.length();
    int carry = 0;

    for (int i = y - 1, j = x - 1; i >= 0; i--, j--)
    {
        int sub = ((a[j] - '0') - (b[i] - '0') - carry);
        if (sub < 0)
        {
            sub = sub + 10;
            carry = 1;
        }
        else
            carry = 0;
        ans = to_string(sub) + ans;
    }
    for (int i = x - y - 1; i >= 0; i--)
    {
        int sub = ((a[i] - '0') - carry);
        if (sub < 0)
        {
            sub = sub + 10;
            carry = 1;
        }
        else
            carry = 0;
        ans = to_string(sub) + ans;
    }
    bool flag = 0;
    for (int i = 0; i < ans.size(); i++)
    {
        if (ans[i] != '0')
        {
            ans = ans.substr(i);
            flag = 1;
            break;
        }
    }
    if (flag == 0)
        ans = "0";
    return ans;
}

string mul(string a, string b)
{
    int x = a.size(), y = b.size();
    string ans(x + y, '0');

    for (int i = y - 1; i >= 0; i--)
    {
        int carry = 0;
        int j = x - 1;
        for (; j >= 0; j--)
        {

            int prod = ((a[j] - '0') * (b[i] - '0')) + (ans[i + j + 1] - '0') + carry;
            carry = prod / 10;
            ans[i + j + 1] = prod % 10 + '0';
        }
        if (carry > 0)
        {
            ans[i + j + 1] += carry;
        }
    }
    bool flag = 0;
    for (int i = 0; i < x + y; i++)
    {
        if (ans[i] != '0')
        {
            ans = ans.substr(i);
            flag = 1;
            // cout << i << "\n";
            break;
        }
    }
    if (flag == 0)
        ans = "0";

    return ans;
}

string add(string a, string b)
{

    if (a.length() > b.length())
        swap(a, b);

    string ans = "";
    int x = a.length();
    int y = b.length();

    int carry = 0;
    int j = y - 1;
    for (int i = x - 1; i >= 0; i--, j--)
    {
        int sum = ((a[i] - '0') + (b[j] - '0') + carry);
        ans = to_string(sum % 10) + ans;
        carry = sum / 10;
    }

    for (; j >= 0; j--)
    {
        int sum = ((b[j] - '0') + carry);
        ans = to_string(sum % 10) + ans;
        carry = sum / 10;
    }

    // Add remaining carry
    if (carry)
        ans = to_string(carry) + ans;

    return ans;
}

string longDivision(string numerator, string denominator)
{
    if (!compare(numerator, denominator))
        return numerator;
    int i = 0;
    int x = numerator.size();
    int y = denominator.size();
    string remainder, quotient;
    string temp;
    // i+=y;
    for (; i < x;)
    {
        int j = 0;
        int rd = 0;
        if (remainder == "0")
            rd = 0;
        else
            rd = remainder.size();
        if (rd != 0)
            j = y - rd;
        else
            j = y;

        if (j == 0)
            j++;

        if (i + j > x)
            j = x - i;

        if (remainder == "0")
            temp = numerator.substr(i, j);
        else
            temp = remainder + numerator.substr(i, j);
        int tq = 0, tr;
        string td = "0";
        while (compare(temp, td) && tq < 10)
        {
            tq++;

            string q = to_string(tq);
            td = mul(denominator, q);
        }
        tq--;
        quotient = quotient + to_string(tq);
        string q = to_string(tq);
        td = mul(denominator, q);
        remainder = subs(temp, td);

        string x = numerator.substr(i, j);

        i += (j);
    }
    bool flag = 0;
    for (int i = 0; i < remainder.size(); i++)
    {
        if (remainder[i] != '0')
        {
            remainder = remainder.substr(i);
            flag = 1;
            // cout << i << "\n";
            break;
        }
    }
    if (flag == 0)
        remainder = "0";

    return remainder;
}

string exp(string a, long long int b)
{
    string ans = "1";
    if (a == "0")
        return "0";
    while (b > 0)
    {
        if (b % 2)
            ans = mul(ans, a), b--;

        b = b / 2;
        a = mul(a, a);
    }
    return ans;
}

string fac(int n)
{
    if (n == 0 || n == 1)
        return "1";
    string no1 = to_string(n);
    string no2 = to_string(n - 1);
    string ans = mul(no1, no2);
    for (int i = n - 2; i >= 2; i--)
    {
        string no = to_string(i);
        ans = mul(ans, no);
    }

    return ans;
}

string gcd(string a, string b)
{
    if (a == "0") return b;
    string temp = longDivision(b, a);
    return gcd(temp, a);
}

int main()
{

    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    

    int t = 1;
    cin >> t;
    while (t--)
    {

        int q ; cin >> q;
        if(q == 1){
            string s ; cin >> s;
            long long int b; cin >> b;
            cout << exp(s,b) << "\n";
        }
        if(q == 2){
            string a , b; cin >> a >> b;
            cout << gcd(a,b) << "\n";
        }
        if(q==3){
            int n; cin >> n;
            cout << fac(n) <<"\n";
        }
    }

    return 0;
}
