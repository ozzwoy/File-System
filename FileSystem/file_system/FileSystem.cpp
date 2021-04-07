#include <cstring>
#include <iostream>
#include "FileSystem.h"
#include "entities/DirectoryEntry.h"
#include "../io_system/IOSystem.h"


FileSystem::FileSystem(IOSystem &io_system) {
    this->io_system = io_system;
    oft.entries[0].block = new char[64];
    io_system.read_block(1, oft.entries[0].block);
    oft.entries[0].current_position=0;

    char *bitmap_block = new char[64];
    io_system.read_block(0, bitmap_block);
    bitMap.parse(bitmap_block);
    for(int i = 0; i < 10; i++) {
        bitMap.setBitValue(i,true);
    }
    bitMap.copyBytes(bitmap_block);
    io_system.write_block(0, bitmap_block);
    delete[] bitmap_block;
}

FileSystem::~FileSystem() {

}




int FileSystem::createFile(const char* file_name){

    return 0;
}

int FileSystem::destroyFile(const char* file_name){

    return 0;
}

int FileSystem::open(const char *file_name){
    Descriptor file_descriptor;

    int descriptor_index = -1;
    char *file_name2 = new char[5];


    if(oft.entries[0].current_position / 64 > 0){
        io_system.read_block(1, oft.entries[0].block);
    }
    oft.entries[0].current_position = 0;

    bool found = false;
    for(int i = 1; i < 4 && !found; i++) {
        for(int j=0; j < 8; j++) {
            directory_entry.parse(oft.entries[0].block + j * 8);
            directory_entry.copyFileName(file_name2);
            if (std::strcmp(file_name2, file_name) == 0) {
                found = true;
                descriptor_index = directory_entry.getDescriptorIndex();
                break;
            }
        }
        if(i < 3 && !found) {
            io_system.read_block(i, oft.entries[0].block);
        }

    }

    if (descriptor_index == -1) {
        char message[100] = "File with name \"";
        std::strcat(message, file_name);
        std::strcat(message, "\" doesn't exist.");
        throw std::invalid_argument(message);
    }

    int oft_index = 0;
    while (oft.entries[oft_index].descriptor_index != -1 && oft_index < 4) {
        oft_index++;
    }

    if(oft_index == 4) {
        throw std::invalid_argument("Can't open one more file.");
    } else{
        oft.entries[oft_index].descriptor_index = descriptor_index;
        oft.entries[oft_index].current_position = 0;

        int shift = descriptor_index % 4;
        int block_num = descriptor_index / 4 + 4;
        char *block = new char[64];
        io_system.read_block(block_num, block);
        file_descriptor.parse(block + 16 * shift);

        io_system.read_block(file_descriptor.getBlockIndex(0), oft.entries[oft_index].block);

    }
    return oft_index;
}

int FileSystem::close(int index) {
    if (index < 0 || index > 3 || oft.entries[index].descriptor_index == -1)
    {
        throw std::invalid_argument("File wasn't opened.");
    }

    if(oft.entries[index].modified) {
        Descriptor file_descriptor;
        char *descriptors_block = new char[64];
        int shift = oft.entries[index].descriptor_index % 4;
        io_system.read_block(oft.entries[index].descriptor_index / 4, descriptors_block);
        file_descriptor.parse(descriptors_block + shift * 4);

        int relative_block_num = oft.entries[index].current_position / 64;
        int absolute_block_num = file_descriptor.getBlockIndex(relative_block_num);
        io_system.write_block(absolute_block_num, oft.entries[index].block);

        file_descriptor.setFileSize(oft.entries[index].current_position);
        int descriptor_num = oft.entries[index].descriptor_index % 4;
        file_descriptor.copyBytes(descriptors_block + descriptor_num * 16);
        io_system.write_block(oft.entries[index].descriptor_index / 4, descriptors_block);

    }else {
    }

    oft.entries[index].descriptor_index = -1;
    oft.entries[index].current_position = -1;
    oft.entries[index].modified = false;
    delete[] oft.entries[index].block;
    oft.entries[index].block = nullptr;

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

