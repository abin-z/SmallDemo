#include <cstdio>
#include <cstdlib>
#include <memory>

typedef struct
{
  int x;
  int y;
  int z;
} obj;

////// 典型的C风格函数：通过输出参数返回分配的对象

// ❌ 错误版本：改的是局部副本, 无法修改调用者的指针
int create_obj_wrong(obj *out)
{
  out = (obj *)malloc(sizeof(obj));  // 只改了函数局部变量 out, ❌ <<<内存泄漏了!!!>>>
  if (out != nullptr)
  {
    out->x = 42;
    out->y = 43;
    out->z = 44;
  }
  return 0;
}

// ✅ 正确版本：改的是调用者本体, 这种模式在C++23中可以用 std::out_ptr 简化(配合智能指针)
int create_obj_correct(obj **out)
{
  *out = (obj *)malloc(sizeof(obj));  // 改了调用者的指针
  if (*out != nullptr)
  {
    (*out)->x = 42;
    (*out)->y = 43;
    (*out)->z = 44;
  }
  return 0;
}

////// C++ 风格函数：通过引用指针返回分配的对象
void create_obj_correct2(obj *&p)  // 引用指针
{
  p = new obj;  // 分配对象
  p->x = 42;
  p->y = 43;
  p->z = 44;
}

// 使用智能指针版本
void create_obj_correct3(std::unique_ptr<obj> &p)  // 引用智能指针
{
  p = std::make_unique<obj>();  // 分配对象
  p->x = 42;
  p->y = 43;
  p->z = 44;
}

// 返回智能指针版本, 推荐
std::unique_ptr<obj> create_obj_correct4()
{
  auto p = std::make_unique<obj>();
  p->x = 42;
  p->y = 43;
  p->z = 44;
  return p;
}

int func_wrong()
{
  printf("==============func_wrong==============\n");
  obj *p = nullptr;
  printf("before wrong: p = %p\n", p);
  create_obj_wrong(p);                  // ❌ 传进的是 p 的拷贝
  printf("after  wrong: p = %p\n", p);  // 仍然 nullptr
  if (p != nullptr)
  {
    printf("obj: x=%d, y=%d, z=%d\n", p->x, p->y, p->z);
    free(p);  // 别忘了释放内存
  }
  return 0;
}

int func_correct()
{
  printf("==============func_correct==============\n");
  obj *p = nullptr;
  printf("before correct: p = %p\n", p);
  create_obj_correct(&p);                 // ✅ 传进的是 p 的地址
  printf("after  correct: p = %p\n", p);  // 现在指向新分配的内存
  if (p != nullptr)
  {
    printf("obj: x=%d, y=%d, z=%d\n", p->x, p->y, p->z);
    free(p);  // 别忘了释放内存
  }
  return 0;
}

int func_cpp_style()
{
  printf("==============func_cpp_style==============\n");

  // 1️⃣ 通过引用指针
  obj *p3 = nullptr;
  create_obj_correct2(p3);
  printf("p3: %p, x=%d, y=%d, z=%d\n", p3, p3->x, p3->y, p3->z);
  delete p3;  // RAII 手动释放

  // 2️⃣ 通过智能指针引用
  std::unique_ptr<obj> p4;
  create_obj_correct3(p4);
  printf("p4: %p, x=%d, y=%d, z=%d\n", p4.get(), p4->x, p4->y, p4->z);

  // 3️⃣ 返回智能指针（最推荐）
  auto p5 = create_obj_correct4();
  printf("p5: %p, x=%d, y=%d, z=%d\n", p5.get(), p5->x, p5->y, p5->z);

  return 0;
}

int main()
{
  func_wrong();
  func_correct();
  func_cpp_style();
  return 0;
}