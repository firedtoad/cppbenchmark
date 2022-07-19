add_executable(logging logging.cpp)
target_include_directories(
        logging PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
)