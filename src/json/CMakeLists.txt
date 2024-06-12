add_executable(rapid_boost rapid_boost.cpp yyjson.c)

target_compile_options(rapid_boost PRIVATE -fsanitize=address)
target_link_options(rapid_boost PRIVATE -fsanitize=address)