#pragma once

// std libs
#include <stack>
#include <string>
#include <vector>

namespace Back_serv {

// DOES: проверяет, содержится ли символ в строке
bool c_in_s (char c, const std::string& s);

// DOES: проверяет, содержится ли елемент в векторе
template <typename T> bool elem_in_vec (T elem, const std::vector<T>& vec)
{
    return find(vec.begin(), vec.end(), elem) != vec.end();
}

// DOES: переводит строку (лексему) в символ
char s_to_c (const std::string& s);

bool is_double (const std::string& str);

std::string spaces_deleted (const std::string& s);

// DOES: считает модуль для double
// (чтобы избежать неявного преобразования, используя std::abs)
double absolute (double x);

// DOES: заменяет кусочек строки другим, модифицируя саму строку
bool replace (std::string& str, const std::string& from, const std::string& to);

}  // namespace Back_serv