#include "back_func.h"

void vec_print (const std::vector<std::string>& v)
{
    std::cout << "{ ";
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << "''" << v[i] << "''";
        if (i != v.size() - 1)
            std::cout << ", ";
    }
    std::cout << " }" << std::endl;
}

bool str_is_double (std::string str)
{
    std::istringstream iss(str);
    double f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

void modify (std::vector<std::string>& v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        if (str_is_double(v[i]))
            v[i] = "numbers";
    }
    std::sort(v.begin(), v.end());
    auto last = std::unique(v.begin(), v.end());
    v.erase(last, v.end());
}

int main ()
{
    // D:/Programs/2023-2024/C++/build/graphic_calculator/UL_develop/backend.exe <
    // D:/Programs/2023-2024/C++/graphic_calculator/UL_develop/test.txt >
    // D:/Programs/2023-2024/C++/graphic_calculator/UL_develop/test_out.txt 2>&1
    std::string s;
    while (std::cin)
    {
        std::vector<std::string> res{};
        std::getline(std::cin, s);
        if (!std::cin)
            break;
        std::cout << "y = " << s << std::endl;
        for (double x = -10; x < 10; x += 0.1)
        {
            // std::cout << "      x = " << x << "; y = ";
            try
            {
                res.push_back(std::to_string(calc(reverse_polish(lexeme(s)), x)));
                // std::cout << calc(reverse_polish(lexeme(s)), x) << ";";
            }
            catch (std::exception& exc)
            {
                res.push_back(exc.what());
                // std::cout << exc.what() << ";";
            }
            catch (...)
            {
                res.push_back("exception");
                // std::cout << "exception;";
            }
            // std::cout << std::endl;
        }
        modify(res);
        std::cout << "results: "
                  << " ";
        vec_print(res);
    }

    return 0;
}