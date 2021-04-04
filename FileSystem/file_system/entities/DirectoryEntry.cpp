#include <cstring>
#include "DirectoryEntry.h"
#include "../utils/Utils.h"

DirectoryEntry::DirectoryEntry() {
    file_name = new char[5];
    file_name[4] = '\0';
    descriptor_index = -1;
}

DirectoryEntry::~DirectoryEntry() {
    delete[] file_name;
}

void DirectoryEntry::parse(const char *bytes) {
    for (size_t i = 0; i < 4; i++) {
        file_name[i] = bytes[i];
    }
    descriptor_index = Utils::bytesToInt32(bytes + 4);
}

bool DirectoryEntry::isFree() const {
    return descriptor_index == -1;
}

void DirectoryEntry::copyFileName(char *buffer) const {
    for (size_t i = 0; i < 5; i++) {
        buffer[i] = file_name[i];
    }
}

int DirectoryEntry::getDescriptorIndex() const {
    return descriptor_index;
}

void DirectoryEntry::setFileName(const char *new_file_name) {
    for (size_t i = 0; i < 4; i++) {
        file_name[i] = new_file_name[i];
        if (new_file_name[i] == '\0') {
            break;
        }
    }
}

void DirectoryEntry::setDescriptorIndex(int new_descriptor_index) {
    descriptor_index = new_descriptor_index;
}

void DirectoryEntry::copyBytes(char *buffer) const {
    for (size_t i = 0; i < 4; i++) {
        buffer[i] = file_name[i];
    }

    char *descriptor_index_bytes = Utils::intToBytes(descriptor_index);
    for (size_t i = 0; i < 4; i++) {
        buffer[4 + i] = descriptor_index_bytes[i];
    }
    delete[] descriptor_index_bytes;
}