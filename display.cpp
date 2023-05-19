#include "BPTree.h"

void BPTree::display(Node* cursor, unsigned int space) {
    // if the cursor is null, then return
    if (!cursor) {
        if(cursor == root) {
            cout << "No Data in the B+ tree yet!" << endl;
        }
        return ;
    }

    // cursor is root or internal node
    if (!cursor->isLeaf) {
        cout << setw(space);
        cout << '(';
        for (int i = 0; i < cursor->keys.size(); i++) {
            cout << cursor->keys[i];
            if (i < maxNodeLimit - 1) {
                cout << ':';
            }
        }
        for (int i = cursor->keys.size(); i < maxNodeLimit; i++) {
            cout << '_';
            if(i < maxNodeLimit - 1) {
                cout << ':';
            }
        }
        cout << ')' << endl;

        // recursive children
        for(auto child : cursor->children) {
            display(child, space + 4);
        }
    }
    // cursor is leaf node
    else {
        cout << setw(space);
        cout << '[';
        for (int i = 0; i < cursor->keys.size(); i++) {
            cout << cursor->keys[i];
            if (i < maxNodeLimit - 1) {
                cout << ',';
            }
        }
        for (int i = cursor->keys.size(); i < maxNodeLimit; i++) {
            cout << '_';
            if(i < maxNodeLimit - 1) {
                cout << ',';
            }
        }
        cout << ']' << endl;
    }
}