
#add_executable(coroutine generator.cpp)
#target_compile_features(coroutine PRIVATE cxx_std_20)


add_executable(fiber_boost fiber/boost/boost.cpp)
target_link_libraries(fiber_boost PRIVATE boost_coroutine boost_context boost_system boost_fiber)

add_executable(fiber_folly fiber/folly/folly.cpp)
target_link_libraries(fiber_folly PRIVATE folly double-conversion boost_program_options boost_context boost_filesystem boost_system boost_atomic.a gflags glog ssl crypto unwind fmt event dl )

add_executable(folly_sorted fiber/folly/sorted.cpp)
target_link_libraries(folly_sorted PRIVATE folly double-conversion boost_program_options boost_context boost_filesystem boost_system boost_atomic.a gflags glog ssl crypto unwind fmt event dl )

add_executable(fiber_brpc fiber/brpc/brpc.cpp)
target_link_libraries(fiber_brpc PRIVATE brpc.a gflags protobuf.a ssl.a crypto.a dl z pthread)