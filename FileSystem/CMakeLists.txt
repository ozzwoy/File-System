cmake_minimum_required(VERSION 3.17)
project(file_system)

set(CMAKE_CXX_STANDARD 14)

add_executable(file_system tests/test.cpp file_system/FileSystem.h file_system/FileSystem.cpp file_system/utils/Utils.h
        file_system/Descriptor.h file_system/Descriptor.cpp file_system/DirectoryEntry.cpp file_system/DirectoryEntry.h)