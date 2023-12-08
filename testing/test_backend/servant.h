#pragma once

#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using std::endl;

// вспомогательная функция "содержится ли символ в строке?"
bool c_in_s (char c, const std::string& s);

// вспомогательная функция, переводящая строку (лексему) в символ
char transform_to_char (const std::string& s);

bool is_float (std::string str);

template <typename T> void print (const std::vector<T>& v);

template <typename T> void print (std::stack<T> st);

std::string spaces_deleted (const std::string& s);

template <typename T> size_t last_i (const std::vector<T>& v);

const char point = '.';
const char plus = '+';
const char minus = '-';
const char mul = '*';
const char divi = '/';
const char power = '^';
const char open_br = '(';
const char closed_br = ')';
const char number = 'n';
const char var_x = 'x';

const char c_sin = 's';
const char c_cos = 'c';
const char c_tan = 't';
const char c_exp = 'e';
const char c_ln = 'l';
const char uminus = 'u';