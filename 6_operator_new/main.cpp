#include <fmt/core.h>
#include <new>
#include "person.hpp"

/*
在C++中，operator new和new operator还是很有区别。new operator是c++内建的，无法改变其行为；而operator new 是可以根据自己的内存分配策略去重载的。

1.new operator
概念：new operator 是 new 运算符，是一种语法，用于分配内存并构造对象。
功能：
调用全局或自定义的 operator new 分配原始内存。
调用对象的构造函数（如果是类类型）。
作用范围：开发者在代码中直接使用 new 时，调用的是 new operator。
实现过程：new operator 调用 operator new 分配内存，然后调用构造函数。

2. operator new
概念：operator new 是一个函数，负责底层内存分配。它是 new operator 调用的底层实现。
功能：仅负责分配原始内存，不涉及对象的构造。
作用范围：可以通过重载自定义内存分配行为，也可以直接调用（但这很少直接使用）。
实现方式：operator new 是 C++ 标准库提供的，可以全局或类级别重载。

operator new 唯一的任务就是分配内存，new operator的任务是取得 operator new 分配的内存，并将之转换为一个对象，返回一个指向它的指针；
同理operator delete的唯一任务是释放内存空间，它不能调用析构函数；


3. Placement new 特点
不分配内存：
Placement new 不会调用任何分配内存的操作（如 malloc 或 operator new）。
它只负责调用构造函数。

必须提供已有的内存块：
使用者必须显式提供一块足够大的内存，以存储对象。

常用于内存池：
Placement new 经常用于自定义的内存池或固定分配区域，以优化性能。

需要显式析构：
因为 Placement new 不管理内存，所以你需要手动调用析构函数，并管理内存释放。
*/

/// @brief 重载全局的operator new
/// @param size
/// @return
void *operator new(std::size_t size)
{
  // avoid std::malloc(0) which may return nullptr on success
  if (size == 0)
  {
    ++size;
  }
  fmt::println("global void *operator new(std::size_t size) called. size = {}", size);
  if (void *ptr = std::malloc(size))
  {
    return ptr;
  }
  throw std::bad_alloc{};
}

/// @brief 重载全局的 operator delete
/// @param p
void operator delete(void *ptr) noexcept
{
  fmt::println("global void operator delete(void *p) called.");
  std::free(ptr);
}

/// @brief 重载全局的operator new[]
/// @param size
/// @return
void *operator new[](std::size_t size)
{
  if (size == 0)
  {
    ++size;
  }
  fmt::println("global void *operator new[](std::size_t size) called. size = {}", size);
  if (void *ptr = std::malloc(size))
  {
    return ptr;
  }
  throw std::bad_alloc{};
}

/// @brief 重载全局的operator delete[]
/// @param ptr
void operator delete[](void *ptr) noexcept
{
  fmt::println("global void operator delete[](void *ptr) called.");
  std::free(ptr);
}

/// @brief 重载不抛异常的全局operator new
/// @param size
/// @return
void *operator new(std::size_t size, const std::nothrow_t &) noexcept
{
  // avoid std::malloc(0) which may return nullptr on success
  if (size == 0)
  {
    ++size;
  }
  fmt::println("global void *operator new(std::size_t size, const std::nothrow_t&) noexcept called. size = {}", size);
  return std::malloc(size);
}

/// @brief 重载不抛异常的全局operator new[]
/// @param size
/// @return
void *operator new[](std::size_t size, const std::nothrow_t &) noexcept
{
  // avoid std::malloc(0) which may return nullptr on success
  if (size == 0)
  {
    ++size;
  }
  fmt::println("global void *operator new[](std::size_t size, const std::nothrow_t&) noexcept called. size = {}", size);
  return std::malloc(size);
}

/// @brief 全局的placement new, 不会调用任何分配内存的操作
/// @param size
/// @param ptr
/// @return
// void* operator new(std::size_t size, void* ptr) noexcept {
//     return ptr;
// }

int main()
{
  fmt::println("==================================================");
  // 调用operator new
  int *p = new int{10};
  delete p;

  // 调用operator new[]
  int *arr = new int[10];
  delete[] arr;

  // 调用不抛异常版的 operator new
  int *ptr = new (std::nothrow) int{20};
  if (!ptr)
    *ptr = 30;
  delete ptr;

  // 调用不抛异常版的 operator new[]
  int *ptrArr = new (std::nothrow) int[20];
  delete[] ptrArr;

  fmt::println("==================================================");
  Person *p1 = new Person(10);
  delete p1;

  fmt::println("==================================================");
}
