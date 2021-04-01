cmake_minimum_required(VERSION 3.14)
project(FileSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp)
target_include_directories(main PRIVATE ./)

endif()