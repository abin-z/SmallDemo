#include <basetsd.h>
#include <fmt/core.h>

#include <cstdint>
#include <type_traits>

/**
 * 引用限定符 是 C++ 中一个很特别的功能，它用于限定成员函数可以作用的对象类型（左值或右值）。
 * 通过引用限定符，你可以根据对象的值类别（左值或右值）来重载成员函数。
 * 引用限定符主要包括 &（左值引用）和 &&（右值引用），并可以与 const 一起使用来进一步限定函数的行为。
 */

class Object
{
 public:
  void func()
  {
    fmt::println("Calling func()");
  }

  void func() const
  {
    fmt::println("Calling func() const");
  }

  // 只能作用于左值对象
  void func02() &
  {
    fmt::println("Calling func() & on lvalue: {}", num);
  }

  // 只能作用于右值对象
  void func02() &&
  {
    fmt::println("Calling func() && on rvalue: {}", num);
  }

  // 只能作用于常量左值对象
  void func02() const &
  {
    fmt::println("Calling func() const& on const lvalue: {}", num);
  }

  // 只能作用于常量右值对象
  void func02() const &&
  {
    fmt::println("Calling func() const&& on const rvalue: {}", num);
  }

 private:
  int num = 0;
};

const Object getCObj()
{
  return Object();
}

struct Number
{
  int data = 0;

  // 类型转换操作符
  operator int() const
  {
    return data;
  }

  // 通用类型转换操作符，只允许转换为特定的类型（int、double、float、long、long long、long double）
  template <typename T>
  operator T() const
  {
    // clang-format off
    static_assert(std::is_same_v<T, int> || 
                    std::is_same_v<T, double> || 
                    std::is_same_v<T, float> ||
                    std::is_same_v<T, int16_t> || 
                    std::is_same_v<T, int64_t> || 
                    std::is_same_v<T, long double>,
                  "Only int, double, float, long, long long, long double, are allowed for conversion.");
    // clang-format on
    return static_cast<T>(data);
  }

  // 类型转换操作符
  operator char() const
  {
    return static_cast<char>(data);
  }
};

/*
C++ 规定模板的显式特化应该在 命名空间作用域 中进行，而不是类的内部。
*/

// 普通类的成员模版函数全特化-只能在类外特化
template <>
Number::operator int16_t() const
{
  return static_cast<int16_t>(data);
}

class Printer
{
 public:
  // 成员函数模板
  template <typename T>
  void print(T value)
  {
    fmt::println("Generic print: {}", value);
  }
#ifdef _MSC_VER  // MSVC 编译器代码
  // 模版函数特化 - 建议在外部特化, 内部支持的编译器较少, 不标准的做法
  template <>
  void print<double>(double value)
  {
    fmt::println("Specialized print for double: {}", value);
  }
  // 模版函数特化
  template <>
  void print(int64_t value)
  {
    fmt::println("Specialized print for long long: {}", value);
  }
#endif
  // 函数重载, 优先级高于特化版本
  void print(int32_t value)
  {
    fmt::println("print for long : {}", value);
  }
};

// 类外进行全特化-推荐
template <>
void Printer::print<int>(int value)
{
  fmt::println("Specialized print for int: {}", value);
}

int main()
{
  fmt::println("========== member reference qualifier ==========");
  Object obj;         // 左值对象
  const Object cobj;  // const 左值对象
  obj.func();
  cobj.func();

  obj.func02();
  cobj.func02();

  Object{}.func02();                             // 右值对象
  static_cast<const Object>(Object{}).func02();  // const 右值对象
  getCObj().func02();                            // 右值对象

  fmt::println("========== type conversion operators ==========");
  Number num;
  num.data = 99;
  int n = num;  // 隐式转换
  double d = num;
  long double ld = num;
  int64_t ll = num;
  char c = num;
  int16_t s = num;
  fmt::println("short s = {}", s);

  fmt::println("========== template full specialization ==========");
  Printer printer;
  printer.print(1);
  printer.print(3.14);
  printer.print("hello");
  printer.print(999999999LL);
  printer.print(999999L);
  return 0;
}