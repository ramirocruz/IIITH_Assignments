#include <bits/stdc++.h>

using namespace std;
template <typename T>
class Deque
{
    T *arr;
    long long int CAPACITY = 10;
    long long int size_queue;
    long long int f, r;

public:
    Deque()
    {
        size_queue = CAPACITY;
        arr = new T[CAPACITY];

        f = r = -1;
    }
    Deque(long long int n, T x)
    {
        CAPACITY = n + 1;
        arr = new T[CAPACITY]{x};
        for (long long int i = 0; i < n; i++)
            arr[i] = x;
        f = 0;
        r = n;
        size_queue = CAPACITY;
    }
    T back()
    {
        return arr[r - 1];
    }
    T front()
    {
        return arr[f];
    }
    void push_back(T value)
    {
        if (((r + 1) % size_queue == f))
        {
            resize(size() + 1, value);
        }
        else if (f == -1)
        {
            f = r = 0;
            arr[r++] = value;
            // cout<<r << " " << f << "\n";
        }
        else
        {
            arr[r] = value;
            r = ++r % size_queue;
        }
    }
    void push_front(T value)
    {
        if (size() == size_queue - 1)
        {
            resize(size() + 1, value, 1);
        }
        else if (f == -1)
        {
            f = r = 0;
            arr[f] = value;
            r = 1;
            // cout << f << " " << arr[f] << "\n";
            return;
        }
        else
        {
            f = (--f) % size_queue;
            if (f < 0)
                f = size_queue + f;
            arr[f] = value;
            // cout << f << " " << arr[f]<< "\n";
        }
    }
    long long int size()
    {
        if (r >= f)
            return r - f;
        else
            return (size_queue - f + r);
    }
    void resize(long long int n, T x)
    {
        if (n <= size())
        {
            while (size() != n)
            {
                pop_back();
            }
        }
        if (n > size())
        {
            if (n + 1 > CAPACITY)
            {
                CAPACITY = 2 * n;
                T *temp = new T[CAPACITY];
                for (long long int i = 0, point = f; i < size(); i++)
                {
                    temp[i] = arr[point];
                    point = (++point) % size_queue;
                }
                for (long long int i = size(); i < n; i++)
                {
                    temp[i] = x;
                }
                delete[] arr;
                arr = temp;
                f = 0;
                r = n;
                size_queue = CAPACITY;
            }
            else
            {
                // size_queue = n;
                while (size() != n)
                {
                    push_back(x);
                }
            }
        }
    }
    void resize(long long int n, T x, bool pf)
    {
        if (n <= size())
        {
            while (size() != n)
            {
                pop_back();
            }
        }
        if (n > size())
        {
            if (n > CAPACITY)
            {
                CAPACITY = 2 * n;
                T *temp = new T[CAPACITY];
                for (long long int i = 0, point = f; i < size(); i++)
                {
                    temp[i] = arr[point];
                    point = (++point) % size_queue;
                }
                temp[CAPACITY - 1] = x;
                delete[] arr;
                arr = temp;
                r = size();

                f = CAPACITY - 1;
                size_queue = CAPACITY;
            }
            else
            {
                // size_queue = n;
                while (size() != n)
                {
                    push_front(x);
                }
            }
        }
    }
    void pop_back()
    {
        if (!empty())
        {
            r = (--r) % size_queue;
            if (r < 0)
                r = size_queue + r;
        }
        else
            cout << "The Dequeue is empty";
    }

    void pop_front()
    {
        if (!empty())
        {
            f = (++f) % size_queue;
        }
        else
            cout << "The Dequeue is empty";
    }
    void clear()
    {
        // size_queue = 0;
        f = r = -1;
    }
    T &operator[](long long int index)
    {
        if (index >= size())
        {
            cout << "invalid index\n";
        }
        else
            return arr[(f + index) % size_queue];
    }
    bool empty()
    {
        if (f == r)
        {
            return true;
        }
        else
            return false;
    }
    void display()
    {
        for (int i = f; i != r; i = (i + 1) % size_queue)
        {
            cout << arr[i] << " ";
        }
        cout << "\n";
    }
};

int main()
{
    Deque<string> *arr = NULL;
    long long int t, query, n;
    string x;
    cin >> t;
    while (t--)
    {

        cin >> query;

        switch (query)
        {

        case 1:
            cin >> x;
            arr->push_front(x);
            arr->display();

            break;
        case 2:
            arr->pop_front();
            arr->display();

            break;

        case 3:
            cin >> x;
            arr->push_back(x);
            arr->display();
            
            break;

        case 4:
            arr->pop_back();
            arr->display();
            
            break;

        case 5:
            if (arr != NULL)
                delete arr;
            
            arr = new Deque<string>();
            cout << "\n";
            
            break;

        case 6:
            cin >> n >> x;
            if (arr != NULL)
                delete arr;
            
            arr = new Deque<string>(n, x);
            cout << "\n";
            
            break;

        case 7:
            cout << arr->front() << endl;
            break;

        case 8:
            cout << arr->back() << endl;
            break;

        case 9:
            if (arr->empty())
                cout << "true\n";
            else
                cout << "false\n";
            
            break;

        case 10:
            cout << arr->size() << endl;
            
            break;

        case 11:
            cin >> n >> x;
            arr->resize(n, x);
            arr->display();
            
            break;

        case 12:
            arr->clear();
            arr->display();
            
            break;

        case 13:
            cin >> n;
            cout << (*arr)[n] << endl;
            
            break;

        case 14:
            arr->display();
            
            break;

        default:
            
            break;
        }
    }

    delete arr;
    return 0;
}

