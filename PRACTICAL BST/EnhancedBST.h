#include <climits>
#include <queue>
#include <iostream>
using namespace std;

#ifndef ENHANCED_BST_H
#define ENHANCED_BST_H

class TreeNode {
public:
    int value;
    TreeNode* leftChild;
    TreeNode* rightChild;
    int leftCount;      // Nodes in left subtree
    int rightCount;     // Nodes in right subtree
    int subtreeMin;     // Minimum in subtree
    int subtreeMax;     // Maximum in subtree
    int totalSum;       // Sum in subtree
    float meanValue;    // Average in subtree
    
    TreeNode(int val) {
        this->value = val;
        leftChild = nullptr;
        rightChild = nullptr;
        leftCount = 0;
        rightCount = 0;
        subtreeMin = val;
        subtreeMax = val;
        totalSum = val;
        meanValue = val;
    }
};

class EnhancedBST {
public:
    TreeNode* rootNode;
    int treeSize;
    
    EnhancedBST();
    void addElement(int key);
    void removeElement(int key);
    TreeNode* findElement(int key);
    int getTotalNodes();
    int countLeaves();
    bool validateStructure();
    int computeHeight();
    int computeDiameter();
    bool isBalanced();
    void traversePreorder();
    void traverseInorder();
    void traversePostorder();
    void traverseLevelOrder();
    void displayByLevels();
    int findLCA(int val1, int val2);
    int getNodeRank(int key);
    int findKthMax(int k);

private:
    pair<TreeNode*, bool> addElementRecursive(int key, TreeNode* node);
    pair<TreeNode*, bool> removeElementRecursive(int key, TreeNode* node);
    TreeNode* findElementRecursive(int key, TreeNode* node);
    int getTotalNodesRecursive(TreeNode* node);
    int countLeavesRecursive(TreeNode* node);
    bool validateStructureRecursive(TreeNode* node, int minVal, int maxVal);
    int computeHeightRecursive(TreeNode* node);
    pair<int, int> computeDiameterRecursive(TreeNode* node);
    pair<bool, int> isBalancedRecursive(TreeNode* node);
    void traversePreorderRecursive(TreeNode* node);
    void traversePostorderRecursive(TreeNode* node);
    void traverseInorderRecursive(TreeNode* node);
    int findLCARecursive(TreeNode* node, int val1, int val2);
    int getNodeRankRecursive(TreeNode* node, int key);
    TreeNode* findKthMaxRecursive(TreeNode* node, int k);
};

#endif