set(tgt_name variadic_template)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS *.h *.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.c *.cpp *.cc *.cxx)

add_executable(${tgt_name})
target_sources(${tgt_name} PUBLIC ${headers})
target_sources(${tgt_name} PRIVATE ${sources})

target_include_directories(${tgt_name} PUBLIC .)