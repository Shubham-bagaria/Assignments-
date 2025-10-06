#include "BalancedTree.h"
#include <algorithm>
using namespace std;

// Constructor
BalancedTree::BalancedTree(){
    this->rootPtr = nullptr;
}

// Get Depth
int BalancedTree::getDepth(BalancedNode* node){
    return (node == nullptr) ? -1 : node->nodeDepth;
}

// Calculate Balance
int BalancedTree::calculateBalance(BalancedNode* node){
    return (node == nullptr) ? 0 : getDepth(node->leftBranch) - getDepth(node->rightBranch);
}

// Check if Left Dominant
bool BalancedTree::isLeftDominant(BalancedNode* node){
    return calculateBalance(node) > 1;
}

// Check if Right Dominant
bool BalancedTree::isRightDominant(BalancedNode* node){
    return calculateBalance(node) < -1;
}

// Pivot Left
BalancedNode* BalancedTree::pivotLeft(BalancedNode* anchor){
    BalancedNode* temporary = anchor;
    anchor = anchor->rightBranch;
    BalancedNode* temporary1 = anchor->leftBranch;
    anchor->leftBranch = temporary;
    anchor->leftBranch->rightBranch = temporary1;
    
    temporary->nodeDepth = max(this->getDepth(temporary->leftBranch), 
                               this->getDepth(temporary->rightBranch)) + 1;
    anchor->nodeDepth = max(this->getDepth(anchor->leftBranch), 
                           this->getDepth(anchor->rightBranch)) + 1;
    
    return anchor;
}

// Pivot Right
BalancedNode* BalancedTree::pivotRight(BalancedNode* anchor){
    BalancedNode* temporary = anchor;
    anchor = anchor->leftBranch;
    BalancedNode* temporary1 = anchor->rightBranch;
    anchor->rightBranch = temporary;
    anchor->rightBranch->leftBranch = temporary1;
    
    temporary->nodeDepth = max(this->getDepth(temporary->leftBranch), 
                               this->getDepth(temporary->rightBranch)) + 1;
    anchor->nodeDepth = max(this->getDepth(anchor->leftBranch), 
                           this->getDepth(anchor->rightBranch)) + 1;
    
    return anchor;
}

// Restore Balance
BalancedNode* BalancedTree::restoreBalance(BalancedNode* node){
    if(node == nullptr) return node;

    if(isRightDominant(node)){
        int balanceCoeff = calculateBalance(node->rightBranch);
        if(balanceCoeff <= 0){ // Right-Right scenario
            BalancedNode* newAnchor = pivotLeft(node);
            return newAnchor;
        }
        else { // Right-Left scenario
            node->rightBranch = pivotRight(node->rightBranch);
            BalancedNode* newAnchor = pivotLeft(node);
            return newAnchor;
        }
    }
    else if(isLeftDominant(node)){
        int balanceCoeff = calculateBalance(node->leftBranch);
        if(balanceCoeff >= 0) { // Left-Left scenario
            BalancedNode* newAnchor = pivotRight(node);
            return newAnchor;
        }
        else { // Left-Right scenario
            node->leftBranch = pivotLeft(node->leftBranch);
            BalancedNode* newAnchor = pivotRight(node);
            return newAnchor;
        }
    }
    return node;
}

// Add Element
void BalancedTree::addElement(int key){
    pair<BalancedNode*, bool> outcome = addElementRecursive(key, this->rootPtr);
    if(outcome.second){
        this->rootPtr = outcome.first;
    }
}

