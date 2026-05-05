/*
 * if constexpr 是 编译期条件分支，属于一种基于常量表达式的条件判断。
 * 基本形式如下：
 *  if constexpr (condition) {
 *      // 编译期为 true 才会被编译
 *  } else {
 *      // 编译期为 false 才会被编译
 *  }
 * 关键点：未被选中的分支在编译阶段直接丢弃（不会参与编译）
 *
 * 二、和普通 if 的区别（关键点）
 *
 *  普通 if：
 *    if (std::is_integral_v<T>) {
 *        // ❗即使条件为 false，这段代码仍然必须是合法的
 *    }
 *
 *  if constexpr：
 *    if constexpr (std::is_integral_v<T>) {
 *        // ✅ 只有条件为 true 才会编译
 *    } else {
 *        // ✅ false 分支才会编译
 *    }
 *
 *  👉 也就是说：
 *  错误代码可以“藏”在不会被选中的分支里，编译器不会报错。
 */

#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
void print(T value)
{
  if constexpr (std::is_integral_v<T>)
  {
    std::cout << "integer: " << value << "\n";
  }
  else
  {
    std::cout << "other: " << value << "\n";
  }
}

template <typename T>
void process(T t)
{
  if constexpr (std::is_pointer_v<T>)
  {
    std::cout << *t << "\n";
  }
  else
  {
    std::cout << t << "\n";
  }
}

template <typename T>
std::string to_string(T value)
{
  if constexpr (std::is_same_v<T, std::string>)
  {
    return value;  // 特化 std::string 类型，直接返回
  }
  else
  {
    return std::to_string(value);  // 通用实现，调用 std::to_string
  }
}

int main()
{
  print(42);       // 输出: integer: 42
  print(3.14);     // 输出: other: 3.14
  print("hello");  // 输出: other: hello

  // if constexpr 的一个重要应用是实现编译期多态（如模板元编程）
  // 可以根据类型特征选择不同的实现路径，避免运行时开销。

  int x = 10;
  process(x);  // 输出: 10
  int *p = &x;
  process(p);                                         // 输出: 10
  std::cout << to_string(123) << "\n";                // 输出: "123"
  std::cout << to_string(std::string("hi")) << "\n";  // 输出: "hi"

  return 0;
}