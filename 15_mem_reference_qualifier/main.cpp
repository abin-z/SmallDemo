#include <fmt/core.h>
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
    static_assert(
        std::is_same<T, int>::value ||
            std::is_same<T, double>::value ||
            std::is_same<T, float>::value ||
            std::is_same<T, long>::value ||
            std::is_same<T, long long>::value ||
            std::is_same<T, long double>::value,
        "Only int, double, float, long, long long, long double, are allowed for conversion.");
    return static_cast<T>(data);
  }

  // 模板特化为 char 类型
  operator char() const
  {
    return static_cast<char>(data);
  }

  // 模板特化为 short 类型
  operator short() const
  {
    return static_cast<short>(data);
  }
};

int main()
{
  fmt::println("========== member reference qualifier ==========");
  Object obj;        // 左值对象
  const Object cobj; // const 左值对象
  obj.func();
  cobj.func();

  obj.func02();
  cobj.func02();

  Object{}.func02();                            // 右值对象
  static_cast<const Object>(Object{}).func02(); // const 右值对象
  getCObj().func02();                           // 右值对象

  Number num;
  num.data = 99;
  int n = num; // 隐式转换
  double d = num;
  long double ld = num;
  long long ll = num;
  char c = num;

  return 0;
}