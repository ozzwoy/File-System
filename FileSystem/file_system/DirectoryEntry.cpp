#include <cstring>
#include "DirectoryEntry.h"
#include "utils/Utils.h"

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

char* DirectoryEntry::getFileName() const {
    char *copy = new char[5];

    for (size_t i = 0; i < 5; i++) {
        copy[i] = file_name[i];
    }

    return copy;
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

char* DirectoryEntry::toBytes() const {
    char *bytes = new char[8];

    for (size_t i = 0; i < 4; i++) {
        bytes[i] = file_name[i];
    }

    char *descriptor_index_bytes = Utils::intToBytes(descriptor_index);
    for (size_t i = 0; i < 4; i++) {
        bytes[4 + i] = descriptor_index_bytes[i];
    }
    delete[] descriptor_index_bytes;

    return bytes;
}
