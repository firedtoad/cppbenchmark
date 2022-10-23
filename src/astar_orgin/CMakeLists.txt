project(AStar C CXX)
add_executable(astar_orgin spath.cpp bench_astar.cpp AStar.cpp)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -fno-omit-frame-pointer")
#target_link_libraries(astar PRIVATE absl_raw_hash_set absl_low_level_hash absl_hash)

