#ifndef BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_H

#include <iostream>
#include <vector>
#include "dsexceptions.H"
using namespace std; // bad practice

// Binomial queue class
// CONSTRUCTION: no parameters
// public interface:
// void insert(x)
// deleteMin()
// Comparable findMin()
// bool isEmpty()
// void makeEmpty()
// void merge( rhs )

template <typename Comparable>
class BinomialQueue
{
 public:
  BinomialQueue() : theTrees( DEFAULT_FOREST_SIZE ){
    // what do you want to do with each tree in the forest?
    for (auto & tree : theTrees) {
      tree = nullptr; // try the safe init here! Let's see if it works!
    }
    currentSize = 0;
  }
  // default ctor. Use the default ctor of Vector<> to init one with size DEFAULT_FOREST_SIZE
  // parametric ctor with 1 item given. This one serves the insert function
  BinomialQueue( const Comparable & item ) : theTrees(1), currentSize{1}{
    theTrees[0] = new BinomialNode{item, nullptr, nullptr};
  }
  // copy constructor. You need to init theTrees. so you need its size
  BinomialQueue( const BinomialQueue & rhs)
      : theTrees(rhs.theTrees.size()), currentSize{rhs.currentSize}
  {
    // iterate over the forest and copy the trees
    // you probably need a method to clone a tree here. The method takes a heap tree as an argument and return a cloned heap tree. A heap tree is just a pointer to a BinomialNode, and BinomialNode is private, so clone should be private
    for ( int i = 0; i < rhs.theTrees.size(); i++) {
      theTrees[i] = clone( rhs.theTrees[i] );
    }
  }
  // assignment operator
  BinomialQueue & operator=( const BinomialQueue & rhs ) {
    // check for self-assignment
    // cheating: make a copy first and then move it
    BinomialQueue copy = rhs; //The heavy lifting left to the copy constructor
    std::swap( *this, copy ); //Steal the guts of copy. It will get destroyed after allB
    return *this;
  }
  // move ctor
  BinomialQueue( BinomialQueue && rhs )
      : theTrees{std::move(rhs.theTrees)}, currentSize{rhs.currentSize}
  {}
  // move assignment operator
  BinomialQueue & operator=( BinomialQueue && rhs ) {
    std::swap(currentSize, rhs.currentSize);
    std::swap(theTrees, rhs.theTrees);
    return *this;
  }

  ~BinomialQueue() { // destructor
    makeEmpty();
  }
  // the interface:
  // 0. makeEmpty() for the constructor to call
  void makeEmpty() {
    currentSize = 0;
    for (auto & tree : theTrees)
      makeEmpty(tree);
  }
  // 1. findMin() is the most canonical operation. Lg(n) complexity
  // why do you need a private method? can you do differently? yes, but because you
  // reuse the findMinIndex method in other functions you should write one separately
  const Comparable & findMin() const {
    if (isEmpty()){
      throw UnderflowException{};
    }
    return theTrees[findMinIndex()]->element; // findMinIndex() has complexity of lgN
  }
  // 2. insert is the second most canonical operation. In fact it's a special case of merge
  // so we do merge first and then we later write insert
  void insert( const Comparable & x) {
    BinomialQueue oneItem{x};
    merge(oneItem);
  }
  void insert( Comparable && x) {
    BinomialQueue oneItem{ std::move(x)};
    merge(oneItem);
  }
  int size() { return currentSize; }
  bool isEmpty() { return currentSize == 0; }
  void deleteMin(){
    Comparable x;
    deleteMin(x);
  }

  void deleteMin(Comparable & minItem) { // remove and store in a Comparable object
    // check if the tree is empty
    if(isEmpty()){
      throw UnderflowException{};
    }

    // first find the tree with the smallest item and cut it out
    int minIndex = findMinIndex();// the index start with 0, so the minTree's size is 1<<minIndex
    minItem = theTrees[minIndex]->element;
    BinomialNode* tokill = theTrees[minIndex];

    BinomialNode* minTree = tokill->leftChild;
    // now remove the smallest item from minTree
    delete tokill;
    // build a new BinomialQueue to hold it first
    BinomialQueue stuff;
    stuff.theTrees.resize(minIndex + 1);
    stuff.currentSize = (1 << minIndex) - 1;
    for (int i = minIndex - 1; i >= 0; i--){
      stuff.theTrees[i] = minTree;
      minTree = minTree->nextSibling;
      stuff.theTrees[i]->nextSibling = nullptr;
    }

    theTrees[ minIndex ] = nullptr;
    currentSize -= stuff.currentSize + 1;
    // now merge stuff with this
    merge(stuff);
  }


