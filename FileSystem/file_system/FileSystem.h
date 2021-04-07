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
	
	//OFT oft;
    OFT oft;
    IOSystem io_system;
    BitMap bitMap;
    DirectoryEntry directory_entry;


public:

	FileSystem(IOSystem &io_system);
	~FileSystem();


	int createFile(const char* file_name);
	int destroyFile(const char* file_name);
	int open(const char* file_name);
	int close(int index);
	int read(int index, void* mem_area, int count);
	int write(int index, void* mem_area, int count);
	int lseek(int index, int pos);
	int directory() const;

};


