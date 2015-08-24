#include "Queue.h"
#include "gtest/gtest.h"

class QueueTest : public testing::Test {
 protected:
  virtual void SetUp() {
    //NOT YET IMPLEMENTED
  }

  virtual void TearDown() {
  }
  // setup fixtures
  Queue<int> q0_;
};

TEST_F(QueueTest, DefaultConstructor) {
  EXPECT_NE(0u, 1u);
}
