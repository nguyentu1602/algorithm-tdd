#include "Queue.h"
#include "gtest/gtest.h"
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

class QueueTest : public testing::Test {
 protected:
  // SetUp() & TearDown() are virtual functions from testting::Test,
  // so we just signify that we override them here
  virtual void SetUp() {
    q1_.enqueue(1);
    q1_.enqueue(2);
    q2_.enqueue(1);
    q2_.enqueue(3);
  }

  virtual void TearDown() {
  }

  // setup fixtures
  Queue<int> q0_;
  Queue<int> q1_;
  Queue<int> q2_;
};

// Use TEST_F to test with fixtures.
TEST_F(QueueTest, DefaultConstructor) {
  EXPECT_EQ(0u, q0_.size());
}

TEST_F(QueueTest, Enqueue){
  EXPECT_EQ(2u, q1_.size());
  EXPECT_EQ(2u, q2_.size());
  q2_.enqueue(100);
  EXPECT_EQ(3u, q2_.size());
}