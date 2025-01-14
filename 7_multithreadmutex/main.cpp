#include <thread>
#include <mutex>
#include <chrono>
#include <fmt/core.h>

std::mutex mtx; // 全局互斥量
int sharedData = 0;


void incrementWithoutMutex()
{
  for (size_t i = 0; i < 100000; ++i)
  {
    ++sharedData;
  }
}

void incrementWithMutex()
{
  for (size_t i = 0; i < 100000; ++i)
  {
    std::lock_guard<std::mutex> locker(mtx);  // 自动加锁和解锁
    ++sharedData;
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
  fmt::println("===============================================");
  fmt::println("incrementWithoutMutex sharedData = {}, is error!", sharedData);

  fmt::println("===============================================");
  sharedData = 0;
  std::thread tt1(incrementWithMutex);
  std::thread tt2(incrementWithMutex);
  std::thread tt3(incrementWithMutex);

  tt1.join();
  tt2.join();
  tt3.join();
  fmt::println("incrementWithMutex sharedData = {}", sharedData);
  fmt::println("===============================================");
  return 0;
}
