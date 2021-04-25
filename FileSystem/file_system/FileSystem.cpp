#include "FileSystem.h"
#include "entities/DirectoryEntry.h"
#include "utils/BlockParser.h"
#include "../io_system/IOSystem.h"
#include <stdexcept>
#include <iostream>
#include <cstring>


FileSystem::FileSystem(IOSystem &io_system, bool is_initialized = false) {
    this->io_system = io_system;

    if (is_initialized) {
        char *directory_block = new char[64];
        io_system.readBlock(1, directory_block);
        oft.entries[0].block = directory_block;
        oft.entries[0].current_position = 0;
        return;
    }

    char *bitmap_block = new char[64];
    io_system.readBlock(0, bitmap_block);
    bitMap.parse(bitmap_block);
    for (int i = 0; i < 10; i++) {
        bitMap.setBitValue(i,true);
    }
    bitMap.copyBytes(bitmap_block);
    io_system.writeBlock(0, bitmap_block);
    delete[] bitmap_block;

    char *directory_block = new char[64];
    DirectoryEntry directory_entry;
    for (int directoryIndexInBlock = 0; directoryIndexInBlock < 8; directoryIndexInBlock++) {
        directory_entry.copyBytes(directory_block + directoryIndexInBlock * 8);
    }
    for(int directoryBlocksIndex = 1; directoryBlocksIndex < 4; directoryBlocksIndex++) {
        io_system.writeBlock(directoryBlocksIndex, directory_block);
    }
    oft.entries[0].block = directory_block;
    oft.entries[0].current_position = 0;

    char *descriptor_block = new char[64];
    Descriptor descriptor;
    for (int descriptorIndexInBlock = 0; descriptorIndexInBlock < 4; descriptorIndexInBlock++) {
        descriptor.copyBytes(descriptor_block + descriptorIndexInBlock * 16);
    }
    for(int descriptorsBlocksIndex = 4; descriptorsBlocksIndex < 10; descriptorsBlocksIndex++) {
        io_system.writeBlock(descriptorsBlocksIndex, descriptor_block);
    }
    delete[] descriptor_block;
}

FileSystem::~FileSystem() {
    for (OFT::Entry &entry : oft.entries) {
        delete[] entry.block;
    }
}

void FileSystem::createFile(const char* file_name) {
    const int descriptor_size = 16;

    // if file name is incorrect
    if (strlen(file_name) > 4 ){
        throw std::length_error("File name must contain up to 4 characters.");
    }

    // find a free file descriptor
    int free_descriptor_index = -1;
    char *descriptor_block = new char[descriptor_size];
    Descriptor descriptor;

    for (int block_index = 4; block_index < 10 && free_descriptor_index == -1; block_index++) {
        io_system.readBlock(block_index, descriptor_block);
        for (int i = 0; i < 4; i++) {
            descriptor.parse(descriptor_block + i * descriptor_size);
            if (descriptor.isFree()){
                free_descriptor_index = block_index * 4 + i;
                descriptor.setFileSize(0);
                break;
            }
        }
    }

    // if there is no free file descriptor
    if (free_descriptor_index == -1) {
        throw std::length_error("Cannot create more than 24 files.");
    }

    // find a free directory entry
    int free_directory_entry_index = -1;
    DirectoryEntry directory_entry;
    char *entry_block = new char[64];

    for (int i = 1; i < 4; i++) {
        io_system.readBlock(i, entry_block);
        for (int j = 0; j < 8; j++) {
            directory_entry.parse(entry_block + j * 8);

            if (free_directory_entry_index == -1 && directory_entry.isFree()) {
                io_system.readBlock(i, entry_block);
                free_directory_entry_index = j;
                directory_entry.setFileName(file_name);
                directory_entry.setDescriptorIndex(free_descriptor_index);
            } else {
                char file_name2[5];
                directory_entry.copyFileName(file_name2);
                if (std::strcmp(file_name2, file_name) == 0) {
                    char message[100];
                    std::sprintf(message, "File with name \"%s\" already exists.", file_name);
                    throw std::invalid_argument(message);
                }
            }
        }
    }

    if (free_directory_entry_index == -1) {
        throw std::length_error("Cannot create more than 24 files.");
    }

    // fill descriptor
    descriptor.copyBytes(descriptor_block + (free_descriptor_index % 4) * descriptor_size);
    io_system.writeBlock(free_descriptor_index / 4 + 4, descriptor_block);
    delete[] descriptor_block;

    // fill directory entry
    directory_entry.copyBytes(entry_block + free_directory_entry_index * 8);
    io_system.writeBlock(free_directory_entry_index / 8 + 1, entry_block);
    delete[] entry_block;
}

