#include "words_counter.h"

int main() {
    std::map<string, int> map;
	std::ifstream file("file.txt");
    if(file.is_open()) {
        string word;
        while(!file.eof()) {
            file >> word;
            word = prepare(word);
            map[word]++;
        }
        std::vector< std::pair< string, int > > vec(map.begin(), map.end() );
        std::sort(vec.begin(), vec.end(), compare);
        for (auto p : vec)
            std::cout << p.first << ' ' << p.second << std::endl;
        file.close();
    }
}

