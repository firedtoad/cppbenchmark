add_executable(pb_reserve reserve.cpp report.pb.cc)
add_executable(pb_tricks tricks.cpp report.pb.cc)

target_link_libraries(pb_reserve PRIVATE protobuf)
target_link_libraries(pb_tricks PRIVATE protobuf)

target_compile_options(pb_reserve PRIVATE -fsanitize=thread)
target_link_options(pb_reserve PRIVATE -fsanitize=thread)