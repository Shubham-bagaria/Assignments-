#include <iostream>
#include <vector>
#include "LexiconTree.h"
#include "AlphabetIndex.h"
using namespace std;

class HierarchicalDictionary {
public:
    AlphabetIndex* alphabetTree;
    
    HierarchicalDictionary() {
        alphabetTree = new AlphabetIndex();
    }
    
    void addWord(DictionaryEntry entry) {
        AlphabetNode* charNode = alphabetTree->findCharacter(entry.term[0]);
        
        if(charNode == nullptr || charNode->wordTree == nullptr) {
            alphabetTree->addCharacter(entry.term[0]);
            charNode = alphabetTree->findCharacter(entry.term[0]);
            charNode->wordTree->addEntry(entry);
        } else {
            charNode->wordTree->addEntry(entry);
        }
    }
    
    void bulkLoadSorted(vector<DictionaryEntry> entries, int start, int end) {
        if(start > end) {
            return;
        }
        
        int middle = start + (end - start) / 2;
        addWord(entries[middle]);
        bulkLoadSorted(entries, start, middle - 1);
        bulkLoadSorted(entries, middle + 1, end);
    }
    
    void findWord(string term) {
        AlphabetNode* charNode = alphabetTree->findCharacter(term[0]);
        
        if(charNode == nullptr || charNode->wordTree == nullptr) {
            cout << "\nTerm not found in dictionary\n";
        } else {
            DictionaryEntry result = charNode->wordTree->lookupTerm(term);
            cout << "\nTerm: " << result.term 
                 << "\nDefinition: " << result.meaning << "\n";
        }
    }
    
    void removeWord(string term) {
        AlphabetNode* charNode = alphabetTree->findCharacter(term[0]);
        
        if(charNode == nullptr || charNode->wordTree == nullptr) {
            cout << "\nTerm not found in dictionary\n";
        } else {
            charNode->wordTree->removeEntry(term);
        }
    }
};

int main() {
    vector<DictionaryEntry> entries = loadDictionaryContent();
    HierarchicalDictionary myDict;
    
    cout << "====== Hierarchical Dictionary System ======\n\n";
    cout << "Loading " << entries.size() << " words into dictionary...\n\n";
    
    myDict.bulkLoadSorted(entries, 0, entries.size() - 1);
    
    cout << "\n====== Testing Search Operation ======\n";
    myDict.findWord("Wane");
    
    cout << "\n====== Testing Delete Operation ======\n";
    cout << "Deleting 'Wane' from dictionary...\n";
    myDict.removeWord("Wane");
    
    cout << "\n====== Verifying Deletion ======\n";
    myDict.findWord("Wane");
    
    cout << "\n====== Testing Other Words ======\n";
    myDict.findWord("Zenith");
    myDict.findWord("Alchemy");
    myDict.findWord("Ubiquitous");
    
    cout << "\n====== Program Completed Successfully ======\n";
    
    return 0;
}