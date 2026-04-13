#include <iostream>
#include <map>

int main()
{
  std::map<int, std::string> myMap{{1, "one"}, {2, "two"}, {3, "three"}};
  myMap[4] = "four";  // 直接使用下标, 如果元素不存在, 会先创建一个默认值, 然后再赋值
  auto [iter1, inserted1] = myMap.insert_or_assign(5, "five");   // 插入新元素, 如果key不存在, 直接插入不构造默认值
  auto [iter2, inserted2] = myMap.insert_or_assign(5, "five2");  // 插入新元素
  std::cout << "Inserted: " << inserted1 << ", Key: " << iter1->first << ", Value: " << iter1->second << '\n';
  std::cout << "Inserted: " << inserted2 << ", Key: " << iter2->first << ", Value: " << iter2->second << '\n';

  if (auto [iter1, inserted1] = myMap.insert_or_assign(26, "twenty-six"); inserted1)
    std::cout << iter1->first << " inserted...\n ";

  // 常规的迭代器访问
  for (const auto &pair : myMap)
  {
    std::cout << "Key: " << pair.first << ", Value: " << pair.second << '\n';
  }

  // 使用C++17的结构化绑定访问
  for (const auto &[key, value] : myMap)
  {
    std::cout << "Key: " << key << ", Value: " << value << '\n';
  }

  std::cout << "Map Demo\n";
  return 0;
}