#include "EnhancedBST.h"
#include <iostream>
#include <algorithm>
using namespace std;

// Constructor
EnhancedBST::EnhancedBST() {
    this->rootNode = nullptr;
    this->treeSize = 0;
}

// Add Element
void EnhancedBST::addElement(int key) {
    pair<TreeNode*, bool> result = addElementRecursive(key, rootNode);
    if (result.second)
        this->treeSize++;
    this->rootNode = result.first;
}

pair<TreeNode*, bool> EnhancedBST::addElementRecursive(int key, TreeNode* node) {
    if (node == nullptr) {
        node = new TreeNode(key);
        pair<TreeNode*, bool> result(node, true);
        return result;
    } else {
        pair<TreeNode*, bool> outcome;
        if (node->value > key) {
            if (node->subtreeMin > key) {
                node->subtreeMin = key;
            }
            pair<TreeNode*, bool> result = addElementRecursive(key, node->leftChild);
            node->leftChild = result.first;
            if (result.second)
                node->leftCount += 1;
            outcome = {node, result.second};
        }
        else if (node->value < key) {
            if (node->subtreeMax < key) {
                node->subtreeMax = key;
            }
            pair<TreeNode*, bool> result = addElementRecursive(key, node->rightChild);
            node->rightChild = result.first;
            if (result.second)
                node->rightCount += 1;
            outcome = {node, result.second};
        }
        else {
            cout << "\nDuplicate element detected\n";
            outcome = {node, false};
        }
        if (outcome.second) {
            node->totalSum += key;
            node->meanValue = (node->totalSum) / (float)(node->rightCount + node->leftCount + 1);
        }
        return outcome;
    }
}

// Remove Element
void EnhancedBST::removeElement(int key) {
    pair<TreeNode*, bool> result = removeElementRecursive(key, rootNode);
    if (result.second)
        this->treeSize--;
    this->rootNode = result.first;
}

pair<TreeNode*, bool> EnhancedBST::removeElementRecursive(int key, TreeNode* node) {
    if (node == nullptr) {
        cout << "Element '" << key << "' not found." << endl;
        pair<TreeNode*, bool> result(nullptr, false);
        return result;
    }
    
    pair<TreeNode*, bool> outcome;
    if (node->value > key) {
        pair<TreeNode*, bool> result = removeElementRecursive(key, node->leftChild);
        if (result.second)
            node->leftCount--;
        node->leftChild = result.first;
        outcome = {node, result.second};
    }
    else if (node->value < key) {
        pair<TreeNode*, bool> result = removeElementRecursive(key, node->rightChild);
        if (result.second)
            node->rightCount--;
        node->rightChild = result.first;
        outcome = {node, result.second};
    }
    else {
        if (node->leftChild == nullptr && node->rightChild == nullptr) {
            delete node;
            outcome = {nullptr, true};
        }
        else if (node->leftChild == nullptr) {
            TreeNode* tempRight = node->rightChild;
            node->rightChild = nullptr;
            delete node;
            outcome = {tempRight, true};
        }
        else if (node->rightChild == nullptr) {
            TreeNode* tempLeft = node->leftChild;
            node->leftChild = nullptr;
            delete node;
            outcome = {tempLeft, true};
        }
        else {
            TreeNode* successor = node->rightChild;
            while (successor->leftChild != nullptr) {
                successor = successor->leftChild;
            }
            int successorVal = successor->value;
            node->value = successorVal;
            pair<TreeNode*, bool> tempResult = removeElementRecursive(successorVal, node->rightChild);
            node->rightChild = tempResult.first;
            outcome = {node, true};
        }
    }
    
    if (outcome.second && node != nullptr) {
        node->totalSum = node->value;
        if (node->leftChild) node->totalSum += node->leftChild->totalSum;
        if (node->rightChild) node->totalSum += node->rightChild->totalSum;
        node->meanValue = node->totalSum / (float)(node->leftCount + node->rightCount + 1);
        
        if (node->subtreeMin == key) {
            if (node->leftChild) {
                node->subtreeMin = node->leftChild->subtreeMin;
            } else {
                node->subtreeMin = node->value;
            }
        }
        if (node->subtreeMax == key) {
            if (node->rightChild) {
                node->subtreeMax = node->rightChild->subtreeMax;
            } else {
                node->subtreeMax = node->value;
            }
        }
    }
    return outcome;
}

