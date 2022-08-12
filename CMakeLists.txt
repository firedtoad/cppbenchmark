cmake_minimum_required(VERSION 3.16)
project(bench C CXX)

set(CMAKE_CXX_STANDARD 17)
link_libraries(benchmark pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -march=native  -Wno-unused-parameter -Wno-unused-variable -Wno-ignored-qualifiers -fdiagnostics-color=auto ")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-limit-debug-info")
    set(CMAKE_CXX_FLAGS_RELEASE  "-O2 -DNDEBUG")
endif ()
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=gold")

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    MESSAGE(STATUS ">> CCACHE ON")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
else ()
    MESSAGE(STATUS ">> CCACHE OFF")
endif ()

include_directories(
        /usr/local/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/flat_hash_map
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/llvm
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/parallel_hashmap
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/sparsepp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/robin_hood
)

link_directories(/usr/local/lib)

add_subdirectory(src/adt)
add_subdirectory(src/string)
add_subdirectory(src/allocator)
add_subdirectory(src/arithmetic)
add_subdirectory(src/astar)
add_subdirectory(src/container)
add_subdirectory(src/coroutine)
add_subdirectory(src/branch)
add_subdirectory(src/logging)
add_subdirectory(src/compiler)
add_subdirectory(src/lockfree)
add_subdirectory(src/cpu)
add_subdirectory(src/handler)
add_subdirectory(src/polymorphism)
add_subdirectory(src/mutex)
add_subdirectory(src/random)
add_subdirectory(src/ub)
