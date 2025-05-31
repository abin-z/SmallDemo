#include <fmt/core.h>

#include <cstddef>

class Object
{
 public:
  Object()
  {
    fmt::println("Object constructor called.");
  }

  ~Object()
  {
    fmt::println("Object destructor called.");
  }
  // Object-specific allocation functions
  static void *operator new(std::size_t size)
  {
    fmt::println("Object scope void *operator new(std::size_t size) called. size = {}", size);
    return ::operator new(size);
  }
  // Object-specific deallocation function
  static void operator delete(void *ptr) noexcept
  {
    fmt::println("Object scope void operator delete(void *ptr) called.");
    return ::operator delete(ptr);
  }
  // Placement new and delete
  static void *operator new(std::size_t size, void *ptr) noexcept
  {
    fmt::println("Object scope void *operator new(std::size_t size, void *ptr) called. size = {}", size);
    return ptr;
  }
  // Matching placement delete, must exist so the compiler can call operator delete in case of exceptions
  static void operator delete(void *ptr, void *place) noexcept
  {
    fmt::println("Object scope void operator delete(void *ptr, void *place) called.");
    // No action needed, as placement new does not allocate memory.
  }

  // Overloaded new and delete for arrays
  void *operator new[](std::size_t size)
  {
    fmt::println("Object scope void *operator new[](std::size_t size) called. size = {}", size);
    return ::operator new(size);
  }
  // Overloaded delete for arrays
  void operator delete[](void *ptr) noexcept
  {
    fmt::println("Object scope void operator delete[](void *ptr) called.");
    return ::operator delete(ptr);
  }

  // Placement new and delete for arrays
  void *operator new[](std::size_t size, void *ptr) noexcept
  {
    fmt::println("Object scope void *operator new[](std::size_t size, void *ptr) called. size = {}", size);
    return ptr;
  }
  // Matching placement delete for arrays, must exist so the compiler can call operator delete in case of exceptions
  void operator delete[](void *ptr, void *place) noexcept
  {
    fmt::println("Object scope void operator delete[](void *ptr, void *place) called.");
    // No action needed, as placement new does not allocate memory.
  }

 private:
  int id_ = 0;
};

int main()
{
  fmt::println("==================================================");
  auto *obj1 = new Object();  // Calls Object's operator new
  fmt::println("obj1 address = {:p}", static_cast<void *>(obj1));
  delete obj1;  // Calls Object's operator delete
  fmt::println("==================================================");

  void *ptr = operator new(sizeof(Object));  // Calls global operator new
  Object *obj2 = new (ptr) Object();         // Calls Object's placement new
  fmt::println("obj2 address = {:p}", static_cast<void *>(obj2));
  obj2->~Object();       // Manually call destructor
  operator delete(ptr);  // Calls global operator delete
  fmt::println("==================================================");

  return 0;
}