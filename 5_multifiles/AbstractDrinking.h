#pragma once

/// @brief 饮料抽象类
class AbstractDrinking
{
public:
  // 定义接口
  // 烧水
  virtual void Boil() = 0;
  // 冲泡
  virtual void Brew() = 0;
  // 倒入杯中
  virtual void PourInCup() = 0;
  // 加入辅料
  virtual void PutSomething() = 0;
  // 制作流程
  void makeDrink();

  AbstractDrinking();
  // 抽象类需要虚析构函数
  virtual ~AbstractDrinking();
};