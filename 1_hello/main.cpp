#include <iostream>
#include <fmt/core.h>

/// @brief 主函数，程序的入口点
/// @param argc 命令行参数的数量
/// @param argv 命令行参数的数组
/// @return 返回 0 表示程序成功执行
int main(int argc, char **argv)
{
  std::cout << "Hello World! I am CMake." << std::endl;
  fmt::println("Hello World! I am from fmt library.");
  return 0;
}
