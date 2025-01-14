#include <thread>
#include <chrono>
#include <string>
#include <utility>
#include <fmt/core.h>

/*
多线程是一种并发编程技术，允许程序在多个线程中并行执行任务。每个线程都有自己的指令序列和局部变量，但共享全局内存。

t.join()：主线程等待子线程完成。
t.detach()：让线程在后台运行，主线程无需等待
*/


void task01(int num)
{
  std::size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
  fmt::println("task01 thread id = {:#x}", threadId);
  // 模拟耗时任务
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  fmt::println("task01 num = {}", num);
}

void task02(double x, double y, double z, double &result)
{
  // 模拟耗时任务
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  result = x * y * z;
  std::size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
  fmt::println("task02 thread id = {:#x}", threadId);
}

int main()
{
  fmt::println("==============main start...  main thread id = {:#x}", std::hash<std::thread::id>{}(std::this_thread::get_id()));
  // 直接使用lambda表达式
  std::thread t0([]{
    fmt::println("Hello, I am thread0. thread id = {:#x}", std::hash<std::thread::id>{}(std::this_thread::get_id()));
  });
  t0.detach(); // 必须调用join或者detach, 否则会异常

  // 带参任务, 拷贝参数
  std::thread t1(task01, 100);
  t1.join();

  // 带参任务, 使用 std::ref 传递引用
  double ret2 = 0;
  std::thread t2(task02, 111, 3, 3, std::ref(ret2));
  t2.join();
  fmt::println("ret2 = {}", ret2);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  fmt::println("==============main end...");
}