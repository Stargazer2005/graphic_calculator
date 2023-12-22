#pragma once

// std libs
#include <string>
#include <vector>

namespace Backend {

// функция, которая преобразует все y_n в func'и, которые ими являются
class expose_func_str
{
  public:
    expose_func_str(const std::vector<std::string>& _all_funcs_str, std::string& _func_str);

    // methods

    std::vector<size_t> get_dependences () { return dependences; }

    bool is_looped () const { return loop_checked; }

  private:
    // methods

    // функция, раскрывающая все зависимости
    void expose_dep_func ();

    // variables

    // список со всеми func'ами
    std::vector<std::string> all_funcs_str;
    // строка текущей func
    std::string& func_str;
    // общий список всех n func'ов, которые зависят от текущей
    std::vector<size_t> dependences;

    bool loop_checked{false};
};

}  // namespace Backend