#include "back_func.h"
#include "back_helpers.h"

using std::string;
using std::vector;

bool checker (const string& expression)
{
    string expr = spaces_deleted(expression);

    // проверка на пустую строку
    if (expr.empty())
    {
        throw std::invalid_argument("empty expression");
        return false;
    }

    // строка со всеми разрешенными символами
    const string calc_chars = ".1234567890+-*/^()cosinexptal";
    const string oper = "+-*/^";  // строка с операциями

    // проверка на скобочки
    if (count(expr.begin(), expr.end(), open_br) != count(expr.begin(), expr.end(), closed_br))
    {
        throw std::invalid_argument("number of brackets mismatch");
        return false;
    }
    // первый и последний символы не должны быть знаками или точками (кроме
    // минуса)
    if ((c_in_s(expr[0], oper + point) && expr[0] != minus) ||
        c_in_s(expr[expr.size() - 1], oper + point))
    {
        throw std::invalid_argument("invalid syntax at the end or beginning of an expression");
        return false;
    }
    int count_brackets = 0;
    for (size_t i = 0; i < expr.size(); i++)
    {
        char c = expr[i];                 // текущий символ
        char prev_c = ' ', next_c = ' ';  // предыдущий и следующий символы
        if (i > 0)
            prev_c = expr[i - 1];
        if (i < (expr.size() - 1))
            next_c = expr[i + 1];

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
        else if ((isdigit(c) || c == 'x') && expr.size() > 1)
        {
            // вспомогательная функция для проверки рядом стоящего с числом
            // символа
            auto is_neighborhood_ok = [&expr, &oper] (char s, char bracket)
            { return (isdigit(s) || s == point || c_in_s(s, oper) || s == bracket); };
            bool is_left_ok = 1, is_right_ok = 1;

            if (i == 0)
            {
                // у числа справа может быть: число, точка, знак или closed_br
                is_right_ok = is_neighborhood_ok(next_c, closed_br);
                // cout << expr[i - 1] << expr[i + 1] << " " << is_left_ok <<
                // is_right_ok << endl;
                if (!is_right_ok)
                {
                    throw std::invalid_argument("invalid syntax near digit or variable");
                    return false;
                }
            }
            else if (i == expr.size() - 1)
            {
                // у числа слева может быть: число, точка, знак или open_br
                is_left_ok = is_neighborhood_ok(prev_c, open_br);
                // cout << expr[i - 1] << expr[i + 1] << " " << is_left_ok <<
                // is_right_ok << endl;
                if (!is_left_ok)
                {
                    throw std::invalid_argument("invalid syntax near digit or variable");
                    return false;
                }
            }
            else
            {
                // у числа справа может быть: число, точка, знак или closed_br
                is_right_ok = is_neighborhood_ok(next_c, closed_br);
                // у числа слева может быть: число, точка, знак или open_br
                is_left_ok = is_neighborhood_ok(prev_c, open_br);
                // cout << expr[i - 1] << expr[i + 1] << " " << is_left_ok <<
                // is_right_ok << endl;
                if (!is_left_ok || !is_right_ok)
                {
                    throw std::invalid_argument("invalid syntax near digit or variable");
                    return false;
                }
            }
        }
    }
    return true;
}

bool checker (const vector<string>& lexs)
{
    const string func = "sctelu";  // строка с функциями
                                   // (да, унарный минус - тоже функция)
    const vector<string> functions{"sin", "cos", "tan", "exp", "ln", "um"};
    for (size_t i = 0; i < lexs.size(); i++)
    {
        // string l = lexs[i];
        char l_c = transform_to_char(lexs[i]);
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
                        throw std::invalid_argument("usage of extra variable");
                    return false;
                }
            }
            else if (l_c != 'x')  // если имя начинается не с разрешенной буквы и
                                  // не является x, то оно постороннее
            {
                throw std::invalid_argument("usage of extra variable");
                return false;
            }
        }
        // cout << i << " name test: ok" << endl;
    }
    return true;
}

vector<string> lexeme (const string& expr)
{
    if (!checker(expr))
        return {};

    vector<string> lexs;
    string s;
    const string oper = "+-*/^u";
    for (size_t i = 0; i < expr.size(); i++)
    {
        switch (expr[i])
        {
        case minus:
        {
            if (s.size() > 0)
            {
                lexs.push_back(s);
                s = "";
            }
            if (i == 0)
                lexs.push_back("um");
            else if (c_in_s(transform_to_char(lexs[lexs.size() - 1]), oper + "("))
                lexs.push_back("um");
            else
                lexs.push_back("-");
            break;
        }
        case open_br:
        case closed_br:
        case plus:
        case mul:
        case divi:
        case power:
        {
            if (s.size() > 0)
            {
                lexs.push_back(s);
                s = "";
            }
            s = expr[i];
            lexs.push_back(s);
            s = "";
            break;
        }
        case point:
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            if (s.size() > 0 && !isdigit(s[0]))
            {
                lexs.push_back(s);
                s = "";
            }
            s += expr[i];
            break;
        }
        default:
        {
            if (!isblank(expr[i]))
            {
                if (s.size() > 0 && isdigit(s[0]))
                {
                    lexs.push_back(s);
                    s = "";
                }
                s += expr[i];
            }
        }
        }
    }
    if (s.size() > 0)
        lexs.push_back(s);
    // cout << "lexeme:";
    // print(lexs);
    return (lexs);
}

