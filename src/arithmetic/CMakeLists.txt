add_executable(div div.cpp)
target_link_libraries(div PRIVATE benchmark pthread)

add_executable(divide divide.c)

add_executable(mul mul.cpp)
target_link_libraries(mul PRIVATE benchmark pthread)

