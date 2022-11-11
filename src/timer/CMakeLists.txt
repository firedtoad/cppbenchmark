cmake_minimum_required(VERSION 3.16)
project(bench_timer CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(folly_timer folly.cpp)
add_executable(boost_timer boost.cpp)
add_executable(uv_timer uv_timer.cpp)
add_executable(event_timer event_timer.cpp)
add_executable(wheel_timer wheel_timer.cpp timer.cpp)
target_link_libraries(folly_timer PRIVATE folly double-conversion boost_context gflags glog unwind fmt event dl pthread)
target_link_libraries(boost_timer PRIVATE pthread)

target_link_libraries(uv_timer PRIVATE uv pthread)
target_link_libraries(event_timer PRIVATE event pthread)
target_link_libraries(wheel_timer PRIVATE pthread)