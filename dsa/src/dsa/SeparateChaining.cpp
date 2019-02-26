#include <iostream>
#include "SeparateChaining.H"
using namespace std;

bool isPrime(int n) {
  if (n == 2 || n == 3) {
    return true;
  }
  if ( n == 1 || n % 2 == 0) {// because modulo 2 is very fast
    return false;
  }
  for (int i = 3; i * i <= n; i += 2) {
    if ( n % i == 0) {
      return false;
    }
  }
  return true;
}

/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */

int nextPrime(int n) {
  if ( n % 2 == 0) {
    ++n;
  }

  for ( ; !isPrime(n); n += 2) // add 2 to an odd number until you get a prime
    ;

  return n;
}

size_t hash(const string & key){
  size_t hashVal = 0;
  for (char ch : key) {
    hashVal = 37 * hashVal + ch;
  }
  return hashVal;
}

size_t hash(int key) {
  return key;
}
