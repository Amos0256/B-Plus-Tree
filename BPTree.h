#ifndef BPTREE_H
#define BFTREE_H

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class Node {
    /*
		Generally size of the this node should be equal to the block size. 
        Which will limit the number of disk access and increase the accesssing time.
		Intermediate nodes only hold the Tree pointers which is of considerably small size
        (so they can hold more Tree pointers) and only Leaf nodes hold the data pointer directly to the disc.

		IMPORTANT := All the data has to be present in the leaf node
	*/
    public:
        // true for leaf, false for others
        bool isLeaf;
        // key
        vector<int> keys;
        // pointer to prev node for leaf nodes
        Node *prevLeaf;
        // pointer to next node for leaf nodes
        Node *nextLeaf;
        
        // array of pointers to children sub-trees for internal Nodes
        vector<Node *> children;
        
        // access private members of the class Node
        friend class BPTree;  
        
        // constructor
        Node();
};

class BPTree {
    /*
		::For Root Node :=
			The root node has, at least two tree pointers
		::For Internal Nodes:=
			1. ceil(maxIntChildLimit/2)     <=  #of children <= maxIntChildLimit
			2. ceil(maxIntChildLimit/2)-1  <=  #of keys     <= maxIntChildLimit -1
		::For Leaf Nodes :=
			1. ceil(maxLeafNodeLimit/2)   <=  #of keys     <= maxLeafNodeLimit -1
	*/
    private:
        // # of node, 2 * order
        unsigned int maxNodeLimit;
        // pointer to the B+ Tree root
        Node* root;
        
        Node** findParent(Node* cursor, Node* child);
        Node* firstLeftLeaf(Node* cursor);

    public:
        // default constructor, default order is 1
        BPTree();
        // constructor
        BPTree(unsigned int order);
        Node* getRoot();
        unsigned int getMaxNodeLimit();
        void setRoot(Node *);
        // void display(Node* cursor);
        // sequential display the B+ tree
        void sequentialDisplay(Node* cursor);
        void search(int key);
        // insert number
        void insertLeaf(int key);
        // insert index from child in cursor(parent)
        void insertInternal(int index, Node** cursor, Node** child);
        void removeKey(int key);
        void removeInternal(int x, Node* cursor, Node* child);
};

#endif