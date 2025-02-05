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
  std::shared_ptr<int> sp = std::make_shared<int>(42); // 创建并初始化
  fmt::print("sp value = {}\n", *sp);                  // 访问值
  // 作用域结束时，资源自动释放
}

void test02()
{
  std::shared_ptr<Object> osp = std::make_shared<Object>(200);
  fmt::print("osp id = {}\n", osp->id()); // 访问值
  auto sp2 = osp; // 多个shared_ptr共享资源
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
  if(filePtr)
  {
    fmt::println("文件存在");
  }else
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