#ifndef FILE_SYSTEM_BLOCKPARSER_H
#define FILE_SYSTEM_BLOCKPARSER_H


#include "../entities/DirectoryEntry.h"
#include "../entities/Descriptor.h"
#include <algorithm>

class BlockParser {
public:
    static void parseBlock(const char* block, Descriptor* buffer) {
        Descriptor descriptor;
        for (int i = 0; i < 4; i++) {
            descriptor.parse(block + i * 16);
            buffer[i] = descriptor;
        }
    }

    static void copyBytes(Descriptor* descriptors, char* buffer) {
        char *descriptor_bytes = new char[16];
        for (int i = 0 ; i < 4; i++) {
            descriptors[i].copyBytes(descriptor_bytes);
            std::copy(descriptor_bytes, descriptor_bytes + 16, buffer);
        }
    }
};


#endif //FILE_SYSTEM_BLOCKPARSER_H
