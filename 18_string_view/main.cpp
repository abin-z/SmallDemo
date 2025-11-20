#include <array>
#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "fmt/core.h"

/**
 * @brief std::string_view（C++17）的简要说明
 *
 * std::string_view 是一个对字符串数据的“非拥有”视图，它只保存指针和长度，
 * 不负责管理底层内存，也不会拷贝数据。常用于高性能场景下的只读字符串传递。
 *
 * 主要特性:
 *   - 非拥有：不管理所指向的字符序列，生命周期由外部保证。
 *   - 零拷贝：构造与传参非常轻量，仅为指针 + 大小。
 *   - 不要求以 '\0' 结尾：可用于二进制安全的子串操作。
 *
 * 使用注意:
 *   1. string_view 所引用的数据必须在它的整个生命周期内保持有效。
 *      示例: std::string_view sv = std::string("tmp"); // 错误: 临时对象立即销毁
 *
 *   2. sv.data() 不保证以 '\0' 结尾，不能直接当作 C 字符串传给 printf 等 C API。
 *
 *   3. 不可修改其所指向的数据，它是一个只读视图。
 *
 * 示例:
 *   std::string s = "hello";
 *   std::string_view sv = s;  // OK
 *   auto sub = sv.substr(1, 3); // 得到 "ell"
 *
 * 适用场景:
 *   - 函数参数的只读字符串输入（比 const std::string& 更通用）
 *   - 高性能解析器（如 INI、JSON、HTTP 等）
 *   - 子串操作无需分配新的 std::string
 */

#include <string>
#include <string_view>

void examples()
{
  // 1. 从 const char* 构造（字符串字面量）
  std::string_view sv1 = "hello";  // OK，指向静态存储区
  fmt::print("sv1 = {}\n", sv1);

  // 2. 从 std::string 构造
  std::string s = "world";
  std::string_view sv2 = s;  // OK，引用 s 的数据，不拷贝
  fmt::print("sv2 = {}\n", sv2);

  // 3. 从 char* + 长度构造
  const char* p = "abcdefg";
  std::string_view sv3(p, 3);  // sv3 = "abc"
  fmt::print("sv3 = {}\n", sv3);

  // 4. 从 string 的子串构造
  std::string_view sv4(s.data() + 1, 3);  // 指向 "orl"
  fmt::print("sv4 = {}\n", sv4);

  // 5. 使用 substr() 构造子视图（零拷贝）
  std::string_view sv5 = sv2.substr(1, 3);  // "orl"
  fmt::print("sv5 = {}\n", sv5);

  // 6. 指向非 '\0' 结尾的字符缓冲区
  const char buf[] = {'A', 'B', 'C', 'D', 'E'};
  std::string_view sv6(buf, 5);  // "ABCDE"，无 '\0'，二进制安全
  fmt::print("sv6 = {}\n", sv6);

  // 7. 从 vector<char> 构造
  std::vector<char> v = {'x', 'y', 'z'};
  std::string_view sv7(v.data(), v.size());  // "xyz"
  fmt::print("sv7 = {}\n", sv7);

  // 8. 从 std::array<char> 构造
  std::array<char, 5> arr = {'h', 'e', 'l', 'l', 'o'};
  std::string_view sv8(arr.data(), arr.size());  // "hello"
  fmt::print("sv8 = {}\n", sv8);

  // ---⚠️ 错误示例（会悬垂）---
  // std::string_view bad = std::string("tmp"); // 错误: 临时对象已销毁
}

void demo_string_string_view()
{
  fmt::print("=== string → string_view ===\n");

  // 1. 从 std::string 转换（零拷贝）
  std::string s = "hello world";
  std::string_view sv1 = s;
  fmt::print("sv1 = {}\n", sv1);

  // 2. 从字符指针转 string_view
  const char* p = "abcdef";
  std::string_view sv2(p, 3);  // 只取前 3 个
  fmt::print("sv2 = {}\n", sv2);

  fmt::print("\n=== string_view → string ===\n");

  // 3. string_view → string（需要拷贝）
  std::string s2 = std::string(sv1);
  fmt::print("s2 = {}\n", s2);

  // 4. 从部分区域构造 string
  std::string_view sv3 = "123456789";
  std::string s3(sv3.substr(2, 4));  // "3456"
  fmt::print("s3 = {}\n", s3);

  // 5. 使用 data()+size() 转换
  std::string s4(sv2.data(), sv2.size());
  fmt::print("s4 = {}\n", s4);

  // fmt::print("\n=== 易错示例：悬挂引用 ===\n");
  // 6. 错误示范：千万不要这样用！
  // std::string_view bad = std::string("temp");
  // 临时 string 马上销毁，bad 内部指针失效
  // fmt::print("bad = {}  (悬挂引用，不能这样写！)\n", bad);
}

inline std::string_view trim_left(std::string_view sv)
{
  size_t start = 0;
  while (start < sv.size() && std::isspace(static_cast<unsigned char>(sv[start]))) ++start;
  return sv.substr(start);
}

inline std::string_view trim_right(std::string_view sv)
{
  size_t end = sv.size();
  while (end > 0 && std::isspace(static_cast<unsigned char>(sv[end - 1]))) --end;
  return sv.substr(0, end);
}

// 去掉前后空白字符（空格、制表符、换行等）
inline std::string_view trim(std::string_view sv)
{
  size_t start = 0;
  size_t end = sv.size();

  while (start < end && std::isspace(static_cast<unsigned char>(sv[start]))) ++start;
  while (end > start && std::isspace(static_cast<unsigned char>(sv[end - 1]))) --end;

  return sv.substr(start, end - start);
}

int main()
{
  std::string str{"Hello, World!"};
  std::string_view str_view{str};
  fmt::print("String: {}\n", str);
  fmt::print("String View: {}\n", str_view);
  examples();
  demo_string_string_view();
  return 0;
}
