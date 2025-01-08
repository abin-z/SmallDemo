#include <string>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

// 空类的大小优化。这种优化意味着，即使一个类没有成员变量，它也可能会占用一定的内存，通常是 1 字节。
class Base
{
};
// 继承Base
struct Derived : public Base
{
    int num;
};
// Base作为成员, 存在内存对齐
struct Component
{
    int num;
    Base base_; // 空类成员
};

// 字节对齐: 编译器会根据每个成员的对齐要求来自动对齐结构体，以提高性能
// 这会导致结构体的内存布局包含填充字节，从而增大结构体的大小。
struct AAA
{
    int a;
    char c;
    int b;
};

struct BBB
{
    double a;
    char c;
    double b;
};

/// 强制一字节对齐, 需要注意，按 1 字节对齐会影响性能，因为 CPU 通常会针对较大对齐的类型(4/8)进行优化
#pragma pack(push, 1)  // 设置 1 字节对齐
struct CCC
{
    int a; 
    char c;   
    int b; 
};

struct DDD
{
    double a; 
    char c;   
    int b; 
};
#pragma pack(pop)  // 恢复默认对齐方式

#pragma pack(push, 2)  // 设置 2 字节对齐
struct MMM
{
    int a; 
    char c;   
    int b; 
};
#pragma pack(pop)  // 恢复默认对齐方式


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
#ifdef _WIN32
    // Windows 系统设置控制台为 UTF-8 编码
    system("chcp 65001");
#endif
    std::cout << "你好, C++. version = " << __cplusplus << std::endl;

    std::cout << "==========================================" << std::endl;
    std::cout << "sizeof(char) = " << sizeof(char) << std::endl;
    std::cout << "sizeof(wchar_t) = " << sizeof(wchar_t) << std::endl;
    std::cout << "sizeof(wchar_t *) = " << sizeof(wchar_t *) << std::endl;

    std::cout << "sizeof(long) = " << sizeof(long) << std::endl;
    std::cout << "sizeof(long long) = " << sizeof(long long) << std::endl;
    std::cout << "sizeof(float) = " << sizeof(float) << std::endl;
    std::cout << "sizeof(double) = " << sizeof(double) << std::endl;
    std::cout << "==========================================" << std::endl;
    Component obj;
    std::cout << "空类 sizeof(Base) = " << sizeof(Base) << std::endl;
    std::cout << "sizeof(Derived) = " << sizeof(Derived) << std::endl;
    std::cout << "sizeof(Component) = " << sizeof(Component) << std::endl;
    std::cout << "sizeof(Component.base_) = " << sizeof(obj.base_) << std::endl;
    std::cout << "sizeof(Base *) = " << sizeof(Base *) << std::endl;
    std::cout << "sizeof(Derived *) = " << sizeof(Derived *) << std::endl;
    std::cout << "sizeof(Component *) = " << sizeof(Component *) << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "存在字节对齐 sizeof(AAA) = " << sizeof(AAA) << std::endl;
    std::cout << "存在字节对齐 sizeof(BBB) = " << sizeof(BBB) << std::endl;
    std::cout << "1字节对齐 sizeof(CCC) = " << sizeof(CCC) << std::endl;
    std::cout << "1字节对齐 sizeof(DDD) = " << sizeof(DDD) << std::endl;
    std::cout << "2字节对齐 sizeof(MMM) = " << sizeof(MMM) << std::endl;
}
