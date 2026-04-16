/*
 * if constexpr 是 编译期条件分支，属于一种基于常量表达式的条件判断。
 * 基本形式如下：
 *  if constexpr (condition) {
 *      // 编译期为 true 才会被编译
 *  } else {
 *      // 编译期为 false 才会被编译
 *  }
 * 关键点：未被选中的分支在编译阶段直接丢弃（不会参与编译）
 */

#include <iostream>
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

int main()
{
  print(42);       // 输出: integer: 42
  print(3.14);     // 输出: other: 3.14
  print("hello");  // 输出: other: hello

  // if constexpr 的一个重要应用是实现编译期多态（如模板元编程）
  // 可以根据类型特征选择不同的实现路径，避免运行时开销。
  return 0;
}