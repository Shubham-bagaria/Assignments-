#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

class TreeNode {
public:
    int value;
    TreeNode* parentNode;
    TreeNode* leftChild;
    TreeNode* rightChild;
    TreeNode* prevNode;
    TreeNode* nextNode;
    int subtreeSize;
    int subtreeSum;
    int minValue;
    int maxValue;
    int treeHeight;
    int longestPath;

    TreeNode(int val) {
        this->value = val;
        this->parentNode = nullptr;
        this->leftChild = nullptr;
        this->rightChild = nullptr;
        this->prevNode = nullptr;
        this->nextNode = nullptr;
        this->subtreeSize = 1;
        this->subtreeSum = val;
        this->minValue = val;
        this->maxValue = val;
        this->treeHeight = 0;
        this->longestPath = 0;
    }
};

class EnhancedBST {
private:
    TreeNode* rootNode;

    void refreshNodeProperties(TreeNode* node) {
        if (node == nullptr) return;

        // left child
        int leftCount, leftMinVal, leftHeight, leftPath, leftTotal;
        if (node->leftChild != nullptr) {
            leftCount = node->leftChild->subtreeSize;
            leftTotal = node->leftChild->subtreeSum;
            leftMinVal = node->leftChild->minValue;
            leftHeight = node->leftChild->treeHeight;
            leftPath = node->leftChild->longestPath;
        } else {
            leftCount = 0;
            leftTotal = 0;
            leftMinVal = node->value;
            leftHeight = -1;
            leftPath = 0;
        }

        // right child
        int rightCount, rightMaxVal, rightHeight, rightPath, rightTotal;
        if (node->rightChild != nullptr) {
            rightCount = node->rightChild->subtreeSize;
            rightTotal = node->rightChild->subtreeSum;
            rightMaxVal = node->rightChild->maxValue;
            rightHeight = node->rightChild->treeHeight;
            rightPath = node->rightChild->longestPath;
        } else {
            rightCount = 0;
            rightTotal = 0;
            rightMaxVal = node->value;
            rightHeight = -1;
            rightPath = 0;
        }

        node->subtreeSize = 1 + leftCount + rightCount;
        node->subtreeSum = node->value + leftTotal + rightTotal;
        node->minValue = min(node->value, leftMinVal);
        node->maxValue = max(node->value, rightMaxVal);
        node->treeHeight = 1 + max(leftHeight, rightHeight);

        // diameter
        int crossPath = leftHeight + rightHeight + 2;
        node->longestPath = max(crossPath, max(leftPath, rightPath));
    }

    void linkNeighborsOnAdd(TreeNode* newNode) {
        TreeNode* before = getPreviousNode(newNode);
        TreeNode* after = getNextNode(newNode);

        newNode->prevNode = before;
        newNode->nextNode = after;

        if (before != nullptr) before->nextNode = newNode;
        if (after != nullptr) after->prevNode = newNode;
    }

    void unlinkNeighborsOnRemove(TreeNode* targetNode) {
        TreeNode* before = targetNode->prevNode;
        TreeNode* after = targetNode->nextNode;

        if (before != nullptr) before->nextNode = after;
        if (after != nullptr) after->prevNode = before;
    }

    TreeNode* replaceNode(TreeNode* oldNode, TreeNode* newNode) {
        if (oldNode->parentNode == nullptr) {
            rootNode = newNode;
        } else if (oldNode == oldNode->parentNode->leftChild) {
            oldNode->parentNode->leftChild = newNode;
        } else {
            oldNode->parentNode->rightChild = newNode;
        }
        if (newNode != nullptr) newNode->parentNode = oldNode->parentNode;
        return oldNode->parentNode;
    }

    TreeNode* getMinimumNode(TreeNode* node) {
        while (node != nullptr && node->leftChild != nullptr) 
            node = node->leftChild;
        return node;
    }

    TreeNode* getMaximumNode(TreeNode* node) {
        while (node != nullptr && node->rightChild != nullptr) 
            node = node->rightChild;
        return node;
    }

    TreeNode* getNextNode(TreeNode* node) {
        if (node == nullptr) return nullptr;
        if (node->rightChild != nullptr) return getMinimumNode(node->rightChild);

        TreeNode* ancestor = node->parentNode;
        while (ancestor != nullptr && node == ancestor->rightChild) {
            node = ancestor;
            ancestor = ancestor->parentNode;
        }
        return ancestor;
    }

    TreeNode* getPreviousNode(TreeNode* node) {
        if (node == nullptr) return nullptr;
        if (node->leftChild != nullptr) return getMaximumNode(node->leftChild);

        TreeNode* ancestor = node->parentNode;
        while (ancestor != nullptr && node == ancestor->leftChild) {
            node = ancestor;
            ancestor = ancestor->parentNode;
        }
        return ancestor;
    }

public:
    EnhancedBST() {
        this->rootNode = nullptr;
    }

