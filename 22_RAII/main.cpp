/*
 * RAII 生效的底层逻辑
 * RAII 的核心思路，是将资源的申请、持有和释放，完全托付给 C++ 对象的生命周期来管理 ，形成一种紧密的 “共生” 关系。
 * 具体来说，当一个对象被构造时，在构造函数中完成资源的获取操作，就如同在一个项目启动时筹备所需物资；
 * 而当对象的使命结束，即将离开其作用域时，析构函数会被自动触发，完成资源的释放，恰似项目结束后清理场地、归还物资。
 * 换句话说：
 *      构造函数 → 获取资源
 *      析构函数 → 释放资源
 * 资源包括但不限于：
 *      内存（new / malloc）
 *      文件句柄（FILE* / fd）
 *      互斥锁（mutex）
 *      网络连接
 *      数据库连接
 *      GPU / 硬件资源
 *
 * RAII的重要前提: 析构函数必须保证不抛异常
 *  有一个重要的前提条件常常被人忽视，那就是析构函数必须保证不抛出异常，这是确保 RAII 完整有效的必要条件。
 *  在 C++ 的异常处理机制中，当异常被抛出后，程序会进入栈展开（Stack
 * Unwinding）过程，即沿着调用栈逆序销毁局部对象，调用它们的析构函数。
 *  如果此时某个析构函数也抛出异常，就会导致程序直接调用 terminate 函数，强制终止运行。
 *  这就好比在一场有序撤离的行动中，突然出现混乱（析构函数抛异常），整个撤离行动（程序运行）就会被迫中断。
 */

#include <iostream>
#include <thread>

// 全局对象
class GlobalObject
{
 public:
  GlobalObject()
  {
    std::cout << "GlobalObject constructed\n";
  }
  ~GlobalObject()
  {
    std::cout << "GlobalObject destructed\n";
  }
};
GlobalObject globalObj;

// 静态全局对象
class StaticGlobalObject
{
 public:
  StaticGlobalObject()
  {
    std::cout << "StaticGlobalObject constructed\n";
  }
  ~StaticGlobalObject()
  {
    std::cout << "StaticGlobalObject destructed\n";
  }
};
static StaticGlobalObject staticGlobalObj;

class MemberObject
{
 public:
  MemberObject()
  {
    std::cout << "MemberObject constructed\n";
  }
  ~MemberObject()
  {
    std::cout << "MemberObject destructed\n";
  }
};

class OuterClass
{
 public:
  OuterClass()
  {
    std::cout << "OuterClass constructed\n";
  }
  ~OuterClass()
  {
    std::cout << "OuterClass destructed\n";
  }

 private:
  MemberObject member;
};

void func()
{
  // 静态局部对象
  static class StaticLocalObject
  {
   public:
    StaticLocalObject()
    {
      std::cout << "StaticLocalObject constructed\n";
    }
    ~StaticLocalObject()
    {
      std::cout << "StaticLocalObject destructed\n";
    }
  } staticLocalObj;

  // 栈对象
  class StackObject
  {
   public:
    StackObject()
    {
      std::cout << "StackObject constructed\n";
    }
    ~StackObject()
    {
      std::cout << "StackObject destructed\n";
    }
  } stackObj;

  thread_local class ThreadLocalObject
  {
   public:
    ThreadLocalObject()
    {
      std::cout << "ThreadLocalObject constructed in thread " << std::this_thread::get_id() << '\n';
    }
    ~ThreadLocalObject()
    {
      std::cout << "ThreadLocalObject destructed in thread " << std::this_thread::get_id() << '\n';
    }
  } threadLocalObj;

  OuterClass outer;
}

int main()
{
  std::cout << "Main function started\n";
  func();
  std::cout << "Main function ended\n";
  return 0;
}