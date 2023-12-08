#include "test_backend/backend.h"
#include <iostream>
#include <sstream>

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

void vec_print (const std::vector<Backend::Segment>& v)
{
    std::cout << "{ ";
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << "[" << v[i].start << "; " << v[i].stop << "]";
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

std::vector<std::string> simple_modify (std::vector<std::string> v)
{
    std::vector<std::string> res{};
    for (size_t i = 0; i < v.size(); i++)
    {
        res.push_back(v[i]);
        if (!str_is_double(v[i]))
            res[i] = "exc";
    }
    return res;
}

int main ()
{
    // ../../../bin/test_grahpic_calculator_backend.exe
    // < ../../../graphic_calculator/testing/test.txt >
    // ../../../graphic_calculator/testing/test_out.txt 2>&1
    std::string s;
    while (std::cin)
    {
        std::vector<std::string> res{};
        std::getline(std::cin, s);
        if (!std::cin)
            break;
        std::cout << "y = " << s << std::endl;
        try
        {
            Backend::function_string gr_s = s;
            for (double x = -10; x < 10; x += 0.1)
            {
                try
                {
                    res.push_back(std::to_string(gr_s.calc(x)));
                }
                catch (std::exception& exc)
                {
                    res.push_back(exc.what());
                }
                catch (...)
                {
                    res.push_back("exception");
                }
            }
            std::cout << "segments: "
                      << " ";
            vec_print(gr_s.domain_segments(-10.0, 10.0, 600.0, 0.001));
        }
        catch (std::exception& exc)
        {
            res.push_back(exc.what());
        }
        catch (...)
        {
            res.push_back("exception");
        }
        auto det_res = simple_modify(res);
        std::cout << "results: "
                  << " ";
        vec_print(det_res);
        modify(res);
        std::cout << "results: "
                  << " ";
        vec_print(res);
    }

    return 0;
}