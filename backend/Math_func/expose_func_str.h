#pragma once

// std libs
#include <string>
#include <vector>

namespace Math_func {

// MEMO: функция, которая преобразует все y_n в мат. функции в виде строк
class expose_func_str
{
  public:
    expose_func_str(const std::vector<std::string>& _all_funcs_str, std::string& _func_str);

    // methods

    std::vector<size_t> get_dependences () { return dependences; }

  private:
    // methods

    // MEMO: функция, раскрывающая все зависимости
    void expose_dep_func ();

    // vars

    // MEMO: список со всеми мат. функциями в виде строк
    std::vector<std::string> all_funcs_str;
    // MEMO: текущая мат. функция в виде строки
    std::string& func_str;
    // MEMO: общий список всех номеров мат. функций, которые зависят от текущей
    std::vector<size_t> dependences;
};

}  // namespace Math_func