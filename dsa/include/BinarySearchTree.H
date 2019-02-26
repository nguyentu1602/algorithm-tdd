#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "dsexceptions.H"
#include <algorithm>
using namespace std;

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted
template <typename Comparable>
class BinarySearchTree {
 public:
  // Default constructor:
  BinarySearchTree() : root{ nullptr } {}
  // copy constructor:
  BinarySearchTree(const BinarySearchTree& rhs) : root { nullptr } {
    root = clone (rhs.root); // calling the internal clone method
  }
  // Move constructor? Why do we need this?
  // We nned this because we want to both COPY OVER and DESTROY the source
  // A normal copy constructor will take a const BST so it can't modify the source
  BinarySearchTree(BinarySearchTree && rhs) : root { rhs.root } {
    rhs.root = nullptr; // Set rhs's pointer to null after moving the tree
  }

  // Assignment operator
  BinarySearchTree & operator=( const BinarySearchTree & rhs ) {
    BinarySearchTree copy = rhs;
    std::swap( *this, copy);
    return *this;
  }
  // Assignment operator for move.
  // It takes a rhs by rvalue reference, then just snatch the root of rhs.
  // so after the deal is done, you move the whole rhs.root into this->root
  // leaving rhs with empty root.
  BinarySearchTree & operator=(BinarySearchTree && rhs) {
    std::swap(root, rhs.root);
    return *this;
  }

  // Destructor
  ~BinarySearchTree() {
    makeEmpty(); // you need to write this private member func
  }
  // contains() calls the internal method
  bool contains (Comparable & x) const {
    return contains( x, root);
  }

  // check if the tree is empty - just need to look at the root
  bool isEmpty() const {
    return root == nullptr;
  }

  // find min in the tree
  const Comparable & findMin() const {
    if ( isEmpty( ) )
      throw UnderflowException{ };
    return findMin( root )->element;
  }

  // find max in the tree
  const Comparable & findMax() const {
    if ( isEmpty())
      throw UnderflowException{ };
    return findMax( root )->element;
  }

  // printTree()
  void printTree( ostream & out ) const {
    if ( isEmpty() )
      out << "Empty tree" << endl;
    else
      printTree( root, out);
  }

  // makeEmpty()
  void makeEmpty() {
    makeEmpty( root );
  }
  // insert()
  void insert(const Comparable & x) {
    insert(x, root);
  }
  void insert(Comparable && x) {
    insert(std::move(x), root);
  }
  // remove
  void remove(const Comparable & x) {
    remove(x, root);
  }

 private:
  // BinaryNode is a struct that represent the building block of BST
  // You need an element and 2 pointers to the left and right children node
  struct BinaryNode {
    Comparable element;
    BinaryNode *left;
    BinaryNode *right;

    // constructor. Rmb you don't need semi colon at the end
    BinaryNode ( const Comparable& theElement, BinaryNode *lt, BinaryNode *rt )
        : element{theElement}, left {lt}, right {rt} {}

    // move constructor
    BinaryNode (Comparable && theElement, BinaryNode *lt, BinaryNode *rt)
        : element{ std:move (theElement) }, left {lt}, right {rt} {}
  };

  BinaryNode *root; // the only data field in a tree object

  // insert(): internal method to insert a comparable into the tree
  void insert(const Comparable & x, BinaryNode* & t) {
    // you have to take reference to a pointer here because you alter its content with
    // the new BinaryNode() construction. Otherwise, insert makes a copy of pointer t,
    // then give the new BinaryNode to that temp pointer, which is destroyed after the scope.
    // if t is a null pointer, make a new BinaryNode, put x in there, make t points to it
    if (t == nullptr)
      t = new BinaryNode{x, nullptr, nullptr}; //new brace-init feature in C++11
    else if (x < t->element)
      insert(x, t->left);
    else if (x > t->element)
      insert(x, t->right);
    else
      ; // do nothing because of duplicate
  } // insert() -- do not move version


  // move insertion: move Comparable x inside tree t instead of making a copy
  void insert (Comparable && x, BinaryNode* & t)  {
    if (t == nullptr)
      t = new BinaryNode{ std::move(x), nullptr, nullptr}; // again, new brace-init in C++11
    else if (x < t->element )
      insert ( std::move(x), t->left );
    else if (x > t->element)
      insert ( std::move(x), t->right );
    else
      ; // do nothing because of duplicate
  } //

  // remove()
  void remove( const Comparable & x, BinaryNode* & t){
    // if t is nullptr then do nothing
    if (t == nullptr)
      return;
    // if x is on the left branch
    else if (x < t->element) {
      remove (x, t->left);
    }
    else if (x > t->element) {
      remove (x, t->right);
    }
    // if we found t:
    else if (t->left != nullptr && t->right != nullptr) { //two children
      // first overwrite its element with its right child's smallest element
      // does this destroy the Comparable in t->element?
      // yes because t->element is an actual holder, so it will be written over.
      t->element = findMin(t->right)->element; //right is not null, must have min
      // then delete its right child's smalles element
      remove(t->element, t->right);
    }
    else { // found, either left empty or right empty
      BinaryNode* oldNode = t; // make a temp pointer to point to the old t
      t = (t->left != nullptr) ? t->left : t->right;
      delete oldNode;
    }
  }


  // contains() is the helper search method. Given an item and a tree,
  // try to find the item in there.
  bool contains (const Comparable & x, BinaryNode *t) const {
    // if the tree is empty, return false
    if (t == nullptr)
      return false;
    // if not, check element at t, then if false, recurse on subtree
    else {
      if (x < t->element)
        return contains (x, t->left);
      else if (x > t->element)
        return contains (x, t->right);
      else
        return true; // this is the match
    }
  } // contains

  // remove()


  // makeEmpty() internal method to clean up a subtree
  void makeEmpty (BinaryNode * & t) {
    if (t != nullptr) {
      makeEmpty( t->left );
      makeEmpty( t->right );
      delete t;
    }
    // set t to null pointer after cleaning up the left and right pointers.
    t = nullptr;
  }


  // findMin() internal method to find smallest item in a subtree t
  // given a tree, start at the root, keep going left as long as there is a left child
  // when there is no left child, return the current node
  BinaryNode * findMin(BinaryNode *t) const {
    if (t == nullptr)
      return nullptr;
    if (t->left == nullptr)
      return t;
    return findMin(t->left);
  } // findMin()

  // findMax() internal method to find biggest item in a subtree t
  BinaryNode * findMax(BinaryNode *t) const {
    if (t == nullptr)
      return nullptr;
    if (t->right == nullptr)
      return t;
    return findMax(t->right);
  } // findMax()

  // printTree() traverse the tree to print out elements in sorted order
  void printTree( BinaryNode *t, ostream & out) const {
    // do nothing if t is nullptr: empty tree
    if (t != nullptr) {
      printTree(t->left, out); // you need to print the left side first
      out << t->element << endl; // then the current node at root of subtree
      printTree(t->right, out); // then you print the right side of the tree.
    }
  }// printTree()

  // clone: internal method to clone subtree
  // how do you clone a subtree? start to clone the current node and then recursively call clone
  // to clone its children

  BinaryNode * clone(BinaryNode *t) const {
    if (t == nullptr)
      return nullptr;
    else
      return new BinaryNode{t->element, clone(t->left), clone(t->right)};
  }
};

#endif
