cmake_minimum_required(VERSION 3.16)
project(memory-allocators LANGUAGES CXX)

set(SOURCES src/Allocator.cpp
        src/CAllocator.cpp
        src/LinearAllocator.cpp
        src/StackAllocator.cpp
        src/PoolAllocator.cpp
        src/FreeListAllocator.cpp
        src/Benchmark.cpp
        src/main.cpp)

add_library(allocators ${SOURCES})

target_include_directories(allocators PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)

target_compile_features(allocators PRIVATE cxx_std_17)