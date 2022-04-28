add_executable(spsc spsc.cpp)
target_link_libraries(spsc cds-s tbb12 boost_thread-mt)
add_executable(mpmc mpmc.cpp)
target_link_libraries(mpmc cds-s tbb12 boost_thread-mt)
