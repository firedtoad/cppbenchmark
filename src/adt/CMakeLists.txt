project(ADT C CXX)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs support core irreader)


include_directories(  ${LLVM_INCLUDE_DIRS})
link_libraries(  ${llvm_libs})

add_executable(adt_list_insert container/list/insert.cpp)
add_executable(adt_list_range container/list/range.cpp)
add_executable(adt_list_remove container/list/remove.cpp)
add_executable(adt_list_splice container/list/splice.cpp)

add_executable(adt_map_decay container/map/decay.cpp)
add_executable(adt_map_find_hetero container/map/find_hetero.cpp)
add_executable(adt_map_order_find container/map/order_find.cpp)
add_executable(adt_map_order_range container/map/order_range.cpp)

add_executable(adt_map_reserve container/map/reserve.cpp)
add_executable(adt_map_interval container/map/interval.cpp)
add_executable(adt_map_unordered_erase container/map/unorder_erase.cpp)
add_executable(adt_map_unordered_find container/map/unorder_find.cpp)
add_executable(adt_map_unordered_range container/map/unorder_range.cpp)

add_executable(adt_set_reserve container/set/reserve.cpp)
add_executable(adt_set_order_find container/set/order_find.cpp)
add_executable(adt_set_order_range container/set/order_range.cpp)

add_executable(adt_set_unordered_erase container/set/unorder_erase.cpp)
add_executable(adt_set_unordered_find container/set/unorder_find.cpp)
add_executable(adt_set_unordered_range container/set/unorder_range.cpp)

add_executable(adt_fast_map container/vfast/fast_map.cpp)
add_executable(adt_fast_set container/vfast/fast_set.cpp)
add_executable(adt_fast_umap container/vfast/fast_unmap.cpp)
add_executable(adt_fast_uset container/vfast/fast_unset.cpp)


add_executable(adt_vec_remove container/vector/remove.cpp)
add_executable(adt_vec_reserve container/vector/reserve.cpp)
add_executable(adt_vec_vectors container/vector/vectors.cpp)
add_executable(adt_vec_row_col container/vector/row_col.cpp)




add_executable(adt_immutable container/immutable/immutable.cpp)
add_executable(adt_small container/small/small.cpp)