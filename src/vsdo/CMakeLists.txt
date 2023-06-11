add_executable(vdso vdso.cpp)
#target_link_options(vdso PRIVATE -static -Wl,-Bdynamic)
#target_compile_options(vdso PRIVATE -static)
