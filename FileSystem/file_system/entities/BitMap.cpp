#include "BitMap.h"

BitMap::BitMap() {
    bitmap = new int[8];
}

BitMap::~BitMap() {
    delete[] bitmap;
}

void BitMap::parse(const char *bytes) {
    for (size_t i = 0; i < 8; i++) {
        bitmap[i] = (int) (unsigned char) bytes[i];
    }
}

void BitMap::copyBytes(char *buffer) const {
    for (size_t i = 0; i < 8; i++) {
        buffer[i] = (char) bitmap[i];
    }
}

bool BitMap::isBitSet(size_t bit) const{
    return (bitmap[bit / 8] & (1 << (7 - bit % 8))) != 0;
}

void BitMap::setBitValue(size_t bit, bool value) {
    (value) ? setBit(bit) : resetBit(bit);
}

void BitMap::setBit(size_t bit) {
    bitmap[bit / 8] |= (1 << (7 - bit % 8));
}

void BitMap::resetBit(size_t bit) {
    bitmap[bit / 8] &= ~ (1 << (7 - bit % 8));
}
