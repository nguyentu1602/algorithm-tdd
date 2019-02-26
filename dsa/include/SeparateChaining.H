#ifndef SEPARATE_CHAINING_H
#define SEPARATE_CHAINING_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional> // what for?
using namespace std; // bad practice

int nextPrime(int n); //helper function

// SeparateChaining Hash Table class
// this operate likes a set.
// operation supported:
// bool insert() constant complexity amortized time
// bool remove() constant complexity amortized time
// bool contains()  constant complexity amortize time
// void makeEmpty()

template <typename HashedObj>
class HashTable
{
 public:
  // constructors! because no pointer involved, we can safely use the default ctors here
  // only need one customized ctor for parametrized initialization
  explicit HashTable (int size = 101)
      : currentSize{0}
  {theLists.resize(101);}

  // the interface:
  bool insert(const HashedObj & x) {
    HashedObj copy = x;
    return insert(std::move(copy));
  }

  bool insert(HashedObj && x) {
    auto & whichList = theLists[ myhash(x)];
    if( find( begin(whichList), end(whichList), x) != end( whichList)) {
      return false;
    } else {
      whichList.push_back(std::move(x));
      // maintain a load factor of atmost 1:
      if(++currentSize > theLists.size()) {
        rehash();
      }
      return true;
    }
  }

  bool remove(const HashedObj & x) {
    list<HashedObj> & whichList = theLists[myhash(x)];
    auto todel = find( begin(whichList), end(whichList), x);
    if (todel == end( whichList)) {
      return false;
    } else {
      whichList.erase(todel);
      --currentSize;
      return true;
    }
  }

  bool contains(const HashedObj& x) {
    list<HashedObj> & whichList = theLists[myhash(x)];
    return (find( begin(whichList), end(whichList), x) != end( whichList));
  }
  void makeEmpty() {
    for (auto & ls : theLists) {
      ls.clear();
    }
  }

 private:
  // load factor - current size and capacity?
  int currentSize;
  // a table storing bunch of linked list
  vector<list<HashedObj> > theLists;
  // internal method to resize the whole thing and then rehash
  void rehash() {
    // store a temp copy to be moved later
    vector<list<HashedObj> > oldLists = theLists;
    // get a new table, new size must still be a prime
    int newsize = nextPrime(2*theLists.size());
    //theLists = vector<list<HashedObj> > (newsize); // init a vector with the new size
    theLists.resize(newsize); // init a vector with the new size
    for (auto & thisList : theLists)
      thisList.clear();
    currentSize = 0;

    for (auto & thisList : oldLists) {
      for (auto & x : thisList) {
        insert(std::move(x));
      }
    }
  }
  // internal method to hash!
  size_t myhash (const HashedObj & x) const {
    // why does it need to be static here? just for performance? merging?
    static hash<HashedObj> hf; // instantiate a functor. In header functional. Or is it in cpp file
    return hf(x) % theLists.size();
  }
};


#endif
