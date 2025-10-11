#include <iostream>
#include <queue>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

class BalancedTree {
private:
    class TreeNode {
    public:
        int value;
        int nodeHeight;
        int balanceValue;
        TreeNode* leftChild;
        TreeNode* rightChild;

        TreeNode(int val) {
            this->value = val;
            this->nodeHeight = 1;
            this->balanceValue = 0;
            this->leftChild = nullptr;
            this->rightChild = nullptr;
        }
    };

    TreeNode* rootNode;

    int getHeight(TreeNode* node) {
        return (node == nullptr) ? 0 : node->nodeHeight;
    }

    void refreshNodeData(TreeNode* node) {
        if (node == nullptr) return;
        
        int leftH = getHeight(node->leftChild);
        int rightH = getHeight(node->rightChild);
        
        node->nodeHeight = 1 + max(leftH, rightH);
        node->balanceValue = rightH - leftH;
    }

    TreeNode* rotateRight(TreeNode* y) {
        TreeNode* x = y->leftChild;
        TreeNode* T2 = x->rightChild;

        x->rightChild = y;
        y->leftChild = T2;

        refreshNodeData(y);
        refreshNodeData(x);

        return x;
    }

    TreeNode* rotateLeft(TreeNode* x) {
        TreeNode* y = x->rightChild;
        TreeNode* T2 = y->leftChild;

        y->leftChild = x;
        x->rightChild = T2;

        refreshNodeData(x);
        refreshNodeData(y);

        return y;
    }

    TreeNode* addNode(TreeNode* node, int val) {
        if (node == nullptr) return new TreeNode(val);

        if (val < node->value) {
            node->leftChild = addNode(node->leftChild, val);
        } else if (val > node->value) {
            node->rightChild = addNode(node->rightChild, val);
        } else {
            return node; // Duplicate
        }

        refreshNodeData(node);
        int balance = node->balanceValue;

        if (balance < -1 && val < node->leftChild->value)
            return rotateRight(node);

        if (balance > 1 && val > node->rightChild->value)
            return rotateLeft(node);

        if (balance < -1 && val > node->leftChild->value) {
            node->leftChild = rotateLeft(node->leftChild);
            return rotateRight(node);
        }

        if (balance > 1 && val < node->rightChild->value) {
            node->rightChild = rotateRight(node->rightChild);
            return rotateLeft(node);
        }

        return node;
    }

    TreeNode* removeNode(TreeNode* node, int val) {
        if (node == nullptr) return nullptr;

        if (val < node->value) {
            node->leftChild = removeNode(node->leftChild, val);
        } else if (val > node->value) {
            node->rightChild = removeNode(node->rightChild, val);
        } else {
            if (node->leftChild == nullptr || node->rightChild == nullptr) {
                node = (node->leftChild == nullptr) ? node->rightChild : node->leftChild;
            } else {
                TreeNode* successor = getMinNode(node->rightChild);
                node->value = successor->value;
                node->rightChild = removeNode(node->rightChild, successor->value);
            }
        }

        if (node == nullptr) return nullptr;

        refreshNodeData(node);
        int balance = node->balanceValue;

        if (balance < -1 && (node->leftChild != nullptr && node->leftChild->balanceValue <= 0))
            return rotateRight(node);

        if (balance < -1 && (node->leftChild != nullptr && node->leftChild->balanceValue > 0)) {
            node->leftChild = rotateLeft(node->leftChild);
            return rotateRight(node);
        }

        if (balance > 1 && (node->rightChild != nullptr && node->rightChild->balanceValue >= 0))
            return rotateLeft(node);

        if (balance > 1 && (node->rightChild != nullptr && node->rightChild->balanceValue < 0)) {
            node->rightChild = rotateRight(node->rightChild);
            return rotateLeft(node);
        }

        return node;
    }

    TreeNode* getMinNode(TreeNode* node) {
        return (node->leftChild == nullptr) ? node : getMinNode(node->leftChild);
    }

