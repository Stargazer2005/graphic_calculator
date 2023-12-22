// header
#include "expose_func_str.h"

// std libs
#include <algorithm>
#include <stack>
#include <stdexcept>
using std::find;
using std::invalid_argument;
using std::stack;
using std::string;
using std::vector;

// servant
#include "servant/servant.h"
// #include "temp_help.h"
using namespace Back_serv;

// namespace Backend {

namespace Backend {

expose_func_str::expose_func_str(const vector<string>& _all_funcs_str, string& _func_str)
    : all_funcs_str{_all_funcs_str}, func_str{_func_str}
{
    expose_dep_func();
}

void expose_func_str::expose_dep_func()
{
    func_str = spaces_deleted(func_str);

    // ветор для записи зависимости, которая еще не дошла до выражения без y_n
    vector<size_t> unexposed_dependences;

    // если 'y' нету, то и делать ничего не надо
    if (!c_in_s('y', func_str))
        return;

    // уже проверили, что есть 'y', значит, должна быть запись y_n
    if (func_str.size() < 3)
        throw invalid_argument("invalid usage 'y'");

    // если 'y' стоит в конце
    if (func_str.rfind('y') > func_str.size() - 3)
    {
        if (c_in_s('_', func_str))
            throw invalid_argument("no number after 'y'");
        else
            throw invalid_argument("invalid usage 'y'");
    }

    // иначе проходимся по строке до тех пор, пока в ней не будут переделаны все y_n
    for (size_t i = 0; i < func_str.size(); i++)
    {
        // текущий символ
        auto ch = func_str[i];
        // следующий символ
        auto next_ch = func_str[i + 1];

        if (ch == 'y')
        {
            // должна быть запись y_
            if (next_ch != '_')
                throw invalid_argument("invalid usage 'y'");

            // считываемый предполагаемый номер функции (может оказаться не валидным)
            string readed_est_number = "";

            // считываем предполагаемый номер (склеиваем все цифры кроме нуля, которые идут
            // после '_')
            for (size_t j = i + 2; j < func_str.size(); j++)
                if (isdigit(func_str[j]) && func_str[j] != '0')
                    readed_est_number += func_str[j];
                else
                    break;

            // не считали, значит, это был либо 0, либо что-то иное
            if (readed_est_number.empty())
                throw invalid_argument("invalid function number");

            // индекс предполагаемой функции
            size_t est_number = stoull(readed_est_number);
            size_t est_index = est_number - 1;

            // номер сильно больше количества возможных к вводу функций
            if (est_number > all_funcs_str.size())
                throw invalid_argument("invalid function number: " + readed_est_number);

            if (elem_in_vec(est_number, unexposed_dependences))
                throw invalid_argument("self-usage or loop");

            // если новое раскрытие содержит y - значит, оно не полное
            if (c_in_s('y', all_funcs_str[est_index]))
                unexposed_dependences.push_back(est_number);
            else
                unexposed_dependences.clear();

            // если неполных раскрытий с этим номером не было, добавляем его в общий список
            if (!elem_in_vec(est_number, dependences))
                dependences.push_back(est_number);

            // заменяем в func_str выражение y_n на то, что оно означает (и заключаем в скобки)
            if (!all_funcs_str[est_index].empty())
                Back_serv::replace(func_str, "y_" + readed_est_number,
                                   '(' + all_funcs_str[est_index] + ')');
            else
                func_str = all_funcs_str[est_index];
        }
    }
}

}  // namespace Backend