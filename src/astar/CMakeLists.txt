project(AStar C CXX)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs support core irreader)
include_directories(  ${LLVM_INCLUDE_DIRS})
link_libraries(  ${llvm_libs})

add_executable(astar bench_astar.cpp AStar.cpp)
#target_link_libraries(astar PRIVATE absl_raw_hash_set absl_low_level_hash absl_hash)


