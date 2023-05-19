#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "BPTree.h"

void insertionMethod(BPTree** bPTree) {
    int number;

    cout << "Enter the number wants to be inserted: ";
    cin >> number;

    (*bPTree)->insertLeaf(number);
    cout << "Insert key " << number << " :" << endl;
    
    // display
    (*bPTree)->display((*bPTree)->getRoot());
}


void deleteMethod(BPTree* bPTree) {
    int number;

    cout << "Enter the number wants to be deleted: ";
    cin >> number;

    bPTree->deleteLeaf(number);

    // display
    bPTree->display(bPTree->getRoot());
}

// void searchMethod(BPTree* bPTree) {
//     int rollNo;
//     cout << "What's the RollNo to Search? ";
//     cin >> rollNo;

//     bPTree->search(rollNo);
// }

void displayMethod(BPTree* bPTree) {
    bPTree->display(bPTree->getRoot());
}

int main() {
    // order of B+ tree 
    unsigned int order;
    // operation mode
    unsigned int mode;

    cout << "Input the order of B+ tree: ";
    cin >> order;
    BPTree* bPTree = new BPTree(order);

    // int maxChildInt = 4, maxNodeLeaf = 3;
    // cout << "Please provide the value to limit maximum child Internal Nodes can have: ";
    // cin >> maxChildInt;
    // cout << "\nAnd Now Limit the value to limit maximum Nodes Leaf Nodes can have: ";
    // cin >> maxNodeLeaf;
    
    bool flag = true;
    while (flag) {
        cout << endl;
        cout << "Select the operation:" << endl;
        cout << "Press [1]: Insert" << endl;
        cout << "Press [2]: Delete" << endl;
        cout << "Press [3]: Display" << endl;
        cout << "Press [4]: Quit" << endl;
        cin >> mode;

        switch (mode) {
            case 1:
                insertionMethod(&bPTree);
                break;
            case 2:
                deleteMethod(bPTree);
                break;
            case 3:
                displayMethod(bPTree);
                break;
            default:
                flag = false;
                break;
        }
    }

    return 0;
}
