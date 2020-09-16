#include <bits/stdc++.h>
using namespace std;

template <typename T>
class calculator_stack
{
    T *array;
    int size;
    int top;

public:
    calculator_stack(int n)
    {
        size = n;
        array = new T[n];
        top = -1;
    }
    void push(T x)
    {
        array[++top] = x;
    }
    T pop()
    {
        if (top != -1)
            return (array[top--]);
        return NULL;
    }
    bool isempty()
    {
        if (top == -1)
            return true;
        return false;
    }
    T return_top()
    {
        if (!isempty())
            return array[top];
        return NULL;
    }
};

bool isoperator(char a)
{
    if (a == '+' || a == '-' || a == '/' || a == '*' || a == '%' || a == '+' || a == ')' || a == '(')
        return true;

    return false;
}

int get_precedence(char x)
{
    if (x == '+' || x == '-')
        return 1;
    if (x == '/' || x == '*' || x == '%')
        return 2;
    if (x == '(')
        return 0;
    return -1;
}

double calculate(double a, double b, char op)
{
    if (op == '+')
        return (a + b);
    if (op == '-')
        return (a - b);
    if (op == '*')
        return (a * b);
    if (op == '/')
        return (a / b);
    if (op == '%')
        return (fmod(a, b));
    return 1;
}
int main()
{
    int t = 1;
    // cin >> t;
    while (t--)
    {
        string s;
        cin >> s;
        calculator_stack<double> operand_stack(s.size());
        calculator_stack<char> operator_stack(s.size());
        string no;

        for (auto a : s)
        {
            if (isoperator(a))
            {
                if (!no.empty())
                {
                    double number = stod(no);
                    operand_stack.push(number);
                    no = "";
                }
                if (a == '(')
                {
                    operator_stack.push(a);
                    continue;
                }
                if (a == ')')
                {
                    while (operator_stack.return_top() != '(')
                    {

                        double right_operand = operand_stack.pop();
                        double left_operand = operand_stack.pop();
                        double ans = calculate(left_operand, right_operand, operator_stack.pop());
                        operand_stack.push(ans);
                    }
                    operator_stack.pop();
                    continue;
                }
                while (get_precedence(a) <= get_precedence(operator_stack.return_top()))
                {
                    double right_operand = operand_stack.pop();
                    double left_operand = operand_stack.pop();
                    double ans = calculate(left_operand, right_operand, operator_stack.pop());
                    operand_stack.push(ans);
                }

                operator_stack.push(a);
            }
            else
            {
                no.push_back(a);
            }
        }
        if (*(s.rbegin()) != ')' && !no.empty())
        {
            double number = stod(no);
            operand_stack.push(number);
            no = "";
        }

        while (!operator_stack.isempty())
        {

            double right_operand = operand_stack.pop();
            double left_operand = operand_stack.pop();
            double ans = calculate(left_operand, right_operand, operator_stack.pop());
            operand_stack.push(ans);
        }
        std::cout << std::fixed << std::setprecision(5);

        cout << operand_stack.pop() << "\n";
    }
}