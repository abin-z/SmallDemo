# 设置 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.20)

# 生成 compile_commands.json 文件（用于 clangd、clang-tidy 等工具）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置默认构建类型为Release，如果未指定构建类型
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif()

# 设置 C++ 标准为 C++17，并强制要求支持 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 强制要求支持指定的 C++ 标准
set(CMAKE_CXX_EXTENSIONS OFF)       # 禁用编译器特定扩展，确保代码更具可移植性

# 设置所有库生成位置无关代码 (Position Independent Code) 对应GCC/Clang的-fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 定义项目名称及支持的语言
project(SmallDemo LANGUAGES C CXX)

# 设置输出目录
# 静态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/lib)
# 动态库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/bin)
# 可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/bin)

# # 仅在 Linux/macOS 上启用 pthread
# if (UNIX)
#     find_package(Threads REQUIRED)
#     set(THREADS_PREFER_PTHREAD_FLAG ON)
#     link_libraries(Threads::Threads)
# endif()

# 添加子目录
# 每个子目录对应一个模块
add_subdirectory(external/fmt)  # 添加 fmt 库
add_subdirectory(1_hello)       # 示例 1：Hello World 模块
add_subdirectory(2_vector)
add_subdirectory(3_sizeof)
add_subdirectory(4_metaprogram)
add_subdirectory(5_multifiles)
add_subdirectory(6_operator_new)
add_subdirectory(6_operator_new2)
add_subdirectory(6_operator_new3)
add_subdirectory(7_multithread)
add_subdirectory(7_multithreadmutex)
add_subdirectory(8_condition_variable)
add_subdirectory(8_condition_variable2)
add_subdirectory(9_future_async)
add_subdirectory(9_future_async2)
add_subdirectory(10_readwritelock)
add_subdirectory(11_locktopic)
add_subdirectory(12_smartpointer)
add_subdirectory(12_smartpointer2)
add_subdirectory(12_smartpointer3)
add_subdirectory(13_tuple_pair)
add_subdirectory(14_callableobject)
add_subdirectory(15_mem_reference_qualifier)
add_subdirectory(16_variadic_templates)
add_subdirectory(17_invoke)
add_subdirectory(18_string_view)
