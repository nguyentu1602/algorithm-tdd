#ifndef LIST_H
#define LIST_H

#include <algorithm>
using namespace std;

template <typename Object>
class List
{
 private:
  // you first need the nested private data type Node
  // Struct is similar to class with all members default to private
  // this node is for doubly linked list
  struct Node {
    Object data;
    Node* prev;
    Node* next;
    // constructor for Node:
    Node( const Object & d = Object {}, Node* p = nullptr, Node* n = nullptr)
        : data{ d }, prev { p }, next { n } { }
    Node( Object && d, Node* p = nullptr, Node* n = nullptr)
        : data { std::move ( d ) }, prev { p }, next { n } { }
  };

 public:
  // the 2 iterator-classes
  class const_iterator
  {
    // This class is nested. It stores a pointers to a Node object and provides basis
    // iterator operations: =, ==, ++, and !=.
    // The tricky part is that it returns
   public:
    // constructor for the const_iterator class:
    // default constructor to put current in nullptr
    const_iterator(): current { nullptr } {}
    // p-ctor is protected. Why?
    // now on to the overloaded operator
    // the * operator on a const_iterator will return the reference to the Object inside the Node
    // that the iterator is pointing to
    const Object & operator*( ) const {
      return retrieve();
    }

    // prefix form ++itr increments first, return the object later
    virtual const_iterator & operator++( ) { // this signature is for the prefix form ++itr
      // move the current pointer to the next Node
      current = current->next; // change the state of the const_iterator obj
      return *this; // return reference to the current object.
    }

    // postfix form itr++ return the current value first, increment later
    virtual const_iterator operator++( int ) {// this signature is for the postfix form itr++
      const_iterator old = *this; // return the pointer to the old const_iterator
      ++(*this); // what does this thing do?
      return old;
    }
    virtual const_iterator & operator--() {
      current = current->prev; // change the state of the const_iterator object
      return *this;
    }
    virtual const_iterator & operator--( int ) {
      const_iterator & old = *this;
      // increment by reference
      --(*this); // require the implemetation of --itr (prefix form)
      return old;
    }

    bool operator==( const const_iterator & rhs ) const {
      return current == rhs.current; // compare the address that two pointer pointing to
    }

    bool operator!= ( const const_iterator & rhs ) const {
      return !( *this == rhs); // compare the references of the two const_iterator objs
    }

   protected:
    // why protected? because we want the derived class iterator to have access to
    // this data member. Thus it must be protected.
    Node* current;
    // retrieve return the object, not just the Node
    Object & retrieve( ) const { return current->data; }
    // protected p-ctor for const_iterator
    // why? because we want only the List class to have access to this constructor
    // iterator is not supposed to be constructed from stay pointers.
    const_iterator( Node *p) : current{ p } { }
    // Why friend? to give the List class access to the protected member.
    // so in the end, the iterator class and the List class have access to these protected
    // members but no other class does.
    friend class List<Object>;
  };

  // class iterator inherites all the public attributes of the class const_iterator
  // we can add new data, add new methods or redefine old methods.
  // When we add new data or change inherited methods we need the virtual keyword to
  // deal with syntactical baggage because we are not adding new data.
  class iterator: public const_iterator
  {
   public:
    // public default constructor for iterator
    // we call the base-class constructor here
    // we must write it here because we have the protected p-ctor, so it will disable this
    // default constructor if we don't explicitly specify it here
    iterator( ) { }
    // accessor
    Object & operator* ( ) { // no more const
      return const_iterator::retrieve( ); // you actually call the base-class retrieve() method
    }

    const Object & operator*( ) const {
      return const_iterator::operator*( );
    }

    // what else do you need to overload?
    // actually you don't really need to do this because you can use the virtual keyword and things seem to be fine!
    //
    // now we need it to return iterator
    // iterator & operator++() {
    //   this->current = this->current->next;
    //   return *this;
    // }
    // iterator operator++( int ) {
    //   iterator & old = *this;
    //   ++(*this);
    //   return old;
    // }
    // WHY do you need this-> to make things work here?
    // because you are not able to directly assume access to current.
    // iterator & operator--() {
    //   this->current = this->current->prev;
    //   return *this;
    // }
    // iterator operator--( int ) {
    //   iterator old = *this;
    //   --( *this );
    //   return old;
    // }
    // we don't need to reimplement the == and != operator because
    // both of those methods return bool and the comparison are similar between
    // const_iterator and iterator
   protected:
    // protected constructor for iterator
    // it actually calls the const_iterator constructor
    iterator( Node* p ): const_iterator{ p } { }
    friend class List<Object>;
  };
 public:
  // the Big-Five: destructor, two copy ctors, two assignment operators
  // 0.default constructor
  List() { init (); } // take no argument and also offload the value-init to init()
  // 1.destructor
  ~List() {
    clear(); // member method to clear everything but the head and the tail
    delete head; // destroy whatever head pointing to, then erase head
    delete tail; // destroy whatever tail pointing to, then erase tail
  }
  // 2.copy constructor - offload to push_back()
  List(const List& rhs) {
    init();
    for ( auto & x : rhs )
      push_back( x );
  }
  // 3.copy assignment operator
  List & operator=(const List & rhs) {
    List copy (rhs);
    std::swap(*this, copy);
    return *this;
  }

