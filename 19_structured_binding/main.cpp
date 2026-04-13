/*
 * 结构化绑定（structured bindings）是 C++17 的语法
 * 可以把一个“结构”拆开成多个变量，非常适合从函数返回多个值、解构 STL 容器元素等场景。
 * 结构化绑定本身是零成本抽象
 * 但默认是“值语义”，你必须主动用 & 才能避免拷贝
 */

#include <iostream>
#include <map>
#include <string>

struct Point
{
  int x;
  int y;
};

Point getPoint()
{
  return {10, 20};
}
void test();
int main()
{
  std::map<std::string, int> myMap = {{"apple", 5}, {"banana", 3}};

  // map 返回值拆解
  auto [iter, inserted] = myMap.insert({"orange", 2});
  auto [iter2, inserted2] = myMap.insert_or_assign("apple", 10);  // 更新已存在的元素
  auto [key, value] = *iter;                                      // 结构化绑定解构迭代器指向的 pair
  std::cout << "Inserted: " << inserted << ", Key: " << key << ", Value: " << value << "\n";
  std::cout << "Inserted: " << inserted2 << ", Key: " << iter2->first << ", Value: " << iter2->second << "\n";

  // 使用结构化绑定访问 map 元素(遍历 map)
  for (const auto &[key, value] : myMap)
  {
    std::cout << key << ": " << value << "\n";
  }

  // 结构化绑定解构 tuple
  std::tuple<int, double, std::string> t{1, 3.14, "hi"};
  auto [i, d, s] = t;  // 默认是 copy 绑定，如果需要引用绑定，写成 auto& [i, d, s] = t;
  std::cout << "i = " << i << ", d = " << d << ", s = " << s << "\n";

  // 结构化绑定解构自定义 struct
  Point p{10, 20};
  auto [x, y] = p;  // 结构化绑定会自动推断成员变量的名称
  std::cout << "x = " << x << ", y = " << y << "\n";

  // 结构化绑定解构数组
  int arr[3] = {1, 2, 3};
  auto [a, b, c] = arr;  // 结构化绑定会自动推断数组元素的名称
  std::cout << "a = " << a << ", b = " << b << ", c = " << c << "\n";

  // 忽略某些元素
  auto [a1, a2, _] = arr;  // 使用 _ 来忽略不需要的元素(不过 C++ 没有真正 _ 忽略变量)
  std::cout << "a1 = " << a1 << ", a2 = " << a2 << "\n";

  int x1 = 0;
  double y1 = 0;
  std::tie(x1, y1, std::ignore) = t;
  std::cout << "x1 = " << x1 << ", y1 = " << y1 << "\n";

  test();
}

void test()
{
  auto [x, y] = getPoint();
  std::cout << "x = " << x << ", y = " << y << "\n";

  // 注意: 结构化绑定需要注意生命周期问题
  //   auto &[rx, ry] = getPoint();  // 错误: 不能绑定到临时对象的引用
  const auto &[rx, ry] = getPoint();  // 正确: 只要绑定的是 const 引用，就能延长整个对象的生命周期
  std::cout << "rx = " << rx << ", ry = " << ry << "\n";
}