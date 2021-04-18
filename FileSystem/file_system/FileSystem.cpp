<<<<<<< Updated upstream
#include "FileSystem.h"

FileSystem::FileSystem() {

}

FileSystem::~FileSystem() {

}

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

=======
#include <stdexcept>
#include <iostream>
#include <cstring>
#include "FileSystem.h"
#include "entities/DirectoryEntry.h"
#include "../io_system/IOSystem.h"


FileSystem::FileSystem(IOSystem &io_system) {
    this->io_system = io_system;
    oft.entries[0].block = new char[64];
    io_system.readBlock(1, oft.entries[0].block);
    oft.entries[0].current_position=0;

    char *bitmap_block = new char[64];
    io_system.readBlock(0, bitmap_block);
    bitMap.parse(bitmap_block);
    for(int i = 0; i < 10; i++) {
        bitMap.setBitValue(i,true);
    }
    bitMap.copyBytes(bitmap_block);
    io_system.writeBlock(0, bitmap_block);
    delete[] bitmap_block;
}

FileSystem::~FileSystem() {
    for (OFT::Entry entry : oft.entries) {
        delete[] entry.block;
    }
}

int FileSystem::createFile(const char* file_name) {
    const int descriptor_size = 16;

    //if file name is incorrect
    if (strlen(file_name) > 4 ){
        throw std::length_error("File name must be less than 5 characters.");
        //return ERROR;
    }

    //find a free file descriptor
    int free_descriptor_index = -1;
    char *descriptor_block_bytes = new char[descriptor_size];
    Descriptor descriptor;

    for (int block_index = 4; block_index < 10 && free_descriptor_index == -1; block_index++) {
        io_system.readBlock(block_index, descriptor_block_bytes);
        for (int i = 0; i < 4; i++) {
            descriptor.parse(descriptor_block_bytes + i * descriptor_size);
            if (descriptor.isFree()){
                free_descriptor_index = block_index * 4 + i;
                descriptor.setFileSize(0);
                break;
            }
        }
    }

    //if there is no free file descriptor
    if (free_descriptor_index == -1){
        throw std::length_error("There is no free file descriptor.");
        //return ERROR;
    }

    //find a free directory entry
    int free_directory_entry_index = -1;
    int free_block_index = -1;
    DirectoryEntry directory_entry;
    char *temp_entry_block = new char[64];
    char *entry_block = new char[64];

    for (int i = 0; i < 4; i++) {
        io_system.readBlock(i, temp_entry_block);
        for (int j = 0; j < 8; j++) {
            DirectoryEntry temp_directory_entry;
            temp_directory_entry.parse(temp_entry_block + j * 8);

            if (free_directory_entry_index == -1 && temp_directory_entry.isFree()) {
                io_system.readBlock(i, entry_block);
                free_directory_entry_index = j;
                free_block_index = i;
                directory_entry.parse(temp_entry_block + j * 8);
                directory_entry.setFileName(file_name);
                directory_entry.setDescriptorIndex(free_descriptor_index);
            } else {
                char *file_name2 = new char[5];
                temp_directory_entry.copyFileName(file_name2);
                if (std::strcmp(file_name2, file_name) == 0) {
                    char message[100];
                    std::sprintf(message, "File with name \"%s\" already exist.", file_name);
                    throw std::invalid_argument(message);
                    //return ERROR;
                }
                delete[] file_name2;
            }
        }
    }
    delete[] temp_entry_block;

    if (free_directory_entry_index == -1) {
        throw std::length_error("There is no free directory entry.");
        //return ERROR;
    }

    //fill descriptor
    descriptor.copyBytes(descriptor_block_bytes + (free_descriptor_index % 4) * descriptor_size);
    io_system.writeBlock((int) free_descriptor_index / 4 + 4, descriptor_block_bytes);
    delete[] descriptor_block_bytes;

    //fill directory entry
    directory_entry.copyBytes(entry_block + free_directory_entry_index * 8);
    io_system.writeBlock(free_block_index, entry_block);
    delete[] entry_block;

    return 0;
}

