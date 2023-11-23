#include <string>
#include <vector>
using std::string;
using std::vector;

// делает Фёдор

// фунция, которая переводит введеную пользователем строку в список лексем
vector<string> lexeme (const string& expr);

// функция, которая переводит список лексем в обратную польскую запись
vector<string> reverse_polish (const vector<string>& lex);

// делает Дмитрий

// функция, проверяющая на корректность скобок, отсутствие лишних символов,
// правильное использование знаков и чисел возле них
bool checker (const string& expr);

// функция, которая по обратной польской записи лексем и значению переменной
// x вычисляет значение y
double calc (const vector<string>& rev_pol, double x);
