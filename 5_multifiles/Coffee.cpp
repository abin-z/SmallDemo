#include "Coffee.h"

#include <fmt/core.h>

void Coffee::Boil()
{
  fmt::println("1.煮农夫山泉水");
}
void Coffee::Brew()
{
  fmt::println("2.冲泡咖啡");
}

void Coffee::PourInCup()
{
  fmt::println("3.倒入咖啡杯中");
}

void Coffee::PutSomething()
{
  fmt::println("4.添加猫屎");
}

Coffee::Coffee()
{
  fmt::println("Coffee 构造函数调用");
}

Coffee::~Coffee()
{
  fmt::println("Coffee 析构函数调用");
}
