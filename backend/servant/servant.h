#pragma once

// std libs
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using std::endl;

namespace Back_serv {

// вспомогательная функция "содержится ли символ в строке?"
bool c_in_s (char c, const std::string& s);

// вспомогательная функция, переводящая строку (лексему) в символ
char transform_to_char (const std::string& s);

bool is_float (std::string str);

template <typename T> void print (const std::vector<T>& v);

template <typename T> void print (std::stack<T> st);

std::string spaces_deleted (const std::string& s);

double absolute (double x);

}