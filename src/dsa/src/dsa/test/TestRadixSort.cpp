#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "UniformRandom.H"
using std::vector;
using std::cout;
using std::endl;
using std::string;


// radix sort array of strings. ASCII
// assume all with the same length
void radixSortA(vector<string> & arr, int stringLen){
  const int NUM_BUCKETS = 256;

  vector<vector<string> > buckets(NUM_BUCKETS);

  // starting with the least significant char (rightmost)
  for (int pos = stringLen - 1; pos >= 0; --pos) {
    for (string & s : arr) {
      buckets[ s[pos] ].push_back(std::move(s)); // s[pos] is implicitly cast to int
    }

    int idx = 0;
    for (auto & thisBucket : buckets ) {
      for (string & s : thisBucket) {
        arr[idx++] = std::move(s);
      }
      thisBucket.clear();
    }
  }
}

// counting radixsort to avoid using vector of vectors of strings
// assume ASCII and same stringLen
void countingRadixSort(vector<string> & arr, int stringLen) {
  const int NUM_BUCKETS = 256;
  int N = arr.size();
  vector<string> buffer(N);
  // using two pointers in and out to refer to arr and buffer
  vector<string> * in = &arr;
  vector<string> * out = &buffer;
  // start the loop with least significant char
  for (int pos = stringLen - 1; pos >= 0; --pos) {
    // count[k+1] stores how many things go into bucket k
    vector<int> count (NUM_BUCKETS + 1);
    for (int i = 0; i < N; i++) {
      ++count[(*in)[i][pos] + 1];
    }
    // now build the offset vector
    for (int b = 1; b <= NUM_BUCKETS; b++) {
      count[b] = count[b] + count[b - 1];
    }
    // now do the retribution:
    for (int i = 0; i < N; i++) {
      (*out)[count[(*in)[i][pos]]++] = std::move((*in)[i]);
    }
    // swap in and out roles
    std::swap(in, out);
  }
  // if odd number of passes, in is buffer, out is arr; so copy back
  if( stringLen % 2 == 1 )
    for( int i = 0; i < arr.size( ); ++i )
      (*out)[ i ] = std::move( (*in)[ i ] );

}


// radix sort an array of strings
// assume all are ASCII
// asuume all have length bounded by maxlen

void radixSort(vector<string> & arr, int maxLen) {
  const int NUM_BUCKETS = 256;
  // grouping by length here
  vector<vector<string> > wordsByLength( maxLen + 1);
  // grouping by character here
  vector<vector<string> > buckets( NUM_BUCKETS );

  // first pass of the array is to buckets the string by lengths
  // each cell in wordsByLength has a vector of strings with similar sizes
  for ( string & s : arr )
    wordsByLength [ s.length() ].push_back( std::move(s));

  // put thing back into arr by length
  int idx = 0;
  for ( auto & wordList : wordsByLength )
    for (string & s : wordList)
      arr[idx++] = std::move(s);

  // now the main loop
  int startingIndex = arr.size(); // rightmost element in arr
  for (int pos = maxLen - 1; pos >= 0; --pos){
    //offset the startingIndex by the number of strings with length pos
    startingIndex -= wordsByLength[ pos + 1].size();
    // by counting backward from the end of arr, we make sure that we examine
    // only the string that long enough to contain a char at the 'pos' position

    for (int i = startingIndex; i < arr.size(); ++i) {
      buckets[ arr[i][pos] ].push_back(std::move(arr[i]));
    } // push stuff in starting from the starting Index to the end of the array
    // the number of strings pushed into the buckets are always the same as the
    // number that get emptied out of buckets of the code below.
    // (Because we push in and take out items in the buckets right away)
    idx = startingIndex;
    for(auto & thisBucket : buckets){
      for (string & s : thisBucket)
        arr[idx++] = std::move(s);

      thisBucket.clear();
    }
  }
}


// main method to test radixSort
int main( )
{
  vector<string> lst;
  UniformRandom r;

  const int LEN = 3;
  const int ADD = 7;

  for( int i = 0; i < 10; ++i )
  {
    int len = LEN + ADD; //r.nextInt( ADD ); // between 5 and 11 characters

    for (int inc_len = LEN; inc_len < len; ++inc_len ) {
      string str = "";
      for( int j = 0; j < inc_len; ++j )
        str += static_cast<char>( 'a' + r.nextInt( 26 ) );
      lst.push_back( str );
    }
  }

  vector<string> arr1 = lst;
  vector<string> arr2 = lst;

  sort( begin( arr1 ), end( arr1 ) );
  radixSort( arr2, LEN + ADD );
  //countingRadixSort( arr2, LEN + ADD );
  for (int j = 0; j < arr1.size(); ++j) {
    cout << arr2[j] << endl;
  }

  for( int i = 0; i < arr1.size( ); ++i )
    if( arr1[ i ] != arr2[ i ] )
      cout << "OOPS!!" << endl;
  return 0;
}
