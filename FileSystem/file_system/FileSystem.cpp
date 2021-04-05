#include <cstring>
#include <iostream>
#include "FileSystem.h"
#include "entities/DirectoryEntry.h"

FileSystem::FileSystem() {

}

FileSystem::~FileSystem() {

}


struct OFT{

    struct Entry{
        int descriptor_index=-1;
        int current_position=-1;
        bool modified=false;
        char *block= nullptr;
    };

    Entry entries[4];
};

int FileSystem::createFile(const char* file_name){

    return 0;
}

int FileSystem::destroyFile(const char* file_name){

    return 0;
}

int FileSystem::open(const char* file_name){

    return 0;
}

int FileSystem::close(int index){

    return 0;
}

int FileSystem::read(int index, void* mem_area, int count){

    return 0;
}

int FileSystem::write(int index, void* mem_area, int count){

    return 0;
}


int FileSystem::lseek(int index, int pos){

    return 0;
}

int FileSystem::directory() const {

    return 0;
}

