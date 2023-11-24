#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

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

    size_t len () { return st.size(); }

    void print ()
    {
        for (auto i : st)
            cout << i << ", ";
        cout << endl;
    }

  private:
    vector<double> st;
};

// вспомогательная функция "содержится ли символ в строке?"
bool c_in_s (char c, const string& s) { return (s.find(c) != string::npos); }

// вспомогательная функция, переводящая строку (лексему) в символ
char transform_to_char (const string& s)
{
    if (s[0] >= '0' && s[0] <= '9')  // если число
        return ('n');                // заменяем на спец. символ
    else
        return (s[0]);  // первая буква отражает название функции
}

// вспомогательная функция, переводящая вектор строку (лексем) в вектор символов
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

bool is_float (string str)
{
    std::istringstream iss(str);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail();
}

void print (const vector<string>& v)
{
    cout << "{ ";
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << "'" << v[i] << "'"
             << ", ";
    }
    cout << "}" << endl;
}

void print (const vector<char>& v)
{
    cout << "{ ";
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << "'" << v[i] << "'"
             << ", ";
    }
    cout << "}" << endl;
}

string spaces_deleted (const string& s) { return (regex_replace(s, regex("\\s+"), "")); }