#pragma once

#include <functional>

using std::string;
using std::vector;

namespace Backend {

struct Segment
{
    double start;
    double stop;
};

class function_string
{
  public:
    // variables:

    // значение строки
    string expr;
    // обратная польская нотация
    vector<string> rev_pol;
    // список лексем
    vector<string> lexs;
    // список отрезков, на котором рисуем функцию
    vector<Segment> segments;

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

    // конструктор по std::string
    function_string(string s);
    // метод, который по значению переменной x вычисляет значение y
    double calc (double x) const;
    // метод-сеттер, формирующий отрезки, на которых определена функция
    void set_segments (int max_x, int max_y, int scale);
    // метод-геттер, возвращающий отрезки, на которых определена функция
    vector<Segment> get_segments ();
    // лямбда-функция, считающая по заданному x значение функции
    std::function<double(double)> calculate = [this] (double x) { return calc(x); };
    // имеет ли строка в себе переменную
    bool has_var;
    // метод, считающий отрезки, где функция будет отрисовываться
    vector<Segment> domain_segments (double l_border, double r_border, double height,
                                     double precision) const;
};
}  // namespace Backend
