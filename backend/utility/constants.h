#pragma once

// std libs
#include <cmath>

namespace Backend_consts {

constexpr char point = '.';
constexpr char plus = '+';
constexpr char minus = '-';
constexpr char mul = '*';
constexpr char divi = '/';
constexpr char power = '^';
constexpr char open_br = '(';
constexpr char closed_br = ')';
constexpr char number = 'n';
constexpr char var_x = 'x';

constexpr char c_sin = 's';
constexpr char c_cos = 'c';
constexpr char c_tan = 't';
constexpr char c_exp = 'e';
constexpr char c_ln = 'l';
constexpr char uminus = 'u';

constexpr double delta_x = 0.00001;

// MEANS: золотое сечение
const double phi = (1 + sqrt(5)) / 2;
constexpr int max_count = 10000;

}  // namespace Backend_consts