// Find Element
TreeNode* EnhancedBST::findElement(int key) {
    return findElementRecursive(key, this->rootNode);
}

TreeNode* EnhancedBST::findElementRecursive(int key, TreeNode* node) {
    if (node == nullptr) {
        cout << "\nElement not present\n";
        return node;
    }
    if (node->value == key) {
        return node;
    }
    else if (node->value > key) {
        return findElementRecursive(key, node->leftChild);
    }
    else {
        return findElementRecursive(key, node->rightChild);
    }
}

// Get Total Nodes
int EnhancedBST::getTotalNodes() {
    return this->treeSize;
}

int EnhancedBST::getTotalNodesRecursive(TreeNode* node) {
    int count = node->leftCount + 1 + node->rightCount;
    return count;
}

// Count Leaves
int EnhancedBST::countLeaves() {
    return countLeavesRecursive(this->rootNode);
}

int EnhancedBST::countLeavesRecursive(TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }
    else if (node->leftChild == nullptr && node->rightChild == nullptr) {
        return 1;
    }
    else {
        return countLeavesRecursive(node->leftChild) + countLeavesRecursive(node->rightChild);
    }
}

// Validate Structure
bool EnhancedBST::validateStructure() {
    return validateStructureRecursive(this->rootNode, INT_MIN, INT_MAX);
}

bool EnhancedBST::validateStructureRecursive(TreeNode* node, int minVal, int maxVal) {
    if (node == nullptr) {
        return true;
    }
    if (node->value <= minVal || node->value >= maxVal) {
        return false;
    } else {
        return validateStructureRecursive(node->leftChild, minVal, node->value - 1) && 
               validateStructureRecursive(node->rightChild, node->value + 1, maxVal);
    }
}

// Compute Height
int EnhancedBST::computeHeight() {
    return computeHeightRecursive(this->rootNode);
}

int EnhancedBST::computeHeightRecursive(TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }
    else {
        return 1 + max(computeHeightRecursive(node->leftChild), 
                      computeHeightRecursive(node->rightChild));
    }
}

// Compute Diameter
int EnhancedBST::computeDiameter() {
    pair<int, int> result = computeDiameterRecursive(this->rootNode);
    return result.first;
}

pair<int, int> EnhancedBST::computeDiameterRecursive(TreeNode* node) {
    if (node == nullptr) {
        pair<int, int> p(0, 0);
        return p;
    }
    
    pair<int, int> leftInfo = computeDiameterRecursive(node->leftChild);
    pair<int, int> rightInfo = computeDiameterRecursive(node->rightChild);
    
    int currentDiameter = leftInfo.second + rightInfo.second;
    int currentHeight = max(leftInfo.second, rightInfo.second) + 1;
    currentDiameter = max(currentDiameter, max(leftInfo.first, rightInfo.first));
    
    pair<int, int> result(currentDiameter, currentHeight);
    return result;
}

// Check Balance
bool EnhancedBST::isBalanced() {
    pair<bool, int> result = isBalancedRecursive(this->rootNode);
    return result.first;
}

pair<bool, int> EnhancedBST::isBalancedRecursive(TreeNode* node) {
    if (node == nullptr) {
        pair<bool, int> p(true, 0);
        return p;
    }
    
    pair<bool, int> leftResult = isBalancedRecursive(node->leftChild);
    pair<bool, int> rightResult = isBalancedRecursive(node->rightChild);
    
    int heightDiff = abs(leftResult.second - rightResult.second);
    bool balanced = heightDiff <= 1 && leftResult.first && rightResult.first;
    int height = max(leftResult.second, rightResult.second) + 1;
    
    pair<bool, int> result(balanced, height);
    return result;
}

// Preorder Traversal
void EnhancedBST::traversePreorder() {
    cout << "\nTree preorder traversal:\n";
    traversePreorderRecursive(this->rootNode);
}

void EnhancedBST::traversePreorderRecursive(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    else {
        cout << node->value << " ";
        traversePreorderRecursive(node->leftChild);
        traversePreorderRecursive(node->rightChild);
    }
}

// Inorder Traversal
void EnhancedBST::traverseInorder() {
    cout << "\nTree inorder traversal:\n";
    traverseInorderRecursive(this->rootNode);
}

