#include <fmt/core.h>
#include <future>
#include <functional>

/**
 * 在 C++ 中，可调用对象是指那些可以通过 `operator()` 被调用的对象。C++ 提供了多种方式来创建和使用可调用对象.
 */

int add(int a, int b) // 普通函数
{
  return a + b;
}

class Math
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
  add_func_t add_func = &add;
  fmt::println("2. Function Pointer: {}", add_func(3, 4));

  // 3. 函数对象（Functor）
  Adder adder;
  fmt::println("3. Functor: {}", adder(3, 4));

  // 4. Lambda 表达式（Anonymous Functions）
  auto funcAdd = [](int a, int b)
  {
    return a + b;
  };
  fmt::println("4. Lambda expression(Anonymous Functions): {}", funcAdd(3, 4));

  // 5. 成员函数指针（Member Function Pointer）
  Math math;
  int (Math::*func_ptr)(int, int) = &Math::add;
  fmt::println("5. Member Function Pointer: {}", (math.*func_ptr)(3, 4));

  // 6. std::function 是 C++11 引入的一个通用的可调用对象包装器
  std::function<int(int, int)> addFunc = add;
  fmt::println("6. std::function: {}", addFunc(3, 4));

  // 7. std::packaged_task `std::packaged_task` 是 C++11 引入的一个任务封装器，支持异步计算，适用于 C++11 及以上版本
  std::packaged_task<int(int, int)> task(add);
  std::future<int> fut = task.get_future();
  task(3, 4);
  fmt::println("7. std::packaged_task: {}", fut.get());

  // std::bind `std::bind` 是 C++11 引入的函数适配器，允许将函数参数绑定，适用于 C++11 及以上版本。
  auto add_fn = std::bind(add, 3, std::placeholders::_1);
  fmt::println("8. std::bind: {}", add_fn(4));

  // 9. std::invoke `std::invoke` 是 C++17 引入的一个统一调用接口，用于调用可调用对象，适用于 C++17 及以上版本。
  fmt::println("9. std::invoke: {}", std::invoke(add, 3, 4));
  fmt::println("9. std::invoke: {}", std::invoke(&Math::add, math, 3, 4)); // 需要传入一个类对象

  // 10. std::mem_fn, 建议使用auto. `std::mem_fn` 是 C++11 引入的，它可以将成员函数转换为一个可调用对象
  std::function<int(Math &, int, int)> add_mem_fn = std::mem_fn(&Math::add);
  fmt::println("10. std::mem_fn: {}", add_mem_fn(math, 3, 4)); // 需要传入一个类对象

  return 0;
}