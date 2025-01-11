#pragma once
#include "AbstractDrinking.h"

/// @brief 咖啡饮品
class Coffee : public AbstractDrinking
{
private:
  // 烧水
  void Boil() override;
  // 冲泡
  void Brew() override;
  // 倒入杯中
  void PourInCup() override;
  // 加入辅料
  void PutSomething() override;

public:
  Coffee();
  ~Coffee();
};