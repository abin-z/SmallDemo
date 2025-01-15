#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <fmt/core.h>

std::mutex mtx; // 全局互斥量
int sharedData = 0;

// 原子操作
std::atomic<int> atomic_sharedData(0);

/// @brief 在多线程下不加锁会出现异常
void incrementWithoutMutex()
{
  for (size_t i = 0; i < 100000; ++i)
  {
    ++sharedData;
  }
}

/// @brief 加锁后保障同一时刻只有一个线程修改该共享资源
void incrementWithMutex()
{
  for (size_t i = 0; i < 100000; ++i)
  {
    std::lock_guard<std::mutex> locker(mtx); // 自动加锁和解锁
    ++sharedData;
  }
}

/// @brief 使用原子整数后, 原子操作就是线程安全的. 不需要锁保护, 适合实现无锁编程 
void incrementAtomic()
{
  for (size_t i = 0; i < 100000; ++i)
  {
    ++atomic_sharedData;
    atomic_sharedData.fetch_add(2);
  }
}

int main()
{
  std::thread t1(incrementWithoutMutex);
  std::thread t2(incrementWithoutMutex);
  std::thread t3(incrementWithoutMutex);

  t1.join();
  t2.join();
  t3.join();
  fmt::println("=======================start========================");
  fmt::println("incrementWithoutMutex sharedData = {}, is incorrect!", sharedData);

  fmt::println("--------------------");
  sharedData = 0;
  std::thread tt1(incrementWithMutex);
  std::thread tt2(incrementWithMutex);
  std::thread tt3(incrementWithMutex);

  tt1.join();
  tt2.join();
  tt3.join();
  fmt::println("incrementWithMutex sharedData = {}, is correct!", sharedData);


  fmt::println("--------------------");
  std::thread ttt1(incrementAtomic);
  std::thread ttt2(incrementAtomic);
  std::thread ttt3(incrementAtomic);

  ttt1.join();
  ttt2.join();
  ttt3.join();
  fmt::println("incrementWithMutex atomic_sharedData = {}, is correct!", atomic_sharedData.load());
  fmt::println("=======================end========================");
  return 0;
}
