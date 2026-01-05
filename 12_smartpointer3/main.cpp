#include <fmt/core.h>
#include <fmt/ranges.h>

#include <memory>

/**
 * weak_ptr：不增加强引用计数，仅观察对象是否存在。需要配合shared_ptr使用.
 * weak_ptr 只是持有一个指向 shared_ptr 所管理对象的弱引用，但它不会影响 shared_ptr 的 use_count（引用计数）。
 * 当最后一个 shared_ptr 销毁时，资源会被释放，此时 weak_ptr 变为悬空指针（expired），不能再安全地访问对象。
 * 由于 weak_ptr 不能直接访问对象，需要通过 lock() 临时提升为 shared_ptr 才能安全地使用。
 *
 *
 * 循环引用（Cyclic Reference）是指两个或多个 std::shared_ptr 互相持有对方，导致它们的引用计数永远无法降为
 * 0，最终导致内存泄漏。 std::shared_ptr 依赖引用计数来管理资源。当 shared_ptr 被拷贝时，引用计数 +1；当 shared_ptr
 * 被销毁时，引用计数 -1。 但如果两个对象互相持有 shared_ptr，即使它们超出了作用域，引用计数仍然不会降到
 * 0，导致资源不会被释放。
 *
 * 在双向关联的类中，一方使用 std::shared_ptr，另一方使用 std::weak_ptr。
 * 如果关系是“强-弱”关系，应该让“主要拥有者”持有 shared_ptr，让“从属对象”持有 weak_ptr。
 */

struct B;  //  前置声明 B
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
struct A
{
  std::shared_ptr<B> ptrB;
  ~A()
  {
    fmt::print("A destroyed\n");
  }
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
struct B
{
  std::shared_ptr<A> ptrA;
  ~B()
  {
    fmt::print("B destroyed\n");
  }
};

void test01()
{
  auto a = std::make_shared<A>();
  auto b = std::make_shared<B>();
  a->ptrB = b;  // A 持有 B
  b->ptrA = a;  // B 持有 A
  // 这里 A 和 B 都不会被销毁，造成内存泄漏！
}

struct BB;  // 先声明 B
struct AA
{
  std::shared_ptr<BB> ptrB;
  ~AA()
  {
    fmt::print("AA destroyed\n");
  }
  AA() = default;
  AA(const AA &) = default;
  AA &operator=(const AA &) = default;
  AA(AA &&) noexcept = default;
  AA &operator=(AA &&) noexcept = default;
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
struct BB
{
  std::weak_ptr<AA>
    ptrA;  // 使用 weak_ptr 不影响强引用计数的智能指针,它不增加 shared_ptr 的引用计数，因此不会导致循环引用问题
  ~BB()
  {
    fmt::print("BB destroyed\n");
  }
};

void test02()
{
  auto a = std::make_shared<AA>();
  auto b = std::make_shared<BB>();
  a->ptrB = b;  // A 持有 B
  b->ptrA = a;  // B 持有 A
  // 这里的ptrA是weak_ptr弱引用指针, 不会增加shared_ptr的引用计数, 能正常释放资源
}

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
struct Object
{
  int id_ = 0;
  explicit Object(int id = 0) : id_(id)
  {
    fmt::print("Object constructor\n");
  }
  ~Object()
  {
    fmt::print("~Object destructor\n");
  }
  [[nodiscard]] int id() const
  {
    return id_;
  }
};

void test03()
{
  std::shared_ptr<Object> sp = std::make_shared<Object>(18);
  std::weak_ptr<Object> wp = sp;  // wp 观察 sp
  fmt::println("use_count = {}", wp.use_count());

  if (auto locked = wp.lock())  // 提升 weak_ptr 为 shared_ptr
  {
    fmt::println("Object is still alive use_count = {}", sp.use_count());
  }
  else
  {
    fmt::println("Object has been destroyed = {}", sp.use_count());
  }
  sp.reset();                   // 释放 shared_ptr 持有的对象
  if (auto locked = wp.lock())  // 提升 weak_ptr 为 shared_ptr
  {
    fmt::println("Object is still use_count = {}", sp.use_count());
  }
  else
  {
    fmt::println("Object has been destroyed = {}", sp.use_count());
  }
  fmt::println(
    "wp.expired() = {}",
    wp.expired());  // 如果 weak_ptr 观察的对象仍然存在，expired() 返回 false；如果对象已经被释放，expired() 返回 true。
}

int main()
{
  test01();
  fmt::print("------------------\n");
  test02();
  fmt::print("------------------\n");
  test03();
  fmt::print("------------------\n");
}