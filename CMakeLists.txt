cmake_minimum_required (VERSION 3.16)
project(bench C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

link_libraries(benchmark pthread)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/flat_hash_map
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/llvm
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/parallel_hashmap
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/sparsepp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib3rd/robin_hood
        )

add_subdirectory(src/allocator)
add_subdirectory(src/arithmetic)
add_subdirectory(src/astar)
add_subdirectory(src/container)
add_subdirectory(src/cpu)
add_subdirectory(src/handler)
add_subdirectory(src/mutex)
add_subdirectory(src/ub)
