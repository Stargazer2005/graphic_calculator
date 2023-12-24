
#include "servant.h"

// std libs
// #include <iostream>
using namespace std;

namespace Back_serv {

bool c_in_s (char c, const string& s) { return (s.find(c) != string::npos); }

// template <typename T> bool elem_in_vec (T elem, const vector<T>& vec)
// {
//     return find(vec.begin(), vec.end(), elem) != vec.end();
// }

char s_to_c (const string& s)
{
    if (s[0] >= '0' && s[0] <= '9')  // если число
        return ('n');                // заменяем на спец. символ
    else
        return (s[0]);  // первая буква отражает название функции
}

bool is_double (string str)
{
    try
    {
        stod(str);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

string spaces_deleted (const string& s)
{
    string new_s;
    for (auto ch : s)
        if (ch != ' ')
            new_s += ch;
    return new_s;
}

double absolute (double x) { return x >= 0 ? x : -x; }

bool replace (string& str, const string& from, const string& to)
{

    size_t start_pos = str.find(from);
    if (start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

}  // namespace Back_serv