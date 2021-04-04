#pragma once

class FileSystem {
private:
	
	//OFT oft;

public:

	FileSystem();
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


