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
public:
    static const int MAX_FILE_SIZE = IOSystem::BLOCK_SIZE * Descriptor::NUM_OF_BLOCKS;
    static const int MAX_FILES_NUM = 10;

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
    static void clearOFTEntry(OFT::Entry &entry);
    int countFiles();

    void doClose(OFT::Entry &entry);
    int doRead(OFT::Entry &entry, char* mem_area, int count);
    int doWrite(OFT::Entry &entry, const char* mem_area, int count);
    void doSeek(OFT::Entry &entry, int pos);
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
