#include <fmt/core.h>

#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

/// @brief 适用于 普通函数、Lambda、仿函数调用 (C++17后有std::invoke很方便)
/// @tparam Callable 可调用对象类型
/// @tparam ...Args 类型参数包
/// @param func 可调用对象
/// @param ...args 函数参数包
/// @return 自动类型推导
template <typename Callable, typename... Args>
auto invoke(Callable &&func, Args &&...args) -> decltype(std::forward<Callable>(func)(std::forward<Args>(args)...))
{
  return std::forward<Callable>(func)(std::forward<Args>(args)...);
}

/// @brief 适用于成员函数
/// @tparam R 成员函数的返回类型
/// @tparam C 成员函数所属的类
/// @tparam T 类的实例（对象）
/// @tparam ...Args 成员函数的参数类型包
/// @param func 成员函数指针
/// @param obj 类对象
/// @param ...args 成员函数参数包
/// @return 自动推导类型
template <typename R, typename C, typename T, typename... Args>
auto invoke(R (C::*func)(Args...), T &&obj, Args &&...args) -> decltype((std::forward<T>(obj).*func)(std::forward<Args>(args)...))
{
  return (std::forward<T>(obj).*func)(std::forward<Args>(args)...);
}

// 此函数在上述函数中添加了 const, 可以调用const 成员函数指针
template <typename R, typename C, typename T, typename... Args>
auto invoke(R (C::*func)(Args...) const, T &&obj, Args &&...args) -> decltype((std::forward<T>(obj).*func)(std::forward<Args>(args)...))
{
  return (std::forward<T>(obj).*func)(std::forward<Args>(args)...);
}

// 测试类
struct Test
{
  int value = 42;

  void sayHello()
  {
    std::cout << "Hello, world!" << std::endl;
  }

  int getValue() const noexcept
  {
    return value;
  }

  void print(const std::string &str) const
  {
    std::cout << str << std::endl;
  }
};

// C++11的实现方式(递归展开), C++17后有折叠表达式（Fold Expressions) 可以避免递归调用
template <typename T>
T getSum(T val)
{
  return val;
}

template <typename T, typename... Args>
auto getSum(T first, Args... args) -> typename std::common_type<T, Args...>::type  // 这里使用decltype(first + getSum(args...))在Linux下不可行
{
  return first + getSum(args...);
}

struct GetSumWrapper
{
  template <typename... Args>
  auto operator()(Args... args) -> decltype(getSum(args...))
  {
    return getSum(args...);
  }
};

void sayhello()
{
  std::cout << "hello world" << std::endl;
}

int sum(int a, int b) noexcept
{
  return a + b;
}

int main()
{
  std::cout << "invoke demo" << std::endl;
  fmt::print("getsum ret = {}\n", getSum(10, 20));
  fmt::print("getsum ret = {}\n", getSum(10, 20, 3.14f));
  fmt::print("getsum ret = {}\n", getSum(10, 20, 5.632));
  fmt::print("getsum ret = {}\n", getSum(3.14, 20, 30, 40));
  fmt::print("getsum ret = {}\n", getSum(10, 20, 30, 40, 50, 60));
  fmt::print("getsum ret = {}\n", getSum(10, 20, 30, 40, 50.0, 60, 70, 80, 90, 100));

  invoke(sayhello);
  int ret = invoke(sum, 100, 200);
  std::cout << "ret = " << ret << std::endl;

  /**
   * getSum 是一个函数模板，它不是一个具体的函数，编译器无法推导 Callable 的类型。
   * invoke 需要一个可调用的具体函数，但 getSum 需要先实例化出具体的 getSum<double, int, int, int> 版本才行。
   */
  // 方式1: 显式实例化 getSum
  int ret2 = invoke(getSum<double, int, int, int>, 3.14, 20, 30, 60);
  // 方式2: 使用结构体仿函数包装getSum
  int ret3 = invoke(GetSumWrapper{}, 3.14, 20, 30, 60);
  // 方式3: 使用std::function包装getSum
  std::function<double(double, int, int, int)> f_wrapper = getSum<double, int, int, int>;
  int ret4 = f_wrapper(3.14, 20, 30, 60);

  // 方式4: 使用lambda, 需要c++17支持
  auto wrapper = [](auto... args) { return getSum(args...); };
  int ret5 = invoke(wrapper, 3.14, 20, 30, 60);

  Test tst;
  invoke(&Test::sayHello, tst);            // 调用成员函数
  int val = invoke(&Test::getValue, tst);  // 调用const成员函数
  std::cout << "invoke(&Test::getValue, tst) ret = " << val << std::endl;
  invoke(&Test::print, tst, "variadic template nice!");  // 注: 在c++11标准下还有问题, 但是在c++17下就没问题

  // 在c++17中有std::invoke, 很方便
  std::invoke(&Test::print, tst, "std::invoke nice!");
}