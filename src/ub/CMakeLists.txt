add_executable(ub ub.cpp)
target_link_libraries(ub PRIVATE tcmalloc_and_profiler.a unwind.a  lzma.a)
target_compile_options(ub PRIVATE -fpermissive)
target_link_libraries(ub PRIVATE tcmalloc )