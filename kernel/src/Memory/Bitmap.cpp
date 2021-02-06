#include "Bitmap.h"

Bitmap::Bitmap(uint8_t* Buffer, size_t size) {
    this->mBuffer = Buffer;
    this->mSize = size;
}
bool Bitmap::operator[](uint64_t index) {
    if(index > mSize * 8) return false;

    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    if((mBuffer[byteIndex] & bitIndexer) > 0)
        return true;
    return false;
}
bool Bitmap::Set(uint64_t index, bool value) {
    if(index > mSize * 8) return false; 
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    mBuffer[byteIndex] &= ~bitIndexer;
    if(value)
        mBuffer[byteIndex] |= bitIndexer;
    return true;
}

bool Bitmap::SetByte(uint64_t index, uint8_t value) {
    if(index > mSize * 8) return false;
    uint64_t byteIndex = index / 8;
    mBuffer[byteIndex] = value;
    return true;
}

size_t Bitmap::GetSize() {
    return mSize;
}