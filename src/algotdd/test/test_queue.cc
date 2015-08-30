#include "queue.h"
#include "gtest/gtest.h"
#include <string>
class queueTest : public testing::Test {
 protected:
  // SetUp() & TearDown() are virtual functions from testting::Test,
  // so we just signify that we override them here
  virtual void SetUp() {
    q1_.push(1);
    q1_.push(2);
    q2_.push(1);
    q2_.push(3);
  }

  virtual void TearDown() {
  }

  // setup fixtures
  queue<int> q0_;
  queue<int> q1_;
  queue<int> q2_;
};

// Use TEST_F to test with fixtures.
TEST_F(queueTest, DefaultConstructor) {
  EXPECT_EQ(0u, q0_.size());
}

TEST_F(queueTest, frontback) {
  EXPECT_EQ(1, q1_.front());
  EXPECT_EQ(1, q2_.front());
  EXPECT_EQ(2, q1_.back());
  EXPECT_EQ(3, q2_.back());
}

TEST_F(queueTest, push){
  EXPECT_EQ(2u, q1_.size());
  EXPECT_EQ(2u, q2_.size());
  q2_.push(100); // this is move push
  EXPECT_EQ(3u, q2_.size());
  EXPECT_EQ(100, q2_.back());
  // test normal push
  int item = 1000;
  q2_.push(item);
  EXPECT_EQ(1000, q2_.back());
  EXPECT_EQ(1000, item);
}

TEST_F(queueTest, pop){
  EXPECT_EQ(2u, q2_.size());
  EXPECT_EQ(1, q2_.front());
  q2_.pop();
  EXPECT_EQ(3, q2_.front());
  q2_.pop();
  EXPECT_TRUE(q2_.empty());
}


TEST_F(queueTest, clear) {
  q2_.clear();
  EXPECT_TRUE(q2_.empty());
}

// test the move version of push
// use std::string here for non-trivial object
// that we can actually increase efficiency by
// the new move operation
TEST_F(queueTest, movepush) {
  queue<std::string> qstr;
  qstr.push("First string");
  EXPECT_EQ(qstr.front(), "First string");
  // test copy push: after the push, str_item is the same
  std::string str_item("Second string");
  qstr.push(str_item);
  EXPECT_EQ(qstr.back(), "Second string");
  EXPECT_EQ(str_item, "Second string");
  // test move push: after the push, str_item is empty
  str_item = "Third string";
  qstr.push(std::move(str_item));
  EXPECT_EQ(qstr.back(), "Third string");
  EXPECT_TRUE(str_item.empty());
}
