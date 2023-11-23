#include "functions.h"
#include <cassert>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;

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
      throw std::range_error("Stack: empty, nothing to pop");
  }

  double last ()
  {
    if (st.size() != 0)
      return st[st.size() - 1];
    else
      throw std::range_error("Stack: empty, no last");
  }

  unsigned long long len () { return st.size(); }

  void print ()
  {
    for (auto i : st)
    {
      std::cout << i << ", ";
    }
    std::cout << std::endl;
  }

private:
  vector<double> st;
};

bool checker (const string& expr) {}

vector<char> transform_to_char (const vector<string>& strs)
{
  vector<char> res{};
  for (auto s : strs)
  {
    if (s[0] >= '0' && s[0] <= '9')
      res.push_back('n');
    else
      res.push_back(s[0]);
  }
  return res;
}

double calc (const vector<string>& rev_pol, double x)
{
  Stack stack;
  for (auto i = 0; i < rev_pol.size(); i++)
  {
    // stack.print();
    double l, p;
    switch (transform_to_char(rev_pol)[i])
    {
    case 'n':
      stack.push(stod(rev_pol[i]));
      break;
    case 'x':
      stack.push(x);
      break;
    case 's':
      l = stack.last();
      stack.pop();
      stack.push(sin(l));
      break;
    case 'c':
      l = stack.last();
      stack.pop();
      stack.push(cos(l));
      break;
    case 't':
      l = stack.last();
      stack.pop();
      stack.push(tan(l));
      break;
    case 'e':
      l = stack.last();
      stack.pop();
      stack.push(exp(l));
      break;
    case 'l':
      l = stack.last();
      stack.pop();
      stack.push(log(l));
      break;
    case 'u':
      l = stack.last();
      stack.pop();
      stack.push(0 - l);
      break;
    case '+':
      l = stack.last();
      stack.pop();
      p = stack.last();
      stack.pop();
      stack.push(p + l);
      break;
    case '-':
      l = stack.last();
      stack.pop();
      p = stack.last();
      stack.pop();
      stack.push(p - l);
      break;
    case '*':
      l = stack.last();
      stack.pop();
      p = stack.last();
      stack.pop();
      stack.push(p * l);
      break;
    case '/':
      l = stack.last();
      stack.pop();
      p = stack.last();
      stack.pop();
      stack.push(p / l);
      break;
    case '^':
      l = stack.last();
      stack.pop();
      p = stack.last();
      stack.pop();
      stack.push(pow(p, l));
      break;
    default:
      throw std::runtime_error("Oops");
      break;
    }
  }
  return stack.last();
}

void print (std::vector<string> v)
{
  std::cout << "{ ";
  for (int i = 0; i < v.size(); i++)
  {
    std::cout << "'" << v[i] << "'"
              << ", ";
  }
  std::cout << "}" << std::endl;
}

bool c_in_s (char c, const string& s)
{
  return (s.find(c) != string::npos);
}

char transform_to_char (const string& s)
{
  if (s[0] >= '0' && s[0] <= '9')  // если число
    return ('n');  // заменяем на спец. символ
  else
    return (s[0]);  // первая буква отражает название функции
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
  print(lexs);
  return lexs;
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

  auto last = [] (vector<string> v)
  { return (v.size() > 0) ? v[v.size() - 1] : ""; };

  auto is_func = [&func, last] (string s)
  { return c_in_s(transform_to_char(s), func); };

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
  print(res);
  return res;
}