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

// делает Фёдор

// фунция, которая переводит введеную пользователем строку в список лексем
std::vector<std::string> lexeme (const std::string& expr);

// функция, которая переводит список лексем в обратную польскую запись
std::vector<std::string> reverse_polish (const std::vector<std::string>& lexs);

// делает Дмитрий

// функция, проверяющая на корректность скобок, отсутствие лишних символов,
// правильное использование знаков и чисел возле них, правильное
// использование точки, правильное использование цифр
bool checker (const std::string& expression);

// функция, проверяющая на корректность математические операции: отсутствие
// деления на ноль, возведения отрицательного числа в дробную степень,
// корректность использования функций (cos, sin, tan, exp, ln, um)
bool checker (const std::vector<std::string>& lexs);

// функция, которая по обратной польской записи лексем и значению переменной
// x вычисляет значение y
double calc (const std::vector<std::string>& rev_pol, double x);

const char point = '.';
const char plus = '+';
const char minus = '-';
const char mul = '*';
const char divi = '/';
const char power = '^';
const char open_br = '(';
const char closed_br = ')';
const char number = 'n';

const char c_sin = 's';
const char c_cos = 'c';
const char c_tan = 't';
const char c_exp = 'e';
const char c_ln = 'l';
const char uminus = 'u';