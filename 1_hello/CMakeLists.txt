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
