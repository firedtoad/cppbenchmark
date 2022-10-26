add_executable(list_insert list/insert.cpp)
add_executable(list_remove list/remove.cpp)
add_executable(list_range list/range.cpp)
add_executable(list_splice list/splice.cpp)

add_executable(map_find_hetero map/find_hetero.cpp)
add_executable(map_find_order map/order_find.cpp)
add_executable(map_range_order map/order_range.cpp)
add_executable(map_find_unorder map/unorder_find.cpp)
add_executable(map_range_unorder map/unorder_range.cpp)
add_executable(map_erase_unorder map/unorder_erase.cpp)
add_executable(map_decay map/decay.cpp)
add_executable(map_reserve map/reserve.cpp)

target_link_libraries(map_find_hetero PRIVATE absl_hash absl_city)
target_link_libraries(map_find_order PRIVATE absl_hash absl_city)
target_link_libraries(map_range_order PRIVATE absl_hash absl_city)
target_link_libraries(map_find_unorder PRIVATE absl_hash absl_city)
target_link_libraries(map_range_unorder PRIVATE absl_hash absl_city glog brpc)
target_link_libraries(map_erase_unorder PRIVATE absl_hash absl_city)
target_link_libraries(map_decay PRIVATE absl_hash absl_raw_hash_set absl_low_level_hash  absl_city)
target_link_libraries(map_reserve PRIVATE absl_hash absl_city)

add_executable(set_find_order set/order_find.cpp)
add_executable(set_range_order set/order_range.cpp)
add_executable(set_erase_order set/order_erase.cpp)
add_executable(set_find_unorder set/unorder_find.cpp)
add_executable(set_range_unorder set/unorder_range.cpp)
add_executable(set_erase_unorder set/unorder_erase.cpp)
add_executable(set_reserve set/reserve.cpp)

target_link_libraries(set_find_order PRIVATE absl_hash absl_city)
target_link_libraries(set_range_order PRIVATE absl_hash absl_city)
target_link_libraries(set_erase_order PRIVATE absl_hash absl_city)
target_link_libraries(set_find_unorder PRIVATE absl_hash absl_city)
target_link_libraries(set_range_unorder PRIVATE absl_hash absl_city)
target_link_libraries(set_erase_unorder PRIVATE absl_hash absl_city)
target_link_libraries(set_reserve PRIVATE absl_hash absl_city)


add_executable(vec_vectors vector/vectors.cpp)
add_executable(vec_remove vector/remove.cpp)
add_executable(vec_reserve vector/reserve.cpp)
add_executable(vec_push_pop vector/push_pop.cpp)

add_executable(sorted_pod sorted/pod.cpp)
add_executable(sorted_set sorted/sorted_set.cpp)
add_executable(sorted_map sorted/sorted_map.cpp)
add_executable(sorted_unset sorted/sorted_unset.cpp)
add_executable(sorted_unmap sorted/sorted_unmap.cpp)

add_executable(vec_unique unique/unique.cpp)

add_executable(fast_set vfast/fast_set.cpp)
add_executable(fast_map vfast/fast_map.cpp)
add_executable(fast_unset vfast/fast_unset.cpp)
add_executable(fast_unmap vfast/fast_unmap.cpp)
add_executable(fast_vector vfast/fast_vector.cpp)

aux_source_directory(hash/internal HASH_SRC)

add_executable(hash hash/hash.cpp ${HASH_SRC})
target_link_libraries(hash PRIVATE absl_hash absl_city glog gflags brpc)

