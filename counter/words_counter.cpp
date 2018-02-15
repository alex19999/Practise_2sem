#include <cctype>
#include <iostream>
#include <map>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>

using std::string;
using std::ifstream;

struct Statistics {
    int count;
    string word;
};

bool compare ( const Statistics& s1, const Statistics& s2) {
    if( s1.count > s2.count) 
        return true;
    if(s1.count == s2.count) {
        if(s1.word.size() > s2.word.size())
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
            std::cout << "Now Here";
            result.pop_back();
        }
        iter++;
    }
    std::cout << "new" << result << "\n";
    return result;
}

void print(const Statistics& s) {
    std::cout << s.word<<":"<<s.count << "\n";
}
    

int main() {
    int iter;
    std::map<string, int> mymap;
    ifstream file("file.txt");
    if(file.is_open()) {
        string word;
        while(!file.eof()) {
            file >> word;
            std::cout << word << "\n";
            word = prepare(word);
            mymap[word]++;
        }
        iter = 0;
        for (auto it = mymap.begin(); it != mymap.end(); ++it) {
            std::cout << (*it).first << " : " << (*it).second << std::endl;
        }
        std::vector<Statistics> s;
        for (auto it = mymap.begin(); it != mymap.end(); ++it) {
            s[iter].count = (*it).second;
            s[iter].word = (*it).first;
            iter++;
        }
        std::sort(s.begin(), s.end(), compare);
        file.close();
    }
}


    



