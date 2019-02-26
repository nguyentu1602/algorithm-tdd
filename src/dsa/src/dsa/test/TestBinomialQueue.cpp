#include "BinomialQueue.H"
#include <iostream>
using namespace std;

int main( )
{
    int numItems = 100;
    BinomialQueue<int> h;
    BinomialQueue<int> h1;
    BinomialQueue<int> h2;
    BinomialQueue<int> h3;
    int i = 37;

    cout << "Begin test..." << endl;
    for( i = 37; i != 0; i = ( i + 37 ) % numItems )
        if( i % 2 == 0 )
            h1.insert( i );
        else
            h.insert( i );
    for (int j = 0; j < 10; j++)
        h3.insert(j);

    cout << "h3 " << h3.size() << endl;
    cout << "h1 " << h1.size() << endl;
    cout << "h " << h.size() << endl;
    h.merge( h1 );
    cout << "h " << h.size() << endl;
    h2 = h;
    for( i = 1; i < numItems; ++i )
    {

        int x = 0;
        cout << h2.size() << " BEFORE DELETE! " << i << endl;
        h2.deleteMin( x );
        if( x != i )
            cout << x << " Oops! " << i << endl;
    }

    if( !h1.isEmpty( ) )
        cout << "Oops! h1 should have been empty!" << endl;

    cout << "End of test... no output is good" << endl;

    return 0;
}
