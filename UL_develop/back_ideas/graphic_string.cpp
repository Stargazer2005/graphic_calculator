#include "backend.h"
#include "helpers.h"
#include <Tracer_lib/tracer.h>
#include <cmath>
#include <stack>

using std::stack;
using namespace Backend;

bool graphic_string::is_str_valid()
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
        else if ((isdigit(c) || c == var_x) && ex.size() > 1)
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
                    throw std::invalid_argument("invalid syntax near digit or variable");
                    return false;
                }
            }
            else if (i == ex.size() - 1)
            {
                // у числа слева может быть: число, точка, знак или open_br
                is_left_ok = is_neighborhood_ok(prev_c, open_br);
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

bool graphic_string::is_lexs_valid()
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
            else if (l_c != var_x)  // если имя начинается не с разрешенной буквы и
                                    // не является x, то оно постороннее
            {
                throw std::invalid_argument("usage of extra variable");
                return false;
            }
        }
    }
    return true;
}

vector<string> graphic_string::lexemes()
{
    if (!is_str_valid())
        return {};

    vector<string> res;
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
                res.push_back(s);
                s = "";
            }
            if (i == 0)
                res.push_back("um");
            else if (c_in_s(transform_to_char(res[res.size() - 1]), oper + "("))
                res.push_back("um");
            else
                res.push_back("-");
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
                res.push_back(s);
                s = "";
            }
            s = expr[i];
            res.push_back(s);
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
                res.push_back(s);
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
                    res.push_back(s);
                    s = "";
                }
                s += expr[i];
            }
        }
        }
    }
    if (s.size() > 0)
        res.push_back(s);

    // std::cout << "lexeme:";
    // print(res);
    return (res);
}

vector<string> graphic_string::reverse_polish()
{
    if (!is_lexs_valid())
        return {};

    // вектор, куда записывается итоговая запись
    vector<string> res;
    // стэк для хранения операций в правильном порядке
    stack<string> st_oper;
    st_oper.push("\0");

    // строка с функциями
    const string func = "sctelu";
    // строка с операциями
    const string oper = "+-*/^";

    for (auto& lex : lexs)
    {
        // символ, обозначающий текущую лексему
        char curr = transform_to_char(lex);
        // символ, обозначающий последний элемент в стеке с операциями
        char last = transform_to_char(st_oper.top());
        switch (curr)
        {
        case number:
        case var_x:
        {
            res.push_back(lex);
            if (st_oper.size() > 0 && last == uminus)
            {
                res.push_back(st_oper.top());
                st_oper.pop();
            }
            break;
        }

        case power:
        {
            while (last == power || c_in_s(last, func))
            {
                res.push_back(st_oper.top());
                st_oper.pop();
                last = transform_to_char(st_oper.top());
            }
            st_oper.push(lex);
            break;
        }

        case mul:
        case divi:
        {
            while (last == mul || last == divi || last == power || c_in_s(last, func))
            {
                res.push_back(st_oper.top());
                st_oper.pop();
                last = transform_to_char(st_oper.top());
            }
            st_oper.push(lex);
            break;
        }

        case plus:
        case minus:
        {
            while (c_in_s(last, oper) || c_in_s(last, func))
            {
                res.push_back(st_oper.top());
                st_oper.pop();
                last = transform_to_char(st_oper.top());
            }
            st_oper.push(lex);
            break;
        }

        case open_br:
        {
            st_oper.push(lex);
            break;
        }

        case closed_br:
        {
            while (true)
            {
                if (last == open_br)
                {
                    st_oper.pop();
                    break;
                }
                else
                {
                    res.push_back(st_oper.top());
                    st_oper.pop();
                }
                last = transform_to_char(st_oper.top());
            }
            break;
        }
        case c_sin:
        case c_cos:
        case c_tan:
        case c_exp:
        case c_ln:
        case uminus:
        {
            st_oper.push(lex);
            break;
        }
        default:
        {
            throw std::runtime_error("Oops");
            break;
        }
        }
    }

    // перевод оставшихся в стеке знаков в обратную польскую запись
    while (st_oper.top() != "\0")
    {
        res.push_back(st_oper.top());
        st_oper.pop();
    }

    // std::cout << "rev_pol:";
    // print(res);
    return res;
}

graphic_string::graphic_string(string s)
{
    // TRACE_FUNC;
    expr = s;
    lexs = lexemes();
    rev_pol = reverse_polish();
}

double graphic_string::calc(double x)
{
    const string oper = "+-*/^";   // строка с операциями
    const string func = "sctelu";  // строка с функциями
    // (да, унарный минус - тоже функция)

    stack<double> Stack;
    for (auto& lex : rev_pol)
    {
        double l;  // последний символ в стэке
        double p;  // предпоследний символ в стэке (последний после удаления l)
        char curr = transform_to_char(lex);  // текущий символ,
                                             // если брать вместо строки (для switch)
        if (c_in_s(curr, func))
        {
            l = Stack.top();  // запоминаем только последний (так как функции
                              // унарны)
            Stack.pop();
            switch (curr)
            {
            case c_sin:
                Stack.push(sin(l));
                break;
            case c_cos:
                Stack.push(cos(l));
                break;
            case c_tan:
                Stack.push(tan(l));
                break;
            case c_exp:
                Stack.push(exp(l));
                break;
            case c_ln:
                Stack.push(log(l));
                break;
            case uminus:
                Stack.push(-l);
                break;
            }
        }
        else if (c_in_s(curr, oper))
        {
            l = Stack.top();
            Stack.pop();
            p = Stack.top();  // также запоминаем предпоследний (так как операции
                              // бинарны)
            Stack.pop();
            switch (curr)
            {
            case plus:
                Stack.push(p + l);
                break;
            case minus:
                Stack.push(p - l);
                break;
            case mul:
                Stack.push(p * l);
                break;
            case divi:
                Stack.push(p / l);
                break;
            case power:
                Stack.push(pow(p, l));
                break;
            }
        }
        else
        {
            switch (curr)
            {
            case number:
                Stack.push(stod(lex));
                break;
            case var_x:
                Stack.push(x);
                break;
            default:
                throw std::runtime_error("Oops");
                break;
            }
        }
        if (std::isnan(Stack.top()) || std::isinf(Stack.top()))
            throw std::invalid_argument("violation of domain of definition of function");
    }  // последнее, что осталось в стэке после всех действий - и есть ответ
    return Stack.top();
}
