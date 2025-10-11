#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

enum NodeColor {
    CRIMSON,
    DARK
};

class TreeNode {
public:
    int value;
    TreeNode* parentNode;
    TreeNode* leftChild;
    TreeNode* rightChild;
    NodeColor nodeColor;

    TreeNode(int val) {
        this->value = val;
        this->nodeColor = CRIMSON;
        this->parentNode = nullptr;
        this->leftChild = nullptr;
        this->rightChild = nullptr;
    }
};

class ColoredBST {
private:
    TreeNode* rootNode;
    TreeNode* nullNode;

    void spinLeft(TreeNode* x) {
        TreeNode* y = x->rightChild;
        x->rightChild = y->leftChild;
        
        if (y->leftChild != nullNode) {
            y->leftChild->parentNode = x;
        }
        
        y->parentNode = x->parentNode;
        
        if (x->parentNode == nullptr) {
            this->rootNode = y;
        } else if (x == x->parentNode->leftChild) {
            x->parentNode->leftChild = y;
        } else {
            x->parentNode->rightChild = y;
        }
        
        y->leftChild = x;
        x->parentNode = y;
    }

    void spinRight(TreeNode* x) {
        TreeNode* y = x->leftChild;
        x->leftChild = y->rightChild;
        
        if (y->rightChild != nullNode) {
            y->rightChild->parentNode = x;
        }
        
        y->parentNode = x->parentNode;
        
        if (x->parentNode == nullptr) {
            this->rootNode = y;
        } else if (x == x->parentNode->rightChild) {
            x->parentNode->rightChild = y;
        } else {
            x->parentNode->leftChild = y;
        }
        
        y->rightChild = x;
        x->parentNode = y;
    }

    void repairAfterAdd(TreeNode* current) {
        while (current != rootNode && current->parentNode->nodeColor == CRIMSON) {
            if (current->parentNode == current->parentNode->parentNode->rightChild) {
                handleAddRightCase(current);
            } else {
                handleAddLeftCase(current);
            }
        }
        rootNode->nodeColor = DARK;
    }

    void handleAddRightCase(TreeNode* current) {
        TreeNode* uncle = current->parentNode->parentNode->leftChild;
        
        if (uncle->nodeColor == CRIMSON) {
            current->parentNode->nodeColor = DARK;
            uncle->nodeColor = DARK;
            current->parentNode->parentNode->nodeColor = CRIMSON;
            current = current->parentNode->parentNode;
        } else {
            if (current == current->parentNode->leftChild) {
                current = current->parentNode;
                spinRight(current);
            }
            current->parentNode->nodeColor = DARK;
            current->parentNode->parentNode->nodeColor = CRIMSON;
            spinLeft(current->parentNode->parentNode);
        }
    }

    void handleAddLeftCase(TreeNode* current) {
        TreeNode* uncle = current->parentNode->parentNode->rightChild;
        
        if (uncle->nodeColor == CRIMSON) {
            current->parentNode->nodeColor = DARK;
            uncle->nodeColor = DARK;
            current->parentNode->parentNode->nodeColor = CRIMSON;
            current = current->parentNode->parentNode;
        } else {
            if (current == current->parentNode->rightChild) {
                current = current->parentNode;
                spinLeft(current);
            }
            current->parentNode->nodeColor = DARK;
            current->parentNode->parentNode->nodeColor = CRIMSON;
            spinRight(current->parentNode->parentNode);
        }
    }

    void swapNodes(TreeNode* u, TreeNode* v) {
        if (u->parentNode == nullptr) {
            rootNode = v;
        } else if (u == u->parentNode->leftChild) {
            u->parentNode->leftChild = v;
        } else {
            u->parentNode->rightChild = v;
        }
        v->parentNode = u->parentNode;
    }

    void removeNodeProcess(TreeNode* startNode, int val) {
        TreeNode* targetNode = nullNode;
        
        while (startNode != nullNode) {
            if (startNode->value == val) {
                targetNode = startNode;
                break;
            }
            if (startNode->value < val) {
                startNode = startNode->rightChild;
            } else {
                startNode = startNode->leftChild;
            }
        }

        if (targetNode == nullNode) {
            cout << "Value not found in the tree\n";
            return;
        }

        TreeNode* successorNode = targetNode;
        NodeColor savedColor = successorNode->nodeColor;
        TreeNode* replacement;

        if (targetNode->leftChild == nullNode) {
            replacement = targetNode->rightChild;
            swapNodes(targetNode, targetNode->rightChild);
        } else if (targetNode->rightChild == nullNode) {
            replacement = targetNode->leftChild;
            swapNodes(targetNode, targetNode->leftChild);
        } else {
            successorNode = findMinimum(targetNode->rightChild);
            savedColor = successorNode->nodeColor;
            replacement = successorNode->rightChild;

            if (successorNode->parentNode == targetNode) {
                replacement->parentNode = successorNode;
            } else {
                swapNodes(successorNode, successorNode->rightChild);
                successorNode->rightChild = targetNode->rightChild;
                successorNode->rightChild->parentNode = successorNode;
            }

            swapNodes(targetNode, successorNode);
            successorNode->leftChild = targetNode->leftChild;
            successorNode->leftChild->parentNode = successorNode;
            successorNode->nodeColor = targetNode->nodeColor;
        }

        delete targetNode;

        if (savedColor == DARK) {
            repairAfterRemove(replacement);
        }
    }