    TreeNode* findNode(TreeNode* node, int val) {
        if (node == nullptr || node->value == val) return node;
        return (val < node->value) ? findNode(node->leftChild, val) : findNode(node->rightChild, val);
    }

public:
    BalancedTree() {
        this->rootNode = nullptr;
    }

    void insertValue(int val) {
        rootNode = addNode(rootNode, val);
    }

    void deleteValue(int val) {
        rootNode = removeNode(rootNode, val);
    }

    bool searchValue(int val) {
        return findNode(rootNode, val) != nullptr;
    }

    void displayTree() {
        if (rootNode == nullptr) {
            cout << "Tree is empty.\n";
            return;
        }

        queue<TreeNode*> q;
        q.push(rootNode);
        int level = 0;

        while (true) {
            int levelCount = q.size();
            if (levelCount == 0) break;

            bool hasNext = false;
            cout << "Level " << level << ": ";

            for (int i = 0; i < levelCount; i++) {
                TreeNode* current = q.front();
                q.pop();

                if (current != nullptr) {
                    cout << current->value << "(BF=" << current->balanceValue << ") ";
                    q.push(current->leftChild);
                    q.push(current->rightChild);

                    if (current->leftChild != nullptr || current->rightChild != nullptr) {
                        hasNext = true;
                    }
                } else {
                    cout << "null ";
                    q.push(nullptr);
                    q.push(nullptr);
                }
            }

            cout << "\n";
            level++;

            if (!hasNext) break;
        }
    }
};

int main() {
    BalancedTree tree;

    cout << "Enter initial numbers :\n";
    cout << "> ";
    
    string initialInput;
    getline(cin, initialInput);

    if (!initialInput.empty()) {
        stringstream ss(initialInput);
        string numStr;
        
        while (ss >> numStr) {
            try {
                int num = stoi(numStr);
                tree.insertValue(num);
            } catch (...) {
                // Skip invalid input
            }
        }
        tree.displayTree();
    }

    cout << "\nCommands: insert <key>, delete <key>, search <key>, print, exit\n";

    while (true) {
        cout << "\n> ";
        string inputLine;
        getline(cin, inputLine);

        if (inputLine.empty()) continue;

        stringstream ss(inputLine);
        string cmd, keyStr;
        ss >> cmd;

        transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (ss >> keyStr) {
            // Has key argument
        } else {
            keyStr = "";
        }

        if (cmd == "insert") {
            if (keyStr.empty()) {
                cout << "\n";
            } else {
                try {
                    int key = stoi(keyStr);
                    tree.insertValue(key);
                    cout << "Inserted " << key << " :\n\n";
                    tree.displayTree();
                } catch (...) {
                    cout << "!! The key must be an integer.\n";
                }
            }
        } else if (cmd == "delete") {
            if (keyStr.empty()) {
                cout << "\n";
            } else {
                try {
                    int key = stoi(keyStr);
                    if (tree.searchValue(key)) {
                        tree.deleteValue(key);
                        cout << "Deleted " << key << ":\n\n";
                        tree.displayTree();
                    } else {
                        cout << "!! Key " << key << " not found\n";
                    }
                } catch (...) {
                    cout << "!! The key must be an integer.\n";
                }
            }
        } else if (cmd == "search") {
            if (keyStr.empty()) {
                cout << "\n";
            } else {
                try {
                    int key = stoi(keyStr);
                    cout << "Key " << key << (tree.searchValue(key) ? " Found." : " Not Found.") << "\n";
                } catch (...) {
                    cout << "!! The key must be an integer.\n";
                }
            }
        } else if (cmd == "print") {
            tree.displayTree();
        } else if (cmd == "exit") {
            cout << "Goodbye! Session ended.\n";
            return 0;
        } else {
            cout << "!! Unknown command\n";
        }
    }

    return 0;
}