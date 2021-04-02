cmake_minimum_required(VERSION 3.17)
project(file_system)

set(CMAKE_CXX_STANDARD 14)

add_executable(file_system test.cpp FileSystem/FileSystem.h FileSystem/Utils.h FileSystem/FileSystem.cpp)