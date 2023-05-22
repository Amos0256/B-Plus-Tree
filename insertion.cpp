#include "BPTree.h"

extern bool insertSuccess;

// in leaf node
// 1st parameter: the value wants to insert
void BPTree::insertLeaf(int key) {
    // cout << "Call the function insertLeaf" << endl;
    // there is no BPTree, insert first node
    if (root == NULL) {
        // create a new node
        root = new Node();

        root->isLeaf = true;
        root->keys.push_back(key);

        return;
    }
    // there is a BPTree
    else {
        // cursor will recursively find the correct place
        Node *cursor = root;
        Node *parent = NULL;
        // searching for the possible position for the given key
        // recursive if cursor is an internal node
        while (cursor->isLeaf == false) {
            parent = cursor;
            
            // find the children pointer that correct position should be
            int index;
            for (index = 0; index < cursor->keys.size(); index++) {
                if(key < cursor->keys[index]) {
                    break;
                }
            }
            cursor = cursor->children[index];
        }

        // the key has saved in the b+ tree
        if (find(cursor->keys.begin(), cursor->keys.end(), key) != cursor->keys.end()) {
            cout << "\033[31m\033[03mThe key " << key << " already in b+ tree!\033[0m" << endl;
            insertSuccess = false;
            return ;
        }

        // now cursor is the leaf node in which we'll insert the new key
        // leaf node is not full, find the correct position for new key and then insert it
        if (cursor->keys.size() < maxNodeLimit) {
            // find the position bigger key
            vector<int>::iterator iter = upper_bound(cursor->keys.begin(), cursor->keys.end(), key);
            cursor->keys.insert(iter, key);
        } 
        // damn !!! leaf node overflow
        // splitting the node
        else {
            // create a vector to record keys of cursor
            vector<int> virtualNode(cursor->keys);
            // vector<FILE*> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);

            //finding the probable place to insert the key
            vector<int>::iterator iter = upper_bound(virtualNode.begin(), virtualNode.end(), key);
            virtualNode.insert(iter, key);

            // create a new node for split
            Node *newLeaf = new Node;
            newLeaf->isLeaf = true;

            // swapping the prev and next ptr
            Node *next = cursor->next;
            // original node pointer revise
            cursor->next = newLeaf;
            if (next) {
                next->prev = newLeaf;
            }
            // new node pointer revise
            newLeaf->prev = cursor;
            newLeaf->next = next;

            // resizing and copying the keys to old node cursor
            cursor->keys.resize(maxNodeLimit / 2);
            // cursor->ptr2TreeOrData.dataPtr.resize((maxLeafNodeLimit) / 2 +1);
            for (int i = 0; i < maxNodeLimit / 2; i++) {
                cursor->keys[i] = virtualNode[i];
            }

            // pushing remain keys to new node newLeaf
            for (int i = maxNodeLimit / 2; i < virtualNode.size(); i++) {
                newLeaf->keys.push_back(virtualNode[i]);
            }

            // if cursor is root after spilt will need a new root
            if (cursor == root) {
                Node *newRoot = new Node;
                
                newRoot->keys.push_back(*newLeaf->keys.begin());
                newRoot->children.push_back(cursor);
                newRoot->children.push_back(newLeaf);

                root = newRoot;
            }
            // cursor is not a root
            // insert the new key to its parent
            else {
                // display(parent);
                // display(newLeaf);
                insertInternal(*newLeaf->keys.begin(), &parent, &newLeaf);
            }
        }
    }
}

