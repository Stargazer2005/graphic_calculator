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
void expose_dep_function (const vector<string>&, string&, const size_t&);

}  // namespace Backend