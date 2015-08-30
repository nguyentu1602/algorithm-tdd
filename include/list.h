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
 public:
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
class list_const_iterator {
  friend class list<E>;
  // this is a hack to allow listIteratorTest to access the parametrized ctor:
  friend class listIteratorTest;
  // this class stores a pointers to a listNode object
  // and provide basic iterator operations: =, ==, ++, and !=
 public:
  list_const_iterator() : current_ {nullptr} {}
  // operators
  const E& operator*() const {
    return retrieve();
  }

  // prefix form ++itr increments first, return the object later
  virtual list_const_iterator& operator++() {
    // this signature is for the prefix form ++itr
    current_ = current_->next_; // change the state of the const_iterator obj
    return *this; // return reference to the current object
  }

  // postfix form itr++ return the current value first, increment later
  virtual list_const_iterator operator++(int) {
    // this signature is for the postfix form of itr++
    list_const_iterator old_ = *this; // return
    ++(*this);
    return old_;
  }

  virtual list_const_iterator& operator--() {
    current_ = current_->prev_;
    return *this;
  }

  virtual list_const_iterator operator--(int) {
    list_const_iterator old_ = *this;
    --(*this);
    return old_;
  }

  bool operator==(const list_const_iterator & rhs) const {
    return current_ == rhs.current_;
  }

  bool operator!=(const list_const_iterator & rhs) const {
    return !(*this == rhs); // compare the references of the two iterator objects
  }
 protected:
  // why protected: because we want the derived class iterator to
  // have access to these data and methods
  listNode<E>* current_;
  // retrieve() must return E&, not just the node
  E& retrieve() const {
    return current_->element_;
  }
  // protected parametrized-ctor for const_iterator
  // why: because we want the ctor available to the list class only
  list_const_iterator(listNode<E>* p) : current_{p} {}
};


template <typename E>
class list {
 public:
  // the iterator type - make them testable!
  typedef list_const_iterator<E> const_iterator;

  // the big five:
  // 0. default ctor
  list() {
    init();
  }
  // 1. destructor
  ~list() { // pop everything then delete the two sentinels
    clear();
    delete head_;
    delete tail_;
  }
  // 2. copy ctor
  // 3. assignment operator
  // 4. move ctor
  // 5. move assignment operator

  // capacity methods
  bool empty() const {
    return size_ == 0;
  }

  size_t size() const {
    return size_;
  }

  void clear() {
    // NOT YET IMPLEMENTED
  }

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
