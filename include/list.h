#ifndef LIST_H_
#define LIST_H_
#include <cstddef>
#include <utility>
#include <iostream>
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
      sort()
      merge()
      splice()
      reverse()
      remove()

   4. Accessors
      front()
      back()

   5. Iterators
      begin
      end
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

/**
 *  @brief A list::list_iterator
 *
 *  All the functions are op overloads
 */
template <typename E>
class list_iterator {
  // hack to make test works - because derived class does not inherit friendship
  friend class listIteratorTest;
  friend class list<E>;

  // typedef
  typedef list_iterator<E>         _Self;
  typedef listNode<E>              _Node;
  typedef std::ptrdiff_t           difference_type;
  typedef E                        value_type;
  typedef E*                       pointer;
  typedef E&                       reference;

 public:
  list_iterator()
      : current_(nullptr) {}

  explicit
  list_iterator(_Node* p)
      : current_(p) {}

  // const cast

  // MUTATOR
  reference
  operator*() const {
    return current_->element_;
  }

  // re-introdued ACCESSOR
  // pointer operator->() const {
  //   return
  // }

  // all operations ++, --(pre & post), == and != are inherited from base
  // provide operators for the inherited class
  _Self&
  operator++() {
    current_ = current_->next_;
    return *this;
  }
  _Self
  operator++( int ) {
    _Self tmp_ = *this;
    current_ = current_->next_;
    return tmp_;
  }

  _Self&
  operator--() {
    current_ = current_->prev_;
    return *this;
  }

  _Self
  operator--( int ) {
    list_iterator temp_ = *this;
    current_ = current_->prev_;
    return temp_;
  }

  bool
  operator==(const _Self& x) const {
    return current_ == x.current_;
  }

  bool
  operator!=(const _Self& x) const {
    return current_ != x.current_;
  }

  // data member:
  _Node* current_;
};


template <typename E>
class list_const_iterator {
  friend class list<E>;
  // this is a hack to allow listIteratorTest to access the parametrized ctor:
  friend class listIteratorTest;

  // this class stores a pointers to a listNode object
  // and provide basic iterator operations: =, ==, ++, and !=

  // typedef for all:
  typedef list_const_iterator<E>   _Self;
  typedef listNode<E>              _Node;
  typedef list_iterator<E>         iterator;
  typedef std::ptrdiff_t           difference_type;
  typedef E                        value_type;
  typedef E*                       pointer;
  typedef E&                       reference;

 public:
  list_const_iterator()
      : current_ {nullptr} {}

  explicit
  list_const_iterator(_Node* p)
      : current_ {p} {}
  // convert from list_iterator to list_const_iterator
  list_const_iterator(const iterator& x)
      : current_ {x.current_} {}
  // operators
  // The overloaded operator * on a const_iterator will return a const reference
  // to the E inside the node.
  // The rhs const enforces that the reference returned is const, i.e. you cannot change
  // the E object. Notice that const on return value of a funtion only makes sense when
  // you return by reference.

  reference
  operator*() const {
    return current_->element_;
  }

  _Self&
  operator++() {
    // this signature is for the prefix form ++itr
    current_ = current_->next_; // change the state of the const_iterator obj
    return *this; // return reference to the current object
  }

  // postfix form itr++ return the current value first, increment later
  _Self
  operator++(int) {
    // this signature is for the postfix form of itr++
    _Self tmp_ = *this; // return
    current_ = current_->next_;
    return tmp_;
  }

  _Self&
  operator--() {
    current_ = current_->prev_;
    return *this;
  }

  _Self
  operator--(int) {
    _Self tmp_ = *this;
    current_ = current_->prev_;
    return tmp_;
  }

  bool
  operator==(const _Self& rhs) const {
    return current_ == rhs.current_;
  }

  bool operator!=(const _Self& rhs) const {
    return current_ != rhs.current_; // compare the references of the two iterator objects
  }
  // data member
  _Node* current_;
};

// compare list_const_iterator and list_iterator
template<typename _Val>
inline bool
operator==(const list_iterator<_Val>& x, const list_const_iterator<_Val>& y) {
  return x.current_ == y.current_;
}

template<typename _Val>
inline bool
operator!=(const list_iterator<_Val>& x, const list_const_iterator<_Val>& y) {
  return x.current_ != y.current_;
}

// class list_iterator inherites all the public attributes of the class
// list_const_iterator; we can add new data, add new methods or redefine old methods.
// When we add new data or change inherited methods we need the virtual keyword in
// the base-class (list_const_iterator)
// The main difference between this class and the base class is that we introduce
// another version of operator*() which acts as a MUTATOR.

