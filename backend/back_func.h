#include "std_lib_facilities.h"

// делает Фёдор
vector<string> reverse_polish (const vector<string>& lex);

vector<string> lexeme (const string& expr);

// мой контент
bool checker (const string& expr);
// проверка на скобки, проверка на мусорные символы, на законченное выражение,
// на отсутствие лишних переменных

vector<char> transform_to_char (const vector<string>& strings);

double calc (const vector<string>& rev_pol, double x);
