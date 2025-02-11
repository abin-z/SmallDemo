#include <fmt/core.h>
#include <future>
#include <functional>

int add(int a, int b) // 普通函数
{
  return a + b;
}

class Utils
{
public:
  int add(int a, int b) // 成员函数
  {
    return a + b;
  }
};

class Adder
{
public:
  int operator()(int a, int b) // 仿函数
  {
    return a + b;
  }
};

int main()
{
  fmt::println("========== callable object ==========");
  // 1.普通函数（Function）
  fmt::println("1. Function: {}", add(3, 4));

  // 2. 函数指针（Function Pointer）
  typedef int (*add_func_t)(int, int);
  add_func_t add_func = add;
  fmt::println("2. Function Pointer: {}", add_func(3, 4));

  // 3. 函数对象（Functor）
  Adder adder;
  fmt::println("3. Functor: {}", adder(3, 4));

  return 0;
}