// in internal node
// 1st parameter: index
// 2nd parameter: the pointer of cursor(parent)
// 3rd parameter: the pointer of child
void BPTree::insertInternal(int index, Node** cursor, Node** child) {
    // the insertion behavior as same as insertLeaf
    // if cursor is not full find the position for the new key
    if ((*cursor)->keys.size() < maxNodeLimit) {
        // find the position bigger key
        int idx;
        for (idx = 0; idx < (*cursor)->keys.size(); idx++) {
            if (index < (*cursor)->keys[idx]) {
                break;
            }
        }
        
        // found
        (*cursor)->keys.insert(next((*cursor)->keys.begin(), idx), index);

        // the internal parent node add the leaf child node
        (*cursor)->children.insert(next((*cursor)->children.begin(), idx + 1), *child);
        // cout << "Inserted key in the internal node :)" << endl;
    }
    // damn !!! internal node overflow
    // splitting the node
    else {
        // create a vector to record keys of cursor
        vector<int> virtualKeyNode((*cursor)->keys);

        // finding the position for index
        vector<int>::iterator iter = upper_bound(virtualKeyNode.begin(), virtualKeyNode.end(), index);
        virtualKeyNode.insert(iter, index);

        for (auto it : virtualKeyNode) {
            cout << it << ' ';
        }
        cout << endl;
        
        // resizing and copying the keys to old node cursor
        (*cursor)->keys.resize(maxNodeLimit / 2);
        // (*cursor)->children.resize(maxNodeLimit / 2);
        for (int i = 0; i < maxNodeLimit / 2; i++) {
            (*cursor)->keys[i] = virtualKeyNode[i];
        }

        // resizing and copying the keys to old node cursor
        Node *newInternalNode = new Node;
        for (int i = maxNodeLimit / 2; i < virtualKeyNode.size(); i++) {
            newInternalNode->keys.push_back(virtualKeyNode[i]);
        }

        // display(*cursor);
        // display(newInternalNode);

        // transfer the child of cursor to newInternalNode
        for (vector<Node *>::iterator iter = (*cursor)->children.begin(); iter < (*cursor)->children.end(); iter++) {
            if (*(*iter)->keys.begin() > *(*cursor)->keys.begin()) {
                newInternalNode->children.push_back(*iter);
                (*cursor)->children.erase(iter);
                iter--;
            }
        }
        
        display(*cursor);
        display(newInternalNode);
        display(*child);
        if (*(*child)->keys.begin() < *newInternalNode->keys.begin()) {
            (*cursor)->children.push_back(*child);
        }

        // if cursor is root after spilt will need a new root
        if ((*cursor) == root) {
            Node *newRoot = new Node;

            // add the new index to new root
            newRoot->keys.push_back(*newInternalNode->keys.begin());
            // the first index of new internal node is pushed up to new root
            newInternalNode->keys.erase(newInternalNode->keys.begin());

            // check if the newInternalNode has incorrect children
            if ((*cursor)->children.size() < maxNodeLimit) {
                (*cursor)->children.push_back(*newInternalNode->children.begin());
                newInternalNode->children.erase(newInternalNode->children.begin());
            }

            if (*(*child)->keys.begin() > *newRoot->keys.begin()) {
                int idx;
                for (idx = 0; idx < newInternalNode->keys.size(); idx++) {
                    if (*(*child)->keys.begin() < newInternalNode->keys[idx]) {
                        break;
                    }
                }
                newInternalNode->children.insert(next(newInternalNode->children.begin(), idx), *child);
            }
            // else {
            //     (*cursor)->children.push_back(*child);
            // }

            // add the children
            newRoot->children.push_back(*cursor);
            newRoot->children.push_back(newInternalNode);

            root = newRoot;
        }
        else {
            // record the keys of newInterNode
            int firstKey = *newInternalNode->keys.begin();
            // the first index of new internal node is pushed up to new root
            newInternalNode->keys.erase(newInternalNode->keys.begin());
            // adjust children
            if (*(*newInternalNode->children.begin())->keys.begin() < firstKey) {
               (*cursor)->children.push_back(*newInternalNode->children.begin());
                newInternalNode->children.erase(newInternalNode->children.begin());
            }
            
            int idx;
            for (idx = 0; idx < newInternalNode->keys.size(); idx++) {
                if (*(*child)->keys.begin() < newInternalNode->keys[idx]) {
                    break;
                }
            }
            newInternalNode->children.insert(next(newInternalNode->children.begin(), idx), (*child));

            insertInternal(firstKey, findParent(root, *cursor), &newInternalNode);
        }
    }
}