#include "back_func.h"
#include "std_lib_facilities.h"

int main ()
{
    // string s;
    // getline(cin, s);
    // vector<char> Rev_pol = reverse_polish(lexeme(s));
    vector<string> exemp1{"1.2", "8", "+", "5", "-"};
    vector<string> exemp2{"x", "4", "2", "*", "1", "5", "-", "2", "^", "/", "+"};

    cout << endl;
    cout << calc(exemp1, 0) << endl;
    cout << calc(exemp2, 3) << endl;
    cout << checker("1+-1*2(-9)");
    return 0;
}