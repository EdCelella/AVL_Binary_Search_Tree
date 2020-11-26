#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>
//#define NDEBUG
#include <cassert>

template<typename K, typename I> 
class AVL{

	public:
		using Key = K;
		using Item = I;
        using Balance = int;

		AVL() = default; // Constructor
        ~AVL(); // Destructor
        AVL(const AVL&); // Copy Constructor
        AVL& operator=(const AVL&); // Copy Assignment Operator
        AVL(AVL&&); // Move Constructor
        AVL & operator=(AVL&&); // Move Assignment Operator

		void insert(Key, Item);
    	Item* lookup(Key);
        void remove(Key);

        void displayEntries();
        void displayTree();
        
    private: 
    	struct Node; 
    	Node* root = nullptr;

    	static bool isLeaf(Node*);
        static bool noChildren(Node*);

        static void deepDelete(Node*&);
        static Node* deepCopy(Node*);
        static Node* moveTree(Node*&);

    	static Item* lookupRec(Key, Node*);
    	static bool insertRec(Key, Item, Node*&);
        static bool removeRec(Key, Node*&);
        static std::pair<Node*,bool> detachMinimumNode(Node*&);

        static void displayEntriesRec(Node*);
        static void displayTreeRec(Node*, int);
        static void displayOverloadRec(std::ostream& os, Node*);

        static void rotateRight(Node*&);
        static void rotateLeft(Node*&);
        static void balanceFactor(Balance&, Balance&, int);
        static bool rebalance(Node*&);
    
    template<typename M, typename N>
    friend std::ostream& operator<<(std::ostream&, const AVL<M,N>&);

};

template<typename K, typename I>
std::ostream& operator<<(std::ostream&, const AVL<K,I>&);

// Binary tree node
template<typename K, typename I>
struct AVL<K,I>::Node {

    Key key;
    Item item;
    Balance balance;

    Node* leftChild;
    Node* rightChild;

    Node(Key newK, Item newI);

};

// Node constructor
template<typename K, typename I>
AVL<K,I>::Node::Node(Key _key, Item _item){

    key = _key;
    item = _item;
    balance = 0;
    leftChild = nullptr;
    rightChild = nullptr;

}

// Checks if node is a null pointer
template<typename K, typename I>
bool AVL<K,I>::isLeaf(Node* n) {

    if(n == nullptr){
        return true;
    }
    else{
        return false;
    }
    
}

// Checks if node has no children
template<typename K, typename I>
bool AVL<K,I>::noChildren(Node* n){

    if(!n->leftChild && !n->rightChild){
        return true;
    }
    else{
        return false;
    }
    
}

// Finds node with matching key and returns pointer to item data
// Calls recursive look up function
template<typename K, typename I>
typename AVL<K,I>::Item* AVL<K,I>::lookup(Key soughtKey){
    
    return lookupRec(soughtKey, root);

}

// Recursive worker function - compares key to current node and traverses tree
template<typename K, typename I>
typename AVL<K,I>::Item* AVL<K,I>::lookupRec(Key soughtKey, Node* currentNode){
    
    if(isLeaf(currentNode)){
        return nullptr;
    }
    else if(soughtKey == currentNode->key){
        return &currentNode->item;
    }
    else if(soughtKey < currentNode->key){
        return lookupRec(soughtKey, currentNode->leftChild);
    }
    else{
        return lookupRec(soughtKey, currentNode->rightChild);
    }

}

// Inserts new node into tree
// Calls recursive insert function
template<typename K, typename I>
void AVL<K,I>::insert(Key newK, Item newI){

    insertRec(newK, newI, root);

}

// Recursive worker function - inserts new node by comparing keys
// Returns boolean to indicate if tree height has changed
template<typename K, typename I>
bool AVL<K,I>::insertRec(Key newK, Item newI, Node*& currentNode){

    bool heightIncrease = false;

    if(isLeaf(currentNode)){
        currentNode = new Node(newK, newI);
        heightIncrease = true;
    }
    else if(newK == currentNode->key){
        currentNode->item = newI;
    }
    else{

        if(newK < currentNode->key){
            heightIncrease = insertRec(newK, newI, currentNode->leftChild);
            if(heightIncrease){
            	currentNode->balance --;
            }
        }
        else{
            heightIncrease = insertRec(newK, newI, currentNode->rightChild);
            if(heightIncrease){
                currentNode->balance ++;
            }
        }

        if(currentNode->balance == 0){
            heightIncrease = false;
        }
   
    }

    if(currentNode->balance == -2 || currentNode->balance == 2){
        if(rebalance(currentNode)){
            heightIncrease = false;
        }
    }

    return heightIncrease;

}

// Displayes in key order all nodes
// Calls recursive display entries function
template<typename K, typename I>
void AVL<K,I>::displayEntries(){

    displayEntriesRec(root);

}

