find_package(absl REQUIRED)
add_executable(mutex BenchmarkMutex.cpp)
add_executable(spin spin.cpp)
#target_link_libraries(mutex  synchronization)


target_link_libraries(spin PRIVATE absl::base absl::base_internal absl::spinlock_wait)