void FileSystem::destroyFile(const char* file_name) {
    const int descriptor_size = 16;

    // if file name is incorrect
    if (strlen(file_name) > 4 ){
        throw std::length_error("File name must contain up to 4 characters.");
    }

    // find the file descriptor by searching the directory
    // find a directory entry
    bool found = false;
    char *temp_entry_block = new char[64];
    int descriptor_index = -1;

    for (int i = 0; i < 4 && !found; i++) {
        io_system.readBlock(i, temp_entry_block);
        for (int j = 0; j < 8; j++) {
            DirectoryEntry directory_entry;
            directory_entry.parse(temp_entry_block + j * 8);

            if (!directory_entry.isFree()) {
                char file_name2[5];
                directory_entry.copyFileName(file_name2);
                if (std::strcmp(file_name2, file_name) == 0) {
                    found = true;
                    descriptor_index = directory_entry.getDescriptorIndex();
                    //remove the directory entry
                    DirectoryEntry empty_entry;
                    empty_entry.copyBytes(temp_entry_block + j * 8);
                    io_system.writeBlock(i, temp_entry_block);
                    break;
                }
            }
        }
    }
    delete[] temp_entry_block;

    // if file was not found
    if (!found) {
        char message[100];
        std::sprintf(message, "File with name \"%s\" does not exist.", file_name);
        throw std::invalid_argument(message);
    }

    Descriptor descriptor;
    char *temp_descriptor_block = new char[descriptor_size];
    io_system.readBlock(descriptor_index / 4, temp_descriptor_block);
    descriptor.parse(temp_descriptor_block + (descriptor_index % 4) * descriptor_size);

    // free the file descriptor
    Descriptor empty_descriptor;
    empty_descriptor.copyBytes(temp_descriptor_block + (descriptor_index % 4) * descriptor_size);
    io_system.writeBlock(descriptor_index / 4 + 4, temp_descriptor_block);
    delete[] temp_descriptor_block;

    // update the bitmap to reflect the freed blocks
    for (int i = 0; i < descriptor.getFileSize() / 64; i++) {
        bitMap.resetBit(descriptor.getBlockIndex(i));
    }

    // save changes to disk (наскрізне кешування?)
    char* bitmap_block = new char[64];
    io_system.readBlock(0, bitmap_block);
    bitMap.copyBytes(bitmap_block);
    io_system.writeBlock(0, bitmap_block);
    delete[] bitmap_block;
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
            char file_name2[5];
            directory_entry.copyFileName(file_name2);
            if (std::strcmp(file_name2, file_name) == 0) {
                found = true;
                descriptor_index = directory_entry.getDescriptorIndex();
                break;
            }
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
        OFT::Entry entry = oft.entries[oft_index];
        entry.descriptor_index = descriptor_index;
        entry.descriptor = getDescriptor(oft_index);
        entry.current_position = 0;
        entry.block = new char[64];
        replaceCurrentBlock(entry, 0);
    }

    return oft_index;
}

void FileSystem::close(int index) {
    checkOFTIndex(index);
    OFT::Entry entry = oft.entries[index];

    if (entry.modified) {
        saveCurrentBlock(entry);
    } else if (entry.reserved_block_index != -1) {
        bitMap.resetBit(entry.reserved_block_index);
        // TODO: bitmap backup
        entry.descriptor.setBlockIndex(entry.reserved_block_index, -1);
        entry.reserved_block_index = -1;
    }

    entry.descriptor_index = -1;
    entry.current_position = -1;
    entry.modified = false;
    delete[] oft.entries[index].block;
    entry.block = nullptr;
}

