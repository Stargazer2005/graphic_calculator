#include "back_func.h"
#include <stdexcept>

// локальный класс (для взаимодействия с обратной польской записью)
// главная фишка - возможность обращения только к последнему элементу
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

// vector<char> transform_to_char (const vector<string>& strs)
// {
//     vector<char> res{};
//     for (auto s : strs)
//     {
//         if (s[0] >= '0' && s[0] <= '9')
//             res.push_back('n');
//         else
//             res.push_back(s[0]);
//     }
//     return res;
// }

// вспомогательная функция, переводящая строку (лексему) в символ
char transform_to_char (const string& s)
{
    if (s[0] >= '0' && s[0] <= '9')  // если число
        return ('n');                // заменяем на спец. символ
    else
        return (s[0]);  // первая буква отражает название функции
}

double calc (const vector<string>& rev_pol, double x)
{
    const string oper = "+-*/^";   // строка с операциями
    const string func = "sctelu";  // строка с функциями
    // да, унарный минус - тоже функция

    Stack stack;
    for (auto lex : rev_pol)
    {
        // stack.print();
        double l, p;  // локальные переменные - последний и предпоследний символы в стеке
        char curr = transform_to_char(lex);  // текущий символ,
                                             // если брать вместо строки (для switch)
        if (func.find(curr) != string::npos)  // получили функцию
        {
            l = stack.last();  // запоминаем только последний, так как операции унарны
            stack.pop();
            switch (curr)
            {
            case 's':
                stack.push(sin(l));
                break;
            case 'c':
                stack.push(cos(l));
                break;
            case 't':
                stack.push(tan(l));
                break;
            case 'e':
                stack.push(exp(l));
                break;
            case 'l':
                stack.push(log(l));
                break;
            case 'u':
                stack.push(0 - l);
                break;
            }
        }
        else if (oper.find(curr) != string::npos)  // получили операцию
        {
            l = stack.last();
            stack.pop();
            p = stack.last();  // также запоминаем предпоследний
            stack.pop();
            switch (curr)
            {
            case '+':
                stack.push(p + l);
                break;
            case '-':
                stack.push(p - l);
                break;
            case '*':
                stack.push(p * l);
                break;
            case '/':
                stack.push(p / l);
                break;
            case '^':
                stack.push(pow(p, l));
                break;
            }
        }
        else  // получили число, x или ошибку
        {
            switch (curr)
            {
            case 'n':
                stack.push(stod(lex));
                break;
            case 'x':
                stack.push(x);
                break;
            default:
                throw runtime_error("Oops");
                break;
            }
        }
    }
    return stack.last();  // последнее, что осталось в стэке после всех действий - и есть ответ
}