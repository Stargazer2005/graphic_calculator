#pragma once

// std libs
#include <functional>
#include <string>
#include <vector>

// Graph_lib
#include <Graph_lib/Graph.h>

using std::string;
using std::vector;

namespace Backend {

class function_string
{
  private:
    // variables:

    // значение строки
    string expr;
    // обратная польская нотация
    vector<string> rev_pol;
    // список лексем
    vector<string> lexs;

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
    vector<string> lexemes () const;
    // метод, который переводит список лексем в обратную польскую запись
    vector<string> reverse_polish () const;
    // метод, который по значению переменной x вычисляет значение y
    double calc (double x) const;

  public:
    function_string(string s);

    // methods:

    // лямбда-функция, считающая по заданному x значение функции
    std::function<double(double)> calculate = [this] (double x) { return calc(x); };
};

}  // namespace Backend
