#pragma once

// std libs
#include <functional>
#include <string>
#include <vector>

// servant
#include "../servant/constants.h"

#include "../temp_help.h"

namespace Math_func {

// MEMO: класс, представляющий собой математическую функцию
class function
{
  public:
    function(std::string _func_str);

    function(const function&);

    // methods:

    function operator= (const function&);

    double operator() (double x) const { return calc(x); }

    // MEMO: лямбда-функция, считающая по заданному x значение функции
    const std::function<double(double)> calculate = [this] (double x) { return calc(x); };

    // MEMO: лямбда-функция, считающая по заданному x значение производной функции
    const std::function<double(double)> differentiate = [this] (double x)
    {
        return (calc(x + Back_consts::delta_x) - calc(x - Back_consts::delta_x)) /
               (2 * Back_consts::delta_x);
    };

    bool has_var () const;

    std::string get_func_str () const { return func_str; }

  private:
    // vars:

    // MEMO: мат. функция в виде строки
    std::string func_str;
    // MEMO: список лексем
    std::vector<std::string> lexs;
    // MEMO: обратная польская нотация
    std::vector<std::string> rev_pol;

    // methods:

    // MEMO: метод, проверяющий на корректность строки
    // (правильное кол-во скобок, отсутствие лишних символов)
    // (правильное использование знаков, точки, цифр)
    bool is_str_valid () const;

    // MEMO: метод, проверяющий на корректность лексем
    // (корректность использования элементар. матю функций (cos, sin, tan, exp, ln, um))
    // (использование лишних переменных)
    bool is_lexs_valid () const;

    // MEMO: метод, который переводит введеную строку в список лексем
    std::vector<std::string> lexemes () const;
    // MEMO: метод, который переводит список лексем в обратную польскую нотацию
    std::vector<std::string> reverse_polish () const;
    // MEMO: метод, который по значению заданному x вычисляет значение функции
    double calc (double x) const;
};

}  // namespace Math_func

// MEMO: перегрузка оператора
// (только для принта, который будет удален позднее)
std::ostream& operator<< (std::ostream& os, Math_func::function func);
