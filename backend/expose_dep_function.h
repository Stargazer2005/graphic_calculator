#pragma once

// std libs
#include <string>
#include <vector>

namespace Backend {

// функция, которая преобразует все y_n в funcs, которые ими являются
class expose_dep_func_string
{
  public:
    expose_dep_func_string(const std::vector<std::string>& all_funcs, std::string& func);

    // expose_dep_func_string(std::vector<std::string>& all_funcs, const size_t& func_index);
    void set_funcs (const std::vector<std::string>& funcs) { all_funcs = funcs; }

    std::vector<size_t> get_dependences () { return dependences; }

  private:
    void expose_dep_func ();
    std::vector<std::string> all_funcs;
    std::string& func;
    std::vector<size_t> dependences;
};

}  // namespace Backend