#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <utility>
#include <chrono>
#include <fmt/core.h>
#include <fmt/ranges.h>

/*
 * C++ 锁机制专题
 *   互斥锁 (std::mutex) 是 C++11 引入的基础锁类型，用来保证多个线程访问共享资源时的互斥性
 *   递归锁 (std::recursive_mutex) 是 std::mutex 的一种变体，它允许同一个线程多次锁定同一互斥量，而不会发生死锁。但对于 不同线程，它和普通的互斥锁一样，会表现为互斥行为
 *   共享锁与独占锁 (std::shared_mutex) 是一种新的锁机制，允许多个线程以“共享”方式并发读共享资源，而写线程则必须获得独占锁，阻塞其他读线程和写线程。
 *           共享锁: std::shared_lock<std::shared_mutex> lock(rwMutex);  // 共享锁
 *           独占锁: std::unique_lock<std::shared_mutex> lock(rwMutex);  // 独占锁
 *
 * 防止死锁的方法：
 *   固定锁的顺序：让多个线程按照固定顺序来获取多个锁。
 *   避免长时间持有锁：避免长时间持有锁的操作，缩小锁的作用范围。
 *   使用 std::lock：std::lock 可以同时锁定多个互斥量，避免死锁。
 *
 *   std::lock 用于同时锁定多个互斥量。它会尝试以相同的顺序获取所有互斥量。如果某个互斥量已经被锁定，它会阻塞直到所有锁都成功获取
 *   std::lock 本身不会管理锁的生命周期，它只是确保多个互斥量不会发生死锁。
 *   你仍然需要使用 std::lock_guard 或其他 RAII 风格的锁来自动释放锁，否则锁会保持在任务完成后仍然处于锁定状态，这会导致其他线程无法获取这些锁，从而发生死锁。
 *   std::lock(mtx1, mtx2, ...);  // 尝试同时锁定 mtx1 和 mtx2
 *   std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);  // 使用 adopt_lock 告诉 lock_guard 不要再尝试锁定 mtx1
 *   std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);  // 使用 adopt_lock 告诉 lock_guard 不要再尝试锁定 mtx2
 *   ...
 */

/// @brief 获取线程id的hash
/// @return
size_t tid()
{
  static thread_local size_t id = std::hash<std::thread::id>{}(std::this_thread::get_id()) % 0xFFFF;
  return id;
}

/// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @brief 互斥锁
std::mutex mtx; // 互斥量
int sharedData = 0;
void increment()
{
  std::lock_guard<std::mutex> locker(mtx); // 互斥锁,自动锁定和释放
  ++sharedData;                            // 操作共享资源
}
void testMutex()
{
  std::thread t1(increment);
  std::thread t2(increment);

  t1.join();
  t2.join();
  fmt::print("互斥锁 (std::mutex) : ret = {}\n", sharedData);
}

/// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @brief 递归锁
std::recursive_mutex rmtx; // 递归互斥量
int counter = 0;
void recursiveFunction(int depth)
{
  std::lock_guard<std::recursive_mutex> locker(rmtx); // 递归锁: 允许同一个线程多次锁定同一互斥量，而不会发生死锁。
  if (depth > 0)
  {
    fmt::print("Recursion depth: {}\n", depth);
    recursiveFunction(depth - 1);
  }
  ++counter; // 操作共享资源
}
void testRecursiveMutex()
{
  std::thread t(recursiveFunction, 5);
  t.join();
  fmt::print("递归锁 (std::recursive_mutex) ret = {}\n", counter);
}

/// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @brief 共享锁
std::shared_mutex rwMtx;
std::vector<int> sharedDataVec; // 共享资源
void readData()
{
  std::shared_lock<std::shared_mutex> locker(rwMtx); // 读共享
  fmt::print("[{:#06x}]: read data {}\n", tid(), sharedDataVec);
}
void writeData(int value)
{
  std::unique_lock<std::shared_mutex> locker(rwMtx); // 写独占
  sharedDataVec.push_back(value);
  fmt::print("[{:#06x}]: write value {}\n", tid(), value);
}
void testSharedMutex()
{
  std::vector<std::thread> writers; // 写线程
  constexpr int writerCount = 3;
  for (int i = 0; i < writerCount; ++i)
  {
    writers.emplace_back(std::thread(writeData, i * 10));
  }
  std::vector<std::thread> readers; // 读取线程
  constexpr int readerCount = 5;
  for (int i = 0; i < readerCount; ++i)
  {
    readers.emplace_back(std::thread(readData));
  }
  for (auto &th : writers)
  {
    th.join();
  }
  for (auto &th : readers)
  {
    th.join();
  }
}

/// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @brief 防范死锁
std::mutex mtx1, mtx2;
void task1()
{
  std::lock(mtx1, mtx2);                                    // 尝试同时锁定 mtx1 和 mtx2, 参数顺序不重要
  std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock); // 使用 adopt_lock 告诉 lock_guard 不要再尝试锁定 mtx1
  std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock); // 使用 adopt_lock 告诉 lock_guard 不要再尝试锁定 mtx2
  fmt::print("Task 1 completed\n");
}
void task2()
{
  std::lock(mtx2, mtx1);                                    // 尝试同时锁定 mtx1 和 mtx2
  std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock); // 使用 adopt_lock 告诉 lock_guard 不要再尝试锁定 mtx1
  std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock); // 使用 adopt_lock 告诉 lock_guard 不要再尝试锁定 mtx2
  fmt::print("Task 2 completed\n");
}
void testVoidDeadlock()
{
  std::thread t1(task1);
  std::thread t2(task2);

  t1.join();
  t2.join();
}

/// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main()
{
  fmt::println("---------------------------------------------------");
  testMutex();
  fmt::println("---------------------------------------------------");
  testRecursiveMutex();
  fmt::println("---------------------------------------------------");
  testSharedMutex();
  fmt::println("---------------------------------------------------");
  testVoidDeadlock();
  fmt::println("---------------------------------------------------");
}