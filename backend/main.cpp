#include "back_func.h"

int main ()
{
    // D:/Programs/2023-2024/C++/build/graphic_calculator/backend/backend.exe < test.txt >
    // test_out.txt 2>&1
    string s;
    while (cin)
    {
        getline(cin, s);
        try
        {
            // cout << "string: " << s << endl;
            cout << calc(reverse_polish(lexeme(s)), 1) << endl << endl;
        }
        catch (exception& exc)
        {
            cerr << exc.what() << endl;
        }
        catch (...)
        {
            cerr << "exception" << endl;
        }
    }
    return 0;
}