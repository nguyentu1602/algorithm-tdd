#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "dsexceptions.H"

/**Vector class
   O(1) access
   member method:
   size(), capacity(), empty(), push_back(), pop_back(), back(), front()
   begin(), end(),
 **/
using std::cout;
using std::endl;


template <typename Object>
class Vector
{
 public:
  // typedef size_t size_type;
  // THE BIG FIVE:
  // 0. Default ctor
  explicit Vector(int initSize = 0)
      : theSize { initSize }, theCapacity{ initSize + SPARE_CAPACITY },
    objects { new Object[ initSize + SPARE_CAPACITY ] } {
      cout << "0. Use default ctor." << endl;
    }
  // 1. Copy ctor
  Vector(const Vector & rhs)
      : theSize { rhs.theSize }, theCapacity { rhs.theCapacity },
    objects { new Object[rhs.theCapacity]}
  {
      // for each item in rhs.objects, copy them over
      for ( int i = 0; i < theSize; i++)
        objects[ i ] = rhs.objects[ i ];
      cout << "1. Use copy ctor." << endl;
  }
  // 2. Assignment operator
  // version 1:
  Vector & operator=(const Vector & rhs) {
    theSize = rhs.theSize;
    theCapacity = rhs.theCapacity;
    objects = new Object[ theCapacity ];
    for ( int i = 0; i < theSize; i++)
      objects[i] = rhs.objects[i];
    cout << "2. Use assignment operator." << endl;
  }
  // version 2: use the move trick:
  // Vector & operator=(const Vector & rhs){
  //   cout << "2. Right before Vector copy = rhs." << endl;
  //   //Vector copy = rhs; //implicit conversion to use the copy constructor
  //   Vector copy = Vector(rhs);
  //   cout << "2. Right after Vector copy = rhs." << endl;
  //   std::swap ( *this, copy );
  //   cout << "2. Right after std::swap." << endl;
  //   cout << "2. Use assignment operator." << endl;
  //   return *this;
  // }

  // 3. Move ctor
  Vector( Vector && rhs)
      : theSize { rhs.theSize }, theCapacity { rhs.theCapacity }, objects { rhs.objects}
  {
        rhs.objects = nullptr;
        rhs.theCapacity = 0;
        rhs.theSize = 0;
        cout << "3. Use move ctor." << endl;
  }
  // 4. Move assignment operator
  Vector & operator=(Vector && rhs) {
    std::swap( theSize, rhs.theSize );
    std::swap( theCapacity, rhs.theCapacity);
    std::swap( objects, rhs.objects );
    cout << "4. Use move assignment operator." << endl;
    return *this;
  }

  // 5. Destructor
  ~Vector() {
    delete [] objects;
    cout << "5. Use destructor." << endl;
  }

  // THE INTERFACE:

  // should have used size_type instead of int for size and capacity return types
  int size() const { return theSize; }
  int capacity () const { return theCapacity; }
  bool empty() const { return theSize == 0; }

  const Object & operator[] (int index) const {
    if ( index < 0 || index >= size())
      throw ArrayIndexOutOfBoundsException {};
    return objects [ index ];
  }

  Object & operator[](int index) {
    if ( index < 0 || index >= size())
      throw ArrayIndexOutOfBoundsException {};
    return objects[ index ];
  }

  void resize(int newSize) {
    if (newSize > theCapacity)
      reserve( newSize * 2 );
    theSize = newSize;
  }
  // reserve a new memory block for moving over
  // new capacity is twice as big as newSize
  void reserve( int newCapacity) {
    if (newCapacity >= theCapacity) {
      auto temp = new Object [ newCapacity ];
      for (int i = 0; i < theSize; i++)
        temp[i] = std::move (objects[i]);

      theCapacity = newCapacity;
      std::swap(objects, temp);
      delete [] temp;
    }
  }
  void push_back(const Object & x) {
    if ( theSize == theCapacity) {
      resize(theCapacity * 2 + 1);
    }
    objects[ theSize++ ] = x;
  }
  void push_back(Object && x) {
    if (theSize == theCapacity) {
      resize( theCapacity * 2 + 1);
    }
    objects[ theSize++ ] = std::move(x);
  }
  void pop_back () {
    if (empty())
      throw UnderflowException{};
    --theSize;
  }
  const Object & back () const {
    if (empty())
      throw UnderflowException{};
    return objects[theSize - 1];
  }

  // HELPER TYPE: ITERATOR
  typedef Object* iterator;
  typedef const Object* const_iterator;

  iterator begin() {
    return &objects[0];
  }
  const_iterator begin() const {
    return &objects[0];
  }
  iterator end() { return &objects[theSize]; }
  const_iterator end() const { return &objects[theSize]; }

  // HELPER CONST:
  static const int SPARE_CAPACITY = 2;

 private:
  int theSize; // keep track of the size of the Vector
  int theCapacity; // keep track of the capacity of the vector
  Object * objects; // pointer to an Object, also array of Objects
};

#endif
