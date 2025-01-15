#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <fmt/core.h>

std::mutex mtx; // 互斥量
std::condition_variable cv; // 条件变量
bool isFinished = false; // 判断条件

std::deque<int> dataQueue;






int main()
{
  fmt::println("hello condition_variable.");
}