#pragma once

#include <functional>
// #include <iostream>

using std::string;
using std::vector;

namespace Backend {

struct Segment
{
    // Segment(double s, double v);
    double start;
    double stop;
};

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
    bool is_str_valid () const;
    // метод, проверяющий на корректность математические операции: отсутствие
    // деления на ноль, возведения отрицательного числа в дробную степень,
    // корректность использования функций (cos, sin, tan, exp, ln, um)
    bool is_lexs_valid () const;
    // метод, который переводит введеную пользователем строку в список лексем
    vector<string> lexemes () const;
    // метод, который переводит список лексем в обратную польскую запись
    vector<string> reverse_polish () const;

  public:
    graphic_string(string s);
    // метод, который по значению переменной x вычисляет значение y
    double calc (double x) const;

    // метод, считающий отрезки, где функция будет отрисовываться
    vector<Segment> domain_segments (double l_border, double r_border, double height,
                                     double precision) const;
    // std::function<double(double)> calked = [this] (double x) { return calc(x); }
};

}  // namespace Backend