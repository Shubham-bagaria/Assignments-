#include <iostream>
#include <vector>
#include "dictionary.h"
using namespace std;

class BSTNode {
public:
    Word data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Word d) {
        data = d;
        left = right = nullptr;
    }
};

class BST {
public:
    BSTNode* root;
    BST() { root = nullptr; }

    BSTNode* insertHelper(BSTNode* node, Word val) {
        if (!node) return new BSTNode(val);
        if (val.word < node->data.word) node->left = insertHelper(node->left, val);
        else if (val.word > node->data.word) node->right = insertHelper(node->right, val);
        return node;
    }

    void insert(Word val) { root = insertHelper(root, val); }

    void inorderHelper(BSTNode* node) {
        if (!node) return;
        inorderHelper(node->left);
        cout << node->data.word << " : " << node->data.meaning << "\n";
        inorderHelper(node->right);
    }

    void inorder() { inorderHelper(root); }

    BSTNode* searchHelper(BSTNode* node, string key) {
        if (!node || node->data.word == key) return node;
        if (key < node->data.word) return searchHelper(node->left, key);
        return searchHelper(node->right, key);
    }

    Word search(string key) {
        BSTNode* res = searchHelper(root, key);
        if (res) return res->data;
        return Word("Not Found", "Not Found");
    }

    BSTNode* findMin(BSTNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    BSTNode* deleteHelper(BSTNode* node, string key) {
        if (!node) return node;
        if (key < node->data.word) node->left = deleteHelper(node->left, key);
        else if (key > node->data.word) node->right = deleteHelper(node->right, key);
        else {
            if (!node->left) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            BSTNode* temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteHelper(node->right, temp->data.word);
        }
        return node;
    }

    void deleteWord(string key) { root = deleteHelper(root, key); }
};

int main() {
    vector<Word> dict = getDictionary();
    BST tree;
    for (auto &w : dict) tree.insert(w);

    int choice;
    do {
        cout << "\n1. Insert\n2. Search\n3. Delete\n4. Display (Inorder)\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string w, m;
            cout << "Enter word: ";
            cin >> w;
            cout << "Enter meaning: ";
            cin.ignore();
            getline(cin, m);
            tree.insert(Word(w, m));
            cout << "Inserted.\n";
        }
        else if (choice == 2) {
            string key;
            cout << "Enter word to search: ";
            cin >> key;
            Word res = tree.search(key);
            cout << res.word << " : " << res.meaning << "\n";
        }
        else if (choice == 3) {
            string key;
            cout << "Enter word to delete: ";
            cin >> key;
            tree.deleteWord(key);
            cout << "Deleted if it existed.\n";
        }
        else if (choice == 4) {
            cout << "Dictionary contents:\n";
            tree.inorder();
        }
    } while (choice != 5);

    return 0;
}
