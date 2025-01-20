#include <thread>
#include <future>
#include <utility>
#include <chrono>
#include <cmath>
#include <fmt/core.h>

/*
 * 要获取带返回值的多线程任务，C++11 提供了 std::async 和 std::future，它们可以帮助你启动异步任务并获取任务的返回值。具体操作步骤如下：
 * 使用 std::async 启动一个异步任务（线程），并返回一个 std::future 对象。
 * 使用 std::future::get() 方法来获取异步任务的结果。get()会阻塞等待, 直到异步任务完成
 */

// 一个简单的函数，返回一个整数
int calculate_square(int x)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(300)); // 模拟耗时任务
  return x * x;
}

bool do_something(bool flag)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(300)); // 模拟耗时任务
  return flag;
}

void say_hello()
{
  fmt::println("hello std::async. std::launch::deferred");
}

auto main() -> int
{
  fmt::println("==========main runing...");
  std::future<int> task01 = std::async(std::launch::async, calculate_square, 5);
  std::future<bool> task02 = std::async(std::launch::async, do_something, true);
  std::future<double> task03 = std::async(std::launch::async, [](double x) { return std::sqrt(x); }, 81);
  std::future<void> task04 = std::async(std::launch::deferred, say_hello); // std::launch::deferred 会推迟任务的执行

  int ret01 = task01.get();
  bool ret02 = task02.get();
  double ret03 = task03.get();

  fmt::println("Square of 5 is: {}", ret01);
  fmt::println("do_something function return: {}", ret02);
  fmt::println("lambda async retrun: {}", ret03);
  task04.get();
  fmt::println("==========main end");
  return 0;
}