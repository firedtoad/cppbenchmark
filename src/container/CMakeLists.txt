find_package(absl REQUIRED)

add_executable(boost_multi_index_map boost/multi_index_map.cpp)
add_executable(boost_multi_index_map_string boost/multi_index_map_string.cpp)
#add_executable(boost_multi_index_ranked boost/multi_index_ranked.cpp)
add_executable(boost_multi_index_set boost/multi_index_set.cpp)
add_executable(boost_multi_index_set_string boost/multi_index_set_string.cpp)

add_executable(list_pmr list/pmr.cpp)
add_executable(list_insert list/insert.cpp)
add_executable(list_remove list/remove.cpp)
add_executable(list_range list/range.cpp)
add_executable(list_splice list/splice.cpp)

add_executable(map_find_hetero map/find_hetero.cpp)
add_executable(map_find_order map/order_find.cpp)
add_executable(map_erase_order map/order_erase.cpp)
add_executable(map_range_order map/order_range.cpp)
add_executable(map_find_unordered map/unorder_find.cpp)
add_executable(map_find_unordered_smart map/unordered_find_smart.cpp)
add_executable(map_range_unordered map/unorder_range.cpp)
add_executable(map_erase_unordered map/unorder_erase.cpp)
add_executable(map_decay map/decay.cpp)
add_executable(map_reserve map/reserve.cpp)
add_executable(map_pmr_order map/order_pmr.cpp)
add_executable(map_pmr_unorder map/unorder_pmr.cpp)

add_executable(map_combine map/combine.cpp)

target_link_libraries(map_find_hetero PRIVATE absl::flat_hash_map)
target_link_libraries(map_find_order PRIVATE absl::flat_hash_map)
target_link_libraries(map_erase_order PRIVATE absl::flat_hash_map)
target_link_libraries(map_range_order PRIVATE absl::flat_hash_map)
target_link_libraries(map_find_unordered PRIVATE absl::flat_hash_map)
target_link_libraries(map_find_unordered_smart PRIVATE absl::flat_hash_map)
target_link_libraries(map_range_unordered PRIVATE absl::flat_hash_map gflags brpc.a protobuf.a ssl.a crypto.a)
target_link_libraries(map_erase_unordered PRIVATE absl::flat_hash_map)
target_link_libraries(map_decay PRIVATE absl::flat_hash_map)
target_link_libraries(map_reserve PRIVATE absl::flat_hash_map)



add_executable(set_find_order set/order_find.cpp)
add_executable(set_range_order set/order_range.cpp)
add_executable(set_erase_order set/order_erase.cpp)
add_executable(set_find_unordered set/unorder_find.cpp)
add_executable(set_range_unordered set/unorder_range.cpp)
add_executable(set_erase_unordered set/unorder_erase.cpp)
add_executable(set_reserve set/reserve.cpp)
add_executable(set_pmr_order set/order_pmr.cpp)
add_executable(set_pmr_unordered set/unorder_pmr.cpp)

target_link_libraries(set_find_order PRIVATE absl::flat_hash_set)
target_link_libraries(set_range_order PRIVATE absl::flat_hash_set)
target_link_libraries(set_erase_order PRIVATE absl::flat_hash_set)
target_link_libraries(set_find_unordered PRIVATE absl::flat_hash_set)
target_link_libraries(set_range_unordered PRIVATE absl::flat_hash_set)
target_link_libraries(set_erase_unordered PRIVATE absl::flat_hash_set)
target_link_libraries(set_reserve PRIVATE absl::flat_hash_set)


add_executable(vec_dimension vector/dimension.cpp)
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

add_executable(value_variant_any value/variant_any.cpp)

add_executable(fast_set vfast/fast_set.cpp)
add_executable(fast_map vfast/fast_map.cpp)
add_executable(fast_unset vfast/fast_unset.cpp)
add_executable(fast_unmap vfast/fast_unmap.cpp)
add_executable(fast_vector vfast/fast_vector.cpp)

aux_source_directory(hash/internal HASH_SRC)

add_executable(hash hash/hash.cpp ${HASH_SRC})
add_executable(hasher_find hash/hasher_find.cpp ${HASH_SRC})
target_link_libraries(hash PRIVATE absl::flat_hash_map gflags brpc.a protobuf.a ssl.a crypto.a)
target_link_libraries(hasher_find PRIVATE absl::flat_hash_map gflags brpc.a protobuf.a ssl.a crypto.a)

