#include "Vector.H"
#include <iostream>
#include <algorithm>
using namespace std;

void print( const Vector<Vector<int>> arr )
{
    int N = arr.size( );

    for( int i = 0; i < N; ++i )
    {
        cout << "arr[" << i << "]:";

        for( int j = 0; j < arr[ i ].size( ); ++j )
            cout << " " << arr[ i ][ j ];

        cout << endl;
    }
}

class CompareVector
{
public:
    bool operator() ( const Vector<int> & lhs, const Vector<int> & rhs ) const
    { return lhs.size( ) < rhs.size( ); }
};

int main( )
{
    const int N = 2;
    //Vector<Vector<int>> arr( N );
    cout << "Make just one vector." << endl;
    Vector<int> v (2);
    v.push_back(100);
    v.push_back(10);
    cout << v.back() << endl;
    Vector<int> n (std::move(v));
    //n = std::move(v);
    cout << v.size() << endl;

    cout << n.back() << endl;
    for( int i = N - 1; i > 0; --i )
    {
        n.push_back( i );
        cout << "Insert number: " << i << endl;
        //arr[ i ] = v;

    }
    cout << "After loop" << endl;
    //cout << v.capacity() << endl;
    //cout << v.size() << endl;

    // print( arr );

    // clock_t start = clock( );
    // std::sort( begin( arr ), end( arr ), CompareVector{ } );
    // clock_t end = clock( );
    // cout << "Sorting time: " << ( end - start ) << endl;

    // print( arr );

    return 0;
}
