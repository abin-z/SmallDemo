#include <fmt/core.h>

#include <atomic>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

std::mutex mtx;              // 互斥量
std::condition_variable cv;  // 条件变量

std::deque<int> dataQueue;  // 共享消费队列
bool isFinished = false;    // 是否全部生产完成, 可以使用std::atomic<bool>

/// @brief 获取线程id
/// @return
size_t tid()
{
  // thread_local
  // 关键字修饰的变量具有线程（thread）周期，这些变量在线程开始的时候被生成，在线程结束的时候被销毁，并且每一个线程都拥有一个独立的变量实例
  static thread_local size_t tid =
    std::hash<std::thread::id>{}(std::this_thread::get_id()) % std::numeric_limits<uint16_t>::max();
  return tid;
}

/// @brief 生产者
void producer(int num)
{
  for (int i = 0; i < num; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int tatget = i + 1;
    {
      std::lock_guard<std::mutex> locker(mtx);
      dataQueue.push_back(tatget);
    }
    fmt::println("[{:#06x}] Produced num = {}", tid(), tatget);
    cv.notify_one();
  }
  // 通知消费者已全部生产完成
  {
    std::lock_guard<std::mutex> locker(mtx);
    isFinished = true;
  }
  cv.notify_all();
}

/// @brief 消费者
void consumer()
{
  while (true)
  {
    std::unique_lock<std::mutex> locker(mtx);
    cv.wait(locker, [] { return !dataQueue.empty() || isFinished; });  // 等待条件满足
    // // 消费方式1 ------------------------start
    // while (!dataQueue.empty())
    // {
    //   int val = dataQueue.front();
    //   dataQueue.pop_front();
    //   fmt::println("[{:#x}] Consumed value = {}", tid(), val);
    // }
    // // 消费方式1 ------------------------end

    // 消费方式2 ------------------------start
    std::deque<int> localQueue;
    std::swap(localQueue, dataQueue);
    for (int val : localQueue)
    {
      fmt::println("[{:#06x}] Consumed value = {}", tid(), val);
    }
    locker.unlock();
    // 消费方式2 ------------------------end

    // 退出循环条件
    if (isFinished && dataQueue.empty())
    {
      break;
    }
  }
}

int main()
{
  fmt::println("hello condition_variable.");

  std::thread t1(producer, 20);
  std::thread t2(consumer);
  std::thread t3(consumer);

  t1.join();
  t2.join();
  t3.join();
  fmt::println("All threads finished.");
  return 0;
}