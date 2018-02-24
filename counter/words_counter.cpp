#include"words_counter.h"

using std::string;

/*
установите себе программу какую-нибудь для автоматического форматирования кода:
после ( не нужен пробел

на следующем семинаре я покажу, что в VS можно сделать для этого.
для vim'а погуглите сами
*/

bool compare ( const std::pair<string, int>& p1, const std::pair<string, int>& p2) {
    /*
    ещё один плюс в пользу структуры с понятными названиями.
    не совмем ясно сходу, что происходит
    */
    if(p1.second == p2.second) 
        return (p1.first.length() < p2.first.length());
    return(p1.second > p2.second);
}

string prepare(const string& str) {
    /*
    можно сразу позвать конструктор копирования
    string result = str;
    */
    string result;
    result = str;
    std::transform(result.begin(), result.end(), result.begin(), tolower);
    /*
    вместо result[result.length() - 1]
    можно писать result.back()
    
    кажется, что строка может заканчиваться, например, троеточием ...
    поэтому видимо вместо if нужен while
    */
    if(!isalpha(result[result.length() - 1])) 
        result.pop_back();
    return result;
}
