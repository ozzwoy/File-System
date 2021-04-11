#ifndef FILE_SYSTEM_DESCRIPTOR_H
#define FILE_SYSTEM_DESCRIPTOR_H


class Descriptor {
private:
    int file_size;
    int *blocks_indices;

public:
    Descriptor();

    virtual ~Descriptor();

    void parse(const char *bytes);

    bool isFree() const;

    int getFileSize() const;

    int getBlockIndex(int i) const;

    void setFileSize(int new_file_size);

    void setBlockIndex(int i, int value);

    void copyBytes(char *buffer) const;
};


#endif //FILE_SYSTEM_DESCRIPTOR_H
