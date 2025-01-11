#include <thread>
#include <memory>

#include <fmt/core.h>
#include "AbstractDrinking.h"
#include "Coffee.h"
#include "Tea.h"

void doWork(std::shared_ptr<AbstractDrinking> drink)
{
  drink->makeDrink();
}

void makeCoffee()
{
  fmt::println("======= 制作咖啡饮料:");
  std::shared_ptr<AbstractDrinking> coffee = std::make_shared<Coffee>();
  doWork(coffee);
}

void makeTea()
{
  fmt::println("======= 制作茶饮料:");
  auto tea = std::make_shared<Tea>();
  doWork(tea);
}

auto main(int argc, char **argv) -> int
{
  (void)argc;
  (void)argv;
  fmt::println("---------------------------------");
  makeCoffee();
  fmt::println("---------------------------------");
  makeTea();
  fmt::println("---------------------------------");
}