#include "back_func.h"

int main ()
{
    // D:/Programs/2023-2024/C++/build/graphic_calculator/UL_develop/backend.exe <
    // D:/Programs/2023-2024/C++/graphic_calculator/UL_develop/test.txt >
    // D:/Programs/2023-2024/C++/graphic_calculator/UL_develop/test_out.txt 2>&1
    std::string s;
    while (std::cin)
    {
        std::getline(std::cin, s);
        if (!std::cin)
            break;
        std::cout << "y = " << s << std::endl;
        for (double x = -10; x < 10; x += 0.1)
        {
            std::cout << "      x = " << x << "; y = ";
            try
            {
                std::cout << calc(reverse_polish(lexeme(s)), x) << ";";
            }
            catch (std::exception& exc)
            {
                std::cout << exc.what() << ";";
            }
            catch (...)
            {
                std::cout << "exception;";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}