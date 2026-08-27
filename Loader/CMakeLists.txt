cmake_minimum_required(VERSION 3.20)
project(Loader)

set(CMAKE_CXX_STANDARD 17)

link_directories(lib)
include_directories(include)
include_directories(include/java)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"
)

add_library(Loader SHARED ${SOURCES} src/export.h)

#set_property(TARGET Loader PROPERTY
#        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(Loader PRIVATE wsock32 jvm)
