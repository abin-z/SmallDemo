#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <fmt/core.h>

std::mutex mtx;             // 互斥量
std::condition_variable cv; // 条件变量
bool isFinished = false;    // 是否全部生产完成

std::deque<int> dataQueue;

void producer()
{
  for (size_t i = 0; i < 10; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    int num = i + 1;
    std::lock_guard<std::mutex> locker(mtx);
    dataQueue.push_back(num);
    fmt::println("product finish num = {}", num);
    cv.notify_one();
  }
  // 通知消费者已全部生产完成
  {
    std::lock_guard<std::mutex> locker(mtx);
    isFinished = true;
  }
  cv.notify_all();
}

void consumer()
{
  while (true)
  {
    std::unique_lock<std::mutex> locker(mtx);
    cv.wait(locker, [](){ return !dataQueue.empty() || isFinished; }); // 等待条件满足
    while (!dataQueue.empty())
    {
      // 消费
      int val = dataQueue.front();
      dataQueue.pop_front();
      fmt::println("consume value = {}", val);
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
}