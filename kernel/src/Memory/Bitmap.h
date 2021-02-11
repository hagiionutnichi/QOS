#pragma once
#include <stddef.h>
#include <stdint.h>

class Bitmap {
    public:
    Bitmap(uint8_t* Buffer, size_t size);
    bool operator[](uint64_t index);
    bool Set(uint64_t index, bool value);
    bool SetByte(uint64_t index, uint8_t value);
    size_t GetSize();
    uint8_t* mBuffer;

    private:
    size_t mSize;
};