#include <iostream>
#include "EnhancedBST.h"
using namespace std;

int main() {
    EnhancedBST tree;
    
    cout << "\n=== Building Tree ===\n";
    int elements[] = {50, 30, 20, 40, 70, 60, 80};
    for (int elem : elements) {
        cout << "Adding " << elem << endl;
        tree.addElement(elem);
    }
    
    cout << "\nPreorder sequence: ";
    tree.traversePreorder();
    cout << "\nInorder sequence: ";
    tree.traverseInorder();
    cout << "\nPostorder sequence: ";
    tree.traversePostorder();
    cout << "\nLevel-order sequence: ";
    tree.traverseLevelOrder();
    
    cout << "\n\nHierarchical display:\n";
    tree.displayByLevels();
    
    cout << "\nTotal nodes: " << tree.getTotalNodes();
    cout << "\nLeaf nodes: " << tree.countLeaves();
    cout << "\nTree depth: " << tree.computeHeight();
    cout << "\nTree diameter: " << tree.computeDiameter();
    cout << "\nStructure valid? " << (tree.validateStructure() ? "Yes" : "No");
    cout << "\nTree balanced? " << (tree.isBalanced() ? "Yes" : "No");
    
    int target = 40;
    cout << "\n\nLocating element " << target << ": ";
    TreeNode* found = tree.findElement(target);
    if (found)
        cout << "Located with value = " << found->value;
    else
        cout << "Element not present";
    
    cout << "\n\nLCA of 20 and 40: ";
    int ancestor = tree.findLCA(20, 40);
    if (ancestor != INT_MAX)
        cout << ancestor;
    else
        cout << "Not found";
    
    cout << "\n\nPosition of 60: " << tree.getNodeRank(60);
    
    int k = 3;
    cout << "\n\n3rd maximum: " << tree.findKthMax(k);
    
    cout << "\n\nRemoving 20...";
    tree.removeElement(20);
    cout << "\nInorder after removal: ";
    tree.traverseInorder();
    
    cout << "\n\nRemoving 30...";
    tree.removeElement(30);
    cout << "\nInorder after removal: ";
    tree.traverseInorder();
    
    cout << "\n\nRemoving 50 (root)...";
    tree.removeElement(50);
    cout << "\nInorder after removal: ";
    tree.traverseInorder();
    
    cout << "\n\nFinal level-order: ";
    tree.traverseLevelOrder();
    cout << endl;
    
    return 0;
}