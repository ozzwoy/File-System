#include "Descriptor.h"
#include "../utils/NumericUtils.h"
#include <algorithm>

Descriptor::Descriptor() {
    file_size = -1;
    blocks_indices = new int[NUM_OF_BLOCKS];
    std::fill(blocks_indices, blocks_indices + NUM_OF_BLOCKS, -1);
}

Descriptor::~Descriptor() {
    delete[] blocks_indices;
}

Descriptor::Descriptor(Descriptor const &other) {
    file_size = other.file_size;
    blocks_indices = new int[NUM_OF_BLOCKS];
    std::copy(other.blocks_indices, other.blocks_indices + NUM_OF_BLOCKS, blocks_indices);
}

Descriptor& Descriptor::operator=(Descriptor const &other) {
    if (this != &other) {
        file_size = other.file_size;
        std::copy(other.blocks_indices, other.blocks_indices + NUM_OF_BLOCKS, blocks_indices);
    }
    return *this;
}

Descriptor::Descriptor(Descriptor &&other) noexcept {
    file_size = other.file_size;
    blocks_indices = other.blocks_indices;
    other.blocks_indices = nullptr;
}

Descriptor& Descriptor::operator=(Descriptor &&other) noexcept {
    if (this != &other) {
        file_size = other.file_size;
        blocks_indices = other.blocks_indices;
        other.blocks_indices = nullptr;
    }
    return *this;
}

void Descriptor::parse(const char *bytes) {
    file_size = Utils::bytesToInt32(bytes);
    for (int i = 0; i < NUM_OF_BLOCKS; i++) {
        blocks_indices[i] = Utils::bytesToInt32(bytes + 4 * (i + 1));
    }
}

void Descriptor::copyBytes(char *buffer) const {
    char *file_size_bytes = new char[sizeof(int)];
    Utils::intToBytes(file_size, file_size_bytes);
    std::copy(file_size_bytes, file_size_bytes + 4, buffer);
    delete[] file_size_bytes;

    char *index_bytes = new char[sizeof(int)];
    for (int i = 0; i < NUM_OF_BLOCKS; i++) {
        Utils::intToBytes(blocks_indices[i], index_bytes);
        std::copy(index_bytes, index_bytes + 4, buffer + 4 * (i + 1));
    }
    delete[] index_bytes;
}

void Descriptor::clear() {
    file_size = -1;
    std::fill(blocks_indices, blocks_indices + NUM_OF_BLOCKS, -1);
}

bool Descriptor::isFree() const {
    return file_size == -1;
}

int Descriptor::getFileSize() const {
    return file_size;
}

int Descriptor::getBlockIndex(int i) const {
    return blocks_indices[i];
}

void Descriptor::setFileSize(int new_file_size) {
    file_size = new_file_size;
}

void Descriptor::setBlockIndex(int i, int value) {
    blocks_indices[i] = value;
}