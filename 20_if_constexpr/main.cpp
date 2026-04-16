/*
 * 结构化绑定（structured bindings）是 C++17 的语法
 * 可以把一个“结构”拆开成多个变量，非常适合从函数返回多个值、解构 STL 容器元素等场景。
 * 结构化绑定本身是零成本抽象
 * 但默认是“值语义”，你必须主动用 & 才能避免拷贝
 */

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>

struct Point
{
  int x;
  int y;
};

Point getPoint()
{
  return {10, 20};
}

class MyClass;
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

// 为自定义类提供结构化绑定接口, 实现方式: 本质就是让类“看起来像 tuple”
class MyClass
{
 public:
  MyClass(int a, std::string n, std::string addr) : age(a), name(std::move(n)), address(std::move(addr)) {}

  void set_age(int a)
  {
    age = a;
  }
  void set_name(const std::string &n)
  {
    name = n;
  }
  void set_address(const std::string &addr)
  {
    address = addr;
  }

  [[nodiscard]] int get_age() const
  {
    return age;
  }
  [[nodiscard]] const std::string &get_name() const
  {
    return name;
  }
  [[nodiscard]] const std::string &get_address() const
  {
    return address;
  }

  // 方式2: 提供结构化绑定接口, 但是需要提供一个 asTuple() 方法来返回一个 tuple 或 pair
  [[nodiscard]] auto asTuple() const
  {
    return std::tie(age, name);  // 只暴露 age 和 name，不暴露 address
  }

 private:
  int age{0};
  std::string name;
  std::string address;  // 不公开
};

// 步骤 1：定义元素个数
namespace std
{
template <>
struct tuple_size<MyClass> : std::integral_constant<size_t, 2>
{
};  // 只暴露 age 和 name，不暴露 address
}  // namespace std

// 步骤 2：定义元素类型
namespace std
{
template <>
struct tuple_element<0, MyClass>
{
  using type = int;
};

template <>
struct tuple_element<1, MyClass>
{
  using type = std::string;
};
}  // namespace std

// 步骤 3：提供 get<N>()（最关键）更专业一点（支持引用 & 修改）
template <size_t N>
auto get(const MyClass &obj)
{
  if constexpr (N == 0)
    return obj.get_age();
  else if constexpr (N == 1)
    return obj.get_name();
}

// 或者直接特化get<N>()函数, 数量少的时候比if constexpr更简单
template <>
auto get<0>(const MyClass &obj)
{
  return obj.get_age();
}
template <>
auto get<1>(const MyClass &obj)
{
  return obj.get_name();
}

void test()
{
  auto [x, y] = getPoint();
  std::cout << "x = " << x << ", y = " << y << "\n";

  // 注意: 结构化绑定需要注意生命周期问题
  //   auto &[rx, ry] = getPoint();  // 错误: 不能绑定到临时对象的引用
  const auto &[rx, ry] = getPoint();  // 正确: 只要绑定的是 const 引用，就能延长整个对象的生命周期
  std::cout << "rx = " << rx << ", ry = " << ry << "\n";

  MyClass obj(30, "Alice", "123 Main St");
  auto [age, name] = obj;  // 结构化绑定会调用 get<N>() 来获取成员变量的值
  std::cout << "age = " << age << ", name = " << name << "\n";
}