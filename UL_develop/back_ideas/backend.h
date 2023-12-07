#pragma once

#include <functional>
// #include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Backend {

class graphic_string
{
  private:
    // variables:

    // значение строки
    string expr;
    // обратная польская нотация
    vector<string> rev_pol;
    // список лексем
    vector<string> lexs;

    // functions:

    // метод, проверяющий на корректность скобок, отсутствие лишних символов,
    // правильное использование знаков и чисел возле них, правильное
    // использование точки, правильное использование цифр
    bool is_str_valid ();
    // метод, проверяющий на корректность математические операции: отсутствие
    // деления на ноль, возведения отрицательного числа в дробную степень,
    // корректность использования функций (cos, sin, tan, exp, ln, um)
    bool is_lexs_valid ();
    // метод, который переводит введеную пользователем строку в список лексем
    vector<string> lexemes ();
    // метод, который переводит список лексем в обратную польскую запись
    vector<string> reverse_polish ();

  public:
    graphic_string(string s);
    // метод, который по значению переменной x вычисляет значение y
    double calc (double x);
    // std::function<double(double)> calked = [this] (double x) { return calc(x); }
};

}  // namespace Backend