#include <iostream>
#include <vector>
using std::cout; 
using std::endl;

template <typename T> void print (const std::vector<T>& v)
{
    cout << " size: " << v.size() << endl;
    cout << "{ ";
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << "''" << v[i] << "''";
        if (i != v.size() - 1)
            cout << endl;
    }
    cout << " }" << endl;
}
