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