#include "BPTree.h"

// in leaf node
// 1st parameter: the value wants to insert
void BPTree::insertLeaf(int key) {
    cout << "Call the function insertLeaf" << endl;
    /*
		1. If the node has an empty space, insert the key/reference pair into the node.
		2. If the node is already full, split it into two nodes, distributing the keys
		evenly between the two nodes. If the node is a leaf, take a copy of the minimum
		value in the second of these two nodes and repeat this insertion algorithm to
		insert it into the parent node. If the node is a non-leaf, exclude the middle
		value during the split and repeat this insertion algorithm to insert this excluded
		value into the parent node.
	*/

    // there is no BPTree, insert first node
    if (root == NULL) {
        // create a new node
        root = new Node();

        root->isLeaf = true;
        root->keys.push_back(key);

        cout << key << ": I AM ROOT!!" << endl;
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
            vector<int>::iterator index = upper_bound(cursor->keys.begin(), cursor->keys.end(), key);
            cursor = cursor->children[index - cursor->keys.begin()];
        }

        // the key has saved in the b+ tree
        if (find(cursor->keys.begin(), cursor->keys.end(), key) != cursor->keys.end()) {
            cout << "\033[31mThe key " << key << " already in b+ tree!\033[0m" << endl;
            return ;
        }

        // now cursor is the leaf node in which we'll insert the new key
        // leaf node is not full, find the correct position for new key and then insert it
        if (cursor->keys.size() < maxNodeLimit) {
            // find the position bigger key
            vector<int>::iterator iter = upper_bound(cursor->keys.begin(), cursor->keys.end(), key);
            // cout << iter - cursor->keys.begin() << endl;
            cursor->keys.insert(iter, key);
            
            // for(auto it : cursor->keys) {
            //     cout << it << " ";
            // }
            // cout << endl;

            cout << "Inserted successfully: " << key << endl;
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
            
            // print virtual node
            // for(auto it : virtualNode) {
            //     cout << it << " ";
            // }
            // cout << endl;

            // create a new node for split
            Node *newLeaf = new Node;
            newLeaf->isLeaf = true;

            // swapping the prev and next ptr
            Node *next = cursor->next;
            // original node pointer revise
            cursor->next = newLeaf;
            if(next) {
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
                cout << "Created new Root!" << endl;
            }
            // cursor is not a root
            // insert the new key to its parent
            else {
                // cout << "parent children: " << parent->children.size() << endl;
                // cout << "parent keys: " << parent->keys.size() << endl;
                // cout << "newLeaf keys: " << newLeaf->keys.size() << endl;
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
    // cout << "Call the function insertInternal" << endl;
    // the insertion behavior as same as insertLeaf
    // if cursor is not full find the position for the new key
    if ((*cursor)->keys.size() < maxNodeLimit) {
        // find the position bigger key
        vector<int>::iterator iter = upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), index);
        // found
        (*cursor)->keys.insert(iter, index);

        // the internal parent node add the leaf child node
        (*cursor)->children.push_back(*child);
        cout << "Inserted key in the internal node :)" << endl;
    }
    // damn !!! internal node overflow
    // splitting the node
    else {
        // create a vector to record keys of cursor
        vector<int> virtualKeyNode((*cursor)->keys);
        
        // cout << "cursor: " << (*cursor)->isLeaf << endl;
        // cout << "child: " << (*child)->isLeaf << endl;

        // finding the position for index
        vector<int>::iterator iter = upper_bound(virtualKeyNode.begin(), virtualKeyNode.end(), index);
        virtualKeyNode.insert(iter, index);

        // cout << "virtualKeyNode :" << endl;
        // for(auto it : virtualKeyNode) {
        //     cout << it << ' ';
        // }
        // cout << endl;
        // cout << "cursor children: " << (*cursor)->children.size() << endl;
        
        
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
        // transfer the last child of cursor to newInternalNode 
        newInternalNode->children.insert(newInternalNode->children.begin(), *((*cursor)->children.end() - 1));
        (*cursor)->children.erase((*cursor)->children.end() - 1);
        // transfer the child to newInternalNode
        newInternalNode->children.push_back(*child);

        // if cursor is root after spilt will need a new root
        if ((*cursor) == root) {
            Node *newRoot = new Node;

            // add the new index to new root
            newRoot->keys.push_back(*newInternalNode->keys.begin());
            // the first index of new internal node is pushed up to new root
            newInternalNode->keys.erase(newInternalNode->keys.begin());
            // add the children
            newRoot->children.push_back(*cursor);
            newRoot->children.push_back(newInternalNode);

            root = newRoot;
            // cout << "Created new ROOT!" << endl;
            // for(auto it : newRoot->keys) {
            //     cout << it << ' ';
            // }
            // cout << endl;
        }
        else {
            // record the keys of newInterNode
            int idx = *newInternalNode->keys.begin();
            // the first index of new internal node is pushed up to new root
            newInternalNode->keys.erase(newInternalNode->keys.begin());
            // display(getRoot());
            // display(newInternalNode);
            insertInternal(idx, findParent(root, *cursor), &newInternalNode);
        }
    }
}