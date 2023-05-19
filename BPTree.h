#ifndef BPTREE_H
#define BFTREE_H

#include <iostream>
#include <algorithm>
#include <iomanip>
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
    private:
        // true for leaf, false for others
        bool isLeaf;
        // key vector
        vector<int> keys;
        // pointer to prev node for leaf nodes
        Node *prev;
        // pointer to next node for leaf nodes
        Node *next;
        // array of pointers to children sub-trees for internal Nodes
        vector<Node *> children;
    public:
        // default constructor
        Node();
        // access private members of the class Node
        friend class BPTree;  
        
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
        // get the maximum node limit that is 2 * order of the b+ tree
        // unsigned int getMaxNodeLimit();
        // get the root of b+ tree
        Node* getRoot();
        // set root
        void setRoot(Node *);
        // display the B+ tree
        void display(Node* cursor, unsigned int space = 0);
        // search the key
        void search(int key);
        // insert key(leaf) to b+ tree
        void insertLeaf(int key);
        // insert index(internal node) to b+ tree
        void insertInternal(int index, Node** cursor, Node** child);
        // delete the key(leaf) from b+ tree
        void deleteLeaf(int key);
        // delete index(internal node) form b+ tree
        void deleteInternal(int index, Node* cursor, Node* child);
};

#endif