add_executable(memory memory.cpp)
add_executable(alloc alloc.cpp)
add_executable(size_allocator size_allocator.cpp)
target_compile_options(size_allocator PRIVATE -fsized-deallocation)