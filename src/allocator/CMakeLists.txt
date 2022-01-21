add_subdirectory(foonathan_memory)
add_subdirectory(memory-allocators)
add_executable(allocator main.cpp
        ll_alloc.c
        win_stub.cc
        )

target_compile_features(allocator PRIVATE cxx_std_17)

target_include_directories(allocator PRIVATE foonathan_memory/include memory-allocators/includes)

target_link_libraries(allocator PRIVATE foonathan_memory allocators)