pair<BalancedNode*, bool> BalancedTree::addElementRecursive(int key, BalancedNode* node){
    pair<BalancedNode*, bool> outcome(nullptr, false);

    if(node == nullptr){
        BalancedNode* newNode = new BalancedNode(key);
        outcome = {newNode, true};
    }
    else{
        if(key > node->element){
            pair<BalancedNode*, bool> subResult = addElementRecursive(key, node->rightBranch);
            if(subResult.second){
                node->rightBranch = subResult.first;
            }
            outcome = {node, subResult.second};
        }
        else if(key < node->element){
            pair<BalancedNode*, bool> subResult = addElementRecursive(key, node->leftBranch);
            if(subResult.second){
                node->leftBranch = subResult.first;
            }
            outcome = {node, subResult.second};
        }
        else{
            cout << "\nDuplicate value detected\n";
            outcome = {node, false};
        }
    }

    if(outcome.first != nullptr) {
        BalancedNode* currentNode = outcome.first;
        currentNode->nodeDepth = max(getDepth(currentNode->leftBranch), 
                                     getDepth(currentNode->rightBranch)) + 1;
    }

    outcome.first = restoreBalance(outcome.first);
    return outcome;
}

// Remove Element
void BalancedTree::removeElement(int key){
    pair<BalancedNode*, bool> outcome = removeElementRecursive(key, this->rootPtr);
    if(outcome.second){
        this->rootPtr = outcome.first;
    }
}

pair<BalancedNode*, bool> BalancedTree::removeElementRecursive(int key, BalancedNode* node){
    pair<BalancedNode*, bool> outcome(nullptr, false);
    if(node == nullptr){
        return {node, false};
    }
    if(node->element > key){
        pair<BalancedNode*, bool> subResult = removeElementRecursive(key, node->leftBranch);
        node->leftBranch = subResult.first;
        outcome = {node, subResult.second};
    }
    else if(node->element < key){
        pair<BalancedNode*, bool> subResult = removeElementRecursive(key, node->rightBranch);
        node->rightBranch = subResult.first;
        outcome = {node, subResult.second};
    }
    else { // Target located
        if(node->leftBranch == nullptr || node->rightBranch == nullptr){
            BalancedNode* replacement = node->leftBranch ? node->leftBranch : node->rightBranch;
            if (replacement == nullptr) { // Leaf case
                delete node;
                outcome = {nullptr, true};
            } else { // Single child case
                *node = *replacement;
                delete replacement;
                outcome = {node, true};
            }
        }
        else{
            BalancedNode* successor = node->rightBranch;
            while(successor->leftBranch != nullptr) successor = successor->leftBranch;
            node->element = successor->element;
            pair<BalancedNode*, bool> tempResult = removeElementRecursive(successor->element, node->rightBranch);
            node->rightBranch = tempResult.first;
            outcome = {node, true};
        }
    }

    if(outcome.first != nullptr) {
        node = outcome.first;
        node->nodeDepth = max(getDepth(node->leftBranch), 
                             getDepth(node->rightBranch)) + 1;
    }

    if(outcome.second){
        outcome.first = restoreBalance(outcome.first);
    }
    return outcome;
}

// Locate Element
BalancedNode* BalancedTree::locateElement(int key){
    return locateElementRecursive(key, this->rootPtr);
}

BalancedNode* BalancedTree::locateElementRecursive(int key, BalancedNode* node) {
    if (node == nullptr) return nullptr;
    if (key == node->element) return node;
    
    if (key < node->element) {
        return locateElementRecursive(key, node->leftBranch);
    } else {
        return locateElementRecursive(key, node->rightBranch);
    }
}

// Inorder Scan
void BalancedTree::scanInorder() {
    cout << "\nInorder Sequence: ";
    scanInorderRecursive(this->rootPtr);
    cout << endl;
}

void BalancedTree::scanInorderRecursive(BalancedNode* node) {
    if (node == nullptr) return;
    scanInorderRecursive(node->leftBranch);
    cout << node->element << " ";
    scanInorderRecursive(node->rightBranch);
}

// Preorder Scan
void BalancedTree::scanPreorder() {
    cout << "\nPreorder Sequence: ";
    scanPreorderRecursive(this->rootPtr);
    cout << endl;
}

void BalancedTree::scanPreorderRecursive(BalancedNode* node) {
    if (node == nullptr) return;
    cout << node->element << " ";
    scanPreorderRecursive(node->leftBranch);
    scanPreorderRecursive(node->rightBranch);
}