/*
fix it: используйте pragma once вместо явных guard'ов
*/
#ifndef _WORDS_COUNTER_H_
#define _WORDS_COUNTER_H_

/*
fix it: не нужно писать все известные вам include'ы в одном месте.
в данном заголовочном файле вы используете только std::pair, std::string
остальное перенесите в cpp, если это используется в нем
*/
#include <cctype>
#include <iostream>
#include <map>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>

using std::string; 

/*
обычно стараются не использовать названия i1, i2 и т.п.
даже там, где вроде как и ясно, что имеется ввиду
лучше lhr, rhs, например (lhs = left hand side) 
*/
bool compare(const std::pair<string, int>& p1, const std::pair<string, int>& p2);

string prepare(const std::string& str);

#endif /* _WORDS_COUNTER_H_ */
