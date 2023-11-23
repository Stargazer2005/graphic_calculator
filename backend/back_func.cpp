#include "back_func.h"
#include <algorithm>
#include <stdexcept>

// локальный класс (для взаимодействия с обратной польской записью)
// главная фишка - возможность обращения только к последнему элементу
class Stack
{
  public:
    Stack() : st{} {}

    void push (double c) { st.push_back(c); }

    void pop ()
    {
        if (st.size() != 0)
            st.pop_back();
        else
            throw range_error("Stack: empty, nothing to pop");
    }

    double last ()
    {
        if (st.size() != 0)
            return st[st.size() - 1];
        else
            throw range_error("Stack: empty, no last");
    }

    unsigned long long len () { return st.size(); }

    void print ()
    {
        for (auto i : st)
            cout << i << ", ";
        cout << endl;
    }

  private:
    vector<double> st;
};

// вспомогательная функция "содержится ли символ в строке?"
bool c_in_s (char c, const string& s) { return (s.find(c) != string::npos); }

bool checker (const string& expr)
{
    // строка со всеми разрешенными символами
    const string calc_chars = ".1234567890+-*/^sctelu()cosinexptal";
    const string oper = "+-*/^";  // строка с операциями

    // проверка на скобочки
    if (count(expr.begin(), expr.end(), '(') != count(expr.begin(), expr.end(), ')'))
    {
        throw invalid_argument("checker: number of brackets mismatch");
        return false;
    }
    // первый и последний символы не должны быть знаками или точками (кроме минуса)
    if ((c_in_s(expr[0], oper + '.') && expr[0] != '-') ||
        c_in_s(expr[expr.size() - 1], oper + '.'))
    {
        throw invalid_argument(
            "checker: expression begin or end with unclosed operation sign or point");
        return false;
    }
    int count_brackets = 0;
    for (unsigned long long i = 0; i < expr.size(); i++)
    {
        char c = expr[i];
        // проверка на отсутсвие лишних символов
        if (!c_in_s(c, calc_chars))
        {
            throw invalid_argument("checker: unknown character");
            return false;
        }
        // возле знака операции не должно быть других операций и точек
        // (елси это не минус, так как он может быть унарный)
        if (c_in_s(c, oper) && c != '-')
        {
            if ((c_in_s(expr[i - 1], oper + '.') || c_in_s(expr[i + 1], oper + '.')) &&
                (expr[i - 1] != '-' && expr[i + 1] != '-'))
            {
                throw invalid_argument(
                    "checker: multiple use of operation sign or point in one place");
                return false;
            }
        }
        // возле точки должны быть только числа
        if (c == '.')
        {
            if ((!isdigit(expr[i - 1]) || !isdigit(expr[i - 1])))
            {
                throw invalid_argument("checker: incorrect use of floating point numbers");
                return false;
            }
        }
        else if (c == '(')
            count_brackets += 1;
        else if (c == ')')
        {
            count_brackets -= 1;
            // после очередной закрытой скобки их оказывается больше
            if (count_brackets < 0)
            {
                throw invalid_argument("checker: extra bracket");
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
                    throw invalid_argument("checker: invalid syntax near number");
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
                    throw invalid_argument("checker: invalid syntax near number");
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
                    throw invalid_argument("checker: invalid syntax near number");
                    return false;
                }
            }
        }
    }
    return true;
}

// bool checker (const vector<string>& lexs) {}

// vector<char> transform_to_char (const vector<string>& strs)
// {
//     vector<char> res{};
//     for (auto s : strs)
//     {
//         if (s[0] >= '0' && s[0] <= '9')
//             res.push_back('n');
//         else
//             res.push_back(s[0]);
//     }
//     return res;
// }

// вспомогательная функция, переводящая строку (лексему) в символ
char transform_to_char (const string& s)
{
    if (s[0] >= '0' && s[0] <= '9')  // если число
        return ('n');                // заменяем на спец. символ
    else
        return (s[0]);  // первая буква отражает название функции
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
            l = stack.last();  // запоминаем только последний, так как операции унарны
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
                stack.push(0 - l);
                break;
            }
        }
        else if (c_in_s(curr, oper))
        {
            l = stack.last();
            stack.pop();
            p = stack.last();  // также запоминаем предпоследний
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