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

  // defalt Dtor
  ~Queue() {
    clear();
  }

  // the public interface:
  size_t size() const {
    return size_;
  }

  bool empty() const {
    return size_ == 0;
  }

  void enqueue(const Element& element) {
    Node* new_node = new Node(element);
    if (empty()) {
      head_ = tail_ = new_node;
    }
    else {
      tail_->next_ = new_node;
      tail_ = new_node;
    }
    size_++;
  }
  // dequeue return a pointer to deal with empty case
  Element* dequeue() {
    if (!head_) {
      return nullptr;
    }
    else {
      Node* prev_head = head_;
      Element* ret_item = new Element(prev_head->element_);
      head_ = head_->next_;
      delete prev_head;
      size_--;
      if (empty()) {
        tail_ = nullptr;
      }
      return ret_item;
    }
  }

  void clear() {
    if (!empty()) {
      Node* to_del = head_;
      Node* next = nullptr;
      // keep deleting
      do {
        next = to_del->next_;
        delete to_del;
        to_del = next;
      } while(to_del);
      // update private members
      size_ = 0;
      head_ = tail_ = nullptr;
    }
  }

 private:
  // A queue should contains a pointer to the first node,
  // a pointer to the last node and size

  // A Node in the queue - implemented as item in a singly-linked list
  struct Node {
    Element element_;
    Node* next_;
    explicit Node(const Element& element) : element_{element}, next_{nullptr} {}

    // disable the default copy ctor and assignment operator:
    const Node& operator=(const Node&);
    Node(const Node&);
  };

  size_t size_;
  Node* head_;
  Node* tail_;
};



#endif // QUEUE_H_
