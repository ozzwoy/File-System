#include "Descriptor.h"
#include "../utils/Utils.h"

Descriptor::Descriptor() {
    file_size = -1;
    blocks_indices = new int[3];
}

Descriptor::~Descriptor() {
    delete[] blocks_indices;
}

void Descriptor::parse(const char *bytes) {
    file_size = Utils::bytesToInt32(bytes);
    for (int i = 0; i < 3; i++) {
        blocks_indices[i] = Utils::bytesToInt32(bytes + 4 * (i + 1));
    }
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

void Descriptor::copyBytes(char *buffer) const {
    char *file_size_bytes = Utils::intToBytes(file_size);
    for (int i = 0; i < 4; i++) {
        buffer[i] = file_size_bytes[i];
    }
    delete file_size_bytes;

    for (int i = 0; i < 3; i++) {
        char *index_bytes = Utils::intToBytes(blocks_indices[i]);
        for (int j = 0; j < 4; j++) {
            buffer[4 * (i + 1) + j] = index_bytes[j];
        }
        delete index_bytes;
    }
}