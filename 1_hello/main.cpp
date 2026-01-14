#include <fmt/core.h>

#include <iostream>

// 普通函数
double sum(double x, double y)
{
  return x + y;
}

// 仿函数
struct Adder
{
  int operator()(int x, int y)
  {
    return x + y;
  }
};

struct Object
{
  int value_ = 10;
  int value() const
  {
    return value_;
  }
  void set_value(int value)
  {
    value_ = value;
  }
  static void printNum(int value)
  {
    std::cout << "Object::printNum value = " << value << std::endl;
  }
};

void test();
void test02();
void libinfo();

/// @brief 主函数，程序的入口点
/// @param argc 命令行参数的数量
/// @param argv 命令行参数的数组
/// @return 返回 0 表示程序成功执行
int main(int argc, char **argv)
{
  std::cout << "Hello World! I am CMake." << std::endl;
  fmt::println("Hello World! I am from fmt library.");
  test();
  test02();
  libinfo();

  return 0;
}

void libinfo()
{
#ifdef _LIBCPP_VERSION
  std::cout << "C++ Standard Library: libc++  "
            << "Version: " << _LIBCPP_VERSION << "\n";
#else
  std::cout << "C++ Standard Library: Not libc++\n";
#endif

#ifdef __GLIBCXX__
  std::cout << "C++ Standard Library: libstdc++  "
            << "Version: " << __GLIBCXX__ << "\n";
#else
  std::cout << "C++ Standard Library: Not libstdc++\n";
#endif
}

void test()
{
  // 函数指针类型
  using funcptr_t = double (*)(double, double);
  typedef double (*func2ptr_t)(double, double);
  // 通过类型定义函数指针
  funcptr_t sumptr1 = sum;
  func2ptr_t sumptr2 = sum;
  sumptr1(1.1, 2.2);
  sumptr2(1.1, 2.2);

  // 直接定义函数指针
  double (*sum_funcptr)(double, double) = sum;
  sum_funcptr(1.1, 2.2);
}

void test02()
{
  // 类的静态成员函数和普通函数类似
  using fun_t = void (*)(int);
  fun_t funcptr = &Object::printNum;  // 静态成员函数, 可以直接调用
  funcptr(333);

  Object obj;
  // 成员函数指针定义
  void (Object::*mem_funcptr)(int) = &Object::set_value;
  // 成员函数指针调用, 必须要一个obj对象
  (obj.*mem_funcptr)(666);

  // const 成员函数定义
  int (Object::*mem_getfuncptr)() const = &Object::value;
  int val = (obj.*mem_getfuncptr)();
  std::cout << "val = " << val << std::endl;

  // 定义成员函数指针类型
  using mem_funcptr_t = void (Object::*)(int);
  using mem_funcptr2_t = int (Object::*)() const;
  // 定义具体的指针
  mem_funcptr_t memfptr1 = &Object::set_value;
  mem_funcptr2_t memfptr2 = &Object::value;
  (obj.*memfptr1)(888);  // 调用函数, 必须使用obj.*, 而且需要()括起来
  val = (obj.*memfptr2)();
  std::cout << "val = " << val << std::endl;
}