#ifndef SORT_H
#define SORT_H

// Several sorting funtions here.
// Insertion sort
// Shell sort
// heapsort
// merge sort
// quick sort

#include <vector>
#include <functional>
using namespace std;

// if you must, how do you write an insertionsort routine?
// you can makes it take a vector
template <typename Comparable>
void insertionSort(vector<Comparable>& a) {
  // error check empty a
  for (int i = 1; i < a.size(); i++) {
    Comparable temp = std::move(a[i]);
    // loop invariant: everything before i is sorted
    int j;
    for (j = i; j > 0 && temp < a[j - 1]; j--){
         a[j] = std::move(a[j - 1]);
      }
    a[j] = std::move(temp);
  }
}

template <typename Comparable>
void shellSort (vector<Comparable> & a){
  // first make the sequences of gaps
  for (int gap = a.size()/2; gap > 0; gap /= 2){
    Comparable temp;

    for (int i = gap; i < a.size(); i++) {
      int j = i;
      temp = std::move(a[i]);
      for (; j >= gap && temp < a [j - gap]; j -= gap) {
        a[j] = std::move(a[j - gap]);
      }
      a[j] = std::move(temp);
    }
  }
}

// heapsort: you build a max heap and then you keep popping items until you're done!
// what do you need? insert, leftChild. Then you inline buildHeap and deleteMax inside the
// main heap sort function.
inline int leftChild (int i) {
  return 2 * i + 1; // it's because you use a vector to store the heap starting at position 0
}

// percDown is the internal method to help with inserting into the heap.
// a is the phisical array to put the heap in
// i is the position from which to percolate down
// n is the logical size of the binary heap
template <typename Comparable>
void percDown( vector<Comparable>& a, int i, int n) {
  int biggerChild;
  Comparable tmp = std::move(a[i]);
  while (leftChild(i) < n) {
    biggerChild = leftChild(i);
    // find the biggerChild
    if((biggerChild + 1 < n) && (a[biggerChild] < a[biggerChild + 1]))
      biggerChild++; // if rightchild exist and larger than leftchild, then make it the bigger child
    // compare tmp to the bigger child
    if(tmp < a[biggerChild]) {
      a[i] = std::move(a[biggerChild]);
      i = biggerChild;
    }
    else {
      break;
    }
  }
  a[i] = std::move(tmp);
}


/**
 * Standard heapsort.
 */
template <typename Comparable>
void heapsort(vector<Comparable> & a) {
  // first build the maxheap up
  for(int i = a.size() / 2 - 1; i >= 0; --i) {
    percDown(a, i, a.size());
  }
  // now tear it down to have the sorted array!
  for(int k = a.size() - 1; k > 0; --k) {
    std::swap(a[0], a[k]); // use the end to save the sorted result
    percDown(a, 0, k); // restore the max heap property
  }
}

// merge sort:
/**
 * helper method that merge to sorted halfves of a subarray
 * a is an array of comparables
 * tmpArray is temp place to place the merged result
 * leftPos is the left-most index of the subarray
 * rightPos is the start of the second half
 * rightEnd is the right-most of the second half
 * YOU NEED TO PUT THIS HELPER FUNCTION ON TOP OF OTHER FUNCTIONS
 */
template <typename Comparable>
void merge(vector<Comparable>& a, vector<Comparable>& tmpArray,
           int leftPos, int rightPos, int rightEnd) {

  int leftEnd = rightPos - 1;
  int tmpPos = leftPos;
  int numElements = rightEnd - leftPos + 1;

  // main loop
  while (leftPos <= leftEnd && rightPos <= rightEnd) {
    if(a[leftPos] <= a[rightPos]){
      tmpArray[ tmpPos++] = std::move(a[leftPos++]);
    }
    else {
      tmpArray[ tmpPos++ ] = std::move(a[rightPos++]);
    }
  }
  // deal with the residue if one array is bigger
  while (leftPos <= leftEnd){
    tmpArray[tmpPos++] = std::move(a[leftPos++]);
  }
  while (rightPos <= rightEnd) {
    tmpArray[tmpPos++] = std::move(a[rightPos++]);
  }
  // copy tmpArray back to the original a array
  for (int i = 0; i < numElements; i++, rightEnd--)
    a[rightEnd] = std::move(tmpArray[rightEnd]);
}

template <typename Comparable>
void mergeSort(vector<Comparable>& a, vector<Comparable>& tmpArray,
               int left, int right) {
  if(left < right) {
    int center = (left + right)/2;
    mergeSort(a, tmpArray, left, center);
    mergeSort(a, tmpArray, center + 1, right);
    merge(a, tmpArray, left, center + 1, right);
  }
  // basecase: left = right, one element, do nothing!
}

template <typename Comparable>
void mergeSort (vector<Comparable>& a) {
  vector<Comparable> tmpArray(a.size());
  mergeSort(a, tmpArray, 0, a.size() - 1);
}

// QUICK SORT - THE DE FACTO SORTING ALGO IN C++
// IN PLACE
// NOTORIOUSLY TRICKY TO IMPLEMENT RIGHT
template <typename Comparable>
const Comparable & median3(vector<Comparable> & a, int left, int right) {
  // this is an internal function that is only called on a with size >= 10
  int center = (left + right)/2;
  if (a[center] < a[left]) {
    std::swap(a[center], a[left]);
  }
  if (a[right] < a[left]){
    std::swap(a[right], a[left]);
  }
  if (a[right] < a[center]){
    std::swap(a[right], a[center]);
  }
  // put the pivot almost at the end of the array:
  std::swap(a[center], a[right - 1]);
  return a[right - 1];
}
// helper insertionSort for quicksort
template <typename Comparable>
void insertionSort(vector<Comparable>& a, int left, int right) {
  for (int i = left + 1; i <= right; ++i) {
    Comparable temp = std::move(a[i]);
    // loop invariant: everything before i is sorted
    int j;
    for (j = i; j > left && temp < a[j - 1]; j--){
      a[j] = std::move(a[j - 1]);
    }
    a[j] = std::move(temp);
  }
}

