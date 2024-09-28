add_executable(pb_reserve reserve.cpp report.pb.cc)
add_executable(pb_tricks tricks.cpp report.pb.cc)
#add_executable(multi_index_pb multi_index_pb.cpp report.pb.cc)

target_link_libraries(pb_reserve PRIVATE protobuf tcmalloc)
target_link_libraries(pb_tricks PRIVATE protobuf tcmalloc)
#target_link_libraries(multi_index_pb PRIVATE protobuf tcmalloc)

#target_compile_options(pb_reserve PRIVATE -fsanitize=thread)
#target_link_options(pb_reserve PRIVATE -fsanitize=thread)