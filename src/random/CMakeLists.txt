add_subdirectory(SFMT)
add_executable(random random.cpp)
add_executable(engine engine.cpp)
add_executable(mt19937 mt19937.cpp)

target_link_libraries(random PRIVATE absl_random_distributions absl_random_internal_platform absl_random_internal_randen absl_random_internal_randen_hwaes absl_random_internal_randen_hwaes_impl absl_random_internal_randen_slow absl_random_internal_pool_urbg)
target_link_libraries(engine PRIVATE absl_random_distributions absl_random_internal_platform absl_random_internal_randen absl_random_internal_randen_hwaes absl_random_internal_randen_hwaes_impl absl_random_internal_randen_slow absl_random_internal_pool_urbg)
target_link_libraries(mt19937 PRIVATE sfmt)