/*
====================== 位域 (Bit-field) 使用注意事项 ======================

1. 内存布局依赖编译器
   - 位域在内存中的排列顺序（低位到高位还是高位到低位）是实现定义的。
   - 同样的位域结构在不同平台、编译器或架构上可能不同。
   - 因此，不建议用位域做跨平台序列化或协议解析。

2. 可用类型限制
   - 位域必须使用整型或枚举类型：
       * int, unsigned int, signed int
       * bool
       * 枚举类型
   - 不建议使用 char、wchar_t、float、double 等类型。
   - 没有指定 signed/unsigned 的 int，符号扩展行为是实现定义的。

3. 位宽不能超过底层类型
   - 例如 uint8_t 位域最大 8 位，uint16_t 最大 16 位，uint32_t 最大 32 位。
   - 超出位宽会报错或被截断。

4. 对齐与填充
   - 编译器可能在位域之间插入填充位，以满足对齐要求。
   - 位域混合普通字段时，结构体大小可能比位域总和大。

5. 位域不能取地址
   - 例如 &obj.flag1 是非法的。
   - 因为位域只是底层整型成员的某些位，没有独立地址。

6. 位域不能是 static 或 extern
   - 因为位域没有独立的内存地址。

7. 位域赋值超出范围
   - unsigned 位域：会截断，只保留低位。
   - signed 位域：行为实现定义。
   - bool 位域：任何非 0 值视为 true。

8. 访问效率
   - 位域访问可能需要掩码和移位操作。
   - 在性能敏感场景下，可能不如手动位操作快。

9. 多线程访问
   - 多个位域可能共用同一存储单元。
   - 并发访问时可能产生数据竞争，需要加锁或避免共享。

10. 典型使用场景
   - 嵌入式硬件寄存器映射
   - 状态标志或配置标志位
   - 小型协议或文件头字段（仅限于同一平台）

=========================================================================
*/

#include <cstdint>
#include <cstdio>

#include "fmt/core.h"

// ==================== 示例 1: 简单位域 ====================
// Flags 是一个 8 位的位域结构体，总共 1+3+4 = 8 位
struct Flags
{
  uint8_t flag1 : 1;  // 占 1 位，用于单一标志
  uint8_t flag2 : 3;  // 占 3 位，可表示 0~7 的数值
  uint8_t flag3 : 4;  // 占 4 位，可表示 0~15 的数值
                      // 编译器通常会把这三个字段打包到一个字节中
};

// -------------------- 8 位寄存器示例 --------------------
union Reg8 {
  uint8_t byte;  // 整体访问
  struct
  {
    uint8_t b0 : 1;  // 最低位
    uint8_t b1 : 1;
    uint8_t b2 : 1;
    uint8_t b3 : 1;
    uint8_t b4 : 1;
    uint8_t b5 : 1;
    uint8_t b6 : 1;
    uint8_t b7 : 1;  // 最高位
  } bits;
};

// -------------------- 16 位寄存器示例（高低字节 + 位访问） --------------------
union Reg16 {
  uint16_t word;  // 整体 16 位访问
  struct
  {
    uint8_t low;   // 低字节
    uint8_t high;  // 高字节
  } bytes;
  struct
  {
    uint16_t b0 : 1;
    uint16_t b1 : 1;
    uint16_t b2 : 1;
    uint16_t b3 : 1;
    uint16_t b4 : 1;
    uint16_t b5 : 1;
    uint16_t b6 : 1;
    uint16_t b7 : 1;
    uint16_t b8 : 1;
    uint16_t b9 : 1;
    uint16_t b10 : 1;
    uint16_t b11 : 1;
    uint16_t b12 : 1;
    uint16_t b13 : 1;
    uint16_t b14 : 1;
    uint16_t b15 : 1;
  } bits;
};

// ==================== 示例 2: 每个位单独命名 ====================
// 8 位位域，每个位单独命名 b_0 ~ b_7
struct BitField8
{
  uint8_t b_0 : 1;  // 位 0
  uint8_t b_1 : 1;  // 位 1
  uint8_t b_2 : 1;  // 位 2
  uint8_t b_3 : 1;  // 位 3
  uint8_t b_4 : 1;  // 位 4
  uint8_t b_5 : 1;  // 位 5
  uint8_t b_6 : 1;  // 位 6
  uint8_t b_7 : 1;  // 位 7
                    // 总共 8 位，通常占用 1 字节
};

// ==================== 示例 3: 16 位位域 ====================
// 每个位单独命名 b_0 ~ b_15
struct BitField16
{
  uint16_t b_0 : 1;  // 位 0
  uint16_t b_1 : 1;
  uint16_t b_2 : 1;
  uint16_t b_3 : 1;
  uint16_t b_4 : 1;
  uint16_t b_5 : 1;
  uint16_t b_6 : 1;
  uint16_t b_7 : 1;
  uint16_t b_8 : 1;  // 位 8
  uint16_t b_9 : 1;
  uint16_t b_10 : 1;
  uint16_t b_11 : 1;
  uint16_t b_12 : 1;
  uint16_t b_13 : 1;
  uint16_t b_14 : 1;
  uint16_t b_15 : 1;  // 位 15
                      // 总共 16 位，占用 2 字节
};

