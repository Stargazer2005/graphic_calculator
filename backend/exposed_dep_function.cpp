// header
#include "exposed_dep_function.h"

#include "function_string.h"
using Backend::function_string;
#include <iostream>

// servant
#include "servant/servant.h"
using Back_serv::c_in_s;

namespace Backend {

string exposed_dep_function (const vector<string>& all_funcs, string func, const size_t& func_index)
{
    std::cout << "exposed_dep_function_used" << std::endl;
    for (size_t i = 0; i < all_funcs.size(); i++)
    {
        std::cout << i << " " << all_funcs[i] << std::endl;
    }
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

                // предполагаемый номер функции (может оказаться не валидным)
                string est_number = "";

                // считываем предполагаемый номер (склеиваем все цифры кроме нуля, которые идут
                // после '_')
                for (size_t j = i + 2; j < func.size(); j++)
                    if (isdigit(func[j]) && func[j] != '0')
                        est_number += func[j];
                    else
                        break;

                // не считали, значит, это был либо 0, либо что-то иное
                if (est_number.empty())
                    throw invalid_argument("invalid expression number");

                // индекс предполагаемой функции
                size_t est_index = stoull(est_number) - 1;

                // считанный номер совпадает с текущей функцией
                // (скорее всего, можно хитро проверить через указатели)
                if (stoull(est_number) == func_number)
                    throw invalid_argument("expression self-usage");

                // номер сильно больше количества возможных к вводу функций
                if (stoull(est_number) > all_funcs.size())
                    throw invalid_argument("invalid expression number: " + est_number);

                // заменяем в func выражение y_n на то, что оно означает (и заключаем в скобки)
                Back_serv::replace(func, "y_" + est_number,
                                   '(' + all_funcs[stoull(est_number) - 1] + ')');

                // функция под этим индексом либо пустая, либо ещё не была вообще добавлена
                if (all_funcs[stoull(est_number) - 1].empty())
                    throw invalid_argument("expression y_" + est_number + " is empty or not exist");

                try
                {
                    std::cout << all_funcs[stoull(est_number) - 1] << " " << stoull(est_number) - 1
                              << std::endl;
                    function_string{exposed_dep_function(
                        all_funcs, all_funcs[stoull(est_number) - 1], stoull(est_number) - 1)};
                }
                catch (const std::exception& e)
                {
                    throw invalid_argument("invalid syntax in exposed");
                }
            }
        }
    }
    return func;
}

}  // namespace Backend