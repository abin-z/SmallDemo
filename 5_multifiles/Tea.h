#pragma once
#include "AbstractDrinking.h"

/// @brief 茶饮品
class Tea : public AbstractDrinking
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
  Tea();
  ~Tea();
};