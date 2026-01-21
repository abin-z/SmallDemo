#include <cstdio>
#include <cstdlib>

typedef struct
{
  int x;
  int y;
  int z;
} obj;

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

// ✅ 正确版本：改的是调用者本体
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

int main()
{
  func_wrong();
  func_correct();
  return 0;
}