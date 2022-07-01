add_executable(spsc spsc.cpp)
target_link_libraries(spsc cds-s tbb boost_thread)
add_executable(mpmc mpmc.cpp)
target_link_libraries(mpmc cds-s tbb boost_thread)