int FileSystem::read(int index, char* mem_area, int count) {
    checkOFTIndex(index);
    OFT::Entry entry = oft.entries[index];

    if (entry.current_position == -1 || entry.current_position + count > entry.descriptor.getFileSize()) {
        char message[100];

        std::sprintf(message, "Operation unsuccessful: file is out of bounds. File size: %d bytes, current position: ",
                     entry.descriptor.getFileSize());
        if (entry.current_position == -1) {
            std::sprintf(message, "EOF");
        } else {
            std::sprintf(message, "%d", entry.current_position);
        }
        std::sprintf(message, ", count: %d.", count);

        throw std::out_of_range(message);
    }

    int shift = entry.current_position % 64;
    int i = 0;

    while (true) {
        for (; i + shift < 64 && i < count; i++) {
            mem_area[i] = entry.block[i + shift];
            entry.current_position++;
        }
        // if whole block was processed and we aren't at the end of file, do read-ahead
        if (i + shift == 64 && entry.current_position != Descriptor::NUM_OF_BLOCKS * 64) {
            entry.current_position--;
            replaceCurrentBlock(entry, entry.current_position / 64);
        }
        if (i == count) {
            break;
        }
        i = 0;
        shift = 0;
    }

    return (entry.current_position == 64 * Descriptor::NUM_OF_BLOCKS) ? -1 : entry.current_position;
}

int FileSystem::write(int index, const char* mem_area, int count) {
    checkOFTIndex(index);
    OFT::Entry entry = oft.entries[index];

    if (entry.current_position == -1 || entry.current_position + count > Descriptor::NUM_OF_BLOCKS * 64) {
        throw std::out_of_range("Operation unsuccessful: file is out of bounds. Maximal size: 192 bytes.");
    }

    int shift = entry.current_position % 64;
    int i = 0;

    while (true) {
        // if current block is reserved, then use it and release reservation
        if (shift == 0 && entry.current_position == entry.descriptor.getFileSize()) {
            saveDescriptor(entry);
            entry.reserved_block_index = -1;
        }
        for (; i + shift < 64 && i < count; i++) {
            entry.block[i + shift] = mem_area[i];
            entry.current_position++;
        }
        // if whole block was processed and we aren't at the end of file, do read-ahead
        if (i + shift == 64 && entry.current_position != Descriptor::NUM_OF_BLOCKS * 64) {
            entry.current_position--;
            replaceCurrentBlock(entry, entry.current_position / 64);
        }
        if (i == count) {
            if (entry.current_position > entry.descriptor.getFileSize()) {
                entry.descriptor.setFileSize(entry.current_position);
            }
            break;
        }
        i = 0;
        shift = 0;
    }

    return (entry.current_position == 64 * Descriptor::NUM_OF_BLOCKS) ? -1 : entry.current_position;
}

void FileSystem::lseek(int index, int pos) {
    checkOFTIndex(index);
    OFT::Entry entry = oft.entries[index];

    if (pos > entry.descriptor.getFileSize()) {
        char message[100];
        std::sprintf(message, "Operation unsuccessful: file is out of bounds. File size: %d bytes.",
                     entry.descriptor.getFileSize());
        throw std::out_of_range(message);
    }

    int new_block_oft_index = pos / 64;
    int current_block_oft_index = oft.entries[index].current_position / 64;
    // if we are at the end of file, then we are on the last possible block
    if (current_block_oft_index == Descriptor::NUM_OF_BLOCKS) {
        current_block_oft_index--;
    }

    if (new_block_oft_index != current_block_oft_index) {
        replaceCurrentBlock(entry, new_block_oft_index);
        // if current block was reserved, free reservation
        if (entry.reserved_block_index != -1) {
            bitMap.resetBit(entry.reserved_block_index);
            // TODO: bitmap backup
            entry.descriptor.setBlockIndex(entry.reserved_block_index, -1);
            entry.reserved_block_index = -1;
        }
    }
    entry.current_position = pos;
}

