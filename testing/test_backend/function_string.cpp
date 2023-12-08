#include "Tracer_lib/tracer.h"
#include "backend.h"
#include "servant.h"
#include <cmath>
#include <stack>

using std::stack;

using namespace Backend;

vector<string> function_string::lexemes() const
{
    // TRACE_FUNC;
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

vector<string> function_string::reverse_polish() const
{
    // TRACE_FUNC;
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

function_string::function_string(string s)
{
    // TRACE_FUNC;
    expr = s;
    lexs = lexemes();
    rev_pol = reverse_polish();
    has_var = c_in_s('x', s);
}

double function_string::calc(double x) const
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