int FileSystem::destroyFile(const char* file_name) {
    const int descriptor_size = 16;

    //if file name is incorrect
    if (strlen(file_name) > 4 ){
        throw std::length_error("File name must be less than 5 characters.");
        //return ERROR;
    }

    //find the file descriptor by searching the directory
    //find a directory entry
    bool found = false;
    char *temp_entry_block = new char[64];
    int descriptor_index = -1;

    for (int i = 0; i < 4 && !found; i++) {
        io_system.readBlock(i, temp_entry_block);
        for (int j = 0; j < 8; j++) {
            DirectoryEntry directory_entry;
            directory_entry.parse(temp_entry_block + j * 8);

            if (!directory_entry.isFree()) {
                char *file_name2 = new char[5];
                directory_entry.copyFileName(file_name2);
                if (std::strcmp(file_name2, file_name) == 0) {
                    found = true;
                    descriptor_index = directory_entry.getDescriptorIndex();
                    //Remove the directory entry
                    directory_entry = DirectoryEntry();
                    directory_entry.copyBytes(temp_entry_block + j * 8);
                    io_system.writeBlock(i, temp_entry_block);
                    break;
                }
                delete[] file_name2;
            }
        }
    }
    delete[] temp_entry_block;

    //if file was not found
    if (!found) {
        char message[100];
        std::sprintf(message, "File with name \"%s\" does not exist.", file_name);
        throw std::invalid_argument(message);
        //return ERROR;
    }

    Descriptor descriptor;
    char *temp_descriptor_block = new char[descriptor_size];
    io_system.readBlock((int) descriptor_index / 4, temp_descriptor_block);
    descriptor.parse(temp_descriptor_block + (descriptor_index % 4) * descriptor_size);

    //Update the bitmap to reflect the freed blocks
    for (int i = 0; i < descriptor.getFileSize()/64; i++) {
        bitMap.resetBit(descriptor.getBlockIndex(i));
    }

    //Free the file descriptor
    descriptor = Descriptor();
    descriptor.copyBytes(temp_descriptor_block + (descriptor_index % 4) * descriptor_size);
    io_system.writeBlock((int) descriptor_index / 4 + 4, temp_descriptor_block);
    delete[] temp_descriptor_block;

    return 0;
}

int FileSystem::open(const char *file_name) {
    int descriptor_index = -1;

    if (oft.entries[0].current_position / 64 > 0) {
        io_system.readBlock(1, oft.entries[0].block);
    }
    oft.entries[0].current_position = 0;

    bool found = false;
    DirectoryEntry directory_entry;
    for (int i = 1; i < 4 && !found; i++) {
        for (int j=0; j < 8; j++) {
            directory_entry.parse(oft.entries[0].block + j * 8);
            char *file_name2 = new char[5];
            directory_entry.copyFileName(file_name2);
            if (std::strcmp(file_name2, file_name) == 0) {
                found = true;
                descriptor_index = directory_entry.getDescriptorIndex();
                break;
            }
            delete[] file_name2;
        }
        if (i < 3 && !found) {
            io_system.readBlock(i, oft.entries[0].block);
        }
    }


    if (descriptor_index == -1) {
        char message[100];
        std::sprintf(message, "File with name \"%s\" doesn't exist.", file_name);
        throw std::invalid_argument(message);
    }

    int oft_index = 1;
    while (oft.entries[oft_index].descriptor_index != -1 && oft_index < 4) {
        oft_index++;
    }

    if (oft_index == 4) {
        throw std::length_error("Can't open one more file.");
    } else {
        oft.entries[oft_index].descriptor_index = descriptor_index;
        oft.entries[oft_index].current_position = 0;
        oft.entries[oft_index].block = new char[64];
        loadNewBlockToOFT(oft_index, 0);
    }

    return oft_index;
}

void FileSystem::close(int index) {
    checkOFTIndex(index);

    if (oft.entries[index].modified) {
        Descriptor descriptor;
        char *descriptors_block = new char[64];
        int shift = oft.entries[index].descriptor_index % 4;
        io_system.readBlock(oft.entries[index].descriptor_index / 4, descriptors_block);
        descriptor.parse(descriptors_block + shift * 4);

        int relative_block_num = oft.entries[index].current_position / 64;
        int absolute_block_num = descriptor.getBlockIndex(relative_block_num);
        io_system.writeBlock(absolute_block_num, oft.entries[index].block);

        descriptor.setFileSize(oft.entries[index].current_position);
        int descriptor_num = oft.entries[index].descriptor_index % 4;
        descriptor.copyBytes(descriptors_block + descriptor_num * 16);
        io_system.writeBlock(oft.entries[index].descriptor_index / 4, descriptors_block);
    }

    oft.entries[index].descriptor_index = -1;
    oft.entries[index].current_position = -1;
    oft.entries[index].modified = false;
    delete[] oft.entries[index].block;
    oft.entries[index].block = nullptr;
}

