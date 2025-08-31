#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Word {
public:
    string word;
    string meaning;
    Word() {
        word = "";
        meaning = "";
    }
    Word(string w, string m) {
        word = w;
        meaning = m;
    }
};

vector<Word> getDictionary();

#endif
