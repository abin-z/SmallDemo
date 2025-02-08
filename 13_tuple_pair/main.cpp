#include <utility>
#include <string>
#include <fmt/core.h>

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


int main()
{
  testPair();
  testTuple();
  testTie();
  return 0;
}