  // 4.move constructor
  List(List && rhs)
      : theSize{ rhs.theSize }, head{ rhs.head }, tail{ rhs.tail }
  {
    rhs.theSize = 0;
    rhs.head = nullptr;
    rhs.tail = nullptr;
  }
  // 5.move assignment operator
  List & operator=(List && rhs) {
    std::swap( theSize, rhs.theSize);
    std::swap( head, rhs.head );
    std::swap( tail, rhs.tail );
    return *this;
  }
  // Member methods:

  // begin(): return iterator representing beginning of list
  // Mutator version first then accessor version
  iterator begin( ) {
    return iterator ( head->next );
  }
  const_iterator begin () const {
    return const_iterator ( head->next );
  } // this could return tail, which is fine

  // end(): return iterator representing end of list - one pass the last,
  // thus this iterator points to the tail node.
  // Mutator version first then accessor version
  iterator end() {
    return iterator ( tail );
  }
  const_iterator end() const {
    return const_iterator ( tail );
  }

  // size()
  int size() const {
    return theSize;
  }
  // empty(): check if the list is empty or not
  bool empty( ) const {
    //return (head->next == tail);
    return size() == 0;
  }

  // clear(): erase the whole list - use pop_front()
  void clear() {
    while ( !empty( ) )
      pop_front( );
  }

  // front, back, push_front, push_back, pop_front, pop_back
  // these are basic double-ended queue operations
  // front(): return the first object in the list
  Object & front() {
    return *begin();
  }
  const Object & front() const {
    return *begin();
  }
  Object & back () {
    return *(--end()); // because end() is one passed last item
  }
  const Object & back ( ) const {
    return *(--end());
  }
  void push_front ( const Object & x ) {
    insert ( begin( ), x );
  }
  void push_front ( Object && x ) {
    insert ( begin( ), std::move( x ) );
  }
  void pop_front () {
    erase( begin() );
  } // why void
  void pop_back () {
    erase ( --end() );
  } // why void?

  // push_back, both normal and move versions
  void push_back( const Object & x) {
    insert ( end(), x);
  }
  void push_back( Object && x ) {
    insert ( end(), std::move( x ));
  }

  // insert is the canonical method to insert Object into the list
  iterator insert( iterator itr, const Object & x) {
    Node* p = itr.current; // make a pointer that copies the current field of itr
    ++theSize;
    // make new node, make the next field in the previous Node points to this new Node
    p->prev->next = new Node(x, p->prev, p);
    p->prev = p->prev->next;
    return iterator(p->prev); // make a new iterator and return it
  }

  iterator insert ( iterator itr, const Object && x ) {
    Node* p = itr.current;
    ++theSize;
    return iterator(p->prev = p->prev->next = new Node{ std::move(x), p->prev, p } );
    // std::move(x) convert lvalue x into a rvalue type, which is necessary so that compiler
    // knows it must use the move constructor, not the copy constructor
  }
  // erase is another canonical method to delete an Object
  // what iterator should it returns?
  iterator erase ( iterator itr ) {
    Node *p = itr.current;
    iterator retVal ( p->next );
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;

    --theSize;
    return retVal;
  }
  iterator erase ( iterator from, iterator to ) {
    for ( iterator itr = from; itr != to; )
      itr = erase (itr);
    return to;
  }

 private:
  // accounting specs and the init method used in the constructor
  int theSize;
  Node *head;
  Node *tail;

  void init ()
  {
    // what do you need for init?
    // make the sentinel nodes
    // init the public and private data field
    theSize = 0;
    head = new Node;
    tail = new Node;
    head->next = tail;
    tail->prev = head;
  }
};

#endif
