add_executable(string_view string_view.cpp)
add_executable(string_split string_split.cpp)

target_link_libraries(string_split PRIVATE absl_strings)