// Recursive worker function - displayes entries in key order (In-order traversal)
template<typename K, typename I>
void AVL<K,I>::displayEntriesRec(Node* currentNode){

    if(!isLeaf(currentNode)){
        displayEntriesRec(currentNode->leftChild);
        std::cout << currentNode->key << " : " << currentNode->item 
        	<< " (" << currentNode->balance << ")" << std::endl;
        displayEntriesRec(currentNode->rightChild);
    }

}

// Displays tree
// Calls recursive display tree function
template<typename K, typename I>
void AVL<K,I>::displayTree() {

    displayTreeRec(root, 0);

}

// Recursive worker function - displays tree (Post-order traversal)
template<typename K, typename I>
void AVL<K,I>::displayTreeRec(Node* currentNode, int level){

    if(isLeaf(currentNode)){
        std::cout << std::setw(level) << "*" << std::endl;
    }
    else{
        std::cout << std::setw(level) << currentNode->key << " (" << currentNode->balance << ")" << std::endl;
        displayTreeRec(currentNode->leftChild, level+6);
        displayTreeRec(currentNode->rightChild, level+6);
    }

}

// Displayes in key order all nodes
// Calls recursive overload operator for <<
template<typename K, typename I>
std::ostream& operator<<(std::ostream& os, const AVL<K,I>& bt){

    AVL<K,I>::displayOverloadRec(os, bt.root);
    return os;

}

// Recursive worker function - adds nodes in key order to object stream (In-order traversal)
template<typename K, typename I>
void AVL<K,I>::displayOverloadRec(std::ostream& os, AVL<K,I>::Node* currentNode){

    if(!AVL<K,I>::isLeaf(currentNode)){
        displayOverloadRec(os, currentNode->leftChild);
        os << currentNode->key << " : " << currentNode->item 
        	<< " (" << currentNode->balance << ")" << std::endl;
        displayOverloadRec(os, currentNode->rightChild);
    }

}

// Removes node from tree
// Calls recursive remove function
template<typename K, typename I>
void AVL<K,I>::remove(Key key){

    removeRec(key, root);

}

// Recursive worker function - finds matching node, removes from tree, rejoins disrupted nodes
// Returns bool value to indicate if subtree has decreased in height
template<typename K, typename I>
bool AVL<K,I>::removeRec(Key deleteKey, Node*& currentNode){

    bool heightDecrease = false;

    if(!isLeaf(currentNode)){
        
        if(deleteKey == currentNode->key){

            Node* replaceNode = nullptr;
            heightDecrease = true;

            if(!noChildren(currentNode)){
            
                if(currentNode->leftChild != nullptr && currentNode->rightChild == nullptr){
                    replaceNode = currentNode->leftChild;
                }
                else if(currentNode->rightChild != nullptr && currentNode->leftChild == nullptr){
                    replaceNode = currentNode->rightChild;
                }
                else{
                    std::pair<Node*,bool> detachReturn;
                    detachReturn = detachMinimumNode(currentNode->rightChild);
                    replaceNode = detachReturn.first;
                    heightDecrease = detachReturn.second;
                    replaceNode->leftChild = currentNode->leftChild;
                    replaceNode->rightChild = currentNode->rightChild;
                }

            }

            delete currentNode;
            currentNode = replaceNode;
            
        }
        else{

            if(deleteKey < currentNode->key){
                heightDecrease = removeRec(deleteKey, currentNode->leftChild);
                if(heightDecrease){
                    currentNode->balance ++;
                }
            }
            else{
                heightDecrease = removeRec(deleteKey, currentNode->rightChild);
                if(heightDecrease){
                    currentNode->balance --;
                }
            }

            if(currentNode->balance == 1 || currentNode->balance == -1){
                heightDecrease = false; 
            }

        }
    }

    if(!isLeaf(currentNode)){

        if(currentNode->balance == 2 || currentNode->balance == -2){

            if(rebalance(currentNode)){
                heightDecrease = true;
            }
        }
    }
    
    return heightDecrease;
}

// Finds the minimum node from given node and detaches it from the tree
// Re-attaches any child nodes
template<typename K, typename I>
std::pair<typename AVL<K,I>::Node*,bool> AVL<K,I>::detachMinimumNode(Node*& currentNode){

    bool heightDecrease = false;

    if(isLeaf(currentNode->leftChild)){
        Node* minimumNode = currentNode;
        currentNode = currentNode->rightChild;
        heightDecrease = true;
        return std::make_pair(minimumNode, heightDecrease);
    }
    else{
        return detachMinimumNode(currentNode->leftChild);
    }
    
}

// Destructor
// Calls recursive deep delete function
template<typename K, typename I>
AVL<K,I>::~AVL<K,I>(){

    deepDelete(root);
  
}

// Recursive worker function - unallocates memory for all nodes (Post-order traversal)
template<typename K, typename I>
void AVL<K,I>::deepDelete(Node*& currentNode) {

    if(!isLeaf(currentNode)){
        deepDelete(currentNode->leftChild);
        deepDelete(currentNode->rightChild);
        delete currentNode;
    }
  
}