int FileSystem::read(int index, char* mem_area, int count) {
    checkOFTIndex(index);

    OFT::Entry entry = oft.entries[index];

    if (entry.current_position == -1 || entry.current_position + count > 3 * 64) {
        throw std::out_of_range("File is out of bounds. Maximal size: 192 bytes.");
    }

    int shift = entry.current_position % 64;
    int i = 0;

    while (true) {
        for (; i + shift < 64 && i < count; i++) {
            mem_area[i] = entry.block[i + shift];
            entry.current_position++;
        }

        if (entry.current_position == 64 * 3) {
            entry.current_position = -1;
            break;
        }
        if (i + shift == 64) {
            loadNewBlockToOFT(index, entry.current_position / 64);
        }
        if (i == count) {
            break;
        }

        i = 0;
        shift = 0;
    }

    return entry.current_position;
}

int FileSystem::write(int index, const char* mem_area, int count) {
    checkOFTIndex(index);

    OFT::Entry entry = oft.entries[index];

    if (entry.current_position == -1 || entry.current_position + count > 3 * 64) {
        throw new std::out_of_range("File is out of bounds. Maximal size: 192 bytes.");
    }

    int shift = entry.current_position % 64;
    int i = 0;

    while (true) {
        for (; i + shift < 64 && i < count; i++) {
            entry.block[i + shift] = mem_area[i];
            entry.current_position++;
        }

        if (entry.current_position == 64 * 3) {
            entry.current_position = -1;
            break;
        }

        if (i + shift == 64) {
            loadNewBlockToOFT(index, entry.current_position / 64);
        }

        if (i == count) {
            break;
        }

        i = 0;
        shift = 0;
    }

    return entry.current_position;
}

void FileSystem::lseek(int index, int pos) {
    checkOFTIndex(index);
    Descriptor descriptor = getDescriptor(index);

    if (pos > descriptor.getFileSize()) {
        char message[100];
        std::sprintf(message, "File is out of bounds. Current size: %d bytes.", descriptor.getFileSize());
        throw std::out_of_range(message);
    }

    loadNewBlockToOFT(index, pos / 64);
}

int FileSystem::directory() const {

    char *block = new char[64];

    for (int i = 0; i < 4; i++) {
        io_system.readBlock(i, block);
        for (int j = 0; j < 8; j++) {
            DirectoryEntry temp_directory_entry;
            temp_directory_entry.parse(block + j * 8);

            if (!temp_directory_entry.isFree()) {
                char *file_name = new char[5];
                temp_directory_entry.copyFileName(file_name);
                int descriptor_index = temp_directory_entry.getDescriptorIndex();
                Descriptor descriptor = getDescriptor(descriptor_index);
                std::cout << file_name << " " << descriptor.getFileSize() << std::endl;
                delete[] file_name;
            }
        }
    }

    delete[] block;
    return 0;
}

void FileSystem::checkOFTIndex(int index) const {
    if (index < 1 || index > 3) {
        throw new std::out_of_range("Invalid index. Should be an integer from 1 to 3.");
    } else if (oft.entries[index].descriptor_index == -1) {
        throw  std::invalid_argument("File wasn't opened.");
    }
}

Descriptor FileSystem::getDescriptor(int oft_entry_index) const {
    Descriptor descriptor;
    char *descriptors_block = new char[64];
    int shift = oft.entries[oft_entry_index].descriptor_index % 4;

    io_system.readBlock(oft.entries[oft_entry_index].descriptor_index / 4, descriptors_block);
    descriptor.parse(descriptors_block + shift * 4);

    delete[] descriptors_block;
    return descriptor;
}

bool FileSystem::loadNewBlockToOFT(int oft_entry_index, int relative_block_index) {
    OFT::Entry entry = oft.entries[oft_entry_index];
    int current_relative_block_index = entry.current_position / 64;

    if (oft_entry_index == 0) {
        if (entry.modified) {
            io_system.writeBlock(current_relative_block_index + 1, entry.block);
        }
        io_system.readBlock(relative_block_index, oft.entries[oft_entry_index].block);
        entry.current_position = 64 * relative_block_index;
        entry.modified = false;
        return true;
    }

    Descriptor descriptor = getDescriptor(oft_entry_index);
    if (entry.modified) {
        int block_index = descriptor.getBlockIndex(current_relative_block_index);
        io_system.writeBlock(block_index, entry.block);
    }
    int block_index = descriptor.getBlockIndex(relative_block_index);
    io_system.readBlock(block_index, entry.block);
    entry.current_position = 64 * relative_block_index;
    entry.modified = false;

    return true;
}
>>>>>>> Stashed changes
