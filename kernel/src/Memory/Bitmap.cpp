#include "Bitmap.h"

Bitmap::Bitmap(uint8_t* Buffer, size_t size) {
    this->mBuffer = Buffer;
    this->mSize = size;
}
bool Bitmap::operator[](uint64_t index) {
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    if((mBuffer[byteIndex] & bitIndexer) > 0)
        return true;
    return false;
}
void Bitmap::Set(uint64_t index, bool value) {
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    mBuffer[byteIndex] &= ~bitIndexer;
    if(value)
        mBuffer[byteIndex] |= bitIndexer;
}

void Bitmap::SetByte(uint64_t index, uint8_t value) {
    uint64_t byteIndex = index / 8;
    mBuffer[byteIndex] = value;
}

size_t Bitmap::GetSize() {
    return mSize;
}