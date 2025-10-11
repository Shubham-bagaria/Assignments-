#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <unordered_map>
using namespace std;

class HeapNode {
public:
    int element;
    HeapNode* firstChild;
    HeapNode* nextSibling;
    HeapNode* prevNode;

    HeapNode(int val) {
        element = val;
        firstChild = nullptr;
        nextSibling = nullptr;
        prevNode = nullptr;
    }
};

class PairingHeapStructure {
private:
    HeapNode* rootNode;
    int heapSize;
    unordered_map<int, HeapNode*> elementMap;

    HeapNode* mergeNodes(HeapNode* h1, HeapNode* h2) {
        if (h1 == nullptr) return h2;
        if (h2 == nullptr) return h1;

        if (h1->element < h2->element) {
            attachChild(h1, h2);
            return h1;
        } else {
            attachChild(h2, h1);
            return h2;
        }
    }

    void attachChild(HeapNode* parent, HeapNode* child) {
        child->prevNode = parent;
        child->nextSibling = parent->firstChild;
        
        if (parent->firstChild != nullptr) {
            parent->firstChild->prevNode = child;
        }
        
        parent->firstChild = child;
    }

    HeapNode* combineSiblings(HeapNode* firstSib) {
        if (firstSib == nullptr || firstSib->nextSibling == nullptr) {
            return firstSib;
        }

        vector<HeapNode*> treeArray;
        
        while (firstSib != nullptr) {
            treeArray.push_back(firstSib);
            firstSib->prevNode->nextSibling = nullptr;
            firstSib = firstSib->nextSibling;
        }

        // First pass: merge pairs left to right
        int i = 0;
        for (; i + 1 < treeArray.size(); i += 2) {
            treeArray[i] = mergeNodes(treeArray[i], treeArray[i + 1]);
        }

        int j = i - 2;
        
        // Handle odd number of trees
        if (j == treeArray.size() - 3) {
            treeArray[j] = mergeNodes(treeArray[j], treeArray[j + 2]);
        }

        // Second pass: merge from right to left
        for (; j >= 2; j -= 2) {
            treeArray[j - 2] = mergeNodes(treeArray[j - 2], treeArray[j]);
        }

        return treeArray[0];
    }

    void displaySubtree(HeapNode* node, string indent, bool isLast) {
        if (node != nullptr) {
            cout << indent;
            
            if (isLast) {
                cout << "\\-- ";
                indent += "    ";
            } else {
                cout << "|-- ";
                indent += "|   ";
            }
            
            cout << node->element << "\n";

            HeapNode* child = node->firstChild;
            while (child != nullptr) {
                bool lastChild = (child->nextSibling == nullptr);
                displaySubtree(child, indent, lastChild);
                child = child->nextSibling;
            }
        }
    }

public:
    PairingHeapStructure() {
        rootNode = nullptr;
        heapSize = 0;
    }

    void addElement(int value) {
        HeapNode* newNode = new HeapNode(value);
        elementMap[value] = newNode;
        rootNode = mergeNodes(rootNode, newNode);
        heapSize++;
        cout << "Inserted element: " << value << "\n";
    }

    int getMinimum() {
        if (rootNode == nullptr) {
            cout << "Heap is empty!\n";
            return -1;
        }
        return rootNode->element;
    }

    int removeMinimum() {
        if (rootNode == nullptr) {
            cout << "Heap is empty!\n";
            return -1;
        }

        int minValue = rootNode->element;
        HeapNode* oldRoot = rootNode;
        
        if (rootNode->firstChild == nullptr) {
            rootNode = nullptr;
        } else {
            rootNode = combineSiblings(rootNode->firstChild);
        }

        elementMap.erase(minValue);
        delete oldRoot;
        heapSize--;
        
        cout << "Deleted minimum: " << minValue << "\n";
        return minValue;
    }

    void reduceKey(int oldValue, int newValue) {
        if (elementMap.find(oldValue) == elementMap.end()) {
            cout << "Element " << oldValue << " not found in heap!\n";
            return;
        }

        if (newValue > oldValue) {
            cout << "New value must be smaller than old value!\n";
            return;
        }

        HeapNode* node = elementMap[oldValue];
        node->element = newValue;

        elementMap.erase(oldValue);
        elementMap[newValue] = node;

        if (node != rootNode) {
            // Remove node from its current position
            if (node->nextSibling != nullptr) {
                node->nextSibling->prevNode = node->prevNode;
            }

            if (node->prevNode->firstChild == node) {
                node->prevNode->firstChild = node->nextSibling;
            } else {
                node->prevNode->nextSibling = node->nextSibling;
            }

            node->nextSibling = nullptr;
            rootNode = mergeNodes(rootNode, node);
        }

        cout << "Decreased key from " << oldValue << " to " << newValue << "\n";
    }

    void combineHeaps(PairingHeapStructure& other) {
        rootNode = mergeNodes(rootNode, other.rootNode);
        heapSize += other.heapSize;
        
        for (auto& pair : other.elementMap) {
            elementMap[pair.first] = pair.second;
        }
        
        other.rootNode = nullptr;
        other.heapSize = 0;
        other.elementMap.clear();
        
        cout << "Merged heaps successfully\n";
    }

    bool checkEmpty() {
        return rootNode == nullptr;
    }

    int getSize() {
        return heapSize;
    }

    void printStructure() {
        if (rootNode == nullptr) {
            cout << "Heap is empty!\n";
            return;
        }
        
        cout << "\nHeap Structure:\n";
        cout << "===============\n";
        displaySubtree(rootNode, "", true);
        cout << "\n";
    }

    void printMinimum() {
        if (rootNode == nullptr) {
            cout << "Heap is empty!\n";
        } else {
            cout << "Minimum element: " << rootNode->element << "\n";
        }
    }
};

int main() {
    cout << "=== Pairing Heap Data Structure ===\n\n";
    
    PairingHeapStructure heap;
    
    cout << "Commands:\n";
    cout << "  insert <value>     - Insert element\n";
    cout << "  delete-min         - Remove minimum element\n";
    cout << "  find-min           - Display minimum element\n";
    cout << "  decrease <old> <new> - Decrease key\n";
    cout << "  print              - Display heap structure\n";
    cout << "  size               - Display heap size\n";
    cout << "  exit               - Exit program\n\n";

    while (true) {
        cout << "> ";
        string inputLine;
        getline(cin, inputLine);

        if (inputLine.empty()) continue;

        stringstream ss(inputLine);
        string cmd;
        ss >> cmd;

        transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (cmd == "insert") {
            int value;
            if (ss >> value) {
                heap.addElement(value);
            } else {
                cout << "Usage: insert <value>\n";
            }
        }
        else if (cmd == "delete-min") {
            heap.removeMinimum();
        }
        else if (cmd == "find-min") {
            heap.printMinimum();
        }
        else if (cmd == "decrease") {
            int oldVal, newVal;
            if (ss >> oldVal >> newVal) {
                heap.reduceKey(oldVal, newVal);
            } else {
                cout << "Usage: decrease <old_value> <new_value>\n";
            }
        }
        else if (cmd == "print") {
            heap.printStructure();
        }
        else if (cmd == "size") {
            cout << "Heap size: " << heap.getSize() << "\n";
        }
        else if (cmd == "exit") {
            cout << "Exiting program...\n";
            break;
        }
        else {
            cout << "Unknown command!\n";
        }
    }

    return 0;
}