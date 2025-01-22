#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>
#include <utility>
#include <chrono>

#include <fmt/core.h>
#include <fmt/ranges.h>

/*
 * 读写锁基本概念：
 * - 共享锁（读锁）：允许多个线程同时读取数据，且不会互相阻塞。
 * - 独占锁（写锁）：只有一个线程可以写入数据，且在写入期间，其他线程不能读也不能写。
 * 
 * 在 C++14 及更高版本中，std::shared_mutex 是一个很好的选择。它提供了两种基本操作：
 * 共享锁（读锁）：std::shared_lock<std::shared_mutex> lock(rwMutex); 允许多个线程同时读取数据。
 * 独占锁（写锁）：std::unique_lock<std::shared_mutex> lock(rwMutex); 只有一个线程可以写入数据，写入时其他线程不能读或写。
 */

/// @brief 获取线程id
/// @return
size_t tid()
{
  static thread_local size_t id = std::hash<std::thread::id>{}(std::this_thread::get_id()) % 0xFFFF;
  return id;
}

/// @brief 共享锁
std::shared_mutex rwMtx;
std::vector<int> sharedData; // 共享资源

void readData()
{
  std::shared_lock<std::shared_mutex> locker(rwMtx); // 共享锁
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  fmt::print("[{:#06x}]: read data {}\n", tid(), sharedData);
}

void writeData(int value)
{
  std::unique_lock<std::shared_mutex> locker(rwMtx);           // 独占锁
  std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 模拟重任务
  sharedData.push_back(value);
  fmt::print("[{:#06x}]: write value {}\n", tid(), value);
}

int main()
{
  fmt::print("Hello ReadWriteLock\n");
  // 写线程
  std::vector<std::thread> writers;
  constexpr int writerCount = 9;
  for (int i = 0; i < writerCount; ++i)
  {
    writers.emplace_back(std::thread(writeData, i * 10));
  }

  // 读取线程
  std::vector<std::thread> readers;
  constexpr int readerCount = 30;
  for (int i = 0; i < readerCount; ++i)
  {
    readers.emplace_back(std::thread(readData));
  }

  // join
  for (auto &th : writers)
  {
    th.join();
  }
  for (auto &th : readers)
  {
    th.join();
  }
  fmt::print("now data : {}\n", sharedData);

  return 0;
}