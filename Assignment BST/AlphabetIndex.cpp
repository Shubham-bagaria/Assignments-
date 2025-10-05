#include "AlphabetIndex.h"
#include <iostream>
using namespace std;

// AlphabetNode Constructor
AlphabetNode::AlphabetNode(char ch) {
    this->alphabet = ch;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->wordTree = new LexiconTree();
}

// AlphabetIndex Constructor
AlphabetIndex::AlphabetIndex() {
    this->rootNode = nullptr;
}

// Public method to add character
void AlphabetIndex::addCharacter(char ch) {
    this->rootNode = addCharacterRecursive(ch, this->rootNode);
}

// Recursive character insertion
AlphabetNode* AlphabetIndex::addCharacterRecursive(char input, AlphabetNode* currentNode) {
    if (currentNode == nullptr) {
        currentNode = new AlphabetNode(input);
    } else {
        if (currentNode->alphabet > input) {
            currentNode->leftChild = addCharacterRecursive(input, currentNode->leftChild);
        } else if (currentNode->alphabet < input) {
            currentNode->rightChild = addCharacterRecursive(input, currentNode->rightChild);
        } else {
            cout << currentNode->alphabet << " Character already indexed\n";
        }
    }
    return currentNode;
}

// Public method to find character
AlphabetNode* AlphabetIndex::findCharacter(char ch) {
    return findCharacterRecursive(ch, this->rootNode);
}

// Recursive character search
AlphabetNode* AlphabetIndex::findCharacterRecursive(char ch, AlphabetNode* currentNode) {
    if(currentNode == nullptr) {
        return nullptr;
    }
    
    if(ch == currentNode->alphabet) {
        return currentNode;
    } else if(ch > currentNode->alphabet) {
        return findCharacterRecursive(ch, currentNode->rightChild);
    } else {
        return findCharacterRecursive(ch, currentNode->leftChild);
    }
}

// Public method to remove character
void AlphabetIndex::removeCharacter(char ch) {
    this->rootNode = removeCharacterRecursive(ch, this->rootNode);
}

// Recursive character deletion
AlphabetNode* AlphabetIndex::removeCharacterRecursive(char input, AlphabetNode* currentNode) {
    if(currentNode == nullptr) {
        return nullptr;
    }
    
    if(input > currentNode->alphabet) {
        currentNode->rightChild = removeCharacterRecursive(input, currentNode->rightChild);
    } else if(input < currentNode->alphabet) {
        currentNode->leftChild = removeCharacterRecursive(input, currentNode->leftChild);
    } else {
        // No children
        if(currentNode->leftChild == nullptr && currentNode->rightChild == nullptr) {
            delete currentNode;
            return nullptr;
        }
        // Only right child
        else if(currentNode->leftChild == nullptr) {
            AlphabetNode* temp = currentNode->rightChild;
            currentNode->rightChild = nullptr;
            delete currentNode;
            return temp;
        }
        // Only left child
        else if(currentNode->rightChild == nullptr) {
            AlphabetNode* temp = currentNode->leftChild;
            currentNode->leftChild = nullptr;
            delete currentNode;
            return temp;
        }
        // Two children - find inorder successor
        else {
            AlphabetNode* successor = currentNode->rightChild;
            while(successor->leftChild != nullptr) {
                successor = successor->leftChild;
            }
            
            char successorValue = successor->alphabet;
            currentNode->alphabet = successorValue;
            currentNode->rightChild = removeCharacterRecursive(successorValue, currentNode->rightChild);
            return currentNode;
        }
    }
    return currentNode;
}