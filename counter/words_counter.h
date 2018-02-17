#ifndef _WORDS_COUNTER_H_
#define _WORDS_COUNTER_H_

#include <cctype>
#include <iostream>
#include <map>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>

using std::string; 

bool compare(const std::pair<string, int>& p1, const std::pair<string, int>& p2);

string prepare(const std::string& str);

#endif
