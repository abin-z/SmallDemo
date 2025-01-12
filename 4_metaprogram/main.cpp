#include <iostream>

// 模版元编程 - 枚举的方式, 注意enum的默认类型是int, 需注意类型不匹配或溢出问题
template <long long n>
struct Factorial
{
  enum : long long
  {
    value = n * Factorial<n - 1>::value
  };
};

template <>
struct Factorial<0>
{
  enum : long long
  {
    value = 1
  };
};

// 模板元编程 - 静态成员变量
template <long long n>
struct Factor
{
  static constexpr long long value = n * Factor<n - 1>::value;
};

template <>
struct Factor<0> // 终止条件
{
  static constexpr long long value = 1;
};

int main(const int argc, const char *argv[])
{
  (void)argc;
  (void)argv;
  std::cout << "hello world" << std::endl;
  std::cout << "Factorial<5>::value  = " << Factorial<5>::value << std::endl;
  std::cout << "Factorial<10>::value = " << Factorial<10>::value << std::endl;
  std::cout << "Factorial<12>::value = " << Factorial<12>::value << std::endl;
  std::cout << "Factorial<15>::value = " << Factorial<15>::value << std::endl;
  std::cout << "Factorial<20>::value = " << Factorial<20>::value << std::endl;

  std::cout << "Factor<10>::value = " << Factor<10>::value << std::endl;
  std::cout << "Factor<20>::value = " << Factor<20>::value << std::endl;
}
