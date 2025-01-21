#include <thread>
#include <future>
#include <vector>
#include <chrono>
#include <cmath>
#include <fmt/core.h>

/*
 * 要获取带返回值的多线程任务，C++11 提供了 std::async 和 std::future，它们可以帮助你启动异步任务并获取任务的返回值。具体操作步骤如下：
 * 使用 std::async 启动一个异步任务（线程），并返回一个 std::future 对象。
 * 使用 std::future::get() 方法来获取异步任务的结果。get()会阻塞等待, 直到异步任务完成
 */

double mypow(double base, double exponent)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 模拟耗时任务
  return std::pow(base, exponent);
}

void process_element(double &x)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 模拟耗时任务
  x *= 2;                                                       // 将元素乘以 2
}

auto main() -> int
{
  fmt::println("==========main runing...");

  std::vector<double> values{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<std::future<double>> futures;
  for (auto val : values)
  {
    futures.emplace_back(std::async(std::launch::async, mypow, val, 2));
  }

  std::vector<double> values2{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<std::thread> threads;
  for (auto &val : values2)
  {
    threads.emplace_back(std::thread(process_element, std::ref(val)));
  }
  // 调用future::get()获取结果
  for (auto &fut : futures)
  {
    fmt::print("item = {} ", fut.get());
  }
  fmt::print("\n");

  // 等待所有线程完成
  for (auto &th : threads)
  {
    th.join();
  }
  // 输出处理后的 vector
  for (auto val : values2)
  {
    fmt::print("item = {} ", val);
  }
  fmt::print("\n");
  fmt::println("==========main end");
  return 0;
}