#include "fmt/core.h"

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
}