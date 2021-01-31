#pragma once
#include "EfiMemory.h"
#include "Bitmap.h"

class PageFrameAllocator {
    public:
    PageFrameAllocator();
    void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* Map, uint64_t mMapSize, uint64_t mMapDescSize);
    void LockPage(void* address);
    void LockPages(void* address, uint64_t pageCount);
    void* RequestPage();
    void FreePage(void* address);
    void FreePages(void* address, uint64_t pageCount);
    void UnreservePage(void* address);
    void UnreservePages(void* address, uint64_t pageCount);
    void ReservePage(void* address);
    void ReservePages(void* address, uint64_t pageCount);
    uint64_t GetFreeRAM();
    uint64_t GetUsedRAM();
    uint64_t GetReservedRAM();
    Bitmap PageBitmap = Bitmap(NULL, 0);

    private:
    void InitBitmap(size_t size, void* bufferAddress);
};