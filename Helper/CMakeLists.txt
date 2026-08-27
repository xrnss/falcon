cmake_minimum_required(VERSION 3.20)
project(Helper)

set(CMAKE_CXX_STANDARD 20)

add_library(Helper SHARED dllmain.cpp)

target_include_directories(Helper PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_property(TARGET Helper PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)