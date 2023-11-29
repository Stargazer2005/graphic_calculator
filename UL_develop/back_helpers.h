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
            throw std::range_error("Stack: empty, nothing to pop");
    }

    double last ()
    {
        if (st.size() != 0)
            return st[st.size() - 1];
        else
            throw std::range_error("Stack: empty, no last");
    }

    size_t len () { return st.size(); }

    void print ()
    {
        for (auto i : st)
            std::cout << i << ", ";
        std::cout << std::endl;
    }

  private:
    std::vector<double> st;
};

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
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

void print (const std::vector<std::string>& v)
{
    std::cout << "{ ";
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << "'" << v[i] << "'"
                  << ", ";
    }
    std::cout << "}" << std::endl;
}

void print (const std::vector<char>& v)
{
    std::cout << "{ ";
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << "'" << v[i] << "'"
                  << ", ";
    }
    std::cout << "}" << std::endl;
}

std::string spaces_deleted (const std::string& s)
{
    std::string new_s;
    for (auto ch : s)
        if (ch != ' ')
            new_s += ch;
    return new_s;
}