#ifndef LEXICON_TREE_H
#define LEXICON_TREE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class DictionaryEntry {
public:
    string term;
    string meaning;
    
    DictionaryEntry();
    DictionaryEntry(string t, string m);
};

class LexiconNode {
public:
    DictionaryEntry entry;
    LexiconNode* leftChild;
    LexiconNode* rightChild;
    
    LexiconNode(DictionaryEntry e);
};

class LexiconTree {
public:
    LexiconNode* rootNode;
    
    LexiconTree();
    void addEntry(DictionaryEntry entry);
    DictionaryEntry lookupTerm(string term);
    void removeEntry(string term);
    void displayPreorder();
    void displayInorder();
    
private:
    LexiconNode* addEntryRecursive(DictionaryEntry input, LexiconNode* currentNode);
    LexiconNode* removeEntryRecursive(string term, LexiconNode* currentNode);
    LexiconNode* lookupTermRecursive(string term, LexiconNode* currentNode);
    void displayPreorderRecursive(LexiconNode* currentNode);
    void displayInorderRecursive(LexiconNode* currentNode);
};

vector<DictionaryEntry> loadDictionaryContent();

#endif