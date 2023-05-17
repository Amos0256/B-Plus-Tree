#include "BPTree.h"

// default constructor
Node::Node() {
    this->isLeaf = false;
    this->prevLeaf = nullptr;
    this->nextLeaf = nullptr;
}

// default constructor
BPTree::BPTree() {
    // default order 1
    this->maxNodeLimit = 2;
    this->root = nullptr;
}

// constructor
BPTree::BPTree(unsigned int order) {
    this->maxNodeLimit = 2 * order;
    this->root = nullptr;
}

// return maximum node
unsigned int BPTree::getMaxNodeLimit() {
    return this->maxNodeLimit;
}

// return root
Node* BPTree::getRoot() {
    return this->root;
}

// set root
void BPTree::setRoot(Node *ptr) {
    this->root = ptr;
}

// find the first left node of the node cursur
Node* BPTree::firstLeftLeaf(Node* cursor) {
    // if node is a leaf node, then it is the first left node
    if (cursor->isLeaf)
        return cursor;
    
    // iterate to find the first non-null node
    for (auto child : cursor->children) {
        if(child != nullptr) {
            return firstLeftLeaf(child);
        }
    }

    return nullptr;
}


Node** BPTree::findParent(Node* cursor, Node* child) {
    static Node *parent;
    /*
		Finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
		also ignores second last level because we will never find parent of a leaf node during insertion using this function
	*/

    if (cursor->isLeaf || cursor->children[0]->isLeaf)
        return nullptr;

    for (auto iter : cursor->children) {
        if (iter == child) {
            parent = cursor;
        }
        else {
            findParent(iter, child);
        }
    }

    return &parent;
}