    void addNode(int val) {
        if (rootNode == nullptr) {
            rootNode = new TreeNode(val);
            cout << "Tree created with root " << val << "\n\n";
            return;
        }

        TreeNode* curr = rootNode;
        TreeNode* par = nullptr;

        while (curr != nullptr) {
            par = curr;
            if (val < curr->value) {
                curr = curr->leftChild;
            } else if (val > curr->value) {
                curr = curr->rightChild;
            } else {
                cout << "Error: Node with value " << val << " already exists.\n";
                return;
            }
        }

        TreeNode* freshNode = new TreeNode(val);
        freshNode->parentNode = par;

        if (val < par->value) {
            par->leftChild = freshNode;
        } else {
            par->rightChild = freshNode;
        }

        cout << "Inserted " << val << "\n\n";

        linkNeighborsOnAdd(freshNode);

        TreeNode* updateNode = par;
        while (updateNode != nullptr) {
            refreshNodeProperties(updateNode);
            updateNode = updateNode->parentNode;
        }
    }

    void removeNode(int val) {
        TreeNode* targetNode = findNode(val);
        if (targetNode == nullptr) {
            cout << "Error: Node with value " << val << " not found.\n";
            return;
        }

        unlinkNeighborsOnRemove(targetNode);

        TreeNode* startUpdateNode;
        if (targetNode->leftChild == nullptr) {
            startUpdateNode = replaceNode(targetNode, targetNode->rightChild);
        } else if (targetNode->rightChild == nullptr) {
            startUpdateNode = replaceNode(targetNode, targetNode->leftChild);
        } else {
            TreeNode* replacement = getMinimumNode(targetNode->rightChild);
            startUpdateNode = replacement->parentNode;

            if (replacement->parentNode != targetNode) {
                replaceNode(replacement, replacement->rightChild);
                replacement->rightChild = targetNode->rightChild;
                replacement->rightChild->parentNode = replacement;
            } else {
                startUpdateNode = replacement;
            }

            replaceNode(targetNode, replacement);
            replacement->leftChild = targetNode->leftChild;
            replacement->leftChild->parentNode = replacement;
        }

        cout << "Deleted " << val << "\n\n";

        TreeNode* curr = startUpdateNode;
        while (curr != nullptr) {
            refreshNodeProperties(curr);
            curr = curr->parentNode;
        }

        delete targetNode;
    }

    TreeNode* findNode(int val) {
        TreeNode* curr = rootNode;
        while (curr != nullptr && curr->value != val) {
            if (val < curr->value) {
                curr = curr->leftChild;
            } else {
                curr = curr->rightChild;
            }
        }
        return curr;
    }

    void displayNodeInfo(int val) {
        TreeNode* node = findNode(val);
        if (node == nullptr) {
            cout << "Node with value " << val << " not found.\n";
            return;
        }

        cout << "--- Details for Node " << val << " ---\n";

        // Structural Properties
        string parentVal;
        if (node->parentNode != nullptr) {
            parentVal = to_string(node->parentNode->value);
        } else {
            parentVal = "null";
        }
        cout << "Parent: " << parentVal << "\n";

        string leftVal;
        if (node->leftChild != nullptr) {
            leftVal = to_string(node->leftChild->value);
        } else {
            leftVal = "null";
        }
        cout << "Left Child: " << leftVal << "\n";

        string rightVal;
        if (node->rightChild != nullptr) {
            rightVal = to_string(node->rightChild->value);
        } else {
            rightVal = "null";
        }
        cout << "Right Child: " << rightVal << "\n";

        // In-Order Properties (from stored pointers)
        string prevVal;
        if (node->prevNode != nullptr) {
            prevVal = to_string(node->prevNode->value);
        } else {
            prevVal = "null";
        }
        cout << "Stored Predecessor: " << prevVal << "\n";

        string nextVal;
        if (node->nextNode != nullptr) {
            nextVal = to_string(node->nextNode->value);
        } else {
            nextVal = "null";
        }
        cout << "Stored Successor: " << nextVal << "\n";

        // Augmented Subtree Properties
        cout << "Size: " << node->subtreeSize << "\n";
        cout << "Sum: " << node->subtreeSum << "\n";
        cout << "Min: " << node->minValue << "\n";
        cout << "Max: " << node->maxValue << "\n";
        cout << "Height: " << node->treeHeight << "\n";
        cout << "Diameter: " << node->longestPath << "\n";
        cout << "\n";
    }
};

int main() {
    EnhancedBST tree;

    cout << "Commands: insert <key>, delete <key>, print <key>, exit\n";

    while (true) {
        cout << "> ";
        string inputLine;
        getline(cin, inputLine);

        stringstream stream(inputLine);
        string cmd;
        stream >> cmd;

        // Convert command to lowercase
        transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        try {
            if (cmd == "insert") {
                int val;
                if (stream >> val) {
                    tree.addNode(val);
                } else {
                    cout << "Usage: insert <key>\n";
                }
            } else if (cmd == "delete") {
                int val;
                if (stream >> val) {
                    tree.removeNode(val);
                } else {
                    cout << "Usage: delete <key>\n";
                }
            } else if (cmd == "print") {
                int val;
                if (stream >> val) {
                    tree.displayNodeInfo(val);
                } else {
                    cout << "Usage: print <key>\n";
                }
            } else if (cmd == "exit") {
                cout << "Exiting.\n";
                return 0;
            } else if (cmd == "") {
                // Empty command, do nothing
            } else {
                cout << "Unknown command. Available commands: insert, delete, print, exit\n";
            }
        } catch (...) {
            cout << "Error: Invalid number format for key.\n";
        }
    }

    return 0;
}