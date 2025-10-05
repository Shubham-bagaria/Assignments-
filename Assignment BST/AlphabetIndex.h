#ifndef ALPHABET_INDEX_H
#define ALPHABET_INDEX_H

#include "LexiconTree.h"

class AlphabetNode {
public:
    char alphabet;
    AlphabetNode* leftChild;
    AlphabetNode* rightChild;
    LexiconTree* wordTree;
    
    AlphabetNode(char ch);
};

class AlphabetIndex {
public:
    AlphabetNode* rootNode;
    
    AlphabetIndex();
    void addCharacter(char ch);
    void removeCharacter(char ch);
    AlphabetNode* findCharacter(char ch);
    
private:
    AlphabetNode* addCharacterRecursive(char input, AlphabetNode* currentNode);
    AlphabetNode* removeCharacterRecursive(char input, AlphabetNode* currentNode);
    AlphabetNode* findCharacterRecursive(char ch, AlphabetNode* currentNode);
};

#endif