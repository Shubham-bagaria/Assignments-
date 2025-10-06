#include <iostream>
#include <climits>
using namespace std;

#ifndef BALANCED_TREE_H
#define BALANCED_TREE_H

class BalancedNode {
public:
    int element;
    BalancedNode* leftBranch;
    BalancedNode* rightBranch;
    int nodeDepth;

    BalancedNode(int val) {
        this->element = val;
        leftBranch = nullptr;
        rightBranch = nullptr;
        nodeDepth = 0;
    }
};

class BalancedTree {
public:
    BalancedNode* rootPtr;

    BalancedTree();

    void addElement(int key);
    void removeElement(int key);
    BalancedNode* locateElement(int key);
    void scanInorder();
    void scanPreorder();

private:
    pair<BalancedNode*, bool> addElementRecursive(int key, BalancedNode* node);
    pair<BalancedNode*, bool> removeElementRecursive(int key, BalancedNode* node);
    BalancedNode* locateElementRecursive(int key, BalancedNode* node);

    int getDepth(BalancedNode* node);
    int calculateBalance(BalancedNode* node);
    bool isLeftDominant(BalancedNode* node);
    bool isRightDominant(BalancedNode* node);
    
    BalancedNode* pivotLeft(BalancedNode* anchor);
    BalancedNode* pivotRight(BalancedNode* anchor);
    
    BalancedNode* restoreBalance(BalancedNode* node);

    void scanInorderRecursive(BalancedNode* node);
    void scanPreorderRecursive(BalancedNode* node);
};

#endif