  // 3. merge the Queue with another Queue; require iterating through theTrees arrays of both and
  // merge one by oneitem. Call combinetrees here as a helper function
  // pre:
  // post: rhs become empty
  void merge(BinomialQueue & rhs) {
    // check for seff-merge. 'this' is an address
    if (this == &rhs){
      return; // end the operation here.
    }
    // calculate the new required size and resize if needed. The +1 is for the carry
    currentSize += rhs.currentSize;
    if (currentSize > capacity()) {
      int oldForestSize = theTrees.size();
      int newForestSize = max( theTrees.size(), rhs.theTrees.size()) + 1;
      theTrees.resize(newForestSize); // resize() do the heavy lifting up until oldForestSize
      for(int i = oldForestSize; i < newForestSize; i++) {
        theTrees[i] = nullptr;
      }
    }
    // ample room in place! Let's do the weight lifting
    // // we can think of this merging as adding 2 binary numbers
    BinomialNode* carry = nullptr;
    for (int i = 0, j = 1; j <= currentSize; ++i, j *= 2) {
      BinomialNode* t1 = theTrees[i];
      BinomialNode* t2 = i < rhs.theTrees.size() ? rhs.theTrees[i] : nullptr;
      int whichCase = t1 == nullptr ? 0 : 1;
      whichCase += t2 == nullptr ? 0 : 2;
      whichCase += carry == nullptr ? 0 : 4;
      switch( whichCase ) {
        case 0: // all three ptr is empty yay do nothing!
        case 1: // only this
          break;
        case 2: // only rhs
          theTrees[i] = t2;
          rhs.theTrees[i] = nullptr;
          break;
        case 3: // rhs and this have trees
          carry = combineTrees(t1, t2);
          theTrees[i] = nullptr;
          rhs.theTrees[i] = nullptr;
          break;
        case 4: // only carry has a tree
          theTrees[ i ] = carry;
          carry = nullptr;
          break;
        case 5: // this and carry have trees
          carry = combineTrees(carry, t1);
          theTrees[i] = nullptr;
          break;
        case 6: // rhs and carry have trees
          carry = combineTrees(carry, t2);
          rhs.theTrees[i] = nullptr;
          break;
        case 7: // all three have trees
          theTrees[i] = carry;
          carry = combineTrees(t1, t2);
          rhs.theTrees[i] = nullptr;
          break;
      }
    }
    // make sure we deep clean rhs.theTrees
    for ( auto & tree : rhs.theTrees ) {
      tree = nullptr;
    }
    rhs.currentSize = 0;
  }

 private:
  struct BinomialNode {
    Comparable element;
    BinomialNode* leftChild;
    BinomialNode* nextSibling;
    // parametric ctors
    BinomialNode(const Comparable& e, BinomialNode* lt, BinomialNode* rt)
        : element{e}, leftChild{lt}, nextSibling{rt} {}
    BinomialNode(Comparable&& e, BinomialNode* lt, BinomialNode* rt)
        : element{std::move(e)}, leftChild{lt}, nextSibling{rt} {}
  };
  const static int DEFAULT_FOREST_SIZE = 1;
  // The forest holder, or the B-array. You just need to declare here. Resize later
  vector<BinomialNode* > theTrees;
  // Number of items in the priority queue. Notice you want to use size_type here for robustness
  int currentSize;
  // private method to return the index of the tree contained the smallest.
  // theTrees size can't be 0. Must be at least 1.
  // the queue must not be empty. Return the index of the tree containing the smallest item
  // int findMinIndex() const {
  //   int minIndex = 0;
  //   for (int i = 1; i < theTrees.size(); ++i){
  //     // if the ptr in both indexes are not null
  //     if(theTrees[i] != nullptr &&
  //        theTrees[i]->element < theTrees[minIndex]->element) {
  //     minIndex = i;
  //     }
  //   }
  //   return minIndex;
  // }

  int findMinIndex( ) const
  {
    int i;
    int minIndex;
    // advancing until you reach the first empty tree
    for( i = 0; theTrees[ i ] == nullptr; ++i )
      ;
    // if
    for( minIndex = i; i < theTrees.size( ); ++i )
      if( theTrees[ i ] != nullptr &&
          theTrees[ i ]->element < theTrees[ minIndex ]->element )
        minIndex = i;

    return minIndex;
  }

  // return the capacity of the tree
  int capacity() const { return ( 1<<theTrees.size()) - 1; }
  // return the result of merging equal-sized t1 and t2
  // required that the two trees are equal-sized and not null

  BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2) {
    // check empty
    // compare which trees has smaller min
    if ( t1->element <= t2->element ) { // then t2 is new child of t1
      t2->nextSibling = t1->leftChild;
      t1->leftChild = t2;
      return t1;
    } else {
      return combineTrees( t2, t1 );
    }
  }

  void makeEmpty(BinomialNode* t) {
    //base case: do nothing because t is already nullptr
    if (t != nullptr) {
      makeEmpty(t->leftChild);
      makeEmpty(t->nextSibling);
      // after both leftChild and nextSibling are set to nullptrs, safely detete t
      delete t; // destroy what t point to. Left t pointing to the address still
      t = nullptr;
    }
  }

  // Internal method to clone subtree
  // subtree starts with a Node that has no sibling but has
  // recursive for sure
  BinomialNode* clone(const BinomialNode* clt) const {
    // base case:
    if (clt == nullptr) {
      return nullptr;
    } else {
      return new BinomialNode( clt->element, clone(clt->leftChild), clone(clt->nextSibling));
    }
  }
};

#endif
