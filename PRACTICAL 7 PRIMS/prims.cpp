#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <algorithm>
using namespace std;

const int INF = numeric_limits<int>::max();

class FibNode {
public:
    int vertex;
    int priority;
    int degree;
    bool marked;
    FibNode* parentPtr;
    FibNode* childPtr;
    FibNode* leftSib;
    FibNode* rightSib;

    FibNode(int v, int p) {
        vertex = v;
        priority = p;
        degree = 0;
        marked = false;
        parentPtr = nullptr;
        childPtr = nullptr;
        leftSib = this;
        rightSib = this;
    }
};

class FibonacciHeap {
private:
    FibNode* minNode;
    int totalNodes;
    unordered_map<int, FibNode*> nodeMap;

    void linkNodes(FibNode* child, FibNode* parent) {
        // Remove child from root list
        child->leftSib->rightSib = child->rightSib;
        child->rightSib->leftSib = child->leftSib;

        // Make child a child of parent
        child->parentPtr = parent;
        if (parent->childPtr == nullptr) {
            parent->childPtr = child;
            child->rightSib = child;
            child->leftSib = child;
        } else {
            child->leftSib = parent->childPtr;
            child->rightSib = parent->childPtr->rightSib;
            parent->childPtr->rightSib->leftSib = child;
            parent->childPtr->rightSib = child;
        }

        parent->degree++;
        child->marked = false;
    }

    void consolidateHeap() {
        int maxDegree = static_cast<int>(log2(totalNodes)) + 1;
        vector<FibNode*> degreeTable(maxDegree + 1, nullptr);

        vector<FibNode*> rootList;
        FibNode* current = minNode;
        if (current != nullptr) {
            do {
                rootList.push_back(current);
                current = current->rightSib;
            } while (current != minNode);
        }

        for (FibNode* w : rootList) {
            FibNode* x = w;
            int d = x->degree;

            while (degreeTable[d] != nullptr) {
                FibNode* y = degreeTable[d];
                if (x->priority > y->priority) {
                    swap(x, y);
                }
                linkNodes(y, x);
                degreeTable[d] = nullptr;
                d++;
            }
            degreeTable[d] = x;
        }

        minNode = nullptr;
        for (FibNode* node : degreeTable) {
            if (node != nullptr) {
                if (minNode == nullptr) {
                    minNode = node;
                    node->leftSib = node;
                    node->rightSib = node;
                } else {
                    node->leftSib = minNode;
                    node->rightSib = minNode->rightSib;
                    minNode->rightSib->leftSib = node;
                    minNode->rightSib = node;
                    if (node->priority < minNode->priority) {
                        minNode = node;
                    }
                }
            }
        }
    }

    void cutNode(FibNode* x, FibNode* y) {
        // Remove x from child list of y
        if (x->rightSib == x) {
            y->childPtr = nullptr;
        } else {
            x->leftSib->rightSib = x->rightSib;
            x->rightSib->leftSib = x->leftSib;
            if (y->childPtr == x) {
                y->childPtr = x->rightSib;
            }
        }
        y->degree--;

        // Add x to root list
        x->leftSib = minNode;
        x->rightSib = minNode->rightSib;
        minNode->rightSib->leftSib = x;
        minNode->rightSib = x;

        x->parentPtr = nullptr;
        x->marked = false;
    }

    void cascadingCut(FibNode* y) {
        FibNode* z = y->parentPtr;
        if (z != nullptr) {
            if (!y->marked) {
                y->marked = true;
            } else {
                cutNode(y, z);
                cascadingCut(z);
            }
        }
    }

public:
    FibonacciHeap() {
        minNode = nullptr;
        totalNodes = 0;
    }

    void insertNode(int vertex, int priority) {
        FibNode* node = new FibNode(vertex, priority);
        nodeMap[vertex] = node;

        if (minNode == nullptr) {
            minNode = node;
        } else {
            node->leftSib = minNode;
            node->rightSib = minNode->rightSib;
            minNode->rightSib->leftSib = node;
            minNode->rightSib = node;
            if (node->priority < minNode->priority) {
                minNode = node;
            }
        }
        totalNodes++;
    }

