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
  list<int> list_1;
  list<int> list_2;
  list<std::string> list_str;
};
