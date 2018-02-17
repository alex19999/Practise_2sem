#include<algorithm>
#include<vector>
#include<string>

using std::string;

bool compare ( const std::pair<string, int>& p1, const std::pair<string, int>& p2) {
    if( p1.second < p2.second) 
        return false;
    if(p1.second == p2.second) {
        if(p1.first.size() < p2.first.size())
            return true;
    }
}

string prepare(const string& str) {
        string result;
        result = str;
        int iter = 0;
        std::transform(result.begin(), result.end(), result.begin(), tolower);
        while(iter < result.size()) {
            if(ispunct(result[iter])) {
                result.pop_back();
            }
            iter++;
        }
        return result;
}
