#include "list.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;

class listTest : public testing::Test {
 protected:
  virtual void SetUp() {
    list_1.insert(list_1.begin(), 10);
    int_iter = list_1.insert(list_1.end(), 30);
    int_iter = list_1.insert(int_iter, 20);
    str = "a string";
  }
  virtual void TearDown() {}
  // class constant

  // setup fixtures
  ::list<int> list_1;
  ::list<int> list_2;
  ::list<int> list_3;
  list<int> list_empty;
  ::list<std::string> list_str;
  list<int>::iterator int_iter;
  list<std::string>::iterator str_iter;
  std::string str;
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

TEST_F(listIteratorTest, Iterator) {
  list_iterator<int> iter_temp = iter;
  // testing increment and decrement of iterator
  // which are all inherited from base
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

TEST_F(listTest, DefaultCtor) {
  EXPECT_EQ(0u, list_empty.size());
  EXPECT_TRUE(list_empty.empty());
}

TEST_F(listTest, CopyCtor) {
  for (int i = 0; i < 100; i++) {
    list_1.push_back(i);
  }
  const list<int> list_temp = list_1;
  EXPECT_EQ(list_temp.size(), list_1.size());
  int_iter = list_1.begin();
  list<int>::const_iterator temp_iter = list_temp.begin();
  while(temp_iter != list_temp.end()
        && int_iter != list_1.end())
  {
    EXPECT_EQ(*temp_iter++, *int_iter++);
  }

  // edge case: empty list
  list<int> list_nothing = list_empty;
  EXPECT_TRUE(list_nothing.empty());
}

TEST_F(listTest, AssigmentOperator) {
  for (int i = 0; i < 100; i++) {
    list_1.push_back(i);
  }
  list_2 = list_1;
  EXPECT_EQ(list_2.size(), list_1.size());
  int_iter = list_1.begin();
  list<int>::const_iterator temp_iter = list_2.begin();
  while(temp_iter != list_2.end()
        && int_iter != list_1.end())
  {
    EXPECT_EQ(*temp_iter++, *int_iter++);
  }
  // use a const list as rhs
  const list<int> const_list = list_1;
  list_3 = const_list;
  EXPECT_EQ(const_list.size(), list_3.size());
  temp_iter = const_list.begin();
  int_iter = list_3.begin();
  while (temp_iter != const_list.end()) {
     EXPECT_EQ(*int_iter++, *temp_iter++);
  }

  // edge case: empty list
  list_1 = list_empty;
  EXPECT_TRUE(list_1.empty());

  // edge case: self-assign
  const list<int> old_list_2 = list_2;
  list_2 = list_2;
  // make sure list_2 is not emptied
  EXPECT_EQ(list_2.size(), old_list_2.size());
  // TODO: compare list_2 and old_list_2 elementwise
}

TEST_F(listTest, MoveCtor) {
  for (int i = 0; i < 100; i++) {
    list_1.push_back(i);
  }

  list<int> old_list_1 = list_1; // require copy ctor to work
  // invoke move ctor here
  const list<int> list_temp = std::move(list_1);
  EXPECT_EQ(list_temp.size(), old_list_1.size());
  EXPECT_TRUE(list_1.empty());
  int_iter = old_list_1.begin();
  list<int>::const_iterator temp_iter = list_temp.begin();
  while(temp_iter != list_temp.end()
        && int_iter != old_list_1.end())
  {
    EXPECT_EQ(*temp_iter++, *int_iter++);
  }

  // edge case: empty list
  list<int> list_nothing = std::move(list_empty);
  EXPECT_TRUE(list_nothing.empty());
}

TEST_F(listTest, MoveAssignmentOperator) {
  for (int i = 0; i < 100; i++) {
    list_1.push_back(i);
  }
  list_2.push_back(2);
  list<int> old_list_1 = list_1; // require copy ctor to work
  list<int> list_temp = list_2;
  // invoke move assignment operator here
  list_temp = std::move(list_1);
  EXPECT_EQ(list_temp.size(), old_list_1.size());
  EXPECT_TRUE(list_1.empty());
  int_iter = old_list_1.begin();
  list<int>::const_iterator temp_iter = list_temp.begin();
  while(temp_iter != list_temp.end()
        && int_iter != old_list_1.end())
  {
    EXPECT_EQ(*temp_iter++, *int_iter++);
  }

  // edge case: empty list
  list<int> list_nothing = list_2;
  list_nothing = std::move(list_empty);
  EXPECT_TRUE(list_nothing.empty());
}

TEST_F(listTest, BeginEnd) {
  EXPECT_EQ(list_empty.begin(), list_empty.end());
  // decrement pass begin() and increment pass end() should be ok
  EXPECT_NO_FATAL_FAILURE(--list_empty.begin());
  EXPECT_NO_FATAL_FAILURE(++list_empty.end());
}

TEST_F(listTest, InsertLvalue) {
  // iter is pointing to tail_
  list<int>::iterator iter = list_empty.begin();
  // testing iterator insert (iterator, const E&)
  int x = 10;
  list<int>::iterator ret_iter = list_empty.insert(iter, x);
  EXPECT_TRUE(--iter == ret_iter);
  // iter--;
  EXPECT_EQ(*ret_iter, 10);
  EXPECT_EQ(list_empty.size(), 1);
  EXPECT_EQ(++ret_iter, list_empty.end());
  --ret_iter; // set ret_iter to point to 10
  ret_iter = list_empty.insert(ret_iter, x + 10);
  EXPECT_EQ(list_empty.size(), 2);
  EXPECT_EQ(*ret_iter, 20);
  EXPECT_EQ(*(++ret_iter), 10);
  EXPECT_TRUE(++ret_iter == list_empty.end());

  // insert pass begin() and pass end() should fail
  // the following would not even compile:
  // list_empty.insert(++list_empty.end(), 100);
  // list_empty.insert(--list_empty.begin(), 100);
}

TEST_F(listTest, InsertRvalue) {
  size_t old_size = list_str.size();
  str_iter = list_str.insert(list_str.begin(), std::move(str));
  EXPECT_EQ(*str_iter, "a string");
  EXPECT_TRUE(str_iter == list_str.begin());
  EXPECT_EQ(++old_size, list_str.size());
  EXPECT_TRUE(str.empty());
}

TEST_F(listTest, InsertConstObject) {
  // insert const object by lvalue:
  const std::string const_str("const string");
  const std::string CONST_STR(const_str);

  size_t old_size = list_str.size();
  str_iter = list_str.insert(list_str.begin(), const_str);
  EXPECT_EQ(++old_size, list_str.size());
  EXPECT_EQ(*str_iter, CONST_STR);
  EXPECT_EQ(const_str, CONST_STR);

  // insert const objects by rvalue:
  // std::move automatically degrade to a copy insert
  str_iter = list_str.insert(list_str.begin(), std::move(const_str));
  EXPECT_EQ(++old_size, list_str.size());
  EXPECT_EQ(*str_iter, CONST_STR);
  EXPECT_EQ(const_str, CONST_STR);
}

TEST_F(listTest, EraseAtIter) {
  int_iter = list_1.erase(list_1.begin()); // erase 10
  EXPECT_EQ(list_1.size(), 2);
  EXPECT_EQ(*list_1.begin(), 20);
  EXPECT_EQ(*int_iter, 20);
  int_iter = list_1.erase(int_iter); // erase 20
  int_iter = list_1.erase(int_iter); // erase 30
  // test for seg fault if erase end()
  // it trigers a memory leak error right before core dump,
  // which it is expected. Turned off when running memtest
  //EXPECT_DEATH(int_iter = list_1.erase(list_1.end()), "");
}

TEST_F(listTest, PushFront) {
  // lvalue version
  size_t old_size = list_1.size();
  list_1.push_front(100);
  EXPECT_EQ(++old_size, list_1.size());
  EXPECT_EQ(*list_1.begin(), 100);

  list_1.push_front(200);
  EXPECT_EQ(++old_size, list_1.size());
  EXPECT_EQ(*list_1.begin(), 200);
  EXPECT_EQ(*(++list_1.begin()), 100);
  EXPECT_TRUE(--(++list_1.begin()) == list_1.begin());

  // rvalue version
  old_size = list_str.size();
  list_str.push_front(std::move(str));
  EXPECT_EQ(++old_size, list_str.size());
  EXPECT_EQ(*list_str.begin(), "a string");
  EXPECT_TRUE(str.empty());
}

TEST_F(listTest, PushBack) {
  // lvalue version
  size_t old_size = list_1.size();
  list_1.push_back(1000);
  EXPECT_EQ(++old_size, list_1.size());
  EXPECT_EQ(*(--list_1.end()), 1000);

  list_1.push_back(2000);
  EXPECT_EQ(++old_size, list_1.size());
  EXPECT_EQ(*(--list_1.end()), 2000);
  EXPECT_EQ(*(--(--list_1.end())), 1000);
  EXPECT_TRUE(++(--list_1.end()) == list_1.end());

  // rvalue version
  old_size = list_str.size();
  list_str.push_back(std::move(str));
  EXPECT_EQ(++old_size, list_str.size());
  EXPECT_EQ(*(--list_str.end()), "a string");
  EXPECT_TRUE(str.empty());
}

TEST_F(listTest, FrontBack){
  // testing front()
  // tests on normal list
  list_2.push_front(100);
  list_2.push_front(200);
  const list<int> list_const = list_2;
  EXPECT_EQ(list_2.front(), *list_2.begin());
  EXPECT_EQ(list_2.front(), 200);

  // can we modify the front reference?
  int& rep = list_2.front();
  rep = 10;
  EXPECT_EQ(10, list_2.front());

  // test on const list
  EXPECT_EQ(list_const.front(), *list_const.begin());
  EXPECT_EQ(list_const.front(), 200);

  // testing back()
  // tests on normal lisk
  EXPECT_EQ(list_2.back(), *(--list_2.end()));
  EXPECT_EQ(list_2.back(), 100);

  // can we modify the back reference?
  int& rep_back = list_2.back();
  rep_back = 20000;
  EXPECT_EQ(20000, list_2.back());

  // testing on const list
  EXPECT_EQ(list_const.back(), *(--list_const.end()));
  EXPECT_EQ(list_const.back(), 100);
}
