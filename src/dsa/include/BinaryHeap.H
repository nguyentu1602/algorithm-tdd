#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "dsexceptions.H"
#include <vector>
using namespace std;

// BinaryHeap class
//
// ******* public operation
// insert(), deleteMin(), findMind(), isEmpty(), makeEmpty()
//
// ******* internal class member:
// data member: currenSize, array
// buildHeap()
// percolateDown()
//

template <typename Comparable>
class BinaryHeap
{
 public:
  // I. The big five
  // ***************
  // 0. The defaul ctor - you just want an empty heap with 100 elements by default
  explicit BinaryHeap(int capacity = 100)
      : currentSize { 0 }, array ( capacity + 1) {} // so the array is initialized with def. ctor

  explicit BinaryHeap( const vector<Comparable> & items)
      : array( items.size() + 10 ), currentSize {items.size()}
  {
    for ( int i = 0; i < items.size(); i++)
      array[ i + 1 ] = items[ i ];
    buildHeap();
  }

  // 1. The copy constructor // don't need one here. Use the default one
  // 2. The copy assignment operator // don't need one here. Use the default one
  // 3. The move constructor // default one
  // 4. The move copy constructor // default one
  // 5. Destructor - you don't need one because each BinaryHeap contains well-defined containers
  // with well defined destructors

  // II. The interface:
  // **************
  bool isEmpty() const {
    return currentSize == 0;
  }

  void makeEmpty() {
    currentSize = 0;
  }

  const Comparable & findMin() const
  {
    if ( isEmpty())
      throw UnderflowException{};
    return array [ 1 ];
  }
  // Insert is the tricky part
  // How do you insert anything?
  // increase the CurrentSize by 1. Put the new item in there.
  // compare the item and its parent. If the item is smaller, swap it with the parent.
  // repeat the current process until the item is not smaller than its parent anymore
  void insert ( const Comparable & x ) {
    if ( currentSize == array.size() - 1)
      array.resize( array.size() * 2 );

    // percolate
    int hole = ++currentSize;
    Comparable copy = x;
    array [ 0 ] = std::move( copy );
    // how to percolate up?
    for ( ; x < array[ hole / 2 ]; hole /= 2 )
      array[ hole ] = std::move( array [ hole / 2 ]);
    array [ hole ] = std::move( array[0] );
  }


  void insert ( Comparable && x ) {
    // first check if the array size is sufficient
    if (currentSize == array.size() - 1)
      array.resize( array.size() * 2 );

    // percolate the item up
    int hole = ++currentSize;
    // when x is still smaller than the parent of hole, move hole to parent of hole
    for ( ; hole > 1 && x < array[ hole / 2 ]; hole /= 2 )
      array[ hole ] = std::move(array [ hole / 2 ]); // division is automatically truncated
    // the above std::move requires Comparable to have a move assignment operator
    // when either hole = 1 or x >= array [ hole / 2 ] (the parent)
    // then copy x into the hole because we're done percolating upward
    array [ hole ] = std::move( x );
  }

  void deleteMin() {
    if ( isEmpty() )
      throw UnderflowException{};
    array[ 1 ] = std::move(array [ currentSize-- ]);
    percolateDown(1);
  }
  // deleteMin and save into minItem
  void deleteMin( Comparable & minItem ) {
    if ( isEmpty() )
      throw UnderflowException{};
    minItem = std::move ( array[ 1 ]);
    array[ 1 ] = std::move(array [ currentSize-- ]);
    percolateDown(1);
  }

 private:
  int currentSize; // must keep track of the current size. Should I use size_type here? Yes
  vector<Comparable> array; // the heap aray

  void buildHeap(){
    // given an arbitrary array, you can establish the heap order property
    // run in linear time
    // you only need to percolate down from one level above the leaves
    for ( int i = currentSize / 2; i > 0; --i )
      percolateDown( i );
  }
  // percolateDown - what does it do?
  // hole is the index at which the percolate begin
  void percolateDown( int hole )
  {
    int child = 0;
    Comparable tmp = std::move(array[ hole ]);
    // compare the left child with the currentSize
    for (; hole * 2 <= currentSize; hole = child )
    {
      child = hole * 2;
      // if the child is the last node or the right child is greater than the left child
      if ( child != currentSize && array [ child + 1 ] < array [ child ])
        ++child;
      // now compare tmp with the child and move child up to hole if tmp is greater
      if ( array [ child ] < tmp )
        array[ hole ] = std::move( array[ child ] );
      else // if tmp is not greater than child, we're done
        break;
    }
    // after the for loop either done or break, we're left with a hole where tmp should end up
    array[ hole ] = std::move( tmp);
  } // percolateDown()
};

#endif
