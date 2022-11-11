add_executable(spsc spsc.cpp)
target_link_libraries(spsc tbb boost_thread)
add_executable(mpmc mpmc.cpp)
target_link_libraries(mpmc tbb boost_thread)
