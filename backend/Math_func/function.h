#pragma once

// std libs
#include <functional>
#include <string>
#include <vector>

// servant
#include "../servant/constants.h"

#include "../temp_help.h"

namespace Math_func {

// MEANS: класс, представляющий собой математическую функцию
class function
{
  public:
    function(std::string _func_str);

    function(const function&);

    // methods:

    // IDK: лямбда-выражения - это поля, которые я понимаю, как методы, но правильно ли это?

    // DOES: считает по заданному x значение мат. функции
    const std::function<double(double)> calculate = [this] (double x) { return calc(x); };

    // DOES: считает по заданному x значение мат. функции
    inline double operator() (double x) const { return calc(x); }

    // DOES: считает по заданному x значение производной мат. функции
    const std::function<double(double)> differentiate = [this] (double x)
    {
        return (calc(x + Back_consts::delta_x) - calc(x - Back_consts::delta_x)) /
               (2 * Back_consts::delta_x);
        // IDK: delta_x не должно быть переменной, зависящей от масштаба?
    };

    // нету подходящего оператора для удобного получения производной :(
    //. DOES: считает по заданному x значение производной мат. функции
    // inline double operator[] (double x) const { return differentiate(x); }

    bool has_var () const;

    // DOES: возвращает мат. функцию в виде строки
    inline std::string get_func_str () const { return func_str; }

    function operator= (const function&);

    // ~methods:

  private:
    // vars:

    // MEANS: мат. функция в виде строки
    std::string func_str;

    // MEANS: вектор лексем от мат. функции
    std::vector<std::string> lexs;

    // MEANS: вектор лексем, записанный в обратной польской нотации
    std::vector<std::string> rev_pol;

    // ~vars:

    // methods:

    // DOES: проверяет мат. функцию в виде строки, на валидность
    // (правильное кол-во скобок, отсутствие лишних символов)
    // (правильное использование знаков, точек, цифр)
    bool is_str_valid () const;

    // DOES: проверяет вектор лексем от мат. функции на валидность
    // (правильное использование элементар. мат. функций (cos, sin, tan, exp, ln, um))
    // (отсутствие лишних переменных)
    bool is_lexs_valid () const;

    // DOES: переводит введеную строку в список вектор лексем от мат. функции
    std::vector<std::string> lexemes () const;

    // DOES: переводит вектор лексем, записанный в обратной польской нотации
    std::vector<std::string> reverse_polish () const;

    // DOES: считает по заданному x значение мат. функции
    double calc (double x) const;

    // ~methods:
};

}  // namespace Math_func

// DOES: выводит в поток мат. функцию в виде строки
// (только для принта, который будет удален позднее)
inline std::ostream& operator<< (std::ostream& os, Math_func::function func)
{
    return os << func.get_func_str();
}