vector<string> reverse_polish (const vector<string>& lexs)
{
    if (!checker(lexs))
        return {};

    vector<string> res;
    vector<string> oper;
    const string func = "sctelu";

    auto last = [] (vector<string> v) { return (v.size() > 0) ? v[v.size() - 1] : ""; };

    auto is_func = [&func, last] (string s) { return c_in_s(transform_to_char(s), func); };

    for (auto& str : lexs)
    {
        if (str == "um")
            oper.push_back(str);

        else if (is_float(str) || str == "x")
        {
            res.push_back(str);
            if (oper.size() > 0 && last(oper) == "um")
            {
                res.push_back(last(oper));
                oper.pop_back();
            }
        }

        else if (str == "^")
        {
            while (last(oper) == "^" || is_func(last(oper)))
            {
                res.push_back(last(oper));
                oper.pop_back();
            }
            oper.push_back(str);
        }

        else if (str == "*" || str == "/")
        {
            while (last(oper) == "*" || last(oper) == "/" || last(oper) == "^" ||
                   is_func(last(oper)))
            {
                res.push_back(last(oper));
                oper.pop_back();
            }
            oper.push_back(str);
        }

        else if (str == "+" || str == "-")
        {
            while (last(oper) == "*" || last(oper) == "/" || last(oper) == "^" ||
                   last(oper) == "+" || last(oper) == "-" || is_func(last(oper)))
            {
                res.push_back(last(oper));
                oper.pop_back();
            }
            oper.push_back(str);
        }

        else if (str == "(")
            oper.push_back(str);

        else if (str == ")")
        {
            while (true)
            {
                if (last(oper) == "(")
                {
                    oper.pop_back();
                    break;
                }
                else
                {
                    res.push_back(last(oper));
                    oper.pop_back();
                }
            }
        }
        else if (is_func(str))
            oper.push_back(str);
    }
    while (oper.size() > 0)
    {
        res.push_back(last(oper));
        oper.pop_back();
    }
    // cout << "rev_pol:";
    // print(res);
    return res;
}

double calc (const vector<string>& rev_pol, double x)
{
    const string oper = "+-*/^";   // строка с операциями
    const string func = "sctelu";  // строка с функциями
    // (да, унарный минус - тоже функция)

    Stack stack;
    for (auto& lex : rev_pol)
    {
        // stack.print();
        double l;  // последний символ в стэке
        double p;  // предпоследний символ в стэке (последний после удаления l)
        char curr = transform_to_char(lex);  // текущий символ,
                                             // если брать вместо строки (для switch)
        if (c_in_s(curr, func))
        {
            l = stack.last();  // запоминаем только последний (так как функции
                               // унарны)
            stack.pop();
            switch (curr)
            {
            case c_sin:
                stack.push(sin(l));
                break;
            case c_cos:
                stack.push(cos(l));
                break;
            case c_tan:
                stack.push(tan(l));
                break;
            case c_exp:
                stack.push(exp(l));
                break;
            case c_ln:
                stack.push(log(l));
                break;
            case uminus:
                stack.push(-l);
                break;
            }
        }
        else if (c_in_s(curr, oper))
        {
            l = stack.last();
            stack.pop();
            p = stack.last();  // также запоминаем предпоследний (так как операции
                               // бинарны)
            stack.pop();
            switch (curr)
            {
            case plus:
                stack.push(p + l);
                break;
            case minus:
                stack.push(p - l);
                break;
            case mul:
                stack.push(p * l);
                break;
            case divi:
                stack.push(p / l);
                break;
            case power:
                stack.push(pow(p, l));
                break;
            }
        }
        else
        {
            switch (curr)
            {
            case number:
                stack.push(stod(lex));
                break;
            case 'x':
                stack.push(x);
                break;
            default:
                throw std::runtime_error("Oops");
                break;
            }
        }
        if (std::isnan(stack.last()) || std::isinf(stack.last()))
            throw std::invalid_argument("violation of domain of definition of function");
    }  // последнее, что осталось в стэке после всех действий - и есть ответ
    return stack.last();
}