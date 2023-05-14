add_executable(logging logging.cpp)
target_include_directories(
        logging PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
)

add_executable(logging_lockfree logging_lockfree.cpp)
target_include_directories(
        logging_lockfree PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lockfree/include
)