#include "words_counter.h"

int main() {
    std::ifstream file("file.txt");
    std::ofstream fout("fout.txt");
    std::map<string, int> map;
    if(file.is_open()) {
        string word;
        while(!file.eof()) {
            file >> word;
            word = prepare(word);
            map[word]++;
        }
        /*
        вопрос дискуссионный, но когда пишут код помимо скорости, потребления памяти и т.п. уделяют большое внимание понятности (читаемости) кода.
        из std::pair и последующего использования p.first, p.second неясно, что это за пара, какие данные хранит.
        вероятно в данном случае отдельная структура с понятными названиями полей была бы предпочтительнее 
        */
        std::vector< std::pair< string, int > > vec(map.begin(), map.end());
        std::sort(vec.begin(), vec.end(), compare);
        /*
        ваша std::pair уже не совмем легкая, т.к. хранит строку.
        написав auto p вы на каждой итерации цикла вызываете конструктор копирования для пары.
        лучше написать const auto& p : vec,
        копировать ссылку дешево, а const, чтобы случайно не изменить vec
        */
        for (auto p : vec) {
            std::cout << p.first << ' ' << p.second << std::endl;
            fout << p.first << ' ' << p.second << "\n";
        }
        fout.close();
        file.close();
    }
}

