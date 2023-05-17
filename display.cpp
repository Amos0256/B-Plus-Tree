#include <iostream>
#include <queue>
#include "BPTree.h"

void BPTree::sequentialDisplay(Node* cursor) {
    Node *firstLeaf = firstLeftLeaf(cursor);

    if (firstLeaf == nullptr) {
        cout << "No Data in the B+ tree yet!" << endl;
        return;
    }

    while (firstLeaf != nullptr) {
        cout << '[';
        for (int i = 0; i < firstLeaf->keys.size(); i++) {
            cout << firstLeaf->keys[i];
            if (i < maxNodeLimit - 1) {
                cout << ',';
            }
        }
        for (int i = firstLeaf->keys.size(); i < maxNodeLimit; i++) {
            cout << '_';
            if(i < maxNodeLimit) {
                cout << ',';
            }
        }
        cout << ']';
        cout << endl;

        firstLeaf = firstLeaf->nextLeaf;
    }
}