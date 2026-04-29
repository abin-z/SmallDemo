// CTAD（Class Template Argument Deduction，C++17）
//
// 作用：允许编译器根据构造函数参数自动推导类模板的模板参数，
//       从而在使用时可以省略显式的 <T>，减少重复代码，提升可读性。
//
// 解决的问题：
//   - 避免重复书写模板参数（参数中已包含类型信息）
//   - 不再依赖 make_xxx 这类辅助函数来做类型推导
//   - 使类模板的使用方式更接近函数模板（自动类型推导）
//
// 实现机制：
//   - 基于构造函数参数进行模板参数推导
//   - 编译器生成或使用“推导指引（deduction guide）”来完成推导
//
// 示例：
//   std::pair p(1, 2.0);  // 推导为 std::pair<int, double>

#include <string>
#include <tuple>
#include <utility>

#include "fmt/core.h"

template <typename T>
struct Box
{
  explicit Box(T t) : value(std::move(t)) {}
  [[nodiscard]] std::string toString() const
  {
    return fmt::format("Box contains: {}", value);
  }

 private:
  T value;
};

// 推导指引
Box(const char *) -> Box<std::string>;

int main()
{
  fmt::print("Hello, CTAD!\n");

  // 在 C++17 之前：
  std::pair<int, int> p0(1, 2);  // 必须写类型

  // C++17 之后（CTAD）：
  std::pair p1(1, 2);  // 自动推导为 pair<int, int>, 背后靠的就是默认推导规则, 也就是构造函数参数的类型来推断模板参数

  // C++17 引入了类模板参数推导(Class Template Argument Deduction (CTAD)
  // 允许编译器根据构造函数的参数自动推断类模板的类型参数。
  // 这使得我们在创建对象时不需要显式指定模板参数，代码更加简洁和易读。
  std::pair p2{42, "C++"};  // 编译器自动推断为 std::pair<int, const char*>
  fmt::print("p1.first = {}, p1.second = {}\n", p1.first, p1.second);
  fmt::print("p2.first = {}, p2.second = {}\n", p2.first, p2.second);

  std::tuple t1{1, 3.14, "hello"};  // 编译器自动推断为 std::tuple<int, double, const char*>
  fmt::print("t1: {}, {}, {}\n", std::get<0>(t1), std::get<1>(t1), std::get<2>(t1));

  Box b1(10);      // 推导为 Box<int>
  Box b2(3.14);    // 推导为 Box<double>
  Box b("hello");  // Box<std::string>，而不是 Box<const char*>
  fmt::print("b1: {}\n", b1.toString());
  fmt::print("b2: {}\n", b2.toString());
  fmt::print("b: {}\n", b.toString());
}