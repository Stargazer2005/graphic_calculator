#include "back_func.h"
#include "back_helpers.h"
#include <algorithm>
#include <stdexcept>

bool checker (const string& expr)
{
    // строка со всеми разрешенными символами
    const string calc_chars = ".1234567890+-*/^()cosinexptal";
    const string oper = "+-*/^";  // строка с операциями

    // проверка на скобочки
    if (count(expr.begin(), expr.end(), '(') != count(expr.begin(), expr.end(), ')'))
    {
        throw invalid_argument("number of brackets mismatch");
        return false;
    }
    // первый и последний символы не должны быть знаками или точками (кроме минуса)
    if ((c_in_s(expr[0], oper + '.') && expr[0] != '-') ||
        c_in_s(expr[expr.size() - 1], oper + '.'))
    {
        throw invalid_argument("expression begin or end with unclosed operation sign or point");
        return false;
    }
    int count_brackets = 0;
    for (size_t i = 0; i < expr.size(); i++)
    {
        char c = expr[i];
        // проверка на отсутсвие лишних символов
        if (!c_in_s(c, calc_chars))
        {
            throw invalid_argument("unknown character");
            return false;
        }
        // возле знака операции не должно быть других операций и точек
        // (если это не минус, так как он может быть унарный)
        if (c_in_s(c, oper) && c != '-')
        {
            if ((c_in_s(expr[i - 1], oper + '.') || c_in_s(expr[i + 1], oper + '.')) &&
                (expr[i - 1] != '-' && expr[i + 1] != '-'))
            {
                throw invalid_argument("multiple use of operation sign or point in one place");
                return false;
            }
        }
        // возле точки должны быть только числа
        if (c == '.')
        {
            if ((!isdigit(expr[i - 1]) || !isdigit(expr[i - 1])))
            {
                throw invalid_argument("incorrect use of floating point numbers");
                return false;
            }
        }
        else if (c == '(')
        {
            count_brackets += 1;
            if (expr[i + 1] == ')')
            {
                throw invalid_argument("empty brackets");
                return false;
            }
        }
        else if (c == ')')
        {
            count_brackets -= 1;
            // после очередной закрытой скобки их оказывается больше
            if (count_brackets < 0)
            {
                throw invalid_argument("extra bracket");
                return false;
            }
        }
        else if (isdigit(c))
        {
            // вспомогательная функция для проверки рядом стоящего с числом символа
            auto is_neighborhood_ok = [&expr, &oper] (char s, char bracket)
            { return (isdigit(s) || s == '.' || c_in_s(s, oper) || s == bracket); };
            bool is_left_ok = 1, is_right_ok = 1;

            if (i == 0)
            {
                // у числа справа может быть: число, точка, знак или ')'
                is_right_ok = is_neighborhood_ok(expr[i + 1], ')');
                // cout << expr[i - 1] << expr[i + 1] << " " << is_left_ok << is_right_ok << endl;
                if (!is_right_ok)
                {
                    throw invalid_argument("invalid syntax near number");
                    return false;
                }
            }
            else if (i == expr.size() - 1)
            {
                // у числа слева может быть: число, точка, знак или '('
                is_left_ok = is_neighborhood_ok(expr[i - 1], '(');
                // cout << expr[i - 1] << expr[i + 1] << " " << is_left_ok << is_right_ok << endl;
                if (!is_left_ok)
                {
                    throw invalid_argument("invalid syntax near number");
                    return false;
                }
            }
            else
            {
                // у числа справа может быть: число, точка, знак или ')'
                is_right_ok = is_neighborhood_ok(expr[i + 1], ')');
                // у числа слева может быть: число, точка, знак или '('
                is_left_ok = is_neighborhood_ok(expr[i - 1], '(');
                // cout << expr[i - 1] << expr[i + 1] << " " << is_left_ok << is_right_ok << endl;
                if (!is_left_ok || !is_right_ok)
                {
                    throw invalid_argument("invalid syntax near number");
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
        string l = lexs[i];
        char l_c = transform_to_char(lexs[i]);
        // проверка деления на ноль
        if (l_c == '/' && transform_to_char(lexs[i + 1]) == '0')
        {
            throw invalid_argument("zero division");
            return false;
        }
        // проверка возведения отрицательного числа в дробную степень
        if (l_c == '^' && stod(lexs[i - 1]) < 0 && c_in_s('.', lexs[i + 1]))
        {
            throw invalid_argument("raising a negative number to a floating point power");
            return false;
        }
        // проверка использования постороннего имени
        if (c_in_s(l_c, func))
        {
            size_t j = func.find(l_c);
            if (lexs[i] != functions[j])
            {
                if (lexs[i + 1] == "(")
                    throw invalid_argument("usage of wrong function name");
                else
                    throw invalid_argument("usage of extra variable");
                return false;
            }
        }
    }
}

vector<string> lexeme (const string& expr)
{
    // if (checker(expr))
    // {
    //   throw std::runtime_error("Incorrect input");
    // }
    vector<string> lexs;
    string s;
    const string oper = "+-*/^";
    for (size_t i = 0; i < expr.size(); i++)
    {
        switch (expr[i])
        {
        case '-':
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
        case '(':
        case ')':
        case '+':
        case '*':
        case '/':
        case '^':
        {
            if (s.size() > 0)
            {
                lexs.push_back(s);
                s = "";
            }
            s = (1, expr[i]);
            lexs.push_back(s);
            s = "";
            break;
        }
        case '.':
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
}

bool is_float (string str)
{
    std::istringstream iss(str);
    float f;
    iss >> noskipws >> f;
    return iss.eof() && !iss.fail();
}

vector<string> reverse_polish (const vector<string>& lex)
{
    vector<string> res;
    vector<string> oper;
    const string func = "sctel";

    auto last = [] (vector<string> v) { return (v.size() > 0) ? v[v.size() - 1] : ""; };

    auto is_func = [&func, last] (string s) { return c_in_s(transform_to_char(s), func); };

    for (auto str : lex)
    {
        if (str == "um")
            oper.push_back(str);

        else if (is_float(str))
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
        else if (str == "x")
            oper.push_back(str);
    }
    while (oper.size() > 0)
    {
        res.push_back(last(oper));
        oper.pop_back();
    }
    return res;
}

double calc (const vector<string>& rev_pol, double x)
{
    const string oper = "+-*/^";   // строка с операциями
    const string func = "sctelu";  // строка с функциями
                                   // (да, унарный минус - тоже функция)

    Stack stack;
    for (auto lex : rev_pol)
    {
        // stack.print();
        double l;  // последний символ в стэке
        double p;  // предпоследний символ в стэке (последний после удаления l)
        char curr = transform_to_char(lex);  // текущий символ,
                                             // если брать вместо строки (для switch)
        if (c_in_s(curr, func))
        {
            l = stack.last();  // запоминаем только последний (так как функции унарны)
            stack.pop();
            switch (curr)
            {
            case 's':
                stack.push(sin(l));
                break;
            case 'c':
                stack.push(cos(l));
                break;
            case 't':
                stack.push(tan(l));
                break;
            case 'e':
                stack.push(exp(l));
                break;
            case 'l':
                stack.push(log(l));
                break;
            case 'u':
                stack.push(-l);
                break;
            }
        }
        else if (c_in_s(curr, oper))
        {
            l = stack.last();
            stack.pop();
            p = stack.last();  // также запоминаем предпоследний (так как операции бинарны)
            stack.pop();
            switch (curr)
            {
            case '+':
                stack.push(p + l);
                break;
            case '-':
                stack.push(p - l);
                break;
            case '*':
                stack.push(p * l);
                break;
            case '/':
                stack.push(p / l);
                break;
            case '^':
                stack.push(pow(p, l));
                break;
            }
        }
        else
        {
            switch (curr)
            {
            case 'n':
                stack.push(stod(lex));
                break;
            case 'x':
                stack.push(x);
                break;
            default:
                throw runtime_error("Oops");
                break;
            }
        }
    }
    return stack.last();  // последнее, что осталось в стэке после всех действий - и есть ответ
}