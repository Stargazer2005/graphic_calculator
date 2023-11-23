#include "back_func.h"

int main ()
{
    // string s;
    // getline(cin, s);
    // vector<char> Rev_pol = reverse_polish(lexeme(s));
    // vector<string> exemp1{"1.2", "8", "+", "5", "-"};
    // vector<string> exemp2{"x", "4", "2", "*", "1", "5", "-", "2", "^", "/", "+"};

    // cout << endl;
    // cout << calc(exemp1, 0) << endl;
    // cout << calc(exemp2, 3) << endl;
    // D:/Programs/2023-2024/C++/build/graphic_calculator/backend/backend.exe < test.txt >
    // test_out.txt 2>&1
    string s;
    while (cin)
    {
        getline(cin, s);
        try
        {
            cout << "string: " << s << endl;
            cout << calc(reverse_polish(lexeme(s)), 1) << endl << endl;
        }
        catch (exception& exc)
        {
            cerr << "exception: " << exc.what() << endl;
        }
        catch (...)
        {
            cerr << "exception" << endl;
        }
    }
    return 0;
}