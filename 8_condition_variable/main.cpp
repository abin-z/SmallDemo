#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <utility>

#include <fmt/core.h>

std::mutex mtx;             // 互斥量
std::condition_variable cv; // 条件变量
bool isFinished = false;    // 是否全部生产完成

std::deque<int> dataQueue;

/// @brief 获取线程id
/// @return 
size_t tid()
{
  return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

/// @brief 生产者
void producer()
{
  for (size_t i = 0; i < 20; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int num = i + 1;
    std::lock_guard<std::mutex> locker(mtx);
    dataQueue.push_back(num);
    fmt::println("[{:#x}] product finish num = {}", tid(), num);
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
    cv.wait(locker, []{ return !dataQueue.empty() || isFinished; }); // 等待条件满足
    while (!dataQueue.empty())
    {
      // 消费
      int val = dataQueue.front();
      dataQueue.pop_front();
      fmt::println("[{:#x}] consume value = {}", tid(), val);
    }
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

  std::thread t1(producer);
  std::thread t2(consumer);
  std::thread t3(consumer);

  t1.join();
  t2.join();
  t3.join();
  return 0;
}