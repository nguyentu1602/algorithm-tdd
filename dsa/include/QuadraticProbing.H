#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional> // this is for the hash functor
#include <string>
using namespace std;

int nextPrime(int n);
// interface of the class:
// bool contains(const HashedObj & x)
// bool insert(HashedObj & x)
// bool remove(HashedObj & x)
// void makeEmpty()
// something to hashstrings?

template <typename HashedObj>
class HashTable {
 public:
  // The big five or lack there of? - lack there of because the internal stuff are very simple
  explicit HashTable( int size = 101 ) : array ( nextPrime ( size )) {
    makeEmpty(); // to set all .info fields into EMPTY
  }
  bool contains(const HashedObj & x) const {
    return isActive(findPos(x));
  }

  bool insert(HashedObj && x){
    // how do you insert?
    // step 1:
    int iPos = findPos(x);
    // step 2:
    if (isActive(iPos)){
      return false;
    }
    // step 3: insert
    array[iPos].element = std::move(x);
    array[iPos].info = ACTIVE;

    // step 4: rehash
    ++currentSize;
    if (( array.size() - currentSize) < currentSize) {
      rehash();
    }
    return true;
  }

  bool insert(const HashedObj & x) {
    HashedObj copy = x;
    return insert(std::move(copy));
  }

  bool remove(const HashedObj & x) {
    int delPos = findPos(x);
    if(!isActive(delPos)) {
      return false;
    }
    array[delPos].info = DELETED;
    return true;
  }

  void makeEmpty() {
    currentSize = 0;
    for(auto & item : array){
      item.info = EMPTY;
    }
  }

  // an enum to restrict type of entry
  enum EntryType {ACTIVE, EMPTY, DELETED};// to make sure find works after deleting something
 private:
  // internal type to represent each entry.
  struct HashEntry {
    HashedObj element;
    EntryType info;

    HashEntry(const HashedObj & e = HashedObj{}, EntryType i = EMPTY)
        : element {e}, info {i} {}
    HashEntry(HashedObj && e, EntryType i = EMPTY)
        : element {std::move(e)}, info {i} {}
  };
  // internal array to hold entries
  vector<HashEntry> array;
  int currentSize;

  // internal method needed:

  // one to probe if a position is ACTIVE or not
  bool isActive(int currentPos) const {
    return array[currentPos].info == ACTIVE;
  }
  // one to find position to insert - collision resolution!
  int findPos (const HashedObj & x) const {
    int offset = 1;
    int currentPos = myhash(x);
    while (array[currentPos].info != EMPTY &&
           array[currentPos].element != x) {
      currentPos += offset; // compute ith probe
      offset += 2;
      if (currentPos >= array.size()) {
        currentPos -= array.size();
      }
    }
    return currentPos;
  }

  // one to rehash
  void rehash() {
    vector<HashEntry> oldArray = array;

    array.resize(nextPrime(2 * oldArray.size()));
    for (auto & item : array) {
      item.info = EMPTY;
    }
    currentSize = 0;

    for (auto & olditem : oldArray) {
      if(olditem.info == ACTIVE){
        insert(std::move(olditem.element));
      }
    }
  }
  // one to hash:
  size_t myhash(const HashedObj & x) const {
    static hash<HashedObj> hf;
    return hf(x) % array.size();
  }

};


#endif
