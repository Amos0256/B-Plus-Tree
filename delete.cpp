#include "BPTree.h"

void BPTree::deleteLeaf(int key) {
	Node *root = getRoot();

	// if tree is empty
	if (root == nullptr) {
		cout << "\033[31m\033[03mB+ tree is empty!\033[0m" << endl;
		return ;
	}

	Node *cursor = root;
	Node *parent;
	int leftSibling, rightSibling;

	// go to the leaf node which may contain the key
    while (!cursor->isLeaf) {
        for (int idx = 0; idx < cursor->keys.size(); idx++) {
            parent = cursor;
            // left side of the parent node
            leftSibling = idx - 1;
            // right side of the parent node
            rightSibling = idx + 1;

            // [child_1][key_1][child_2][key_2][child_3]
            if (key < cursor->keys[idx]) {
                cursor = cursor->children[idx];
                break;
            }
            // the last key
            if (idx == cursor->keys.size() - 1) {
                leftSibling = idx;
                rightSibling = idx + 2;
                cursor = cursor->children[idx + 1];
                break;
            }
        }
	}

	// check if the value exists in this leaf node
    vector<int>::iterator iter = find(cursor->keys.begin(), cursor->keys.end(), key);
    // if the key is not in the key vector, return
	if (iter == cursor->keys.end()) {
		cout << "\033[31m\033[03mKey not found in the b+ tree!\033[0m" << endl;
		return ;
	}
    // if the key is in the key vector, delete
    else {
        cursor->keys.erase(iter);
    }

	// if this leaf as well as the root node
	if (cursor == root) {
		if (cursor->keys.size() == 0) {
			// tree becomes empty
			setRoot(nullptr);
			cout << "Ohh!! Our Tree is Empty Now :(" << endl;
		}
	}
	
	cout << "Deleted " << key << " from leaf node successfully" << endl;

    // check whether the underflow happened
    // sufficient node available for invariant to hold, underflow not happened
    // internal node: order <= n <= 2 * order
    // root: 1 <= n <= 2 * order
    if (cursor->keys.size() >= maxNodeLimit / 2 || cursor == root) {
		return ;
	}

	cout << "Damn it !!! UnderFlow in the leaf node happended :(" << endl;
	cout << "Starting Redistribution..." << endl;

	// case 1: try to borrow a key from left sibling
    // left sibling exists and can borrow from it
	if (leftSibling >= 0) {
        cout << "leftSibling: " << leftSibling << endl;
		Node *leftNode = parent->children[leftSibling];

		// check if left sibling has extra key to borrow
		if (leftNode->keys.size() - 1 >= maxNodeLimit / 2) {
			// transfer the maximum key from the left sibling
            vector<int>::iterator leftMaxKey = leftNode->keys.end() - 1;
            // insert the left node maximum key into cursor
            cursor->keys.insert(cursor->keys.begin(), *leftMaxKey);
            // delete the maximum node from leftNode
            leftNode->keys.pop_back();

			// update Parent
			parent->keys[leftSibling] = *cursor->keys.begin();
            cout << "Transferred from left sibling of leaf node" << endl;
			return ;
		}
	}

	// case 2: try to borrow a key from right sibling
    // right sibling exists and can borrow from it
	if (rightSibling < parent->children.size()) {
        cout << "rightSibling: " << rightSibling << endl;
		Node *rightNode = parent->children[rightSibling];

		// check if right sibling has extra key to borrow
		if (rightNode->keys.size() - 1 >= maxNodeLimit / 2) {
			// transfer the minimum key from the right sibling
			vector<int>::iterator rightMinKey = rightNode->keys.begin();
            // insert the left node maximum key into cursor
            cursor->keys.push_back(*rightMinKey);
            // delete the maximum node from leftNode
            rightNode->keys.erase(rightMinKey);
			
			// update Parent
			parent->keys[rightSibling - 1] = *rightNode->keys.begin();
			cout << "Transferred from right sibling of leaf node" << endl;
			return ;
		}
	}
    
    // can not borrow from left or right leaf sibling, then merge and delete node
    // if left sibling exists, merge the left sibling and cursor
	if (leftSibling >= 0) {
		Node *leftNode = parent->children[leftSibling];

		// transfer key to left sibling
        leftNode->keys.insert(leftNode->keys.end(), cursor->keys.begin(), cursor->keys.end());
		// set the next node of left node
		leftNode->next = cursor->next;
		cout << "leftSibling Merging two leaf Nodes" << endl;
        
        // delete parent Node Key
		deleteInternal(parent->keys[leftSibling], parent, cursor);
	}
    // if right sibling exists, merge the left sibling and cursor
	else if (rightSibling <= parent->keys.size()) {
		Node *rightNode = parent->children[rightSibling];
        
        // cout << *cursor->keys.begin() << endl;
        // cout << *rightNode->keys.begin() << endl;
		
        // transfer key to cursor
        cursor->keys.insert(cursor->keys.end(), rightNode->keys.begin(), rightNode->keys.end());
        // set the next node of cursor node
		cursor->next = rightNode->next;
		cout << "rightSibling Merging two leaf Nodes" << endl;
        
        // delete parent Node Key
		deleteInternal(parent->keys[rightSibling - 1], parent, rightNode);
	}
}

