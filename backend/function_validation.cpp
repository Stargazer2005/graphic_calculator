// header
#include "function.h"

// std libs
#include <stdexcept>
using std::string;
using std::vector;

// servant
#include "servant/servant.h"
using namespace Back_serv;
using namespace Back_consts;
#include "temp_help.h"

namespace Backend {

bool function::is_str_valid() const
{
    // проверка на пустую строку
    if (func_str == "")
        throw std::invalid_argument("empty expression");

    // строка со всеми разрешенными символами
    const string calc_chars = ".1234567890+-*/^()cosinexptal";
    const string oper_chars = "+-*/^";  // строка с операциями

    // проверка на скобочки
    if (count(func_str.begin(), func_str.end(), open_br) !=
        count(func_str.begin(), func_str.end(), closed_br))
        throw std::invalid_argument("brackets number mismatch");

    // первый и последний символы не должны быть знаками или точками (кроме
    // минуса)
    if ((c_in_s(func_str[0], oper_chars + point) && func_str[0] != minus) ||
        c_in_s(func_str[func_str.size() - 1], oper_chars + point))
        throw std::invalid_argument("invalid syntax at edges");

    int count_brackets = 0;
    for (size_t i = 0; i < func_str.size(); i++)
    {
        char c = func_str[i];             // текущий символ
        char prev_c = ' ', next_c = ' ';  // предыдущий и следующий символы
        if (i > 0)
            prev_c = func_str[i - 1];
        if (i < (func_str.size() - 1))
            next_c = func_str[i + 1];

        // проверка на отсутсвие лишних символов
        if (!c_in_s(c, calc_chars))
        {
            if (isalpha(c))
                throw std::invalid_argument("extra variable '" + string{c} + "'");
            else
                throw std::invalid_argument("unknown character '" + string{c} + "'");
        }
        // возле знака операции не должно быть других операций и точек, а также открытой скобки
        // слева и закрытой скобки справа (если это не минус, так как он может быть унарный)
        if (c_in_s(c, oper_chars) && c != minus)
        {
            if ((c_in_s(prev_c, oper_chars + point) || c_in_s(next_c, oper_chars + point)) &&
                next_c != minus)  // но после знака минус стоять может
                throw std::invalid_argument("invalid syntax near sign");

            // минус не может стоять по обе стороны от знака
            else if (prev_c == minus && next_c == minus)
                throw std::invalid_argument("invalid syntax near sign");

            // проверка на правильность скобок возле знака
            if (prev_c == open_br || next_c == closed_br)
                throw std::invalid_argument("invalid brackets near sign");
        }
        // возле точки должны быть только числа
        if (c == point)
        {
            if ((!isdigit(prev_c) || !isdigit(next_c)))
                throw std::invalid_argument("invalid syntax near point");
        }
        // считаем скобки
        else if (c == open_br)
        {
            count_brackets += 1;
            // и проверяем, что нету пустых
            if (next_c == closed_br)
                throw std::invalid_argument("empty brackets");
        }
        else if (c == closed_br)
        {
            count_brackets -= 1;
            // случай, когда после очередной закрытой скобки - закрытых скобок оказывается больше
            if (count_brackets < 0)
                throw std::invalid_argument("extra bracket");
        }
        // (при строке из одной цифры, эта проверка не подходит)
        else if (isdigit(c) && func_str.size() > 1)
        {  // возле числа должен стоять либо знак, либо точка, либо скобка

            // вспомогательная функция для проверки рядом стоящего с числом
            // символа
            auto is_neighborhood_ok = [&oper_chars] (char s, char bracket)
            { return (isdigit(s) || s == point || c_in_s(s, oper_chars) || s == bracket); };
            bool is_left_ok = 1, is_right_ok = 1;

            if (i == 0)
            {
                // у числа справа может быть: число, точка, знак или closed_br
                is_right_ok = is_neighborhood_ok(next_c, closed_br);
                if (!is_right_ok)
                    throw std::invalid_argument("invalid syntax near digit");
            }
            else if (i == func_str.size() - 1)
            {
                // у числа слева может быть: число, точка, знак или open_br
                is_left_ok = is_neighborhood_ok(prev_c, open_br);
                if (!is_left_ok)
                    throw std::invalid_argument("invalid syntax near digit");
            }
            else
            {
                // у числа справа может быть: число, точка, знак или closed_br
                is_right_ok = is_neighborhood_ok(next_c, closed_br);
                // у числа слева может быть: число, точка, знак или open_br
                is_left_ok = is_neighborhood_ok(prev_c, open_br);
                if (!is_left_ok || !is_right_ok)
                    throw std::invalid_argument("invalid syntax near digit");
            }
        }
    }
    return true;
}

bool function::is_lexs_valid() const
{
    const string oper_chars = "+-*/^";   // строка с операциями
    const string func_chars = "sctelu";  // строка с функциями
    const vector<string> functions{"sin", "cos", "tan", "exp", "ln", "um"};
    for (size_t i = 0; i < lexs.size(); i++)
    {
        // string l = lexs[i];
        char l_c = s_to_c(lexs[i]);
        char next_c = ' ';  // следующий символ
        if (i < (lexs.size() - 1))
            next_c = s_to_c(lexs[i + 1]);
        // проверка использования постороннего имени
        if (isalpha(l_c) && !(is_double(lexs[i])))
        {
            if (c_in_s(l_c, func_chars))
            {
                size_t j = func_chars.find(l_c);
                if (lexs[i] != functions[j])
                {
                    if (lexs.size() > 1)
                    {
                        // если есть скобка, то это лишняя функция
                        if (lexs[i + 1] == "(")
                            throw std::invalid_argument("invalid function name '" + lexs[i] + "'");
                        else
                            throw std::invalid_argument("extra variable '" + lexs[i] + "'");
                    }
                    else
                        throw std::invalid_argument("extra variable '" + lexs[i] + "'");
                }
                // ситуации рода: tan | x + tan
                else if (l_c != uminus && (lexs.size() < 4 || i == lexs.size() - 1))
                    throw std::invalid_argument("wrong function usage");

                // ситуация рода: x + tan + x
                else if (l_c != uminus && next_c != open_br)
                    throw std::invalid_argument("wrong function usage");
            }
            else if (l_c != var_x)  // если имя начинается не с разрешенной буквы и
                                    // не является x, то оно постороннее
                throw std::invalid_argument("extra variable '" + string{l_c} + "'");

            else if (l_c == var_x)
            {
                if (lexs.size() > 1 && i != lexs.size() - 1)
                {
                    // если есть скобка, то это лишняя функция
                    // ситуация рода: "x(x)"
                    if (lexs[i + 1] == "(")
                        throw std::invalid_argument("invalid function name '" + lexs[i] + "'");
                }
                // ситуация рода: "xx"
                if (lexs[i].size() > 1)
                    throw std::invalid_argument("extra variable '" + lexs[i] + "'");
            }
        }
    }
    return true;
}

}  // namespace Backend