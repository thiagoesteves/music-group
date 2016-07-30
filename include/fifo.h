#ifndef FIFO_H
#define FIFO_H

#include <queue>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include "error.h"

namespace music_group {

template <typename T>
class Fifo {
 public:
  Fifo(const uint32_t& max_size) : max_size_(max_size) {}

  error_t push(const T& value) {
    std::unique_lock<std::mutex> lck(mtx_);
    if (fifo_.size() >= max_size_) {
      return ERROR_FAIL;
    }
    fifo_.push(value);
    cv_.notify_all();
    return ERROR_OK;
  }

  error_t pop(T* value) {
    std::unique_lock<std::mutex> lck(mtx_);
    while(fifo_.empty()) { cv_.wait(lck); }
    *value = fifo_.front();
    fifo_.pop();
    cv_.notify_all();
    return ERROR_OK;
  }

  error_t pop_try(T* value) {
    std::unique_lock<std::mutex> lck(mtx_);
    if (!fifo_.empty()) {
      *value = fifo_.front();
      fifo_.pop();
      cv_.notify_all();
      return ERROR_OK;
    }
    return ERROR_FAIL;
  }

  private:
    std::queue<T>  fifo_;
    const uint32_t max_size_;
    std::mutex     mtx_;
    std::condition_variable cv_;
};

}  // music_group

#endif

