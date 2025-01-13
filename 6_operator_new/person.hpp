#pragma once
#include <fmt/core.h>
#include <string>

class Person
{
private:
  int id_ = 0;

public:
  Person(int age);
  ~Person();
  std::string toString();

  /// class-specific allocation functions
  static void *operator new(std::size_t size);
  static void operator delete(void *ptr) noexcept;

  /// placement new
  static void *operator new(std::size_t size, void *ptr) noexcept; 
  // Matching placement delete, 必须存在, 编译器在异常情况下能够调用 operator delete 来正确处理资源
  static void operator delete(void* ptr, void *place) noexcept; 
};

Person::Person(int age) : id_(age)
{
  fmt::println("person constructor called.");
}

Person::~Person()
{
  fmt::println("person destructor called.");
}

inline std::string Person::toString()
{
  return "{age = " + std::to_string(id_) + "}";
}

/// @brief 具体类的operator new
/// @param size 
/// @return 
inline void *Person::operator new(std::size_t size)
{
  fmt::println("Person scope void *person::operator new(std::size_t size) called. size = {}", size);
  if(void *ptr = std::malloc(size))
  {
    return ptr;
  }
  throw std::bad_alloc{};
}

/// @brief  具体类的operator delete
/// @param ptr 
inline void Person::operator delete(void *ptr) noexcept
{
  fmt::println("Person scope void person::operator delete(void *ptr) called.");
  std::free(ptr);
}

/// @brief 具体类的placement new, 不需要申请内存
/// @param size 
/// @param ptr 
/// @return 
inline void *Person::operator new(std::size_t size, void *ptr) noexcept
{
  fmt::println("Person scope placement new called. size = {}, ptr = {}", size, ptr);
  return ptr;
}

/// @brief 重载了 placement new 时，也需要重载对应的 placement delete
/// @param ptr 
/// @param place 
inline void Person::operator delete(void *ptr, void *place) noexcept
{
  fmt::println("Person scope Matching placement delete called. size = {}, ptr = {}", ptr, place);
  // 不需要释放内存，因为由用户管理传入的 place
}
