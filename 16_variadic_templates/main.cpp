#include <iostream>
#include <vector>
#include <tuple>

/// @brief
/// @tparam ...Args 类型参数包，可以接收 0 个或多个类型
/// @param ...args  函数参数包，用于存储实际传入的参数
/// @return
template <typename... Args>
size_t getsize(Args... args)
{
  return sizeof...(args); // 获取参数包的个数, 和 sizeof 没有关系
}

template <typename... Args>
std::tuple<Args...> makeTuple(Args... args)
{
  return std::tuple<Args...>(args...);
}

/**************************** 使用递归展开参数包 ********************************/
// 终止递归的基函数(方式1)
// void print()
// {
//   std::cout << std::endl;
// }

// 终止递归的基函数(方式2)
template <typename T>
void print(const T &value)
{
  std::cout << value << std::endl;
}

// 递归展开参数包
template <typename T, typename... Args>
void print(const T &value, Args... args)
{
  std::cout << "[" << getsize(args...) << "]" << value << ", "; // 处理当前参数
  print(args...);                                               // 递归调用，处理剩余参数
}

/**************************** 使用非递归展开参数包 ********************************/
#if __cplusplus >= 201703L
template <typename... Args>
void print4(Args... args)
{
  (std::cout << ... << args) << std::endl; // C++17的折叠表达式
}

template <typename... Args>
void print5(Args... args)
{
  ((std::cout << args << " "), ...); // C++17的折叠表达式
  std::cout << std::endl;
}
#endif

// 普通的模板函数
template <typename T>
void func(const T &value)
{
  std::cout << value << " ";
}

template <typename... Args>
void print1(Args... args)
{
  (void)std::initializer_list<int>{(func(args), 0)...}; // 利用 std::initializer_list ，即初始化列表展开可变参数
  std::cout << std::endl;
}

template <typename... Args>
void print2(Args... args)
{
  (void)std::initializer_list<char>{([&]
                                     { std::cout << args << " "; }(), '\0')...}; // std::initializer_list + lambda表达式
  std::cout << std::endl;
}

/*
  假设 print3(1, 2, 3);，参数 args... 展开为 1, 2, 3，则：
  std::initializer_list<char>{
      (std::cout << 1 << " ", '\0'),
      (std::cout << 2 << " ", '\0'),
      (std::cout << 3 << " ", '\0')
  };
*/
template <typename... Args>
void print3(Args... args)
{
  (void)std::initializer_list<char>{(std::cout << args << " ", '\0')...}; // 使用 std::initializer_list + 逗号运算符
  std::cout << std::endl;
}

int main()
{
  std::cout << "__cplusplus: " << __cplusplus << std::endl;
  std::cout << "Variadic Template Parameter Pack size = " << getsize() << std::endl;
  std::cout << "Variadic Template Parameter Pack size = " << getsize(true) << std::endl;
  std::cout << "Variadic Template Parameter Pack size = " << getsize(true, false) << std::endl;
  std::cout << "Variadic Template Parameter Pack size = " << getsize(true, false, 3.14) << std::endl;
  std::cout << "Variadic Template Parameter Pack size = " << getsize(true, false, 3.14, 200) << std::endl;
  std::cout << "Variadic Template Parameter Pack size = " << getsize(true, false, 3.14, 200, 'c') << std::endl;
  std::cout << "Variadic Template Parameter Pack size = " << getsize(true, false, 3.14, 200, 'c', "string") << std::endl;

  print(true, false, 3.14, 200, 'c', "string");
  print1(true, false, 3.14, 200, 'c', "string");
  print2(true, false, 3.14, 200, 'c', "string");
  print3(true, false, 3.14, 200, 'c', "string");
#if __cplusplus >= 201703L
  print4(true, false, 3.14, 200, 'c', "string");
  print5(true, false, 3.14, 200, 'c', "string");
#endif

  auto t = makeTuple(1, 2.5, "Hello");
  std::cout << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << std::endl;
}