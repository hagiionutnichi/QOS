#pragma once
#include <stddef.h>
#include <stdint.h>

class Bitmap {
    public:
    Bitmap(uint8_t* Buffer, size_t size);
    bool operator[](uint64_t index);
    void Set(uint64_t index, bool value);
    void SetByte(uint64_t index, uint8_t value);
    size_t GetSize();

    private:
    size_t mSize;
    uint8_t* mBuffer;
};