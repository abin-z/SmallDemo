#include <utility>
#include <string>
#include <typeinfo>
#include <fmt/core.h>

/**
 * C++ 中，std::tuple、std::pair 和 std::tie 是用来处理多种数据类型组合的工具。它们允许你将多个值捆绑在一起并传递或返回。
 *
 * std::pair：存储两个相关的值，支持不同类型。常用于关联数据结构，例如字典或映射。
 * std::tuple：可以存储任意数量和类型的值。适用于返回多个不同类型的值或捆绑多个值。
 * std::tie：解构元组或 pair，将元组中的元素绑定到变量上。非常适用于多返回值的场景。
 * 这些工具在 C++ 中提供了灵活的数据组织和返回机制，尤其在处理函数返回多个值、存储复合数据时非常有用。
 *
 */

void testPair()
{
  std::pair<int, std::string> pair = std::make_pair(3, "hello world!");
  fmt::print("pair.first = {}, pair.second = {}\n", pair.first, pair.second);
}

void testTuple()
{
  std::tuple<int, std::string, double, float> tp = {22, "xiaomi", 3.14, 2.1};
  fmt::print("tuple get<0> = {}\n", std::get<0>(tp));
  fmt::print("tuple get<1> = {}\n", std::get<1>(tp));
  fmt::print("tuple get<2> = {}\n", std::get<2>(tp));
  fmt::print("tuple get<3> = {}\n", std::get<3>(tp));
}

std::tuple<int, std::string> getData()
{
  return std::make_tuple(99, "hello tuple");
}

void testTie()
{
  std::tuple<int, std::string, double, float> tp = {88, "google", 33.33, 44.44};
  int a = 0;
  std::string str;
  double n = 0;
  float m = 0;
  std::tie(a, str, n, m) = tp;
  fmt::print("tuple a = {}\n", a);
  fmt::print("tuple str = {}\n", str);
  fmt::print("tuple n = {}\n", n);
  fmt::print("tuple m = {}\n", m);

  std::tie(a, str) = getData();
  fmt::print("tuple a = {}\n", a);
  fmt::print("tuple str = {}\n", str);
}

struct point
{
  double x = 0;
  double y = 0;
  double z = 0;
};

void testTuple02()
{
  using funcType = std::tuple<int, std::string> (*)(); // 函数指针类型
  std::tuple<int, std::string, double, point, int64_t, funcType> tp{32, "Apple", 4.5, {1, 2, 3}, 99999, getData};
  // std::tuple_size 用于获取 std::tuple 类型中元素的数量。它是一个类型特征，返回一个表示元组元素数量的常量。
  size_t size = std::tuple_size<decltype(tp)>::value;
  fmt::print("std::tuple_size<decltype(tp)>::value = {}\n", size);
  // std::tuple_element 用于获取 std::tuple 中指定索引位置的元素类型。它是一个类型特征，返回元组中指定位置的元素类型。
  fmt::println("std::tuple_element<0, decltype(tp)>::type).name() = {}", typeid(std::tuple_element<0, decltype(tp)>::type).name());
  fmt::println("std::tuple_element<1, decltype(tp)>::type).name() = {}", typeid(std::tuple_element<1, decltype(tp)>::type).name());
  fmt::println("std::tuple_element<2, decltype(tp)>::type).name() = {}", typeid(std::tuple_element<2, decltype(tp)>::type).name());
  fmt::println("std::tuple_element<3, decltype(tp)>::type).name() = {}", typeid(std::tuple_element<3, decltype(tp)>::type).name());
  fmt::println("std::tuple_element<4, decltype(tp)>::type).name() = {}", typeid(std::tuple_element<4, decltype(tp)>::type).name());
  fmt::println("std::tuple_element<5, decltype(tp)>::type).name() = {}", typeid(std::tuple_element<5, decltype(tp)>::type).name());
}

int main()
{
  testPair();
  testTuple();
  testTie();
  testTuple02();
  return 0;
}