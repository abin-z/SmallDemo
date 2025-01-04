#include <iostream>
#include <vector>

int main()
{
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 999};
  std::cout << "vector<int> v = ";
  for (const auto &item : v)
  {
    std::cout << item << " ";
  }
  std::cout << std::endl;
  return 0;
}
