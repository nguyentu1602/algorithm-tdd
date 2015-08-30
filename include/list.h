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


// double linked list have nodes that contain pointers to previous and next nodes
template <typename E>
class listNode {
  friend class list<E>;
 private:
  E element_;
  listNode* prev_;
  listNode* next_;
  // ctor
  listNode(const E& e = E{}, listNode* p = nullptr, listNode* n = nullptr)
      : element_{e}, prev_{p}, next_{n} {}
  // move ctor
  listNode(E&& e, listNode* p = nullptr, listNode* n = nullptr)
      : element_{std::move(e)}, prev_{p}, next_{n} {}

};

template <typename E>
class list {
  // NOT YET IMPLEMENTED
 private:
  // each list will have a size_ counter and two pointers to the beginning and end
  // we can make to sentinel nodes for head_ and tail_ to simplify implementation
  size_t size_;
  listNode<E>* head_;
  listNode<E>* tail_;

  // make the two sentinels nodes with init()
  void init() {
    size_ = 0;
    head_ = new listNode<E>();
    tail_ = new listNode<E>();
    head_->next_ = tail_;
    tail_->prev_ = head_;
  }
};

#endif //LIST_H_
