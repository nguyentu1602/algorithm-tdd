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
  // The overloaded operator * on a const_iterator will return a const reference
  // to the E inside the node.
  // The rhs const enforces that the reference returned is const, i.e. you cannot change
  // the E object. Notice that const on return value of a funtion only makes sense when
  // you return by reference.
  // The lhs const enforces that the function receives a "const *this" pointer
  // to self only; thus allowing the method to operate on const object. It is a promise
  // not to change anything about the object, so in this case you cannot alter the
  // "current_" data member of the const_iterator object.
  // in short, the two consts make a const_iterator "const", as opposed to a normal iterator.

  const E& operator*() const {
    return retrieve();
  }

  // The following virtual keywords allow derived class to override these operators,
  // and make sure if we have a pointer of type pointing to base, that actually points
  // to an object of derived type, then the method called will be the override method
  // defined in the derived class, not the original method in the base class.
  // TODO: check whether we need virtual here.

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
  // retrieve() must return E&, not just the node, and only on
  E& retrieve() const {
    return current_->element_;
  }
  // protected parametrized-ctor for const_iterator
  // why: because we want the ctor available to the list class only
  list_const_iterator(listNode<E>* p) : current_{p} {}
};

// class list_iterator inherites all the public attributes of the class
// list_const_iterator; we can add new data, add new methods or redefine old methods.
// When we add new data or change inherited methods we need the virtual keyword in
// the base-class (list_const_iterator)
// The main difference between this class and the base class is that we introduce
// another version of operator*() which acts as a MUTATOR.

template <typename E>
class list_iterator : public list_const_iterator<E> {
  // hack to make test works - because derived class does not inherit friendship
  friend class listIteratorTest;
  friend class list<E>;
 public:
  list_iterator() {}

  // newly-introduced MUTATOR
  E& operator*() {
    return list_const_iterator<E>::retrieve();
  }

  // re-introdued ACCESSOR
  const E& operator*() const {
    return list_const_iterator<E>::operator*();
  }

  // all operations ++, --(pre & post), == and != are inherited from base

 protected:
  // protected constructor for the iterator
  // it actually calls the const_iterator constructor - DRY here.
  list_iterator(listNode<E>* p) : list_const_iterator<E> {p} {}
};


template <typename E>
class list {
 public:
  // the iterator type - make them testable!
  typedef list_const_iterator<E> const_iterator;
  typedef list_iterator<E> iterator;
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

  // modifiers
  // insert take an iterator pointing to a node and insert another node
  // right BEFORE iter, then return a new iterator pointing to that node
  iterator insert(iterator iter, const E& element) {
    listNode<E>* ptr = iter.current_;
    ptr->prev_->next_ = new listNode<E> (element, ptr->prev_, ptr);
    ptr->prev_ = ptr->prev_->next_;
    size_++;
    return iterator(ptr->prev_);
  }

  // insert rvalue using move semantic
  iterator insert(iterator iter, E&& element) {
    listNode<E>* ptr = iter.current_;
    ptr->prev_->next_ = new listNode<E> (std::move(element), ptr->prev_, ptr);
    ptr->prev_ = ptr->prev_->next_;
    size_++;
    return iterator(ptr->prev_);
  }

  // erase the node that the iterator is pointing to
  // return an iterator pointing to one node after the deleted
  iterator erase(iterator iter) {
    listNode<E>* to_del = iter.current_;
    iterator to_return (to_del->next_);
    to_del->prev_->next_ = to_del->next_;
    to_del->next_->prev_ = to_del->prev_;
    delete to_del;
    --size_;
    return to_return;
  }


  void clear() {
    while(!empty()) {
      erase(begin());
    }
  }

  // iterators
  iterator begin() {
    return iterator(head_->next_);
  }

  iterator end() {
    return iterator(tail_);
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
