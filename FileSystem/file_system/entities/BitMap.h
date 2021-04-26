#ifndef FILE_SYSTEM_BITMAP_H
#define FILE_SYSTEM_BITMAP_H


#include "Entity.h"

class BitMap : public Entity {
public:
    static const int BITMAP_SIZE = 8;

private:
    int* bitmap;

public:
    BitMap();
    ~BitMap() override;

    // Define copy and move operations
    BitMap(BitMap const &other);
    BitMap& operator=(BitMap const &other);
    BitMap(BitMap &&other) noexcept;
    BitMap& operator=(BitMap &&other) noexcept;

    void parse(const char *bytes) override;
    void copyBytes(char *buffer) const override;
    void clear() override;

    [[nodiscard]] bool isBitSet(size_t bit) const;
    void setBitValue(size_t bit, bool value);
    void setBit(size_t bit);
    void resetBit(size_t bit);
    [[nodiscard]] int findZeroBit() const;
};


#endif //FILE_SYSTEM_BITMAP_H
