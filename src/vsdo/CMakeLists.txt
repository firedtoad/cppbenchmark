add_executable(vsdo vsdo.cpp)
target_link_options(vsdo PRIVATE -static -Wl,-Bdynamic)
target_compile_options(vsdo PRIVATE -static)