template <typename Comparable>
void quicksort(vector<Comparable> & a, int left, int right) {
  // quicksort on a of size >= 10
  if (left + 10 <= right) {
    // partitioning first
    const Comparable & pivot = median3(a, left, right);
    // after median3, pivot is at [right - 1]
    // put i and j to be one past there correct values (i to the left and j to the right)
    // motive: to eliminate the need for special case handling; i and j always move atleast 1
    int i = left;
    int j = right - 1;
    for (;;) { // eternal loop - must have a break inside
      while ( a[++i] < pivot ) {} // i happy to move forward when smaller items are left of pivot
      while ( pivot < a[--j]) {} // j happy to move backward bigger items are right of pivot
      // when both i and j stops, they are either waiting to be swapped or already past each other
      if (i < j) {
        std::swap(a[i], a[j]);
      }
      else {
        break; // done here - i pasts j and is pointing to where we should have the pivot
      }
    }
    // put the pivot to correct position
    std::swap(a[i], a[right - 1]);

    // then sort the two halfves
    quicksort(a, left, i - 1);
    quicksort(a, i + 1, right);
  }
  else // insertionSort when the original is divided small enough
  {
    insertionSort(a, left, right);
  }
}

template <typename Comparable>
void quicksort(vector<Comparable> & a) {
  // call the long version
  quicksort(a, 0, a.size() - 1);
}

// ADD SO THAT TestSort.H compile
// NEED TO COME BACK AND WRITE/UNDERSTAND THESE MYSELF

/**
 * Internal selection method that makes recursive calls.
 * Uses median-of-three partitioning and a cutoff of 10.
 * Places the kth smallest item in a[k-1].
 * a is an array of Comparable items.
 * left is the left-most index of the subarray.
 * right is the right-most index of the subarray.
 * k is the desired rank (1 is minimum) in the entire array.
 */
template <typename Comparable>
void quickSelect( vector<Comparable> & a, int left, int right, int k )
{
    if( left + 10 <= right )
    {
        const Comparable & pivot = median3( a, left, right );

            // Begin partitioning
        int i = left, j = right - 1;
        for( ; ; )
        {
            while( a[ ++i ] < pivot ) { }
            while( pivot < a[ --j ] ) { }
            if( i < j )
                std::swap( a[ i ], a[ j ] );
            else
                break;
        }

        std::swap( a[ i ], a[ right - 1 ] );  // Restore pivot

            // Recurse; only this part changes
        if( k <= i )
            quickSelect( a, left, i - 1, k );
        else if( k > i + 1 )
            quickSelect( a, i + 1, right, k );
    }
    else  // Do an insertion sort on the subarray
        insertionSort( a, left, right );
}

/**
 * Quick selection algorithm.
 * Places the kth smallest item in a[k-1].
 * a is an array of Comparable items.
 * k is the desired rank (1 is minimum) in the entire array.
 */
template <typename Comparable>
void quickSelect( vector<Comparable> & a, int k )
{
    quickSelect( a, 0, a.size( ) - 1, k );
}


template <typename Comparable>
void SORT( vector<Comparable> & items )
{
    if( items.size( ) > 1 )
    {
        vector<Comparable> smaller;
        vector<Comparable> same;
        vector<Comparable> larger;

        auto chosenItem = items[ items.size( ) / 2 ];

        for( auto & i : items )
        {
            if( i < chosenItem )
                smaller.push_back( std::move( i ) );
            else if( chosenItem < i )
                larger.push_back( std::move( i ) );
            else
                same.push_back( std::move( i ) );
        }

        SORT( smaller );     // Recursive call!
        SORT( larger );      // Recursive call!

        std::move( begin( smaller ), end( smaller ), begin( items ) );
        std::move( begin( same ), end( same ), begin( items ) + smaller.size( ) );
        std::move( begin( larger ), end( larger ), end( items ) - larger.size( ) );

/*
        items.clear( );
        items.insert( end( items ), begin( smaller ), end( smaller ) );
        items.insert( end( items ), begin( same ), end( same ) );
        items.insert( end( items ), begin( larger ), end( larger ) );
*/
    }
}

/*
 * This is the more public version of insertion sort.
 * It requires a pair of iterators and a comparison
 * function object.
 */
template <typename RandomIterator, typename Comparator>
void insertionSort( const RandomIterator & begin,
                    const RandomIterator & end,
                    Comparator lessThan )
{
    if( begin == end )
        return;

    RandomIterator j;

    for( RandomIterator p = begin+1; p != end; ++p )
    {
        auto tmp = std::move( *p );
        for( j = p; j != begin && lessThan( tmp, *( j-1 ) ); --j )
            *j = std::move( *(j-1) );
        *j = std::move( tmp );
    }
}

/*
 * The two-parameter version calls the three parameter version, using C++11 decltype
 */
template <typename RandomIterator>
void insertionSort( const RandomIterator & begin,
                    const RandomIterator & end )
{
    insertionSort( begin, end, less<decltype(*begin )>{ } );
}






#endif
