#include "gtest/gtest.h"

#include "fifo.h"

using namespace ::music_group;

class FifoTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
};

TEST_F(FifoTest, testing_return){
  int value = 1;

  return_myself(value);

  EXPECT_EQ(1, return_myself(value));
}

