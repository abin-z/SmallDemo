set(tgt_name futureAsync)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS *.h *.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.c *.cpp *.cc *.cxx)

add_executable(${tgt_name})
target_sources(${tgt_name} PUBLIC ${headers})
target_sources(${tgt_name} PRIVATE ${sources})

target_include_directories(${tgt_name} PUBLIC .)

# 链接 fmt 库
target_link_libraries(${tgt_name} PRIVATE fmt)

# 仅在 Linux/macOS 上启用 pthread
if (UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(${tgt_name} PRIVATE Threads::Threads)
endif()