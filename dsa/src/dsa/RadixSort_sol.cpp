#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "UniformRandom.H"
using namespace std;


/*
 * Radix sort an array of Strings
 * Assume all are all ASCII
 * Assume all have same length
 */
void radixSortA( vector<string> & arr, int stringLen )
{
    const int BUCKETS = 256; //why 256? ASCII? Unicode?
    // there are 256 buckets, each is a vector of strings
    vector<vector<string>> buckets( BUCKETS );
    // start with the least significant character (rightmost)
    for( int pos = stringLen - 1; pos >= 0; --pos )
    {
        // s[pos] returns a char, which is then cast to an int to index buckets
        // push s into the bucket associated with s[pos]
        for( string & s : arr )
            buckets[ s[ pos ] ].push_back( std::move( s ) );

        // done arranging by pos
        int idx = 0;
        for( auto & thisBucket : buckets ) // for each bucket
        {
            for( string & s : thisBucket ) // iterate through contained string
                arr[ idx++ ] = std::move( s );  //put them back into arr, now posth sorted

            thisBucket.clear( );
        }
    }
}

/*
 * Counting radix sort an array of Strings
 * Assume all are all ASCII
 * Assume all have same length
 */
void countingRadixSort( vector<string> & arr, int stringLen )
{
    const int BUCKETS = 256;

    int N = arr.size( );
    vector<string> buffer( N );

    vector<string> *in = &arr;
    vector<string> *out = &buffer;

    for( int pos = stringLen - 1; pos >= 0; --pos )
    {
        vector<int> count( BUCKETS + 1 );

        for( int i = 0; i < N; ++i )
            ++count[ (*in)[ i ][ pos ] + 1 ];

        for( int b = 1; b <= BUCKETS; ++b )
            count[ b ] += count[ b - 1 ];

        for( int i = 0; i < N; ++i )
            (*out)[ count[ (*in)[ i ][ pos ] ]++ ] = std::move( (*in)[ i ] );

            // swap in and out roles
        std::swap( in, out );
    }

        // if odd number of passes, in is buffer, out is arr; so copy back
    if( stringLen % 2 == 1 )
        for( int i = 0; i < arr.size( ); ++i )
            (*out)[ i ] = std::move( (*in)[ i ] );
}

/*
 * Radix sort an array of Strings
 * Assume all are all ASCII
 * Assume all have length bounded by maxLen
 */
void radixSort( vector<string> & arr, int maxLen )
{
    const int BUCKETS = 256;

    vector<vector<string>> wordsByLength( maxLen + 1 );
    vector<vector<string>> buckets( BUCKETS );

    for( string & s : arr )
        wordsByLength[ s.length( ) ].push_back( std::move( s ) );

    int idx = 0;
    for( auto & wordList : wordsByLength )
        for( string & s : wordList )
            arr[ idx++ ] = std::move( s );

    int startingIndex = arr.size( );
    for( int pos = maxLen - 1; pos >= 0; --pos )
    {
        startingIndex -= wordsByLength[ pos + 1 ].size( );

        for( int i = startingIndex; i < arr.size( ); ++i )
            buckets[ arr[ i ][ pos ] ].push_back( std::move( arr[ i ] ) );

        idx = startingIndex;
        for( auto & thisBucket : buckets )
        {
            for( string & s : thisBucket )
                arr[ idx++ ] = std::move( s );

            thisBucket.clear( );
        }
    }
}

int main( )
{
    vector<string> lst;
    UniformRandom r;

    const int LEN = 5;
    const int ADD = 7;

    for( int i = 0; i < 1000; ++i )
    {
        string str = "";
        int len = LEN + ADD; //r.nextInt( ADD ); // between 5 and 11 characters

        for( int j = 0; j < len; ++j )
            str += static_cast<char>( 'a' + r.nextInt( 26 ) );

        lst.push_back( str );
    }

    vector<string> arr1 = lst;
    vector<string> arr2 = lst;

    sort( begin( arr1 ), end( arr1 ) );
    //radixSortA( arr2, LEN + ADD );
    countingRadixSort( arr2, LEN + ADD );
    for (int j = 0; j < arr1.size(); ++j) {
        cout << arr2[j] << endl;
    }

    for( int i = 0; i < arr1.size( ); ++i )
        if( arr1[ i ] != arr2[ i ] )
            cout << "OOPS!!" << endl;
    return 0;
}
