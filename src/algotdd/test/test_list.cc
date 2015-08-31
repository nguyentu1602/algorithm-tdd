#include "list.h"
#include "gtest/gtest.h"
#include <string>

class listTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // NOT YET IMPLEMENTED
  }
  virtual void TearDown() {}
  // setup fixtures
  ::list<int> list_1;
  ::list<int> list_2;
  ::list<std::string> list_str;
};

class listIteratorTest : public testing::Test {
  // try to make friend with list_const_iterator DOES NOT WORK here
  // because friendship must be made before template is resolved:
  // friend class list_const_iterator<int>;
 protected:
  virtual void SetUp() {
    head_ = new listNode<int> (0);
    head_->next_ = new listNode<int> (10);
    tail_ = head_->next_->next_ = new listNode<int> (20);
    head_->next_->prev_ = head_;
    tail_->prev_ = head_->next_;
    c_iter = list_const_iterator<int> (head_);
    c_iter_1 = list_const_iterator<int> (head_->next_);
    iter = list_iterator<int> (head_);
    iter_1 = list_iterator<int> (head_->next_);
  }

  virtual void TearDown() {
    delete tail_;
    delete head_->next_;
    delete head_;
  }
  listNode<int>* head_;
  listNode<int>* tail_;
  list_const_iterator<int> c_iter;
  list_const_iterator<int> c_iter_1;
  list_iterator<int> iter;
  list_iterator<int> iter_1;

};

// test the helper class listNodeTest
// this is a canonical example of writing testable code:
// to test private members, just write another helper class
// and make its data public
TEST(listHelperTest, ListNode) {
  int element_1 = 100;
  std::string element_2("Hello world");
  listNode<int> node_1(element_1);
  listNode<std::string> node_2(element_2);
  EXPECT_EQ(element_1, node_1.element_);
  EXPECT_EQ(element_2, node_2.element_);
  EXPECT_EQ(nullptr, node_2.prev_);
  EXPECT_EQ(nullptr, node_2.next_);
  // test the move constructor for node:
  listNode<int> node_3(std::move(element_1));
  listNode<std::string> node_4(std::move(element_2));
  EXPECT_EQ(100, node_3.element_);
  EXPECT_EQ("Hello world", node_4.element_);
  EXPECT_TRUE(element_2.empty());
}

TEST_F(listIteratorTest, ConstIterator) {
  list_const_iterator<int> c_iter_temp = c_iter;
  // testing increment and decrement of const_iterator
  EXPECT_EQ(++c_iter_temp, c_iter_1);
  EXPECT_EQ(--c_iter_temp, c_iter);
  EXPECT_EQ(c_iter_temp++, c_iter);
  EXPECT_EQ(c_iter_temp, c_iter_1);
  EXPECT_EQ(c_iter_temp--, c_iter_1);
  EXPECT_EQ(c_iter_temp, c_iter);
  EXPECT_TRUE(c_iter_temp == c_iter);
  EXPECT_TRUE(c_iter_temp != c_iter_1);

  // testing access capability of const_iterator
  EXPECT_EQ(0, *c_iter_temp);
  EXPECT_EQ(10, *(++c_iter_temp));
  EXPECT_EQ(20, *(++c_iter_temp));
  EXPECT_EQ(10, *(--c_iter_temp));

  // testing non-modifying capability of const_iterator:
  // will not even compile:
  //*c_iter_temp = 100;
}

TEST_F(listIteratorTest, Iterator) {
  list_iterator<int> iter_temp = iter;
  // testing increment and decrement of iterator
  EXPECT_EQ(++iter_temp, iter_1);
  EXPECT_EQ(--iter_temp, iter);
  EXPECT_EQ(iter_temp++, iter);
  EXPECT_EQ(iter_temp, iter_1);
  EXPECT_EQ(iter_temp--, iter_1);
  EXPECT_EQ(iter_temp, iter);
  EXPECT_TRUE(iter_temp == iter);
  EXPECT_TRUE(iter_temp != iter_1);

  // testing access capability of iterator
  EXPECT_EQ(0, *iter_temp);
  EXPECT_EQ(10, *(++iter_temp));
  EXPECT_EQ(20, *(++iter_temp));
  EXPECT_EQ(10, *(--iter_temp));

  // testing modifying capability of iterator:
  *iter_temp = 100;
  EXPECT_EQ(*iter_temp, 100);
}


TEST_F(listTest, DefaultCtor) {
  list<int> list_temp;
  EXPECT_EQ(0u, list_temp.size());
}
