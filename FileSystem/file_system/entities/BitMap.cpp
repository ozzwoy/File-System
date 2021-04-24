#include "BitMap.h"
#include <algorithm>

BitMap::BitMap() {
    bitmap = new int[BITMAP_SIZE];
}

BitMap::~BitMap() {
    delete[] bitmap;
}

BitMap::BitMap(BitMap const &other) {
    bitmap = new int[BITMAP_SIZE];
}

BitMap& BitMap::operator=(BitMap const &other) {
    if (this != &other) {
        bitmap = new int[BITMAP_SIZE];
        std::copy(other.bitmap, other.bitmap + BITMAP_SIZE, bitmap);
    }
    return *this;
}

BitMap::BitMap(BitMap &&other) noexcept {
    bitmap = other.bitmap;
    other.bitmap = nullptr;
}

BitMap& BitMap::operator=(BitMap &&other) noexcept {
    if (this != &other) {
        bitmap = other.bitmap;
        other.bitmap = nullptr;
    }
    return *this;
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

void BitMap::clear() {
    std::fill(bitmap, bitmap + BITMAP_SIZE, 0);
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

int BitMap::findZeroBit() const {
    for (size_t i = 0; i < 64; i++) {
        if (!isBitSet(i)) return i;
    }
    return -1;
}