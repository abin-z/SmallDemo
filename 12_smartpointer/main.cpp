#include <vector>
#include <utility>
#include <chrono>
#include <memory>
#include <cstdio>
#include <fmt/core.h>
#include <fmt/ranges.h>

/**
 * std::unique_ptr 是 C++11 引入的一种智能指针，用于管理动态分配的对象，保证在指针生命周期结束时自动释放内存。
 * 它具有独占所有权的特点，即同一时间内只能有一个 unique_ptr 指向某个对象，因此不允许拷贝，但可以通过移动语义转移所有权。
 * 
 * 可以通过自定义删除器来处理非标准的资源释放需求。
 * 对于那些不是通过 new 分配的资源，比如操作系统资源（文件句柄、socket、数据库连接等），需要使用专门的释放函数（如 fclose、close、sqlite3_close 等）来进行释放。
 * 通过自定义删除器，可以确保在 unique_ptr 生命周期结束时正确调用对应的释放函数，从而防止资源泄露。
 */

void test01()
{
  std::unique_ptr<double> up(new double(3.14));
  std::shared_ptr<long long> sp(new long long(999));
  // 推荐使用 make_xxx 创建智能指针, 除非要制定删除器
  std::unique_ptr<int> up2 = std::make_unique<int>(100); // 使用 std::make_unique（推荐，C++14 及以上）：
  std::shared_ptr<int> sp2 = std::make_shared<int>(200);

  fmt::print("*up = {}\n", *up);
  fmt::print("*sp = {}\n", *sp);
  fmt::print("*up2 = {}\n", *up2);
  fmt::print("*sp2 = {}\n", *sp2);
}

struct Object
{
  Object()
  {
    fmt::print("Object constructor\n");
  }

  ~Object()
  {
    fmt::print("~Object destructor\n");
  }
};

/// @brief unique_ptr转移所有权
void test02()
{
  std::unique_ptr<Object> up = std::unique_ptr<Object>(new Object);
  fmt::print("Pointer value: {:p}\n", static_cast<void*>(up.get()));
  std::unique_ptr<Object> up2 = std::move(up); // 所有权转移，原来的 unique_ptr 会变为空（即内部指针为 nullptr），因此不要在转移后继续使用它。
  fmt::print("Pointer value: {:p}\n", static_cast<void*>(up.get()));
}

/// @brief unique_ptr的释放和重置
void test03()
{
  std::unique_ptr<Object> uptr = std::make_unique<Object>();
  // 释放所有权: 使用 release() 方法可以释放 unique_ptr 对象对资源的管理，并返回内部指针，但不会释放资源，需要手动释放。(智能指针本身将变为空)
  Object *rawPtr = uptr.release();
  delete rawPtr;

  // 重置指针: 使用 reset() 方法可以释放当前资源，并指向新资源（如果传入新的指针）。
  uptr.reset(new Object());
  uptr.reset();
}

// std::unique_ptr 的自定义删除器
struct FileCloser
{
  void operator()(FILE *p) const
  {
    if(p)
    {
      fmt::print("close file {:p}\n", static_cast<void *>(p));
      fclose(p);
    }
  }
};

/// @brief unique_ptr管理文件文件句柄
void test04()
{
  std::unique_ptr<FILE, FileCloser> filePtr(fopen("example.txt", "r"));
  if(!filePtr)
  {
    fmt::print("文件不存在\n");
  }
  else
  {
    fmt::print("文件存在!\n");
  }
  // 使用 filePtr 操作文件...
  // 作用域结束时自动调用传入的删除器关闭文件
}

/// @brief 用智能指针管理原始数组
void test05()
{
  std::unique_ptr<int[]> arrptr = std::unique_ptr<int[]>(new int[3]);
  std::unique_ptr<Object[]> objArr = std::make_unique<Object[]>(3);
}

int main()
{
  test01();
  fmt::println("---------------------------------------------------");
  test02();
  fmt::println("---------------------------------------------------");
  test03();
  fmt::println("---------------------------------------------------");
  test04();
  fmt::println("---------------------------------------------------");
  test05();
  fmt::println("---------------------------------------------------");
}