#include "functions.h"
#include <cassert>
#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>

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

vector<string> lexeme (const string& expr)
{
  //   if (!checker(expr))
  //   {
  //     throw std::runtime_error("Incorrect input");
  //   }
  vector<string> lexs;
  string s;
  for (char ch : expr)
  {
    switch (ch)
    {
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    {
      if (s.size() > 0)
      {
        lexs.push_back(s);
        s = "";
      }
      s = (1, ch);
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
      s += ch;
      break;
    }
    default:
    {
      if (!isblank(ch))
      {
        if (s.size() > 0 && isdigit(s[0]))
        {
          lexs.push_back(s);
          s = "";
        }
        s += ch;
      }
    }
    }
  }
  if (s.size() > 0)
    lexs.push_back(s);
  print(lexs);
}