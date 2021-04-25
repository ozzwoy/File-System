#ifndef FILE_SYSTEM_FILESYSTEM_H
#define FILE_SYSTEM_FILESYSTEM_H


#include <vector>
#include <string>
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
        int reserved_block_index = -1;
        Descriptor descriptor;
    };

    Entry entries[4];
};

class FileSystem {
private:
    OFT oft;
    IOSystem io_system;
    BitMap bitMap;

public:
	explicit FileSystem();
    ~FileSystem();

	void createFile(const char* file_name);
	void destroyFile(const char* file_name);
	int open(const char* file_name);
	void close(int index);
	int read(int index, char* mem_area, int count);
	int write(int index, const char* mem_area, int count);
	void lseek(int index, int pos);
	[[nodiscard]] std::vector<std::string> directory() const;

    bool init(const char *path);
    void save(const char *path);

private:
    void checkOFTIndex(int index) const;
    [[nodiscard]] Descriptor getDescriptor(int oft_entry_index) const;
    void saveDescriptor(OFT::Entry const &entry);
    void saveBitmap();
    void saveCurrentBlock(OFT::Entry const &entry);
    int reserveBlock(OFT::Entry &entry);
    void freeReservation(OFT::Entry &entry);
    void loadBlock(OFT::Entry &entry, int relative_block_index);
    void replaceCurrentBlock(OFT::Entry &entry, int relative_block_index);
    void closeAllFiles();
};


#endif //FILE_SYSTEM_FILESYSTEM_H