// ==================== 示例 4: 32 位位域 ====================
// 32 位位域，不同字段位宽组合
using MyBitField32 = struct
{
  uint32_t flag1 : 16;  // 占 16 位，可表示 0~65535
  uint32_t flag2 : 8;   // 占 8 位，可表示 0~255
  uint32_t flag3 : 8;   // 占 8 位，可表示 0~255
                        // 总共 32 位，占用 4 字节
};

void testReg8()
{
  printf("===============================testReg8===============================\n");
  Reg8 reg = {0};  // initialize all bits to 0

  // set some bits
  reg.bits.b0 = 1;
  reg.bits.b3 = 1;
  reg.bits.b7 = 1;

  // print individual bits
  printf("Bits access: b0=%u b3=%u b7=%u\n", reg.bits.b0, reg.bits.b3, reg.bits.b7);

  // print full byte
  printf("Full byte access: 0x%02X\n", reg.byte);  // 0x89 = 10001001

  // modify the full byte
  reg.byte = 0x55;  // binary 01010101

  // print all bits after modification
  printf("After modifying full byte, bits: ");
  printf("%u %u %u %u %u %u %u %u\n", reg.bits.b0, reg.bits.b1, reg.bits.b2, reg.bits.b3, reg.bits.b4, reg.bits.b5,
         reg.bits.b6, reg.bits.b7);
}

void testReg16()
{
  printf("===============================testReg8===============================\n");
  Reg16 reg = {0};  // initialize all bits to 0

  // -------------------- set some bits --------------------
  reg.bits.b0 = 1;   // lowest bit
  reg.bits.b3 = 1;
  reg.bits.b4 = 1;
  reg.bits.b7 = 1;   // last bit of low byte
  reg.bits.b8 = 1;   // first bit of high byte
  reg.bits.b15 = 1;  // highest bit

  // -------------------- print individual bits --------------------
  printf("Bits access: ");
  for (int i = 0; i < 16; i++)
  {
    // access each bit by name manually
    unsigned int val = 0;
    switch (i)
    {
    case 0:
      val = reg.bits.b0;
      break;
    case 1:
      val = reg.bits.b1;
      break;
    case 2:
      val = reg.bits.b2;
      break;
    case 3:
      val = reg.bits.b3;
      break;
    case 4:
      val = reg.bits.b4;
      break;
    case 5:
      val = reg.bits.b5;
      break;
    case 6:
      val = reg.bits.b6;
      break;
    case 7:
      val = reg.bits.b7;
      break;
    case 8:
      val = reg.bits.b8;
      break;
    case 9:
      val = reg.bits.b9;
      break;
    case 10:
      val = reg.bits.b10;
      break;
    case 11:
      val = reg.bits.b11;
      break;
    case 12:
      val = reg.bits.b12;
      break;
    case 13:
      val = reg.bits.b13;
      break;
    case 14:
      val = reg.bits.b14;
      break;
    case 15:
      val = reg.bits.b15;
      break;
    }
    printf("%u ", val);
  }
  printf("\n");

  // -------------------- print full word --------------------
  printf("Full 16-bit word: 0x%04X\n", reg.word);

  // -------------------- print high and low byte --------------------
  printf("Low byte: 0x%02X, High byte: 0x%02X\n", reg.bytes.low, reg.bytes.high);

  // -------------------- modify low byte and see effect --------------------
  reg.bytes.low = 0xAA;  // binary 10101010
  printf("After modifying low byte to 0xAA:\n");
  printf("Full word: 0x%04X, Low byte: 0x%02X, High byte: 0x%02X\n", reg.word, reg.bytes.low, reg.bytes.high);
}

int main()
{
  // -------------------- Flags 测试 --------------------
  Flags f{};     // 初始化所有位为 0
  f.flag1 = 1;   // 1 位，合法
  f.flag2 = 5;   // 3 位，二进制 101 -> 5
  f.flag3 = 10;  // 4 位，二进制 1010 -> 10

  // 输出 Flags 大小和各位值
  fmt::print("sizeof(Flags): {} bytes, flag1: {}, flag2: {}, flag3: {}\n", sizeof(f), (int)f.flag1, (int)f.flag2,
             (int)f.flag3);

  // -------------------- BitField8 测试 --------------------
  BitField8 bf = {1, 0, 1, 0, 1, 0, 1, 0};  // 初始化每个位
  // 输出每个位的值
  printf("%u %u %u %u %u %u %u %u\n", bf.b_0, bf.b_1, bf.b_2, bf.b_3, bf.b_4, bf.b_5, bf.b_6, bf.b_7);

  // -------------------- 初始化 --------------------
  MyBitField32 bf32{};  // 初始化所有位为 0
  bf32.flag1 = 50000;   // 16 位
  bf32.flag2 = 200;     // 8 位
  bf32.flag3 = 150;     // 8 位

  // -------------------- 输出 --------------------
  fmt::print("sizeof(BitField32) = {} bytes\n", sizeof(bf32));
  fmt::print("flag1 = {}, flag2 = {}, flag3 = {}\n", (int)bf32.flag1, (int)bf32.flag2, (int)bf32.flag3);

  // -------------------- 演示位域截断 --------------------
  bf32.flag2 = 300;  // 超过 8 位最大值 255
  fmt::print("After assigning 300 to flag2 (8-bit), flag2 = {}\n", (int)bf32.flag2);
  // 输出 300 & 0xFF = 44 （只保留低 8 位）

  testReg8();
  testReg16();

  return 0;
}
