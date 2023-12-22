#pragma once

// std libs
#include <functional>
#include <string>
#include <vector>

// servant
#include "servant/constants.h"

namespace Backend {

class function
{
  public:
    function(std::string _func_str);

    function(const function&);

    // methods:

    function operator= (const function&);

    double operator() (double x) { return calc(x); }

    // лямбда-функция, считающая по заданному x значение функции
    const std::function<double(double)> calculate = [this] (double x) { return calc(x); };

    // лямбда-функция, считающая по заданному x значение производной функции
    const std::function<double(double)> differentiate = [this] (double x)
    {
        return (calc(x + Back_consts::delta_x) - calc(x - Back_consts::delta_x)) /
               (2 * Back_consts::delta_x);
    };

    bool has_var () const;

    std::string get_func_str () const { return func_str; }

  private:
    // variables:

    // значение строки
    std::string func_str;
    // список лексем
    std::vector<std::string> lexs;
    // обратная польская нотация
    std::vector<std::string> rev_pol;

    // methods:

    // метод, проверяющий на корректность скобок, отсутствие лишних символов,
    // правильное использование знаков и чисел возле них, правильное
    // использование точки, правильное использование цифр
    bool is_str_valid () const;
    // метод, проверяющий на корректность математические операции: отсутствие
    // деления на ноль, возведения отрицательного числа в дробную степень,
    // корректность использования функций (cos, sin, tan, exp, ln, um)
    bool is_lexs_valid () const;
    // метод, который переводит введеную пользователем строку в список лексем
    std::vector<std::string> lexemes () const;
    // метод, который переводит список лексем в обратную польскую запись
    std::vector<std::string> reverse_polish () const;
    // метод, который по значению переменной x вычисляет значение y
    double calc (double x) const;
};

// перегрузка оператора (только для принта, который будет удален позднее)
std::ostream& operator<< (std::ostream& os, function func);

}  // namespace Backend
