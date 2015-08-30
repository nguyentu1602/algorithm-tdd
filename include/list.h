#ifndef LIST_H_
#define LIST_H_
#include <cstddef>
#include <utility>

/* The list data structure. support constant time insertion and removal
   of element anywhere in the containter. Fast random access is not a
   feature. Follow STL's implementation as double-linked list, thus allow
   bidirectional iteration.

   Interface:
   0. Ctor etc.
      operator=()
      assign()
   1. Capacity:
      empty()
      size()

   2. Modifiers:
      clear()
      insert()
      erase()
      push_back()
      pop_back()
      push_front()
      pop_front()
      resize()
      swap()  // optional

   3. Operations
      merge()
      splice()
      reverse()
      remove()

   4. Accessors
      front()
      back()

   5. Iterators
      begin
      cbegin
      end
      cend
      rbegin
      crbegin
      rend
      crend
 */

template <typename E>
class list;

template <typename E>
class listNode {
  friend class list<E>;
};

template <typename E>
class list {
  // NOT YET IMPLEMENTED
};

#endif //LIST_H_
