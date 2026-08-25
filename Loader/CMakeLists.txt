cmake_minimum_required(VERSION 4.3)
project(Loader)

set(CMAKE_CXX_STANDARD 20)

link_directories(lib)
include_directories(include)
include_directories(include/java)

file(GLOB_RECURSE SOURCES
        *.cpp
        *.hpp)

add_library(Loader SHARED ${SOURCES} src/export.h include/rdi/ReflectiveLoader.c)

target_link_options(Loader PRIVATE
        -Wl,--strip-all)

target_link_libraries(Loader PRIVATE
        -static-libgcc
        -static-libstdc++
        -Wl,-Bstatic
        -lstdc++
        -lpthread)

target_link_libraries(Loader PRIVATE wsock32 jvm)