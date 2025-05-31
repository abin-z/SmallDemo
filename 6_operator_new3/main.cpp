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

  constexpr int count = 4;
  void *arrayPtr = operator new[](sizeof(Object) * count);  // Calls global operator new for arrays
  Object *objArray = new (arrayPtr) Object[count];          // Calls Object's placement new for arrays

  // // 显式逐个构造对象
  // Object *objArray = static_cast<Object *>(arrayPtr);
  // for (int i = 0; i < count; ++i)
  // {
  //    new (&objArray[i]) Object(); // 这就不会触发数组版 placement new 的隐藏行为
  // }

  fmt::println("objArray address = {:p}", static_cast<void *>(objArray));
  for (int i = 0; i < count; ++i)
  {
    fmt::println("objArray[{}] address = {:p}", i, static_cast<void *>(&objArray[i]));
    objArray[i].~Object();  // Manually call destructor for each object
  }
  operator delete[](arrayPtr);  // Calls global operator delete for arrays
  fmt::println("==================================================");

  return 0;
}