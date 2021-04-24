#include <cstring>
#include "DirectoryEntry.h"
#include "../utils/Utils.h"
#include <algorithm>

DirectoryEntry::DirectoryEntry() {
    file_name = new char[MAX_FILE_NAME_SIZE + 1];
    file_name[0] = '\0';
    descriptor_index = -1;
}

DirectoryEntry::~DirectoryEntry() {
    delete[] file_name;
}

DirectoryEntry::DirectoryEntry(DirectoryEntry const &other) {
    file_name = new char[MAX_FILE_NAME_SIZE + 1];
    std::copy(other.file_name, other.file_name + MAX_FILE_NAME_SIZE + 1, file_name);
    descriptor_index = other.descriptor_index;
}

DirectoryEntry& DirectoryEntry::operator=(DirectoryEntry const &other) {
    if (this != &other) {
        std::copy(other.file_name, other.file_name + MAX_FILE_NAME_SIZE + 1, file_name);
        descriptor_index = other.descriptor_index;
    }
    return *this;
}

DirectoryEntry::DirectoryEntry(DirectoryEntry &&other) noexcept {
    file_name = other.file_name;
    descriptor_index = other.descriptor_index;
    other.file_name = nullptr;
}

DirectoryEntry& DirectoryEntry::operator=(DirectoryEntry &&other) noexcept {
    if (this != &other) {
        file_name = other.file_name;
        descriptor_index = other.descriptor_index;
        other.file_name = nullptr;
    }
    return *this;
}

void DirectoryEntry::parse(const char *bytes) {
    std::copy(bytes, bytes + MAX_FILE_NAME_SIZE, file_name);
    descriptor_index = Utils::bytesToInt32(bytes + 4);
}

void DirectoryEntry::copyBytes(char *buffer) const {
    std::copy(file_name, file_name + MAX_FILE_NAME_SIZE, buffer);

    char *descriptor_index_bytes = new char[sizeof(int)];
    Utils::intToBytes(descriptor_index, descriptor_index_bytes);
    std::copy(descriptor_index_bytes, descriptor_index_bytes + 4, buffer + 4);
    delete[] descriptor_index_bytes;
}

void DirectoryEntry::clear() {
    file_name[0] = '\0';
    descriptor_index = -1;
}

bool DirectoryEntry::isFree() const {
    return descriptor_index == -1;
}

void DirectoryEntry::copyFileName(char *buffer) const {
    std::copy(file_name, file_name + MAX_FILE_NAME_SIZE + 1, buffer);
}

int DirectoryEntry::getDescriptorIndex() const {
    return descriptor_index;
}

void DirectoryEntry::setFileName(const char *new_file_name) {
    for (int i = 0; i < MAX_FILE_NAME_SIZE; i++) {
        file_name[i] = new_file_name[i];
        if (new_file_name[i] == '\0') {
            break;
        }
    }
    file_name[MAX_FILE_NAME_SIZE] = '\0';
}

void DirectoryEntry::setDescriptorIndex(int new_descriptor_index) {
    descriptor_index = new_descriptor_index;
}