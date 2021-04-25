cmake_minimum_required(VERSION 3.17)
project(file_system)

set(CMAKE_CXX_STANDARD 17)
option(BUILD_TESTS "Build tests" ON)

add_executable(main main.cpp file_system/FileSystem.h file_system/FileSystem.cpp file_system/utils/NumericUtils.h
        file_system/entities/Descriptor.h file_system/entities/Descriptor.cpp file_system/entities/DirectoryEntry.h
        file_system/entities/DirectoryEntry.cpp io_system/IOSystem.h io_system/IOSystem.cpp io_system/IOUtils.h
        tests/doctest.h file_system/entities/BitMap.cpp file_system/entities/BitMap.h file_system/entities/Entity.h
        file_system/utils/BlockParser.h shell/Shell.h shell/Shell.cpp )
target_include_directories(main PRIVATE ./)

if (BUILD_TESTS)
    add_executable(tests tests/test.cpp)
    target_include_directories(tests PRIVATE ./)
endif()