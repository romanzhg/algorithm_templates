/**
 * A Producer/Consumer queue. Support blocking/non blocking push.
 * Note that this utility is assumed to be used ony by two threads. If there going to be multiple 
 * producer threads or multiple consumer threads, then two conditional variables are needed to split
 * the writer queue and reader queue.
 */
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using std::unique_lock;
using std::mutex;
using std::lock_guard;
using std::condition_variable;
using std::queue;
using std::cout;
using std::endl;

struct BlockingByteQueue {
  BlockingByteQueue() {};

  // A blocking push.
   void Push(uint8_t *data, int len) {
    unique_lock<mutex> ul(mutex_);
    cv_.wait(ul, [this, &len]{return ((int)data_.size()) + len <= kBufferSizeLimit;});
    for (int i = 0; i < len; i++) {
      data_.push((uint8_t) data[i]);
    }
    cv_.notify_one();
  }

  bool PushNonBlocking(uint8_t *data, int len) {
    unique_lock<mutex> ul(mutex_);
    if (((int)data_.size()) + len > kBufferSizeLimit) {
      return false;
    }
    for (int i = 0; i < len; i++) {
      data_.push((uint8_t) data[i]);
    }
    cv_.notify_one();
    return true;
  }

  // A blocking Get.
  uint8_t Get() {
    unique_lock<mutex> ul(mutex_);
    cv_.wait(ul, [this]{return !data_.empty();});
    uint8_t rtn = data_.front();
    data_.pop();
    cv_.notify_one();
    return rtn;
  }

  mutex mutex_;
  condition_variable cv_;
  queue<uint8_t> data_;
  static constexpr int kBufferSizeLimit = 1024 * 1024;
};

void PushThread(BlockingByteQueue* bbq) {
  char *data = "abcdefghij";
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cout << "going to push" << endl;
    bbq->Push(reinterpret_cast<uint8_t *>(data), 10);
    cout << "pushed" << endl;
  }
}

void GetThread(BlockingByteQueue* bbq) {
  char data;
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    data = bbq->Get();
    cout << data;
  }
}

int main() {
  BlockingByteQueue bbq;
  std::thread t0(PushThread, &bbq);

  GetThread(&bbq);
  return 0;
}
