#ifndef FILE_SYSTEM_DIRECTORYENTRY_H
#define FILE_SYSTEM_DIRECTORYENTRY_H


#include "Entity.h"

class DirectoryEntry : public Entity {
public:
    static const int MAX_FILE_NAME_SIZE = 4;
    static const int DESCRIPTOR_INDEX_SIZE = sizeof(int);
    static const int SIZE = MAX_FILE_NAME_SIZE + DESCRIPTOR_INDEX_SIZE;

private:
    char *file_name;
    int descriptor_index;

public:
    DirectoryEntry();
    ~DirectoryEntry() override;

    // Define copy and move operations
    DirectoryEntry(DirectoryEntry const &other);
    DirectoryEntry& operator=(DirectoryEntry const &other);
    DirectoryEntry(DirectoryEntry &&other) noexcept;
    DirectoryEntry& operator=(DirectoryEntry &&other) noexcept;

    void parse(const char *bytes) override;
    void copyBytes(char *buffer) const override;
    void clear() override;

    [[nodiscard]] bool isFree() const;
    void copyFileName(char *buffer) const;
    [[nodiscard]] int getDescriptorIndex() const;
    void setFileName(const char *new_file_name);
    void setDescriptorIndex(int new_descriptor_index);
};


#endif //FILE_SYSTEM_DIRECTORYENTRY_H