    FibNode* extractMinimum() {
        FibNode* z = minNode;
        if (z != nullptr) {
            // Add all children to root list
            if (z->childPtr != nullptr) {
                FibNode* child = z->childPtr;
                do {
                    FibNode* next = child->rightSib;
                    child->leftSib = minNode;
                    child->rightSib = minNode->rightSib;
                    minNode->rightSib->leftSib = child;
                    minNode->rightSib = child;
                    child->parentPtr = nullptr;
                    child = next;
                } while (child != z->childPtr);
            }

            // Remove z from root list
            z->leftSib->rightSib = z->rightSib;
            z->rightSib->leftSib = z->leftSib;

            if (z == z->rightSib) {
                minNode = nullptr;
            } else {
                minNode = z->rightSib;
                consolidateHeap();
            }
            totalNodes--;
            nodeMap.erase(z->vertex);
        }
        return z;
    }

    void decreaseKey(int vertex, int newPriority) {
        FibNode* x = nodeMap[vertex];
        if (x == nullptr || newPriority > x->priority) {
            return;
        }

        x->priority = newPriority;
        FibNode* y = x->parentPtr;

        if (y != nullptr && x->priority < y->priority) {
            cutNode(x, y);
            cascadingCut(y);
        }

        if (x->priority < minNode->priority) {
            minNode = x;
        }
    }

    bool isEmpty() {
        return minNode == nullptr;
    }

    bool containsVertex(int vertex) {
        return nodeMap.find(vertex) != nodeMap.end();
    }
};

class GraphMST {
private:
    int numVertices;
    vector<vector<pair<int, int>>> adjacencyList;

public:
    GraphMST(int vertices) {
        numVertices = vertices;
        adjacencyList.resize(vertices);
    }

    void addEdge(int src, int dest, int weight) {
        adjacencyList[src].push_back({dest, weight});
        adjacencyList[dest].push_back({src, weight});
    }

    void computePrimMST(int startVertex) {
        vector<int> keyValues(numVertices, INF);
        vector<int> parentNodes(numVertices, -1);
        vector<bool> inMST(numVertices, false);

        FibonacciHeap heap;

        keyValues[startVertex] = 0;
        for (int i = 0; i < numVertices; i++) {
            heap.insertNode(i, keyValues[i]);
        }

        cout << "\nComputing Minimum Spanning Tree using Prim's Algorithm:\n";
        cout << "========================================================\n\n";

        int mstWeight = 0;
        vector<pair<int, int>> mstEdges;

        while (!heap.isEmpty()) {
            FibNode* minNode = heap.extractMinimum();
            int u = minNode->vertex;
            inMST[u] = true;

            if (parentNodes[u] != -1) {
                mstEdges.push_back({parentNodes[u], u});
                mstWeight += keyValues[u];
                cout << "Added edge: " << parentNodes[u] << " - " << u 
                     << " (weight: " << keyValues[u] << ")\n";
            }

            for (auto& edge : adjacencyList[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (!inMST[v] && weight < keyValues[v] && heap.containsVertex(v)) {
                    parentNodes[v] = u;
                    keyValues[v] = weight;
                    heap.decreaseKey(v, weight);
                }
            }

            delete minNode;
        }

        cout << "\n========================================================\n";
        cout << "Minimum Spanning Tree Edges:\n";
        for (auto& edge : mstEdges) {
            cout << edge.first << " - " << edge.second << "\n";
        }
        cout << "\nTotal MST Weight: " << mstWeight << "\n";
        cout << "========================================================\n";
    }

    void displayGraph() {
        cout << "\nGraph Adjacency List:\n";
        cout << "=====================\n";
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i << ": ";
            for (auto& edge : adjacencyList[i]) {
                cout << "(" << edge.first << ", " << edge.second << ") ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
};

int main() {
    cout << "=== Prim's Algorithm with Fibonacci Heap ===\n\n";

    int vertices, edges;
    cout << "Enter number of vertices: ";
    cin >> vertices;

    GraphMST graph(vertices);

    cout << "Enter number of edges: ";
    cin >> edges;

    cout << "\nEnter edges (source destination weight):\n";
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        cout << "Edge " << (i + 1) << ": ";
        cin >> src >> dest >> weight;
        graph.addEdge(src, dest, weight);
    }

    graph.displayGraph();

    int startVertex;
    cout << "Enter starting vertex for MST: ";
    cin >> startVertex;

    graph.computePrimMST(startVertex);

    return 0;
}