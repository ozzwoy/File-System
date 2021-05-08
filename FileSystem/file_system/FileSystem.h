#ifndef FILE_SYSTEM_FILESYSTEM_H
#define FILE_SYSTEM_FILESYSTEM_H


#include <vector>
#include <string>
#include "entities/BitMap.h"
#include "../io_system/IOSystem.h"
#include "entities/Descriptor.h"
#include "entities/DirectoryEntry.h"

struct OFT{
    static const int CAPACITY = 4;

    struct Entry{
        int descriptor_index = -1;
        int current_position = -1;
        bool modified = false;
        char *block = nullptr;
        int reserved_block_index = -1;
        Descriptor descriptor;
    };

    Entry entries[CAPACITY];
};

class FileSystem {
public:
    static const int MAX_FILE_SIZE = IOSystem::BLOCK_SIZE * Descriptor::NUM_OF_BLOCKS;
    static const int MAX_FILES_NUM = 10;
    static const int MAX_FILES_OPENED = OFT::CAPACITY;
    static const int DESCRIPTORS_IN_BLOCK = IOSystem::BLOCK_SIZE / Descriptor::SIZE;
    static const int DIRECTORY_ENTRIES_IN_BLOCK = IOSystem::BLOCK_SIZE / DirectoryEntry::SIZE;
    static const int DESCRIPTORS_BLOCKS_NUM = (MAX_FILES_NUM + 1) / DESCRIPTORS_IN_BLOCK +
                                              ((MAX_FILES_NUM + 1) % DESCRIPTORS_IN_BLOCK == 0 ? 0 : 1);

private:
    OFT oft;
    IOSystem io_system;
    BitMap bitMap;
    int files_num;

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
	[[nodiscard]] std::vector<std::string> directory();

    bool init(const char *path = "");
    void save(const char *path);

private:
    void initOFTEntry(OFT::Entry &entry, int descriptor_index);
    void clearOFTEntry(OFT::Entry &entry);
    int countFiles();

    void doClose(OFT::Entry &entry);
    int doRead(OFT::Entry &entry, char* mem_area, int count);
    int doWrite(OFT::Entry &entry, const char* mem_area, int count);
    void doSeek(OFT::Entry &entry, int pos);

    void checkFileName(const char* file_name);
    void checkOFTIndex(int index) const;

    void loadDescriptor(OFT::Entry &entry);
    void saveDescriptor(OFT::Entry const &entry);
    void loadBitmap();
    void saveBitmap();

    int reserveBlock(OFT::Entry &entry);
    void freeReservation(OFT::Entry &entry);
    void loadBlock(OFT::Entry &entry, int relative_block_index);
    void saveBlock(OFT::Entry const &entry);
    void replaceBlock(OFT::Entry &entry, int relative_block_index);

    void closeAllFiles();
};


#endif //FILE_SYSTEM_FILESYSTEM_H
