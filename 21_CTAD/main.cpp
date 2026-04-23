#include "fmt/core.h"

int main()
{
  fmt::print("Hello, CTAD!\n");

  // C++17 引入了类模板参数推导(Class Template Argument Deduction,
  // CTAD)，允许编译器根据构造函数的参数自动推断类模板的类型参数。
  // 这使得我们在创建对象时不需要显式指定模板参数，代码更加简洁和易读。
  std::pair p1{42, "C++"};  // 编译器自动推断为 std::pair<int, const char*>
  fmt::print("p1.first = {}, p1.second = {}\n", p1.first, p1.second);

  std::tuple t1{1, 3.14, "hello"};  // 编译器自动推断为 std::tuple<int, double, const char*>
  fmt::print("t1: {}, {}, {}\n", std::get<0>(t1), std::get<1>(t1), std::get<2>(t1));

  // 对于自定义类，也可以通过定义合适的构造函数来启用 CTAD
}