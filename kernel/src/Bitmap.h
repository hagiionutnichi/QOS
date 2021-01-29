#pragma once
#include <stddef.h>
#include <stdint.h>

class Bitmap {
    public:
    Bitmap(uint8_t* Buffer, size_t size);
    bool operator[](uint64_t index);
    void Set(uint64_t index, bool value);

    private:
    size_t size;
    uint8_t* Buffer;
};