#include <iostream>
#include "BalancedTree.h"
using namespace std;

class TreeDemonstration {
public:
    BalancedTree tree;

    void execute() {
        cout << "---- BALANCED TREE DEMONSTRATION ----\n";

        cout << "\nAdding values: 10, 20, 30, 40, 50, 25\n";
        tree.addElement(10);
        tree.addElement(20);
        tree.addElement(30);
        tree.addElement(40);
        tree.addElement(50);
        tree.addElement(25);

        tree.scanInorder();
        tree.scanPreorder();

        cout << "\nLocating value 25:\n";
        BalancedNode* result = tree.locateElement(25);
        if (result) cout << "Element found: " << result->element << endl;
        else cout << "Element not present\n";

        cout << "\nRemoving 40\n";
        tree.removeElement(40);

        tree.scanInorder();
        tree.scanPreorder();

        cout << "\nRemoving 10\n";
        tree.removeElement(10);

        tree.scanInorder();
        tree.scanPreorder();

        cout << "\nAdding 60\n";
        tree.addElement(60);

        tree.scanInorder();
        tree.scanPreorder();

        cout << "\n---- DEMONSTRATION COMPLETE ----\n";
    }
};

int main() {
    TreeDemonstration demo;
    demo.execute();
    return 0;
}