void FileSystem::directory() const {
    char *block = new char[64];
    DirectoryEntry directory_entries[8];
    Descriptor descriptors[4];
    int descriptor_block_index = 4;

    io_system.readBlock(descriptor_block_index, block);
    BlockParser::parseBlock(block, descriptors);

    for (int i = 1, j; i < 4; i++) {
        io_system.readBlock(i, block);
        BlockParser::parseBlock(block, directory_entries);

        for (DirectoryEntry &directory_entry : directory_entries) {
            if (!directory_entry.isFree()) {
                char file_name[DirectoryEntry::MAX_FILE_NAME_SIZE + 1];
                directory_entry.copyFileName(file_name);
                int descriptor_index = directory_entry.getDescriptorIndex();
                j = descriptor_index % 4;

                // if descriptor doesn't belong to currently cached block
                // (then it necessarily belongs to one of the next blocks)
                if (descriptor_index / 4 != descriptor_block_index - 4)  {
                    descriptor_block_index = descriptor_index / 4 + 4;
                    io_system.readBlock(descriptor_block_index, block);
                    BlockParser::parseBlock(block, descriptors);
                }

                std::cout << file_name << " " << descriptors[j].getFileSize() << std::endl;
            }
        }
    }

    delete[] block;
}

void FileSystem::checkOFTIndex(int index) const {
    if (index < 1 || index > 3) {
        char message[100];
        std::sprintf(message, "Invalid index. File index must be an integer from 1 to 3. Provided: %d.", index);
        throw std::out_of_range(message);
    } else if (oft.entries[index].descriptor_index == -1) {
        char message[100];
        std::sprintf(message, "File with index %d wasn't opened.", index);
        throw  std::invalid_argument(message);
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

void FileSystem::saveDescriptor(OFT::Entry const &entry) {
    char *block = new char[64];
    int block_index = entry.descriptor_index / 4 + 4;
    int shift = (entry.descriptor_index % 4) * 16;

    io_system.readBlock(block_index, block);
    entry.descriptor.copyBytes(block + shift);
    io_system.writeBlock(block_index, block);
    delete[] block;
}

void FileSystem::saveCurrentBlock(OFT::Entry const &entry) {
    if (entry.descriptor.getFileSize() == 0) {
        throw std::invalid_argument("No block to save");
    }

    // save file block
    int current_block_oft_index = entry.current_position / 64;
    int block_index = entry.descriptor.getBlockIndex(current_block_oft_index);
    io_system.writeBlock(block_index, entry.block);
    // save cached descriptor
    saveDescriptor(entry);
}

int FileSystem::allocateNewBlock(OFT::Entry &entry) {
    // find first free position in descriptor
    int new_block_oft_index = -1;
    for (int i = 0; i < Descriptor::NUM_OF_BLOCKS; i++) {
        if (entry.descriptor.getBlockIndex(i) == -1) {
            new_block_oft_index = i;
        }
    }
    if (new_block_oft_index == -1) {
        throw std::out_of_range("Allocation impossible. Maximal number of blocks per file is reached.");
    }

    // find first free position in bitmap
    int absolute_block_index = bitMap.findZeroBit();
    if (absolute_block_index == -1) {
        throw std::length_error("Cannot allocate memory for file. Disk is full.");
    }
    bitMap.setBit(absolute_block_index);
    // TODO: bitmap backup
    entry.descriptor.setBlockIndex(new_block_oft_index, absolute_block_index);

    return absolute_block_index;
}

void FileSystem::replaceCurrentBlock(OFT::Entry &entry, int new_block_oft_index) {
    // save current block only if modified
    if (entry.modified) {
        saveCurrentBlock(entry);
        entry.modified = false;
    }

    // TODO: OFT.entries[0] separate processing

    // load another block (may need allocation in disk memory)
    int absolute_block_index = entry.descriptor.getBlockIndex(new_block_oft_index);
    if (absolute_block_index == -1) {
        // read-ahead with block reservation
        absolute_block_index = allocateNewBlock(entry);
        entry.reserved_block_index = absolute_block_index;
    }
    io_system.readBlock(absolute_block_index, entry.block);
    // set position at the beginning of the block
    entry.current_position = 64 * absolute_block_index;
}