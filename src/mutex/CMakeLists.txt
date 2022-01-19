add_executable(mutex BenchmarkMutex.cpp)
target_compile_features(mutex PRIVATE cxx_std_17)
target_link_libraries(mutex  synchronization)


