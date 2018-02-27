#include<algorithm>
#include<map>
#include<string>

using std::string;

bool compare (const std::pair<string, int>& p1, const std::pair<string, int>& p2) {
    if(p1.second == p2.second) 
        return (p1.first.length() < p2.first.length());
    return(p1.second > p2.second);
}

string prepare(const string& str) {
    string result = str;
    std::transform(result.begin(), result.end(), result.begin(), tolower);
    while(!isalpha(result.back())) 
        result.pop_back();
    return result;
}
