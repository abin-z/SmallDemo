#include "AbstractDrinking.h"

#include <fmt/core.h>

void AbstractDrinking::makeDrink()
{
  Boil();
  Brew();
  PourInCup();
  PutSomething();
}

AbstractDrinking::AbstractDrinking()
{
  fmt::println("AbstractDrinking constructor called");
}

AbstractDrinking::~AbstractDrinking()
{
  fmt::println("AbstractDrinking destructor called");
}
