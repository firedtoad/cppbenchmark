
add_executable(coroutine generator.cpp)
target_compile_features(coroutine PRIVATE cxx_std_20)


add_executable(fiber_boost fiber/boost/boost.cpp)
target_link_libraries(fiber_boost PRIVATE  boost_coroutine boost_context boost_fiber)
set(FOLLY_DIR "/usr/local/lib/cmake/folly")
find_package(FOLLY REQUIRED)
message("FOLLY_INCLUDE_DIR: ${FOLLY_INCLUDE_DIR}")
include_directories(${FOLLY_INCLUDE_DIR})

find_package(gflags REQUIRED)

add_executable(fiber_folly fiber/folly/folly.cpp)
target_compile_options(fiber_folly PRIVATE -fnew-alignment=8)
#target_link_libraries(fiber_folly PRIVATE ${FOLLY_LIBRARIES})

