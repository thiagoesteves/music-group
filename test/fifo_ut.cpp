#include "gtest/gtest.h"

#include "fifo.h"

using namespace ::music_group;

const int max_elements=1000;

/**
 * @brief This class create a FIFO and initialize with max_elements
 */
class FifoTest : public ::testing::Test {
 public:
  FifoTest() : fifo_(NULL) {}
 protected:
  virtual void SetUp() {
    fifo_ = new Fifo<float>(max_elements);
  }

  virtual void TearDown() {
    delete fifo_;
  }

  Fifo<float>* fifo_;
};

/**
 * @brief This test insert and remove elements in the expect order without
 *        errors.
 */
TEST_F(FifoTest, Test_Insert_Remove_Ok) {
  float value=0;

  for (int i=0; i<max_elements; i++) {
    fifo_->push(i);
  }

  for (int i=0; i<max_elements; i++) {
    fifo_->pop(&value);
    EXPECT_EQ(i, value);
  }
}

/**
 * @brief This test insert the maximum number of elements and verify if
 *        the next will return an error.
 */
TEST_F(FifoTest, Test_Error_Max_Elements) {
  float value=0;

  for (int i=0; i<max_elements; i++) {
    fifo_->push(i);
  }

  EXPECT_EQ(ERROR_FAIL, fifo_->push(value));
}

/**
 * @brief This test insert the maximum number of elements and verify if
 *        the function pop_try return error when called.
 */
TEST_F(FifoTest, Test_Error_Empty_Pop_Try) {
  float value=0;

  for (int i=0; i<max_elements; i++) {
    fifo_->push(i);
  }

  for (int i=0; i<max_elements; i++) {
    fifo_->pop(&value);
  }

  EXPECT_EQ(ERROR_FAIL, fifo_->pop_try(&value));
}
