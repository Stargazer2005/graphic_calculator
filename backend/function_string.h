#pragma once

// std libs
#include <functional>
using std::function;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <stack>
using std::stack;

namespace Backend {

class function_string
{
  public:
    function_string(string func);

    // methods:

    // лямбда-функция, считающая по заданному x значение функции
    function<double(double)> calculate = [this] (double x) { return calc(x); };

  private:
    // variables:

    // значение строки
    string func;
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
};

}  // namespace Backend
