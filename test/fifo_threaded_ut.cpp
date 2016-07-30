#include <iostream>
#include <thread>

#include "gtest/gtest.h"
#include "fifo.h"

using namespace ::music_group;

const int max_threads =10;
const int max_elements=max_threads+1;  // For debug purpose, max_elements>max_threads
const char food_to_consume[]="banana";

/**
 * @brief This struct is the type that will be produced
 */
typedef struct {
  std::string food;
} task_t;

// This function consumes the food
void run_thread(Fifo<task_t>* fifo, int id, task_t* task) {
  fifo->pop(task);
  //std::cout << "Thread: " << id << " food: " << task->food << std::endl;
  return;
}

/**
 * @brief This class create a FIFO and initialize with max_elements
 */
class FifoThreadedTest : public ::testing::Test {
 public:
  FifoThreadedTest() : fifo_(NULL) {}
 protected:
  virtual void SetUp() {
    fifo_ = new Fifo<task_t>(max_elements);
  }

  virtual void TearDown() {
    delete fifo_;
  }

  Fifo<task_t>* fifo_;
};

/**
 * @brief This code will test the pop function where the thread must wait
 *        until the resource is available.
 *
 *        Steps:
 *        1-) Configure and start all threads. Each thread will try to consume
 *        the unavailable resource. The pop function will put the respective
 *        thread to wait.
 *        2-) Insert the resource for all threads.
 *        3-) verify that all threads consumed at least one resource.
 */
TEST_F(FifoThreadedTest, Test_Empty_Pop_Push_With_Thread) {
  std::thread threads[max_threads];
  task_t producer[max_threads];
  task_t consumer[max_threads];

  for (int id = 0; id < max_threads; id++) {
    consumer[id].food.clear();
    threads[id] = std::thread(run_thread, fifo_, id, &consumer[id]);
  }

  for (int i=0; i<max_threads; i++) {
    producer[i].food = food_to_consume;
    fifo_->push(producer[i]);
  }

  for (int i = 0; i < max_threads; ++i) {
    threads[i].join();
  }

  for (int id = 0; id < max_threads; id++) {
    ASSERT_STREQ(food_to_consume, producer[id].food.c_str());
  }
}

/**
 * @brief This code will test the push function until the maximum value and
 *         after the consumer will consume the values.
 *
 *        Steps:
 *        1-) Push All values until the maximum value
 *        2-) Push one value more and verify that it is not possible insert
 *        a new element.
 *        3-) Configure and start all threads. Each thread will try to consume
 *        the unavailable resource. The pop function will put the respective
 *        thread to wait.
 *        4-) Insert the resource for all threads.
 *        5-) verify that all threads consumed at least one resource.
 */
TEST_F(FifoThreadedTest, Test_Push_Pop_With_Thread) {
  std::thread threads[max_elements];
  task_t producer[max_elements];
  task_t consumer[max_elements];
  task_t producer_fail;
  task_t consumer_fail;

  for (int i=0; i<max_elements; i++) {
    producer[i].food = food_to_consume;
    ASSERT_EQ(ERROR_OK, fifo_->push(producer[i]));
  }
  producer_fail.food = food_to_consume;
  ASSERT_EQ(ERROR_FAIL, fifo_->push(producer_fail));

  for (int id = 0; id < max_elements; id++) {
    consumer[id].food.clear();
    threads[id] = std::thread(run_thread, fifo_, id, &consumer[id]);
  }

  for (int i = 0; i < max_elements; ++i) {
    threads[i].join();
  }

  for (int id = 0; id < max_elements; id++) {
    ASSERT_STREQ(food_to_consume, producer[id].food.c_str());
  }
}

/**
 * @brief This code will test the pop_try function where the thread must return
 *        if the resource is unavailable.
 *
 *        Steps:
 *        1-) Configure and start all threads. Each thread will try to consume
 *        the unavailable resource. The pop function will put the respective
 *        thread to wait.
 *        2-) Try to access the unavailable resource with pop_try and verify
 *        that the function will return an error.
 *        3-) Insert the resource for all threads+1.
 *        4-) verify that all threads consumed at least one resource,
 *        including the pop_try_consumer.
 */
TEST_F(FifoThreadedTest, Test_Pop_Push_Pop_Try_With_Thread) {
  std::thread threads[max_threads];
  task_t producer[max_threads+1];
  task_t consumer[max_threads];
  task_t pop_try_consumer;

  for (int id = 0; id < max_threads; id++) {
    consumer[id].food.clear();
    threads[id] = std::thread(run_thread, fifo_, id, &consumer[id]);
  }

  pop_try_consumer.food.clear();
  ASSERT_EQ(ERROR_FAIL, fifo_->pop_try(&pop_try_consumer));
  ASSERT_STRNE(food_to_consume, pop_try_consumer.food.c_str());

  for (int i=0; i<(max_threads+1); i++) {
    producer[i].food = food_to_consume;
    fifo_->push(producer[i]);
  }

  for (int i = 0; i < max_threads; ++i) {
    threads[i].join();
  }

  ASSERT_EQ(ERROR_OK, fifo_->pop_try(&pop_try_consumer));

  for (int id = 0; id < max_threads; id++) {
    ASSERT_STREQ(food_to_consume, producer[id].food.c_str());
  }
  ASSERT_STREQ(food_to_consume, pop_try_consumer.food.c_str());
}
