#include <string>

std::string format (double num)
{
    std::string s = std::to_string(num);

    // remove after-dot zeroes
    size_t i = s.length() - 1;
    while (s[i] == '0')
    {
        s.erase(i);
        i--;
    }
    if (s[i] == '.')
        s.erase(i);

    // slice after-dot digits
    int prec = 4;
    i = s.find('.') + prec;
    while (i < s.length())
        s.erase(i);

    return s;
}