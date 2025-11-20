set(tgt_name string_view_demo)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS *.h *.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.c *.cpp *.cc *.cxx)

# # 仅对子项目使用 C++11
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${tgt_name})
target_sources(${tgt_name} PUBLIC ${headers})
target_sources(${tgt_name} PRIVATE ${sources})

target_include_directories(${tgt_name} PUBLIC .)

# 链接 fmt 库
target_link_libraries(${tgt_name} PRIVATE fmt)