// header
#include "servant.h"

namespace Back_serv {

// вспомогательная функция "содержится ли символ в строке?"
bool c_in_s (char c, const std::string& s) { return (s.find(c) != std::string::npos); }

// вспомогательная функция, переводящая строку (лексему) в символ
char transform_to_char (const std::string& s)
{
    if (s[0] >= '0' && s[0] <= '9')  // если число
        return ('n');                // заменяем на спец. символ
    else
        return (s[0]);  // первая буква отражает название функции
}

bool is_float (std::string str)
{
    std::istringstream iss(str);
    double f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

template <typename T> void print (const std::vector<T>& v)
{
    std::cout << " size: " << v.size() << endl;
    std::cout << "{ ";
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << "''" << v[i] << "''";
        if (i != v.size() - 1)
            std::cout << ", ";
    }
    std::cout << " }" << endl;
}

template <typename T> void print (std::stack<T> st)
{
    std::cout << " size: " << st.size() << endl;
    std::cout << "{ ";
    while (!st.empty())
    {
        std::cout << "''" << st.top() << "''";
        st.pop();
        if (!st.empty())
            std::cout << ", ";
    }
    std::cout << " }" << endl;
}

std::string spaces_deleted (const std::string& s)
{
    std::string new_s;
    for (auto ch : s)
        if (ch != ' ')
            new_s += ch;
    return new_s;
}

template <typename T> size_t last_i (const std::vector<T>& v) { return v.size() - 1; }

}  // namespace Back_serv