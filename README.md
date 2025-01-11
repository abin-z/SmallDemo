# SmallDemo
### 一些简单有趣的C++小案例
### 1. 项目介绍
- 本项目包含一些简单有趣的cpp小案例.
- 主要是记录平时练习写的C++代码, 各种demo程序.
- 项目构建工具为CMake, 编辑器是VS Code.
- 子项目都比较小巧, 且很少互相依赖, CMakeLists.txt也写的比较通用.
- 可以跨平台(Linux, Windows, Macos), 配合vscode使用很方便.
- Modern CMake. 以面向目标（Targets）为核心, 自动处理依赖.



### 2. CMakeLists.txt文件介绍
- 项目根目录的`CMakeLists.txt`内容如下:

  ```cmake
  # 设置 CMake 的最低版本要求
  cmake_minimum_required(VERSION 3.20)
  
  # 如果没有指定构建类型，默认设置为 Release
  if (NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE Release)
  endif()
  
  # 设置 C++ 标准为 C++17，并强制要求支持 C++17, 这些设置建议写在project配置前
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON) # 强制要求支持指定的 C++ 标准
  set(CMAKE_CXX_EXTENSIONS OFF)       # 禁用编译器特定扩展，确保代码更具可移植性
  
  # 定义项目名称及支持的语言
  project(SmallDemo LANGUAGES C CXX)
  
  # 设置输出目录
  # 静态库输出目录
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/lib)
  # 动态库输出目录
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/bin)
  # 可执行文件输出目录
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/bin)
  
  # 添加子目录
  # 每个子目录对应一个模块
  add_subdirectory(external/fmt)  # 添加 fmt 库
  add_subdirectory(1_hello)       # 示例 1：Hello World 模块
  add_subdirectory(2_vector)
  add_subdirectory(3_sizeof)
  add_subdirectory(4_metaprogram)
  ```

- 子项目目录下的`CMakelists.txt`内容如下:

  ```cmake
  # 设置目标名称为 hello
  set(tgt_name hello)
  
  # 递归查找头文件，存储在变量 headers 中
  # CONFIGURE_DEPENDS 用于确保当文件系统中匹配的文件发生变化时，会触发重新配置
  file(GLOB_RECURSE headers CONFIGURE_DEPENDS *.h *.hpp)
  
  # 递归查找源文件，存储在变量 sources 中
  file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.c *.cpp *.cc *.cxx)
  
  # 创建一个名为 ${tgt_name} 的目标库（默认是静态库）
  add_executable(${tgt_name})
  
  # 将头文件添加到目标的 PUBLIC 源文件列表中
  # PUBLIC 意味着头文件将对目标的用户可见
  target_sources(${tgt_name} PUBLIC ${headers})
  
  # 将源文件添加到目标的 PRIVATE 源文件列表中
  # PRIVATE 意味着这些文件仅对目标本身可见
  target_sources(${tgt_name} PRIVATE ${sources})
  
  # 指定 ./ 目录，让目标及其用户能够访问 ./ 下的头文件
  # PUBLIC 意味着 . 目录会被传播给目标的依赖者
  target_include_directories(${tgt_name} PUBLIC .)

  # 链接 fmt 库到目标
  # PRIVATE 表示 fmt 库只在目标本身使用，且不会传播给其他依赖此目标的目标
  target_link_libraries(${tgt_name} PRIVATE fmt)
  ```

### 3. 构建和运行流程

- 生成项目过程: 在项目根目录下打开命令行, 执行以下命令.

  ```sh
  # -B 指定构建目录(build directory), build文件夹名称可以自定义
  cmake -B build 
  ```

  执行上述指令后将会根据您所在的平台**生成项目**文件夹`build`.

  - 默认使用当前目录下的 `CMakeLists.txt` 文件.
  - Linux系统下一般生成`Makefiles`或者`Ninja`的项目, 可以使用`-G`制定生成器.
  - Windows系统下一般生成VS的.sln项目.

- 构建项目过程: 生成对应的库或者可执行文件, 执行以下命令.

  ```sh
  # --build 选项用于构建项目，也就是执行编译操作, 后续的build是文件夹名称
  cmake --build build
  ```

  执行上述指令后会开始**构建项目**，也就是执行编译操作. 它会根据之前通过 `cmake` 配置生成的构建系统（如 Makefiles、Ninja 文件等）调用对应的生成工具.

  - 由于本项目指定了输出文件夹`build_output`, 就可以此文件夹中找到生成的库或者可执行文件.
  - 静态库输出目录:`build_output/lib`, 动态库和可执行文件输出目录:`build_output/bin`

- 运行程序: 直接进入文件夹`./name.exe`即可运行. 或者通过点击vscode的CMake插件运行按钮运行.

