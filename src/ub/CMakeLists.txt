add_executable(ub ub.cpp)
target_compile_options(ub PRIVATE -fpermissive)
target_link_libraries(ub PRIVATE tcmalloc )