#pragma once

// std libs
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <stdexcept>
using std::invalid_argument;

namespace Backend {

// функция, которая преобразует все y_n в то, что может принять function_string
// "раскрытая зависимая функция" (зависит от функций, введённых в других полях)
string exposed_dep_function (vector<string> all_funcs, string func, size_t func_number);

}  // namespace Backend