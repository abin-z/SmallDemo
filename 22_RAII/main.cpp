#include <iostream>
#include <thread>

// 全局对象
class GlobalObject
{
 public:
  GlobalObject()
  {
    std::cout << "GlobalObject constructed" << std::endl;
  }
  ~GlobalObject()
  {
    std::cout << "GlobalObject destructed" << std::endl;
  }
};
GlobalObject globalObj;

// 静态全局对象
class StaticGlobalObject
{
 public:
  StaticGlobalObject()
  {
    std::cout << "StaticGlobalObject constructed" << std::endl;
  }
  ~StaticGlobalObject()
  {
    std::cout << "StaticGlobalObject destructed" << std::endl;
  }
};
static StaticGlobalObject staticGlobalObj;

class MemberObject
{
 public:
  MemberObject()
  {
    std::cout << "MemberObject constructed" << std::endl;
  }
  ~MemberObject()
  {
    std::cout << "MemberObject destructed" << std::endl;
  }
};

class OuterClass
{
 public:
  OuterClass()
  {
    std::cout << "OuterClass constructed" << std::endl;
  }
  ~OuterClass()
  {
    std::cout << "OuterClass destructed" << std::endl;
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
      std::cout << "StaticLocalObject constructed" << std::endl;
    }
    ~StaticLocalObject()
    {
      std::cout << "StaticLocalObject destructed" << std::endl;
    }
  } staticLocalObj;

  // 栈对象
  class StackObject
  {
   public:
    StackObject()
    {
      std::cout << "StackObject constructed" << std::endl;
    }
    ~StackObject()
    {
      std::cout << "StackObject destructed" << std::endl;
    }
  } stackObj;

  thread_local class ThreadLocalObject
  {
   public:
    ThreadLocalObject()
    {
      std::cout << "ThreadLocalObject constructed in thread " << std::this_thread::get_id() << std::endl;
    }
    ~ThreadLocalObject()
    {
      std::cout << "ThreadLocalObject destructed in thread " << std::this_thread::get_id() << std::endl;
    }
  } threadLocalObj;

  OuterClass outer;
}

int main()
{
  std::cout << "Main function started" << std::endl;
  func();
  std::cout << "Main function ended" << std::endl;
  return 0;
}