// header
#include "function_string.h"

// servant
#include "servant/constants.h"
using namespace Back_consts;
#include "servant/servant.h"
using namespace Back_serv;

namespace Backend {

bool function_string::is_str_valid() const
{
    string ex = spaces_deleted(expr);

    // проверка на пустую строку
    if (ex.empty())
    {
        throw std::invalid_argument("empty expression");
        return false;
    }

    // строка со всеми разрешенными символами
    const string calc_chars = ".1234567890+-*/^()cosinexptal";
    const string oper = "+-*/^";  // строка с операциями

    // проверка на скобочки
    if (count(ex.begin(), ex.end(), open_br) != count(ex.begin(), ex.end(), closed_br))
    {
        throw std::invalid_argument("number of brackets mismatch");
        return false;
    }

    // первый и последний символы не должны быть знаками или точками (кроме
    // минуса)
    if ((c_in_s(ex[0], oper + point) && ex[0] != minus) || c_in_s(ex[ex.size() - 1], oper + point))
    {
        throw std::invalid_argument("invalid syntax at the end or beginning of an expression");
        return false;
    }
    int count_brackets = 0;
    for (size_t i = 0; i < ex.size(); i++)
    {
        char c = ex[i];                   // текущий символ
        char prev_c = ' ', next_c = ' ';  // предыдущий и следующий символы
        if (i > 0)
            prev_c = ex[i - 1];
        if (i < (ex.size() - 1))
            next_c = ex[i + 1];

        // проверка на отсутсвие лишних символов
        if (!c_in_s(c, calc_chars))
        {
            if (isalpha(c))
            {
                throw std::invalid_argument("usage of extra variable");
                return false;
            }
            else
            {
                throw std::invalid_argument("unknown character");
                return false;
            }
        }
        // возле знака операции не должно быть других операций и точек
        // (если это не минус, так как он может быть унарный)
        if (c_in_s(c, oper) && c != minus)
        {
            if ((c_in_s(prev_c, oper + point) || c_in_s(next_c, oper + point)) &&
                next_c != minus)  // но после знака минус стоять может
            {
                throw std::invalid_argument("invalid syntax near sign or point");
                return false;
            }
            // минус не может стоять по обе стороны от знака
            else if (prev_c == minus && next_c == minus)
            {
                throw std::invalid_argument("invalid syntax near sign or point");
                return false;
            }
        }
        // возле точки должны быть только числа
        if (c == point)
        {
            if ((!isdigit(prev_c) || !isdigit(next_c)))
            {
                throw std::invalid_argument("invalid syntax of floating point numbers");
                return false;
            }
        }
        // считаем скобки
        else if (c == open_br)
        {
            count_brackets += 1;
            // и проверяем, что нету пустых
            if (next_c == closed_br)
            {
                throw std::invalid_argument("empty brackets");
                return false;
            }
        }
        else if (c == closed_br)
        {
            count_brackets -= 1;
            // случай, когда после очередной закрытой скобки - закрытых скобок оказывается больше
            if (count_brackets < 0)
            {
                throw std::invalid_argument("extra bracket");
                return false;
            }
        }
        // при строке из одной цифры, эта проверка не подходит
        else if (isdigit(c) && ex.size() > 1)
        {
            // вспомогательная функция для проверки рядом стоящего с числом
            // символа
            auto is_neighborhood_ok = [&ex, &oper] (char s, char bracket)
            { return (isdigit(s) || s == point || c_in_s(s, oper) || s == bracket); };
            bool is_left_ok = 1, is_right_ok = 1;

            if (i == 0)
            {
                // у числа справа может быть: число, точка, знак или closed_br
                is_right_ok = is_neighborhood_ok(next_c, closed_br);
                if (!is_right_ok)
                {
                    throw std::invalid_argument("invalid syntax near digit");
                    return false;
                }
            }
            else if (i == ex.size() - 1)
            {
                // у числа слева может быть: число, точка, знак или open_br
                is_left_ok = is_neighborhood_ok(prev_c, open_br);
                if (!is_left_ok)
                {
                    throw std::invalid_argument("invalid syntax near digit");
                    return false;
                }
            }
            else
            {
                // у числа справа может быть: число, точка, знак или closed_br
                is_right_ok = is_neighborhood_ok(next_c, closed_br);
                // у числа слева может быть: число, точка, знак или open_br
                is_left_ok = is_neighborhood_ok(prev_c, open_br);
                if (!is_left_ok || !is_right_ok)
                {
                    throw std::invalid_argument("invalid syntax near digit");
                    return false;
                }
            }
        }
    }
    return true;
}

bool function_string::is_lexs_valid() const
{
    // TRACE_FUNC;
    const string oper = "+-*/^";   // строка с операциями
    const string func = "sctelu";  // строка с функциями
    const vector<string> functions{"sin", "cos", "tan", "exp", "ln", "um"};
    for (size_t i = 0; i < lexs.size(); i++)
    {
        // string l = lexs[i];
        char l_c = transform_to_char(lexs[i]);
        char next_c = ' ';  // следующий символ
        if (i < (lexs.size() - 1))
            next_c = transform_to_char(lexs[i + 1]);
        // проверка использования постороннего имени
        if (isalpha(l_c) && !(is_float(lexs[i])))
        {
            if (c_in_s(l_c, func))
            {
                size_t j = func.find(l_c);
                if (lexs[i] != functions[j])
                {
                    if (lexs.size() > 1)
                    {
                        // если есть скобка, то это лишняя функция
                        if (lexs[i + 1] == "(")
                            throw std::invalid_argument("usage of wrong function name");
                        else
                            throw std::invalid_argument("usage of extra variable");
                        return false;
                    }
                    else
                    {
                        throw std::invalid_argument("usage of extra variable");
                        return false;
                    }
                }
                // ситуации рода: tan | x + tan
                else if (l_c != uminus && (lexs.size() < 4 || i == lexs.size() - 1))
                {
                    throw std::invalid_argument("wrong function usage");
                    return false;
                }
                // ситуация рода: x + tan + x
                else if (l_c != uminus && next_c != open_br)
                {
                    throw std::invalid_argument("wrong function usage");
                    return false;
                }
            }
            else if (l_c != var_x)  // если имя начинается не с разрешенной буквы и
                                    // не является x, то оно постороннее
            {
                throw std::invalid_argument("usage of extra variable");
                return false;
            }
            else if (l_c == var_x)
            {
                if (lexs.size() > 1 && i != lexs.size() - 1)
                {
                    // если есть скобка, то это лишняя функция
                    // ситуация рода: "x(x)"
                    if (lexs[i + 1] == "(")
                    {
                        throw std::invalid_argument("usage of wrong function name");
                        return false;
                    }
                }
                // ситуация рода: "xx"
                if (lexs[i].size() > 1)
                {
                    throw std::invalid_argument("usage of extra variable");
                    return false;
                }
            }
        }
    }
    return true;
}

}  // namespace Backend