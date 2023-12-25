#include "utilities.h"

using namespace std;

namespace Backend_utilities {

bool c_in_s (char c, const string& s) { return (s.find(c) != string::npos); }

char s_to_c (const string& s)
{
    if (s[0] >= '0' && s[0] <= '9')  // если число
        return ('n');                // заменяем на спец. символ
    else
        return (s[0]);  // первая буква отражает название функции
}

bool is_double (const std::string& str)
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
    for (const auto& ch : s)
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

}  // namespace Backend_utilities