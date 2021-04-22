#pragma once

#include "entities/BitMap.h"
#include "../io_system/IOSystem.h"
#include "entities/Descriptor.h"
#include "entities/DirectoryEntry.h"

struct OFT{

    struct Entry{
        int descriptor_index = -1;
        int current_position = -1;
        bool modified = false;
        char *block = nullptr;
    };

    Entry entries[4];
};

class FileSystem {
private:
    OFT oft;
    IOSystem io_system;
    BitMap bitMap;

public:
	explicit FileSystem(IOSystem &io_system);
	~FileSystem();

	void createFile(const char* file_name);
	void destroyFile(const char* file_name);
	int open(const char* file_name);
	void close(int index);
	int read(int index, char* mem_area, int count);
	int write(int index, const char* mem_area, int count);
	void lseek(int index, int pos);
	void directory() const;

private:
    void checkOFTIndex(int index) const;
    Descriptor getDescriptor(int oft_entry_index) const;
    bool replaceBlockAtOFT(int oft_entry_index, int new_block_oft_index);
};
