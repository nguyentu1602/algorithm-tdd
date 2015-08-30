#ifndef QUEUE_H_
#define QUEUE_H_
#include <cstddef> // for nullptr
#include <utility> // for std::move

/* The queue data structure - follows stl interface
 * Public interface:
 * modifiers: push(), pop()
 * accessors: front(), back()
 * capacity:  empty(), size()
 */
template <typename Element>
class queue;

template <typename Element>
class queueNode {
 public:
  friend class queue<Element>;

  // accessors
  const Element& element() const {
    return element_;
  }

  Element& element() {
    return element_;
  }

  queueNode* next() {
    return next_;
  }
  const queueNode* next() const {
    return next_;
  }

 private:
  // need constructor and destructors
  // need data members
  Element element_;
  queueNode* next_;

  explicit queueNode(const Element& element) : element_{element}, next_{nullptr} {}
  explicit queueNode(Element&& element) : element_{std::move(element)}, next_{nullptr} {}
  // disable the default copy ctor and assignment operator:
  const queueNode& operator=(const queueNode&);
  queueNode(const queueNode&);
};


template <typename Element>
class queue {
 public:
  // default Ctor
  queue() : size_{0}, head_{nullptr}, tail_{nullptr} {}
  // default Dtor
  ~queue() {
    clear();
  };
  // capacity methods:
  size_t size() const {
    return size_;
  };

  bool empty() const {
    return size_ == 0;
  };

  // modifiers methods:
  // normal push takes an element and makes a copy to put inside the queue
  // move push takes a rvalue and move it into the queue
  void push(const Element& element) {
    Element copied_element = element;
    // employ move push to comply to DRY
    push(std::move(copied_element));
  };

  void push (Element&& element) {
    queueNode<Element>* new_node =
        new queueNode<Element>(std::move(element));
    if (empty()) {
      head_ = tail_ = new_node;
    }
    else {
      tail_->next_ = new_node;
      tail_ = new_node;
    }
    size_++;
  }

  // pop returns nothing because we have accessors to do that
  void pop() {
    if(!empty()) {
      queueNode<Element>* prev_head = head_;
      head_ = head_->next();
      delete prev_head;
      size_--;
      if (empty()) {
        tail_ = nullptr;
      }
    }
  };

  // clear everything
  void clear() {
   if (!empty()) {
      queueNode<Element>* to_del = head_;
      queueNode<Element>* next = nullptr;
      // keep deleting
      do {
        next = to_del->next();
        delete to_del;
        to_del = next;
      } while(to_del);
      // update private members
      size_ = 0;
      head_ = tail_ = nullptr;
    }
  };

  // accessors methods:
  // you have to check whether the queue is empty before calling them
  // otherwise you have undefined behavior
  const Element& front() const {
    return head_->element();
  };

  Element& front() {
    return head_->element();
  };

  const Element& back() const {
    return tail_->element();
  };

  Element& back() {
    return tail_->element();
  };

 private:
  // A queue should contains a pointer to the first node,
  // a pointer to the last node and size

  size_t size_;
  queueNode<Element>* head_;
  queueNode<Element>* tail_;

  // disable copy constructor and assignment operator
  // why? because you can copy a queue, make it const, and
  // you can't call clear() on it, which leads to memory leak
  // this technique result in a compilation error when you try:
  // const queue<type> q1 = q0; // q0 is a non-empty queue
  queue(const queue&);
  const queue& operator= (const queue&);
};


#endif // QUEUE_H_
