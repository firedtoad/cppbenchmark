cmake_minimum_required(VERSION 3.16)
project(cpp_benchmark C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH   ${PROJECT_BINARY_DIR}/lib)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(UseFastestLinker)
include(Ccache)

link_libraries(benchmark pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -Wall -pedantic -Wextra -march=native -Wno-c++20-designator -Wno-deprecated-builtins -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-nested-anon-types -Wno-unused-const-variable -Wno-unused-variable -Wno-ignored-qualifiers -fdiagnostics-color=auto ")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
#    set(CMAKE_CXX_FLAGS_RELEASE  "-O2 -DNDEBUG")
endif ()

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(
        /usr/local/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/openssl
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/libs /usr/local/lib)

add_subdirectory(src/adt)
add_subdirectory(src/string)
add_subdirectory(src/allocator)
add_subdirectory(src/arithmetic)
add_subdirectory(src/astar)
add_subdirectory(src/container)
add_subdirectory(src/coroutine)
add_subdirectory(src/branch)
add_subdirectory(src/logging)
add_subdirectory(src/memory)
add_subdirectory(src/compiler)
add_subdirectory(src/lockfree)
add_subdirectory(src/cpu)
add_subdirectory(src/handler)
add_subdirectory(src/crypto)
add_subdirectory(src/json)
add_subdirectory(src/polymorphism)
add_subdirectory(src/protobuf)
add_subdirectory(src/mutex)
add_subdirectory(src/random)
add_subdirectory(src/timer)
add_subdirectory(src/ub)
add_subdirectory(src/syscall)
