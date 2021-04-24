#ifndef FILE_SYSTEM_BLOCKPARSER_H
#define FILE_SYSTEM_BLOCKPARSER_H


#include "../entities/DirectoryEntry.h"
#include "../entities/Descriptor.h"

class BlockParser {
public:
    static void parseDirectoryBlock(const char* block, DirectoryEntry* buffer) {
        DirectoryEntry entry;
        for (int i = 0; i < 8; i++) {
            entry.parse(block + i * 8);
            buffer[i] = entry;
        }
    }

    static void parseDescriptorsBlock(const char* block, Descriptor* buffer) {
        Descriptor descriptor;
        for (int i = 0; i < 4; i++) {
            descriptor.parse(block + i * 16);
            buffer[i] = descriptor;
        }
    }
};


#endif //FILE_SYSTEM_BLOCKPARSER_H
