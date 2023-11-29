#include "back_func.h"

int main ()
{
    // D:/Programs/2023-2024/C++/build/graphic_calculator/backend/backend.exe < test.txt >
    // test_out.txt 2>&1
    std::string s;
    while (std::cin)
    {
        std::getline(std::cin, s);
        try
        {
            // cout << "string: " << s << endl;
            std::cout << calc(reverse_polish(lexeme(s)), 1) << std::endl << std::endl;
        }
        catch (std::exception& exc)
        {
            std::cerr << exc.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "exception" << std::endl;
        }
    }
    return 0;
}