    void repairAfterRemove(TreeNode* current) {
        while (current != rootNode && current->nodeColor == DARK) {
            if (current == current->parentNode->leftChild) {
                fixRemoveLeftCase(current);
            } else {
                fixRemoveRightCase(current);
            }
        }
        current->nodeColor = DARK;
    }

    void fixRemoveLeftCase(TreeNode* current) {
        TreeNode* sibling = current->parentNode->rightChild;

        if (sibling->nodeColor == CRIMSON) {
            sibling->nodeColor = DARK;
            current->parentNode->nodeColor = CRIMSON;
            spinLeft(current->parentNode);
            sibling = current->parentNode->rightChild;
        }

        if (sibling->leftChild->nodeColor == DARK && 
            sibling->rightChild->nodeColor == DARK) {
            sibling->nodeColor = CRIMSON;
            current = current->parentNode;
        } else {
            if (sibling->rightChild->nodeColor == DARK) {
                sibling->leftChild->nodeColor = DARK;
                sibling->nodeColor = CRIMSON;
                spinRight(sibling);
                sibling = current->parentNode->rightChild;
            }

            sibling->nodeColor = current->parentNode->nodeColor;
            current->parentNode->nodeColor = DARK;
            sibling->rightChild->nodeColor = DARK;
            spinLeft(current->parentNode);
            current = rootNode;
        }
    }

    void fixRemoveRightCase(TreeNode* current) {
        TreeNode* sibling = current->parentNode->leftChild;

        if (sibling->nodeColor == CRIMSON) {
            sibling->nodeColor = DARK;
            current->parentNode->nodeColor = CRIMSON;
            spinRight(current->parentNode);
            sibling = current->parentNode->leftChild;
        }

        if (sibling->rightChild->nodeColor == DARK && 
            sibling->leftChild->nodeColor == DARK) {
            sibling->nodeColor = CRIMSON;
            current = current->parentNode;
        } else {
            if (sibling->leftChild->nodeColor == DARK) {
                sibling->rightChild->nodeColor = DARK;
                sibling->nodeColor = CRIMSON;
                spinLeft(sibling);
                sibling = current->parentNode->leftChild;
            }

            sibling->nodeColor = current->parentNode->nodeColor;
            current->parentNode->nodeColor = DARK;
            sibling->leftChild->nodeColor = DARK;
            spinRight(current->parentNode);
            current = rootNode;
        }
    }

    TreeNode* findMinimum(TreeNode* node) {
        while (node->leftChild != nullNode) {
            node = node->leftChild;
        }
        return node;
    }

    TreeNode* locateNode(TreeNode* node, int val) {
        if (node == nullNode || val == node->value) {
            return node;
        }

        if (val < node->value) {
            return locateNode(node->leftChild, val);
        }
        return locateNode(node->rightChild, val);
    }

    void displayHelper(TreeNode* root, string indent, bool last) {
        if (root != nullNode) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "     ";
            } else {
                cout << "L----";
                indent += "|    ";
            }

            string colorStr = root->nodeColor == CRIMSON ? "RED" : "BLACK";
            cout << root->value << "(" << colorStr << ")\n";
            
            displayHelper(root->leftChild, indent, false);
            displayHelper(root->rightChild, indent, true);
        }
    }

public:
    ColoredBST() {
        nullNode = new TreeNode(0);
        nullNode->nodeColor = DARK;
        nullNode->leftChild = nullNode;
        nullNode->rightChild = nullNode;
        rootNode = nullNode;
    }

    void addValue(int val) {
        TreeNode* newNode = new TreeNode(val);
        newNode->leftChild = nullNode;
        newNode->rightChild = nullNode;

        TreeNode* parentOfNew = nullptr;
        TreeNode* current = this->rootNode;

        while (current != nullNode) {
            parentOfNew = current;
            if (newNode->value < current->value) {
                current = current->leftChild;
            } else {
                current = current->rightChild;
            }
        }

        newNode->parentNode = parentOfNew;

        if (parentOfNew == nullptr) {
            rootNode = newNode;
        } else if (newNode->value < parentOfNew->value) {
            parentOfNew->leftChild = newNode;
        } else {
            parentOfNew->rightChild = newNode;
        }

        if (newNode->parentNode == nullptr) {
            newNode->nodeColor = DARK;
            return;
        }

        if (newNode->parentNode->parentNode == nullptr) {
            return;
        }

        repairAfterAdd(newNode);
    }

    void removeValue(int val) {
        removeNodeProcess(this->rootNode, val);
    }

    TreeNode* findValue(int val) {
        return locateNode(this->rootNode, val);
    }

    void displayTree() {
        displayHelper(this->rootNode, "", true);
    }

    TreeNode* getNullNode() {
        return nullNode;
    }
};

int main() {
    ColoredBST tree;

    cout << "\nCommands: insert <key>, delete <key>, print, exit\n";

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
                cout << "!! Please provide a key to insert.\n";
            } else {
                try {
                    int key = stoi(keyStr);
                    tree.addValue(key);
                    cout << "\n";
                    tree.displayTree();
                } catch (...) {
                    cout << "!! The key must be an integer.\n";
                }
            }
        } else if (cmd == "delete") {
            if (keyStr.empty()) {
                cout << "!! Please provide a key to delete.\n";
            } else {
                try {
                    int key = stoi(keyStr);
                    if (tree.findValue(key) != tree.getNullNode()) {
                        tree.removeValue(key);
                        cout << "\n";
                        tree.displayTree();
                    } else {
                        cout << "!! Key " << key << " not found\n";
                    }
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
            cout << "!! Unknown command. Available commands: insert, delete, print, exit\n";
        }
    }

    return 0;
}