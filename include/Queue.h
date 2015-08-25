#ifndef QUEUE_H_
#define QUEUE_H_
#include <cstddef>

/* The Queue data structure
 * Public interface:
 * enqueue()
 * dequeue()
 * size()
 * clear()
 * begin()
 * end() : one pass the last item
 * empty()
 */

template <typename Element>
class Queue {
 public:
  // default Ctor
  Queue() : size_{0}, head_{nullptr}, tail_{nullptr} {}

  // the public interface:
  size_t size() {
    return size_;
  }

  void enqueue(const Element& element) {
  }

 private:
  // A queue should contains a pointer to the first node,
  // a pointer to the last node and size
  struct Node {
    // NOT YET
  };

  size_t size_;
  Node* head_;
  Node* tail_;

  // you must disable the option of const Queue.

};



#endif // QUEUE_H_
