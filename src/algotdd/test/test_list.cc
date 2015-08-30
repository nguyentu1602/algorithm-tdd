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

// test the helper class listNodeTest
// this is a canonical example of writing testable code:
// to test private members, just write another helper class
// and make its data public
TEST(listNodeTest, DefaultCtor) {
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

TEST_F(listTest, DefaultCtor) {
  list<int> list_temp;
  EXPECT_EQ(0u, list_temp.size());
}