void EnhancedBST::traverseInorderRecursive(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    else {
        traverseInorderRecursive(node->leftChild);
        cout << node->value << " ";
        traverseInorderRecursive(node->rightChild);
    }
}

// Postorder Traversal
void EnhancedBST::traversePostorder() {
    cout << "\nTree postorder traversal:\n";
    traversePostorderRecursive(this->rootNode);
}

void EnhancedBST::traversePostorderRecursive(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    else {
        traversePostorderRecursive(node->leftChild);
        traversePostorderRecursive(node->rightChild);
        cout << node->value << " ";
    }
}

// Level Order Traversal
void EnhancedBST::traverseLevelOrder() {
    queue<TreeNode*> processQueue;
    if (this->rootNode == nullptr) return;
    
    processQueue.push(this->rootNode);
    while (processQueue.size() != 0) {
        TreeNode* current = processQueue.front();
        processQueue.pop();
        cout << current->value << " ";
        
        if (current->leftChild)
            processQueue.push(current->leftChild);
        if (current->rightChild)
            processQueue.push(current->rightChild);
    }
}

// Display by Levels
void EnhancedBST::displayByLevels() {
    queue<TreeNode*> processQueue;
    if (this->rootNode == nullptr) return;
    
    processQueue.push(this->rootNode);
    processQueue.push(nullptr);
    
    while (processQueue.size() != 0) {
        TreeNode* current = processQueue.front();
        processQueue.pop();
        
        if (current == nullptr) {
            cout << "\n";
            if (processQueue.size() != 0) {
                processQueue.push(nullptr);
            }
        } else {
            cout << current->value << " ";
            if (current->leftChild)
                processQueue.push(current->leftChild);
            if (current->rightChild)
                processQueue.push(current->rightChild);
        }
    }
}

// Find LCA
int EnhancedBST::findLCA(int val1, int val2) {
    int result = findLCARecursive(this->rootNode, val1, val2);
    if (result == INT_MAX) {
        cout << "\nCommon ancestor unavailable\n";
    }
    return result;
}

int EnhancedBST::findLCARecursive(TreeNode* node, int val1, int val2) {
    if (node == nullptr) {
        return INT_MAX;
    }
    if (node->value == val1 || node->value == val2)
        return node->value;
        
    int leftSearch = findLCARecursive(node->leftChild, val1, val2);
    int rightSearch = findLCARecursive(node->rightChild, val1, val2);
    
    if (leftSearch == INT_MAX && rightSearch == INT_MAX)
        return INT_MAX;
    if (leftSearch != INT_MAX && rightSearch != INT_MAX) {
        return node->value;
    }
    if (leftSearch != INT_MAX)
        return leftSearch;
    if (rightSearch != INT_MAX)
        return rightSearch;
    
    return INT_MAX;
}

// Get Node Rank
int EnhancedBST::getNodeRank(int key) {
    int position = getNodeRankRecursive(this->rootNode, key);
    return position;
}

int EnhancedBST::getNodeRankRecursive(TreeNode* node, int key) {
    if (node == nullptr) {
        return -1;
    }
    if (node->value == key) {
        return node->leftCount + 1;
    }
    else if (node->value < key) {
        int rightRank = getNodeRankRecursive(node->rightChild, key);
        if (rightRank == -1) {
            return -1;
        }
        return node->leftCount + 1 + rightRank;
    }
    else {
        return getNodeRankRecursive(node->leftChild, key);
    }
}

// Find Kth Max
int EnhancedBST::findKthMax(int k) {
    if (k < 1 || k > this->treeSize) {
        cout << "Error: Position " << k << " out of range." << endl;
        return INT_MIN;
    }
    TreeNode* resultNode = findKthMaxRecursive(this->rootNode, k);
    return resultNode->value;
}

TreeNode* EnhancedBST::findKthMaxRecursive(TreeNode* node, int k) {
    if (node == nullptr) {
        return nullptr;
    }
    
    int positionFromRight = node->rightCount + 1;
    if (k == positionFromRight) {
        return node;
    }
    else if (k < positionFromRight) {
        return findKthMaxRecursive(node->rightChild, k);
    }
    else {
        int adjustedK = k - positionFromRight;
        return findKthMaxRecursive(node->leftChild, adjustedK);
    }
}