template <typename E>
class list {
 protected:
  typedef listNode<E> _Node;
 public:
  // the iterator type - make them testable!
  typedef list_const_iterator<E>    const_iterator;
  typedef list_iterator<E>          iterator;
  typedef E                         value_type;
  typedef E&                        reference;
  typedef size_t                    size_type;
  typedef ptrdiff_t                 difference_type;
  typedef std::bidirectional_iterator_tag iterator_category;
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
  list(const list& rhs) {
    init();
    const_iterator itr = rhs.begin();
    while(itr != rhs.end()) {
      push_back(*itr++);
    }
  }
  // 3 + 5. assignment operator + move assignment operator
  // why not (const list& rhs) as usual? Because of copy elision!
  // for assignment operator, if we pass-by-reference, we will still
  // need to copy the list inside the body of the function. If we
  // pass-by-value, then the copy is done at the parameter list already.
  // in the case that we don't need to copy, the compiler will automatically
  // optimize it for us. So that's a rule of thumb: if you need to copy an
  // object, then let the compiler do it for you at the parameter list, don't
  // do it yourself. Ref:
  // http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
  // This is actually amazing! serve as both assignment operator and move
  // assignment operator! clearly a superior way of doing things!
  list& operator=(list rhs) {
    swap(*this, rhs); // called the overloaded swap function
    return *this;
  }

  // 3.5 swap() function as helper for assignment operator
  friend void swap(list& first, list& second) { //no throw
    using std::swap;
    swap(first.size_, second.size_);
    swap(first.head_, second.head_);
    swap(first.tail_, second.tail_);
  }
  // 4. move ctor
  list(list&& rhs) {
    init();
    swap(*this, rhs);
  }
  // 5. move assignment operator: NO NEED, embedded in assignment operator

  // iterators
  iterator
  begin() {
    return iterator(head_->next_);
  }

  const_iterator
  begin() const {
    return const_iterator(head_->next_);
  }

  iterator
  end() {
    return iterator(tail_);
  }

  const_iterator
  end() const {
    return const_iterator(tail_);
  }

  // capacity methods
  bool empty() const {
    return size_ == 0;
  }

  size_type size() const {
    return size_;
  }

  // modifiers
  // push_front, push_back, pop_front, pop_back
  // these are basic double-ended queue operations
  void push_front(const E& element) {
    insert(begin(), element);
  }

  void push_front(E&& element) {
    insert(begin(), std::move(element));
  }

  void push_back(const E& element) {
    insert(end(), element);
  }

  void push_back(E&& element) {
    insert(end(), std::move(element));
  }

  // accessors
  reference
  front() {
    return *begin();
  }

  const reference
  front() const {
    return *begin();
  }

  reference
  back() {
    return *(--end());
  }
  const reference
  back() const {
    return *(--end());
  }

  // insert take an iterator pointing to a node and insert another node
  // right BEFORE iter, then return a new iterator pointing to that node
  // insert by lvalue calls insert by rvalue to conform to DRY
  iterator
  insert(iterator iter, const E& element) {
    value_type copied(element);
    return insert(iter, std::move(copied));
  }

  // insert rvalue using move semantic
  iterator
  insert(iterator iter, E&& element) {
    _Node* ptr = iter.current_;
    ptr->prev_->next_ = new _Node (std::move(element), ptr->prev_, ptr);
    ptr->prev_ = ptr->prev_->next_;
    size_++;
    return iterator(ptr->prev_);
  }

  // erase the node that the iterator is pointing to
  // return an iterator pointing to one node after the deleted
  iterator
  erase(iterator iter) {
    _Node* to_del = iter.current_;
    iterator to_return (to_del->next_);
    to_del->prev_->next_ = to_del->next_;
    to_del->next_->prev_ = to_del->prev_;
    delete to_del;
    --size_;
    return to_return;
  }

  void
  clear() {
    while(!empty()) {
      erase(begin());
    }
  }

  // advance operation:
  /**
   *  splice transfers the elements in the range [first, last) from other into
   *  *this.
   *  The elements are inserted before the element pointed to by pos.
   */
  void
  splice(const_iterator pos, list& other,
         const_iterator first, const_iterator last) {
    if (first != last) {
      // get the number of item in [first, last)
      size_t difference = _S_distance (first.current_, last.current_);
      const_iterator last_prev = last;
      --last_prev;
      const_iterator pos_prev = pos;
      --pos_prev;

      // 1st step: shorten other
      first.current_->prev_->next_ = last.current_;
      last.current_->prev_ = first.current_->prev_;
      other.size_ -= difference;

      // 2nd step: lengthen *this
      first.current_->prev_ = pos_prev.current_;
      last_prev.current_->next_ = pos.current_;
      pos_prev.current_->next_ = first.current_;
      pos.current_->prev_ = last_prev.current_;
      size_ += difference;
    }
  }

  void
  splice(const_iterator pos, list& other) {
    splice(pos, other, other.begin(), other.end());
  }

  // void
  // splice(const_iterator pos, list&& other,
  //        const_iterator first, const_iterator last) {
  // }


 private:
  // each list will have a size_ counter and two pointers to the beginning and end
  // we can make to sentinel nodes for head_ and tail_ to simplify implementation
  size_t size_;
  _Node* head_;
  _Node* tail_;

  static size_t
  _S_distance(const _Node* first, const _Node* last) {
    size_t _n = 0;
    while(first != last) {
      first = first->next_;
      ++_n;
    }
    return _n;
  }
  // make the two sentinels nodes with init()
  void init() {
    size_ = 0;
    head_ = new _Node();
    tail_ = new _Node();
    head_->next_ = tail_;
    tail_->prev_ = head_;
  }
};

#endif //LIST_H_
