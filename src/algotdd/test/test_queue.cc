#include "queue.h"
#include "gtest/gtest.h"

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
  q2_.push(100);
  EXPECT_EQ(3u, q2_.size());
  EXPECT_EQ(100, q2_.back());
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
