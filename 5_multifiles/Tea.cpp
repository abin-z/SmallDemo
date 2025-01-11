#include "Tea.h"

#include <fmt/core.h>

void Tea::Boil()
{
  fmt::println("1.煮开水");
}
void Tea::Brew()
{
  fmt::println("2.冲泡茶叶");
}

void Tea::PourInCup()
{
  fmt::println("3.倒入保温杯中");
}

void Tea::PutSomething()
{
  fmt::println("4.添加枸杞");
}

Tea::Tea()
{
  fmt::println("Tea 构造函数调用");
}

Tea::~Tea()
{
  fmt::println("Tea 析构函数调用");
}
