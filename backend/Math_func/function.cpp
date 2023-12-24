
#include "function.h"

// std libs
#include <cmath>
#include <stack>
#include <stdexcept>
using std::stack;
using std::string;
using std::vector;

// servant
#include "../servant/servant.h"
using namespace Back_serv;
using namespace Back_consts;

// #include "temp_help.h"

namespace Math_func {

function::function(string _func_str)
    : func_str{spaces_deleted(_func_str)}, lexs{lexemes()}, rev_pol{reverse_polish()}
{
}

function::function(const function& func)
    : func_str{func.func_str}, lexs{func.lexs}, rev_pol{func.rev_pol}
{
}

function function::operator= (const function& func)
{
    func_str = func.func_str;
    lexs = func.lexs;
    rev_pol = func.rev_pol;

    return *this;
}

bool function::has_var() const { return c_in_s('x', func_str); }

vector<string> function::lexemes() const
{
    if (!is_str_valid())
        return {};

    vector<string> res;
    // MEMO: текущая лексема
    string lex;
    // MEMO: строка со всеми разрешенными мат. операциями
    const string math_oper_chars = "+-*/^u";
    // FIXME: разобраться с этим несоответствием векторов знаков и функций в разных местах
    // TODO: перевести эти строки в служебные константы
    for (size_t i = 0; i < func_str.size(); i++)
    {
        // MEMO: текущий символ строки
        char ch = func_str[i];
        switch (ch)
        {
        case minus:
        {
            if (lex.size() > 0)
            {
                res.push_back(lex);
                lex = "";
            }
            if (i == 0)
                res.push_back("um");
            else if (c_in_s(s_to_c(res[res.size() - 1]), math_oper_chars + "("))
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
            if (lex.size() > 0)
            {
                res.push_back(lex);
                lex = "";
            }
            lex = ch;
            res.push_back(lex);
            lex = "";
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
            if (lex.size() > 0 && !isdigit(lex[0]))
            {
                res.push_back(lex);
                lex = "";
            }
            lex += ch;
            break;
        }
        default:
        {
            if (!isblank(ch))
            {
                if (lex.size() > 0 && isdigit(lex[0]))
                {
                    res.push_back(lex);
                    lex = "";
                }
                lex += ch;
            }
        }
        }
    }
    if (lex.size() > 0)
        res.push_back(lex);
    return (res);
}

vector<string> function::reverse_polish() const
{
    if (!is_lexs_valid())
        return {};

    // MEMO: вектор, куда записывается итоговая запись
    vector<string> res;
    // MEMO: стэк для хранения операций в правильном порядке
    stack<string> st_oper;
    st_oper.push("\0");

    // FIXME: Дим, ты пишешь MEMO то рядом, то сверху

    const string math_func_chars = "sctelu";  // MEMO: строка с разрешенными мат. функциями
    const string math_oper_chars = "+-*/^";  // MEMO: строка с разрешенными мат. операциями

    for (const auto& lex : lexs)
    {
        // MEMO: символ текущей лексемы
        char curr = s_to_c(lex);
        // MEMO: символ, обозначающий последний элемент в стеке с операциями
        char last = s_to_c(st_oper.top());

        // TODO: напиши комментарии по работе этого свитча
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
            while (last == power || c_in_s(last, math_func_chars))
            {
                res.push_back(st_oper.top());
                st_oper.pop();
                last = s_to_c(st_oper.top());
            }
            st_oper.push(lex);
            break;
        }

        case mul:
        case divi:
        {
            while (last == mul || last == divi || last == power || c_in_s(last, math_func_chars))
            {
                res.push_back(st_oper.top());
                st_oper.pop();
                last = s_to_c(st_oper.top());
            }
            st_oper.push(lex);
            break;
        }

        case plus:
        case minus:
        {
            while (c_in_s(last, math_oper_chars) || c_in_s(last, math_func_chars))
            {
                res.push_back(st_oper.top());
                st_oper.pop();
                last = s_to_c(st_oper.top());
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
                last = s_to_c(st_oper.top());
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
    return res;
}

double function::calc(double x) const
{
    const string math_oper_chars = "+-*/^";  // MEMO: строка с разрешенными мат. операциями
    const string math_func_chars = "sctelu";  // MEMO: строка с с разрешенными мат. функциями

    stack<double> Stack;
    for (auto& lex : rev_pol)
    {
        double l;  // MEMO: последний символ в стэке
        double p;  // MEMO: предпоследний символ в стэке (последний после удаления l)
        char curr = s_to_c(lex);  // MEMO: текущий символ,
                                  // (если брать вместо строки (для switch))
        if (c_in_s(curr, math_func_chars))
        {
            l = Stack.top();  // запоминаем только последний
                              // (так как функции унарны)
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
        else if (c_in_s(curr, math_oper_chars))
        {
            l = Stack.top();
            Stack.pop();
            p = Stack.top();  // также запоминаем предпоследний
                              // (так как операции бинарны)
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
            throw std::invalid_argument("math invalid number");
    }  // последнее, что осталось в стэке после всех действий - и есть ответ
    return Stack.top();
}

}  // namespace Math_func

std::ostream& operator<< (std::ostream& os, Math_func::function func)
{
    return os << func.get_func_str();
}