void BPTree::deleteInternal(int index, Node* cursor, Node* child) {
	Node* root = getRoot();

	// check if key from root is to deleted
	if (cursor == root) {
        // [child_1][index_1][child_2]
		if (cursor->keys.size() == 1) {
			// if only one key is left and matches with one of the child pointers
			if (cursor->children[1] == child) {
				setRoot(cursor->children[0]);
				// delete cursor;
				cout << "Wow! New Changed Root" <<endl;
				return ;
			}
			else if (cursor->children[0] == child) {
				setRoot(cursor->children[1]);
				// delete cursor;
				cout << "Wow! New Changed Root" << endl;
				return ;
			}
		}
	}

	// delete index from the parent
    vector<int>::iterator idx = find(cursor->keys.begin(), cursor->keys.end(), index);
    cursor->keys.erase(idx);

	// delete the child from the parent
    vector<Node *>::iterator chd = find(cursor->children.begin(), cursor->children.end(), child);
    Node *delChild = *chd;
    cursor->children.erase(chd);
    delete delChild;
    delChild = nullptr;

    // display(cursor);

	// if there is no underflow
	if (cursor->keys.size() >= maxNodeLimit / 2) {
		cout << "Deleted " << index << " from internal node successfully" << endl;
		return ;
	}

	cout << "UnderFlow in internal Node! What did you do :/" << endl;

	if (cursor == root) {
		return ;
	}

	Node **p = findParent(root, cursor);
	// find the parent of cursor
    Node *parent = *p;

	int leftSibling, rightSibling;
	// finding left and right sibling
    for (int idx = 0; idx < parent->children.size(); idx++) {
        if (parent->children[idx] == cursor) {
            leftSibling = idx - 1;
            rightSibling = idx + 1;
            break;
        }
    }

    // case 1: try to borrow a key from left sibling
    // left sibling exists and can borrow from it
	if (leftSibling >= 0) {
		Node* leftNode = parent->children[leftSibling];

		// check if left sibling has extra key to borrow
		if (leftNode->keys.size() - 1 >= maxNodeLimit / 2 ) {
            // transfer the maximum key from the left sibling
            vector<int>::iterator leftMaxKey = leftNode->keys.end() - 1;
            // insert the left node maximum key into cursor
            cursor->keys.insert(cursor->keys.begin(), *leftMaxKey);
            // delete the maximum node from leftNode
            leftNode->keys.pop_back();

			// update Parent
			parent->keys[leftSibling] = *cursor->keys.begin();
            cout << "Transferred from left sibling of internal node" << endl;
			return ;
		}
	}

	// case 2: try to borrow a key from right sibling
    // right sibling exists and can borrow from it
	if (rightSibling < parent->children.size()) {
		Node* rightNode = parent->children[rightSibling];

        // check if right sibling has extra key to borrow
		if (rightNode->keys.size() - 1 >= maxNodeLimit / 2) {
			// transfer the minimum key from the right sibling
			vector<int>::iterator rightMinKey = rightNode->keys.begin();
            // insert the left node maximum key into cursor
            cursor->keys.push_back(*rightMinKey);
            // delete the maximum node from leftNode
            rightNode->keys.erase(rightMinKey);
			
			// update Parent
			parent->keys[rightSibling - 1] = *rightNode->keys.begin();
			cout << "Transferred from right sibling of internal node" << endl;
			return ;
		}
	}

	// can not borrow from left or right internal sibling, then merge and delete node
    // if left sibling exists, merge the left sibling and cursor
	if (leftSibling >= 0) {
		// leftNode + parent key + cursor
		Node *leftNode = parent->children[leftSibling];
        
        // key selection
        // merge parent
        leftNode->keys.push_back(parent->keys[leftSibling]);
        // merge cursor, transfer key to left internal node
        leftNode->keys.insert(leftNode->keys.end(), cursor->keys.begin(), cursor->keys.end());

        // children selection
        // child merge to left internal node
        leftNode->children.insert(leftNode->children.begin(), cursor->children.begin(), cursor->children.end());
		// the pointer of the cursor children set to nullptr
        for(auto &it : cursor->children) {
            it = nullptr;
        }
        // let the keys and children to clear
		cursor->keys.clear();
        cursor->children.clear();
		

		deleteInternal(parent->keys[leftSibling], parent, cursor);
		cout << "Merged with left sibling" << endl;
	}
    // if right sibling exists, merge the left sibling ans cursor
	else if (rightSibling < parent->children.size()) {
		//cursor + parent key +rightNode
		Node* rightNode = parent->children[rightSibling];

        // key selection
        // merge parent
		cursor->keys.push_back(parent->keys[rightSibling - 1]);
        // merge right sibling, tranfer key to cursor
        cursor->keys.insert(cursor->keys.end(), rightNode->keys.begin(), rightNode->keys.end()); 
		
        // children selection
        // child merge to cursor internal node
        cursor->children.insert(cursor->children.end(), rightNode->children.begin(), rightNode->children.end());
		// the pointer of the right node children set to nullptr
        for(auto &it : rightNode->children) {
            it = nullptr;
        }
        // let the keys and children to clear
        rightNode->keys.clear();
        rightNode->children.clear();

		deleteInternal(parent->keys[rightSibling - 1], parent, rightNode);
		cout << "Merged with right sibling" << endl;
	}
}