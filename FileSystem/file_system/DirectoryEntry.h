#ifndef FILE_SYSTEM_DIRECTORYENTRY_H
#define FILE_SYSTEM_DIRECTORYENTRY_H


class DirectoryEntry {
private:
    char *file_name;
    int descriptor_index;

public:
    DirectoryEntry();

    virtual ~DirectoryEntry();

    void parse(const char *bytes);

    char* getFileName() const;

    int getDescriptorIndex() const;

    void setFileName(const char *new_file_name);

    void setDescriptorIndex(int new_descriptor_index);

    char* toBytes() const;
};


#endif //FILE_SYSTEM_DIRECTORYENTRY_H
