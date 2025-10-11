#include <bits/stdc++.h>
using namespace std;

class SkipList {
private:
    struct Node {
        int key;
        vector<Node*> forward;
        Node(int k, int level) : key(k), forward(level + 1, nullptr) {}
    };
    
    const int MAX_LEVEL = 10;
    int level;
    Node* head;

    int randomLevel() {
        int lvl = 0;
        while (((double)rand() / RAND_MAX) < 0.5 && lvl < MAX_LEVEL)
            lvl++;
        return lvl;
    }

public:
    SkipList() {
        level = 0;
        head = new Node(-1, MAX_LEVEL);
        srand(time(0));
    }

    void insert(int key) {
        vector<Node*> update(MAX_LEVEL + 1);
        Node* current = head;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (current && current->key == key) {
            cout << "Key " << key << " already exists.\n";
            return;
        }
        int newLevel = randomLevel();
        if (newLevel > level) {
            for (int i = level + 1; i <= newLevel; i++)
                update[i] = head;
            level = newLevel;
        }
        Node* newNode = new Node(key, newLevel);
        for (int i = 0; i <= newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
        cout << "Key " << key << " inserted successfully.\n";
    }

    bool search(int key) {
        Node* current = head;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
        }
        current = current->forward[0];
        if (current && current->key == key) {
            cout << "Key " << key << " found.\n";
            return true;
        }
        cout << "Key " << key << " not found.\n";
        return false;
    }

    void deleteKey(int key) {
        vector<Node*> update(MAX_LEVEL + 1);
        Node* current = head;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (!current || current->key != key) {
            cout << "Key " << key << " not found. Deletion failed.\n";
            return;
        }
        for (int i = 0; i <= level; i++) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        delete current;
        while (level > 0 && head->forward[level] == nullptr)
            level--;
        cout << "Key " << key << " deleted successfully.\n";
    }

    void printList() {
        for (int i = level; i >= 0; i--) {
            Node* current = head->forward[i];
            cout << "Level " << i << ": Head -> ";
            while (current) {
                cout << current->key << " -> ";
                current = current->forward[i];
            }
            cout << "null\n";
        }
    }
};

int main() {
    SkipList sl;
    // Insert 30 random keys
    for (int i = 0; i < 30; i++)
        sl.insert(rand() % 100);

    sl.printList();

    string command;
    cout << "\nCommands: insert <key>, search <key>, delete <key>, print, exit\n";
    while (true) {
        cout << "\n> ";
        getline(cin, command);
        if (command.empty()) continue;
        stringstream ss(command);
        string cmd;
        ss >> cmd;
        transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
        if (cmd == "insert") {
            int key; if (ss >> key) sl.insert(key); else cout << "!! Provide key\n";
        } else if (cmd == "search") {
            int key; if (ss >> key) sl.search(key); else cout << "!! Provide key\n";
        } else if (cmd == "delete") {
            int key; if (ss >> key) sl.deleteKey(key); else cout << "!! Provide key\n";
        } else if (cmd == "print") sl.printList();
        else if (cmd == "exit") break;
        else cout << "!! Invalid command\n";
    }
    return 0;
}