// Copy constructor
// Calls recursive deep copy function
template<typename K, typename I>
AVL<K,I>::AVL(const AVL<K,I>& original) {
    this->root = deepCopy(original.root); 
}

// Recursive worker function - creates new copy of nodes from input tree
template<typename K, typename I>
typename AVL<K,I>::Node* AVL<K,I>::deepCopy(Node* original)
{
    if(isLeaf(original)){
        return nullptr;
    }
    else{
        Node* copyNode = new Node(original->key, original->item);
        copyNode->balance = original->balance;
        if(!noChildren(original)){
            copyNode->leftChild = deepCopy(original->leftChild);
            copyNode->rightChild = deepCopy(original->rightChild);
        }
        return copyNode;
    }

}

// Copy Assignment Operator
// Overload operator - calls recursive deep copy function
template<typename K, typename I>
AVL<K,I>& AVL<K,I>::operator=(const AVL<K,I>& original) {

    this->root = deepCopy(original.root);
    return *this;

}

// Move constructor
// Calls move tree function, sets current tree root to old tree root
template<typename K, typename I>
AVL<K,I>::AVL(AVL<K,I>&& original){

    this->root = moveTree(original.root);

}

// Move Assignment Operator
// Overload operator - calls move tree function, sets current tree root to old tree root
template<typename K, typename I>
AVL<K,I>& AVL<K,I>::operator=(AVL<K,I>&& original) {

    this->root = moveTree(original.root);
    return *this;

}

// Sets input root to null and returns the position of the input root node to move tree
template<typename K, typename I>
typename AVL<K,I>::Node* AVL<K,I>::moveTree(Node*& rootNode){

    Node* treeRoot = rootNode;
    rootNode = nullptr;
    return treeRoot;

}

// Rotates tree to the right (moves root to left child)
// invariant - root and root left child cannot be a leaf
template<typename K, typename I>
void AVL<K,I>::rotateRight(Node*& localRoot) {

    assert(!isLeaf(localRoot));
    assert(!isLeaf(localRoot->leftChild));

    Node* oldRoot = localRoot;
    Node* newRoot = oldRoot->leftChild;
    Node* displacedSubtree = newRoot->rightChild;

    oldRoot->leftChild = displacedSubtree;
    newRoot->rightChild = oldRoot;

    oldRoot->balance = oldRoot->balance + 1 + std::max(-newRoot->balance, 0);
    newRoot->balance = newRoot->balance + 1 + std::max(oldRoot->balance, 0);

    //balanceFactor(oldRoot->balance, newRoot->balance, 1);

    localRoot = newRoot;
    
}

// Rotates tree to the left (moves root to right child)
// invariant - root and root right child cannot be a leaf
template<typename K, typename I>
void AVL<K,I>::rotateLeft(Node*& localRoot) {

    assert(!isLeaf(localRoot));
    assert(!isLeaf(localRoot->rightChild));

    Node* oldRoot = localRoot;
    Node* newRoot = oldRoot->rightChild;
    Node* displacedSubtree = newRoot->leftChild;

    oldRoot->rightChild = displacedSubtree;
    newRoot->leftChild = oldRoot;

    oldRoot->balance = oldRoot->balance - 1 - std::max(newRoot->balance, 0);
    newRoot->balance = newRoot->balance - 1 - std::max(-oldRoot->balance, 0);

    //balanceFactor(oldRoot->balance, newRoot->balance, -1);

    localRoot = newRoot;
    
}

// Calculates the balance factor of the rotated nodes
template<typename K, typename I>
void AVL<K,I>::balanceFactor(Balance& oldRootBalance, Balance& newRootBalance, int direction){

    oldRootBalance = 
        oldRootBalance + direction + (direction * std::max((direction * newRootBalance), 0));

    newRootBalance = 
        newRootBalance + direction + (direction * std::max((-direction * oldRootBalance), 0));

}

// Checks balance of node and performs required rotations of unbalanced
// Returns bool value indicating if height of tree has decreased
template<typename K, typename I>
bool AVL<K,I>::rebalance(Node*& localRoot) {

    // Node pointer cannot be null in order to check balance and rotate
    assert(localRoot != nullptr);

    // Node balance must be either -2 or 2 for a rebalance to occur
    assert(localRoot->balance == 2 || localRoot->balance == -2);

    if(localRoot->balance == 2){

        if(localRoot->rightChild->balance == 1){
            rotateLeft(localRoot);
            return true;
        }
        else if(localRoot->rightChild->balance == 0){
            rotateLeft(localRoot);
            return false;
        }
        else{
            rotateRight(localRoot->rightChild);
            rotateLeft(localRoot);
            return true;
        }

    }
    else{

        if(localRoot->leftChild->balance == -1){
            rotateRight(localRoot);
            return true;
        }
        else if(localRoot->leftChild->balance == 0){
            rotateRight(localRoot);
            return false;
        }
        else{
            rotateLeft(localRoot->leftChild);
            rotateRight(localRoot);
            return true;
        }

    }

}