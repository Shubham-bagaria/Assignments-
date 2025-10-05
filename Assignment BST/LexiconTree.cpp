#include "LexiconTree.h"
#include <iostream>
using namespace std;

// DictionaryEntry Constructors
DictionaryEntry::DictionaryEntry() {
    this->term = "";
    this->meaning = "";
}

DictionaryEntry::DictionaryEntry(string t, string m) {
    this->term = t;
    this->meaning = m;
}

// LexiconNode Constructor
LexiconNode::LexiconNode(DictionaryEntry e) {
    this->entry = e;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

// LexiconTree Constructor
LexiconTree::LexiconTree() {
    this->rootNode = nullptr;
}

// Public method to add entry
void LexiconTree::addEntry(DictionaryEntry entry) {
    this->rootNode = addEntryRecursive(entry, rootNode);
}

// Recursive entry insertion
LexiconNode* LexiconTree::addEntryRecursive(DictionaryEntry input, LexiconNode* currentNode) {
    if(currentNode == nullptr) {
        currentNode = new LexiconNode(input);
    } else {
        if(currentNode->entry.term > input.term) {
            currentNode->leftChild = addEntryRecursive(input, currentNode->leftChild);
        } else if(currentNode->entry.term < input.term) {
            currentNode->rightChild = addEntryRecursive(input, currentNode->rightChild);
        } else {
            cout << currentNode->entry.term << " Entry already exists in dictionary\n";
        }
    }
    return currentNode;
}

// Public method to lookup term
DictionaryEntry LexiconTree::lookupTerm(string term) {
    LexiconNode* result = lookupTermRecursive(term, this->rootNode);
    
    if(result == nullptr) {
        DictionaryEntry notFound = DictionaryEntry("Not Found", "Not Found");
        return notFound;
    } else {
        return result->entry;
    }
}

// Recursive term search
LexiconNode* LexiconTree::lookupTermRecursive(string term, LexiconNode* currentNode) {
    if(currentNode == nullptr) {
        return nullptr;
    }
    
    if(currentNode->entry.term == term) {
        return currentNode;
    } else {
        if(currentNode->entry.term > term) {
            return lookupTermRecursive(term, currentNode->leftChild);
        } else {
            return lookupTermRecursive(term, currentNode->rightChild);
        }
    }
}

// Public method to remove entry
void LexiconTree::removeEntry(string term) {
    this->rootNode = removeEntryRecursive(term, rootNode);
}

// Recursive entry deletion
LexiconNode* LexiconTree::removeEntryRecursive(string term, LexiconNode* currentNode) {
    if(currentNode == nullptr) {
        return nullptr;
    }
    
    if(term > currentNode->entry.term) {
        currentNode->rightChild = removeEntryRecursive(term, currentNode->rightChild);
    } else if(term < currentNode->entry.term) {
        currentNode->leftChild = removeEntryRecursive(term, currentNode->leftChild);
    } else {
        // Leaf node
        if(currentNode->leftChild == nullptr && currentNode->rightChild == nullptr) {
            delete currentNode;
            return nullptr;
        }
        // Single right child
        else if(currentNode->leftChild == nullptr) {
            LexiconNode* temp = currentNode->rightChild;
            currentNode->rightChild = nullptr;
            delete currentNode;
            return temp;
        }
        // Single left child
        else if(currentNode->rightChild == nullptr) {
            LexiconNode* temp = currentNode->leftChild;
            currentNode->leftChild = nullptr;
            delete currentNode;
            return temp;
        }
        // Two children
        else {
            LexiconNode* successor = currentNode->rightChild;
            while(successor->leftChild != nullptr) {
                successor = successor->leftChild;
            }
            
            DictionaryEntry successorEntry = successor->entry;
            currentNode->entry = successorEntry;
            currentNode->rightChild = removeEntryRecursive(successorEntry.term, currentNode->rightChild);
            return currentNode;
        }
    }
    return currentNode;
}

// Public method for preorder display
void LexiconTree::displayPreorder() {
    displayPreorderRecursive(this->rootNode);
}

// Recursive preorder traversal
void LexiconTree::displayPreorderRecursive(LexiconNode* currentNode) {
    if(!currentNode) {
        return;
    } else {
        cout << currentNode->entry.term << "\n";
        displayPreorderRecursive(currentNode->leftChild);
        displayPreorderRecursive(currentNode->rightChild);
    }
}

// Public method for inorder display
void LexiconTree::displayInorder() {
    displayInorderRecursive(this->rootNode);
}

// Recursive inorder traversal
void LexiconTree::displayInorderRecursive(LexiconNode* currentNode) {
    if(!currentNode) {
        return;
    } else {
        displayInorderRecursive(currentNode->leftChild);
        cout << currentNode->entry.term << "\n";
        displayInorderRecursive(currentNode->rightChild);
    }
}

// Load dictionary content
vector<DictionaryEntry> loadDictionaryContent() {
    vector<DictionaryEntry> content = {
        // Section A
        DictionaryEntry("Alchemy", "A medieval chemical science aiming to transmute base metals into gold."),
        DictionaryEntry("Ambiguous", "Open to more than one interpretation; having a double meaning."),
        DictionaryEntry("Anachronism", "A thing belonging to a period other than that in which it exists."),
        DictionaryEntry("Apex", "The top or highest part of something, especially one forming a point."),
        
        // Section B
        DictionaryEntry("Benevolent", "Well meaning and kindly."),
        DictionaryEntry("Bolster", "Support or strengthen; prop up."),
        DictionaryEntry("Brevity", "Concise and exact use of words in writing or speech."),
        DictionaryEntry("Bellicose", "Demonstrating aggression and willingness to fight."),
        
        // Section C
        DictionaryEntry("Cacophony", "A harsh, discordant mixture of sounds."),
        DictionaryEntry("Catalyst", "A substance that increases the rate of a chemical reaction."),
        
        // Section D
        DictionaryEntry("Deft", "Neatly skillful and quick in one's movements."),
        DictionaryEntry("Dichotomy", "A division into two mutually exclusive groups."),
        
        // Section E
        DictionaryEntry("Eloquent", "Fluent or persuasive in speaking or writing."),
        DictionaryEntry("Ephemeral", "Lasting for a very short time."),
        
        // Section F
        DictionaryEntry("Facetious", "Treating serious issues with deliberately inappropriate humor."),
        DictionaryEntry("Fortuitous", "Happening by accident or chance rather than design."),
        
        // Section G
        DictionaryEntry("Gregarious", "Fond of company; sociable."),
        DictionaryEntry("Guile", "Sly or cunning intelligence."),
        
        // Section H
        DictionaryEntry("Hapless", "Unfortunate; unlucky."),
        DictionaryEntry("Harbinger", "A person or thing that announces or signals the approach of another."),
        
        // Section I
        DictionaryEntry("Iconoclast", "A person who attacks cherished beliefs or institutions."),
        DictionaryEntry("Immutable", "Unchanging over time or unable to be changed."),
        
        // Section J
        DictionaryEntry("Juxtapose", "Place or deal with close together for contrasting effect."),
        DictionaryEntry("Judicious", "Having, showing, or done with good judgment or sense."),
        
        // Section K
        DictionaryEntry("Kindle", "Light or set on fire."),
        DictionaryEntry("Kinetic", "Relating to or resulting from motion."),
        
        // Section L
        DictionaryEntry("Lament", "A passionate expression of grief or sorrow."),
        DictionaryEntry("Lucid", "Expressed clearly; easy to understand."),
        
        // Section M
        DictionaryEntry("Malevolent", "Having or showing a wish to do evil to others."),
        DictionaryEntry("Meticulous", "Showing great attention to detail; very careful."),
        
        // Section N
        DictionaryEntry("Nebulous", "In the form of a cloud or haze; hazy."),
        DictionaryEntry("Nostalgic", "Feeling or inspiring a sentimental longing for the past."),
        
        // Section O
        DictionaryEntry("Obfuscate", "Render obscure, unclear, or unintelligible."),
        DictionaryEntry("Opulent", "Ostentatiously rich and luxurious or lavish."),
        
        // Section P
        DictionaryEntry("Paradigm", "A typical example or pattern of something."),
        DictionaryEntry("Prolific", "Producing much fruit or foliage or many offspring."),
        
        // Section Q
        DictionaryEntry("Quaint", "Attractively unusual or old-fashioned."),
        DictionaryEntry("Querulous", "Complaining in a petulant or whining manner."),
        
        // Section R
        DictionaryEntry("Reclusive", "Avoiding the company of other people; solitary."),
        DictionaryEntry("Resilient", "Able to withstand or recover quickly from difficult conditions."),
        
        // Section S
        DictionaryEntry("Sagacious", "Having or showing keen mental discernment and good judgment."),
        DictionaryEntry("Spurious", "Not being what it purports to be; false or fake."),
        
        // Section T
        DictionaryEntry("Taciturn", "Reserved or uncommunicative in speech; saying little."),
        DictionaryEntry("Tenacious", "Tending to keep a firm hold of something."),
        
        // Section U
        DictionaryEntry("Ubiquitous", "Present, appearing, or found everywhere."),
        DictionaryEntry("Unctuous", "Excessively or ingratiatingly flattering; oily."),
        
        // Section V
        DictionaryEntry("Verbose", "Using or expressed in more words than are needed."),
        DictionaryEntry("Voracious", "Wanting or devouring great quantities of food."),
        
        // Section W
        DictionaryEntry("Wane", "Decrease in vigor, power, or extent; become weaker."),
        DictionaryEntry("Wistful", "Having or showing a feeling of vague or regretful longing."),
        
        // Section X
        DictionaryEntry("Xenophobia", "Dislike of or prejudice against people from other countries."),
        
        // Section Y
        DictionaryEntry("Yearning", "A feeling of intense longing for something."),
        
        // Section Z
        DictionaryEntry("Zenith", "The time at which something is most powerful or successful."),
        DictionaryEntry("Zest", "Great enthusiasm and energy.")
    };
    return content;
}