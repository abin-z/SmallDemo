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

/// @brief 异步执行获取返回值的方式2
/// @param promise
/// @param x 参数
void getResult(std::promise<int> &promise, int x)
{
  try
  {
    if (x > 0)
    {
      int ret = x * x * x;
      promise.set_value(ret); // 设置值
    }
    else
    {
      throw std::runtime_error("Something went wrong at getResult()!");
    }
  }
  catch (...)
  {
    promise.set_exception(std::current_exception()); // 抛出异常给 future
  }
}

auto main() -> int
{
  fmt::println("==========main runing...");
  // 获取返回值的方式1: std::future + std::async
  std::future<int> task01 = std::async(std::launch::async, calculate_square, 5);
  std::future<bool> task02 = std::async(std::launch::async, do_something, true);
  std::future<void> task04 = std::async(std::launch::deferred, say_hello); // std::launch::deferred 会推迟任务的执行
  std::future<double> task03 = std::async(std::launch::async, [](double x)
                                          { return std::sqrt(x); }, 81);
  // 可以使用try-catch包含.get()
  int ret01 = task01.get();
  bool ret02 = task02.get();
  double ret03 = task03.get();

  fmt::println("Square of 5 is: {}", ret01);
  fmt::println("do_something function return: {}", ret02);
  fmt::println("lambda async retrun: {}", ret03);
  task04.get(); // std::launch::deferred 这个时候才会执行task04;




  fmt::println("====================");
  // 获取返回值的方式2: std::promise + std::future
  std::promise<int> prom;
  std::future<int> fut = prom.get_future();
  std::thread t1(getResult, std::ref(prom), 3);
  try
  {
    int ret = fut.get(); // 阻塞直到获取到结果
    fmt::println("getResult() = {}", ret);
  }
  catch (const std::exception &e)
  {
    fmt::println("Caught exception: {}", e.what());
  }
  t1.join();
  fmt::println("==========main end");
  return 0;
}