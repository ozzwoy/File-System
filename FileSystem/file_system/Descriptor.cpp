#include "Descriptor.h"
#include "utils/Utils.h"

Descriptor::Descriptor() {
    file_size = -1;
    blocks_indices = new int[3];
}

Descriptor::~Descriptor() {
    delete[] blocks_indices;
}

void Descriptor::parse(const char *bytes) {
    file_size = Utils::bytesToInt32(bytes);
    for (size_t i = 0; i < 3; i++) {
        blocks_indices[i] = Utils::bytesToInt32(bytes + 4 * (i + 1));
    }
}

int Descriptor::getFileSize() const {
    return file_size;
}

int Descriptor::getBlockIndex(size_t i) const {
    return blocks_indices[i];
}

void Descriptor::setFileSize(int new_file_size) {
    file_size = new_file_size;
}

void Descriptor::setBlockIndex(size_t i, int value) {
    blocks_indices[i] = value;
}

char* Descriptor::toBytes() const {
    char *bytes = new char[16];

    char *file_size_bytes = Utils::intToBytes(file_size);
    for (size_t i = 0; i < 4; i++) {
        bytes[i] = file_size_bytes[i];
    }
    delete file_size_bytes;

    for (size_t i = 0; i < 3; i++) {
        char *index_bytes = Utils::intToBytes(blocks_indices[i]);
        for (size_t j = 0; j < 4; j++) {
            bytes[4 * (i + 1) + j] = index_bytes[j];
        }
        delete index_bytes;
    }

    return bytes;
}