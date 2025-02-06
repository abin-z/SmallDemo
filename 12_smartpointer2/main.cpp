#include <vector>
#include <utility>
#include <chrono>
#include <memory>
#include <cstdio>
#include <fmt/core.h>
#include <fmt/ranges.h>

/**
 * std::shared_ptr 是 C++ 标准库提供的智能指针，用于多个对象共享同一资源的所有权，当最后一个 shared_ptr 离开作用域时，资源会被自动释放。
 *
 * 其核心原理基于引用计数和控制块机制.
 * 
 * 控制块（Control Block）：
 * 每个由 shared_ptr 管理的对象都有一个关联的控制块，存储以下信息：
 * 强引用计数（Strong Reference Count）：记录当前有多少个 shared_ptr 指向该对象。
 * 弱引用计数（Weak Reference Count）：记录当前有多少个 weak_ptr 观察该对象。
 * 删除器（Deleter）：用于销毁对象的函数或函数对象（默认为 delete）。
 * 分配器（Allocator，可选）：用于分配/释放控制块内存的机制。
 *
 * 引用计数规则：
 * 当 shared_ptr 被拷贝时，强引用计数 +1。
 * 当 shared_ptr 被销毁或 被重置时，强引用计数 -1。
 * 当强引用计数归零时，对象被销毁（调用删除器）。
 * 当弱引用计数也归零时，控制块的内存被释放。
 * 强引用和弱引用计数的增减是原子操作，线程安全。
 *
 */

struct Object
{
  int id_ = 0;
  Object(int id = 0) : id_(id)
  {
    fmt::print("Object constructor\n");
  }

  ~Object()
  {
    fmt::print("~Object destructor\n");
  }
  int id()
  {
    return id_;
  }
};

void test01()
{
  std::shared_ptr<int> sp(new int(42)); // 需要两次内存分配：一次给对象，一次给控制块。
  fmt::print("sp value = {}\n", *sp);   // 访问值
  // 作用域结束时，资源自动释放
}

void test02()
{
  std::shared_ptr<Object> osp = std::make_shared<Object>(200); // 一次分配足够大的内存，同时容纳对象和控制块，提高性能。
  fmt::print("osp id = {}\n", osp->id());                      // 访问值
  auto sp2 = osp;                                              // 多个shared_ptr共享资源
  auto sp3 = sp2;
  fmt::print("use_count = {}\n", osp.use_count());
  sp2.reset(); // sp2 不再持有资源
  fmt::print("use_count = {}\n", osp.use_count());
  sp3.reset(); // sp3 不再持有资源
  fmt::print("use_count = {}\n", osp.use_count());
}

struct FileDeleter
{
  void operator()(FILE *file) const
  {
    if (file)
    {
      fmt::print("正在关闭文件...\n");
      fclose(file);
    }
  }
};

void test03()
{
  std::shared_ptr<FILE> filePtr(fopen("test.txt", "w"), FileDeleter()); // 指定删除器
  if (filePtr)
  {
    fmt::println("文件存在");
  }
  else
  {
    fmt::println("文件不存在");
  }
}

int main()
{
  test01();
  fmt::println("---------------------------------------------------");
  test02();
  fmt::println("---------------------------------------------------");
  test03();
  fmt::println("---------------------------------------------------");
  // test04();
  fmt::println("---------------------------------------------------");
  // test05();
  fmt::println("---------------------------------------------------");
}