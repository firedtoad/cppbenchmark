add_subdirectory(SFMT)

find_package(absl REQUIRED)

add_executable(random random.cpp)
add_executable(engine engine.cpp)
add_executable(mt19937 mt19937.cpp)
add_executable(xorshift xorshift.cpp)

target_link_libraries(random PRIVATE absl::random_random)
target_link_libraries(engine PRIVATE absl::random_random)
target_link_libraries(mt19937 PRIVATE sfmt)