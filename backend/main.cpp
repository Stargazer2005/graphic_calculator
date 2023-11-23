#include "functions.h"
#include <string>

int main ()
{
  // string s;
  // getline(cin, s);
  // vector<char> Rev_pol = reverse_polish(lexeme(s));
  //   vector<string> exemp1{"1.2", "8", "+", "5", "-"};
  //   vector<string> exemp2{"x", "4", "2", "*", "1", "5",
  //                         "-", "2", "^", "/", "+"};

  //   cout << endl;
  //   cout << calc(exemp1, 0) << endl;
  //   cout << calc(exemp2, 3);
  //   return 0;
  string s = "3+4*2/(1-5)^2";
  reverse_polish(lexeme(s));
}