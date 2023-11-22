#include "back_func.h"
#include <stdexcept>

class Stack
{
  public:
    Stack() : st{} {}

    void push (double c) { st.push_back(c); }

    void pop ()
    {
        if (st.size() != 0)
            st.pop_back();
        else
            throw range_error("Stack: empty, nothing to pop");
    }

    double last ()
    {
        if (st.size() != 0)
            return st[st.size() - 1];
        else
            throw range_error("Stack: empty, no last");
    }

    unsigned long long len () { return st.size(); }

    void print ()
    {
        for (auto i : st)
        {
            cout << i << ", ";
        }
        cout << endl;
    }

  private:
    vector<double> st;
};

bool checker (const string& expr) {}

vector<char> transform_to_char (const vector<string>& strs)
{
    vector<char> res{};
    for (auto s : strs)
    {
        if (s[0] >= '0' && s[0] <= '9')
            res.push_back('n');
        else
            res.push_back(s[0]);
    }
    return res;
}

double calc (const vector<string>& rev_pol, double x)
{
    Stack stack;
    for (auto i = 0; i < rev_pol.size(); i++)
    {
        // stack.print();
        double l, p;
        switch (transform_to_char(rev_pol)[i])
        {
        case 'n':
            stack.push(stod(rev_pol[i]));
            break;
        case 'x':
            stack.push(x);
            break;
        case 's':
            l = stack.last();
            stack.pop();
            stack.push(sin(l));
            break;
        case 'c':
            l = stack.last();
            stack.pop();
            stack.push(cos(l));
            break;
        case 't':
            l = stack.last();
            stack.pop();
            stack.push(tan(l));
            break;
        case 'e':
            l = stack.last();
            stack.pop();
            stack.push(exp(l));
            break;
        case 'l':
            l = stack.last();
            stack.pop();
            stack.push(log(l));
            break;
        case 'u':
            l = stack.last();
            stack.pop();
            stack.push(0 - l);
            break;
        case '+':
            l = stack.last();
            stack.pop();
            p = stack.last();
            stack.pop();
            stack.push(p + l);
            break;
        case '-':
            l = stack.last();
            stack.pop();
            p = stack.last();
            stack.pop();
            stack.push(p - l);
            break;
        case '*':
            l = stack.last();
            stack.pop();
            p = stack.last();
            stack.pop();
            stack.push(p * l);
            break;
        case '/':
            l = stack.last();
            stack.pop();
            p = stack.last();
            stack.pop();
            stack.push(p / l);
            break;
        case '^':
            l = stack.last();
            stack.pop();
            p = stack.last();
            stack.pop();
            stack.push(pow(p, l));
            break;
        default:
            throw runtime_error("Oops");
            break;
        }
    }
    return stack.last();
}