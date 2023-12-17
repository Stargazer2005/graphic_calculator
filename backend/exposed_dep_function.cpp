// header
#include "exposed_dep_function.h"

#include <vector>
using std::vector;
#include <algorithm>
using std::find;

#include "function_string.h"
using Backend::function_string;
#include <iostream>

// servant
#include "servant/servant.h"
using Back_serv::c_in_s;

template <typename T> bool n_in_vec (const vector<T>& vec, T n)
{
    return find(vec.begin(), vec.end(), n) != vec.end();
}

namespace Backend {

string exposed_dep_function (const vector<string>& all_funcs, string func, const size_t& func_index)
{
    static vector<size_t> prev_indexes;
    std::cout << "func_now: " << func << std::endl;
    // std::cout << "exposed_dep_function_used" << std::endl;
    // for (size_t i = 0; i < all_funcs.size(); i++)
    // {
    //     std::cout << i << " " << all_funcs[i] << std::endl;
    // }

    size_t func_number = func_index + 1;
    // если 'y' нету, то и делать ничего не надо
    if (!c_in_s('y', func))
        return func;

    // уже проверили, что есть 'y', значит, должна быть запись y_n
    if (func.size() < 3)
        throw invalid_argument("invalid usage 'y'");

    // если 'y' стоит в конце
    if (func.rfind('y') > func.size() - 3)
    {
        if (c_in_s('_', func))
            throw invalid_argument("no number after 'y'");
        else
            throw invalid_argument("invalid usage 'y'");
    }

    // иначе проходимся по строке до тех пор, пока в ней не будут переделаны все y_n
    while (c_in_s('y', func))
    {
        for (size_t i = 0; i < func.size(); i++)
        {
            // текущий символ
            auto ch = func[i];
            // следующий символ
            auto next_ch = func[i + 1];

            if (ch == 'y')
            {
                // должна быть запись y_
                if (next_ch != '_')
                    throw invalid_argument("invalid usage 'y'");

                // считываемый предполагаемый номер функции (может оказаться не валидным)
                string readed_est_number = "";

                // считываем предполагаемый номер (склеиваем все цифры кроме нуля, которые идут
                // после '_')
                for (size_t j = i + 2; j < func.size(); j++)
                    if (isdigit(func[j]) && func[j] != '0')
                        readed_est_number += func[j];
                    else
                        break;

                // не считали, значит, это был либо 0, либо что-то иное
                if (readed_est_number.empty())
                    throw invalid_argument("invalid expression number");

                // индекс предполагаемой функции
                size_t est_number = stoull(readed_est_number);
                size_t est_index = est_number - 1;

                // номер сильно больше количества возможных к вводу функций
                if (est_number > all_funcs.size())
                    throw invalid_argument("invalid expression number: " + readed_est_number);

                // заменяем в func выражение y_n на то, что оно означает (и заключаем в скобки)
                Back_serv::replace(func, "y_" + readed_est_number,
                                   '(' + all_funcs[est_index] + ')');

                // функция под этим индексом либо пустая, либо ещё не была вообще добавлена
                if (all_funcs[est_index].empty())
                    throw invalid_argument("expression y_" + readed_est_number +
                                           " is empty or not exist");

                if (n_in_vec(prev_indexes, est_index))
                    throw invalid_argument("self-usage or loop");

                prev_indexes.push_back(est_index);
                std::cout << "prev_indexes: ";
                for (size_t i = 0; i < prev_indexes.size(); i++)
                {
                    std::cout << prev_indexes[i] << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    return func;
}

}  // namespace Backend