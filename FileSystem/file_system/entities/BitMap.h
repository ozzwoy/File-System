#ifndef FILE_SYSTEM_BITMAP_H
#define FILE_SYSTEM_BITMAP_H

class BitMap {
private:
    int* bitmap;

public:
    BitMap();
    ~BitMap();

    void parse(const char *bytes);
    void copyBytes(char *buffer) const;

    bool isBitSet(size_t bit) const;
    void setBitValue(size_t bit, bool value);
    void setBit(size_t bit);
    void resetBit(size_t bit);

    int findZeroBit() const;
};


#endif //FILE_SYSTEM_BITMAP_H
