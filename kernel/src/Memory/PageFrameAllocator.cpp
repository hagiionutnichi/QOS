#include "PageFrameAllocator.h"
#include "EfiMemory.h"
#include "memory.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool initialized = false;
PageFrameAllocator GlobalAllocator;

void PageFrameAllocator::ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* Map, uint64_t mMapSize, uint64_t mMapDescSize) {
    if(initialized) return;
    initialized = true;

    uint64_t mMapEntries = mMapSize / mMapDescSize;

    void* largestFreeSegment = NULL;
    size_t largestFreeSegmentSize = 0;

    for(uint64_t i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t) Map + (i * mMapDescSize));
        if(desc->type == 7) {
            if(desc->numPages * 4096 > largestFreeSegmentSize) {
                largestFreeSegmentSize = desc->numPages * 4096;
                largestFreeSegment = desc->physAddr;
            }
        }
    }

    uint64_t memorySize = GetMemorySize(Map, mMapSize, mMapDescSize);
    freeMemory = memorySize;

    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;
    InitBitmap(bitmapSize, largestFreeSegment);

    LockPages(&PageBitmap, PageBitmap.GetSize() / 4096 + 1);

    for (int i = 0; i < mMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)Map + (i * mMapDescSize));
        if (desc->type != 7){ // not efiConventionalMemory
            ReservePages(desc->physAddr, desc->numPages);
        }
    }

}

void PageFrameAllocator::InitBitmap(size_t size, void* bufferAddress) {
    PageBitmap = Bitmap((uint8_t *)bufferAddress, size);
    for(size_t i = 0; i < size; i++)
        PageBitmap.SetByte(i*8, 0);
}

void PageFrameAllocator::LockPage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == true) return;
    if(PageBitmap.Set(index, true)) {
        freeMemory -= 4096;
        usedMemory += 4096;
    }
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount){
    for (int t = 0; t < pageCount; t++){
        LockPage((void*)((uint64_t)address + (t * 4096)));
    }
}

void* PageFrameAllocator::RequestPage(){
    for (uint64_t index = 0; index < PageBitmap.GetSize() * 8; index++){
        if (PageBitmap[index] == true) continue;
        LockPage((void*)(index * 4096));
        return (void*)(index * 4096);
    }

    return NULL; // Page Frame Swap to file
}

void PageFrameAllocator::FreePage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == false) return;
    if(PageBitmap.Set(index, false)) {
        freeMemory += 4096;
        usedMemory -= 4096;
    }
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount){
    for (int t = 0; t < pageCount; t++){
        FreePage((void*)((uint64_t)address + (t * 4096)));
    }
}

void PageFrameAllocator::UnreservePage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == false) return;
    if(PageBitmap.Set(index, false)){
        freeMemory += 4096;
        reservedMemory -= 4096;
    }
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount){
    for (int t = 0; t < pageCount; t++){
        UnreservePage((void*)((uint64_t)address + (t * 4096)));
    }
}

void PageFrameAllocator::ReservePage(void* address){
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == true) return;
    if(PageBitmap.Set(index, true)){
        freeMemory -= 4096;
        reservedMemory += 4096;
    }
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount){
    for (int t = 0; t < pageCount; t++){
        ReservePage((void*)((uint64_t)address + (t * 4096)));
    }
}

uint64_t PageFrameAllocator::GetFreeRAM(){
    return freeMemory;
}
uint64_t PageFrameAllocator::GetUsedRAM(){
    return usedMemory;
}
uint64_t PageFrameAllocator::GetReservedRAM(){
    return reservedMemory;
}

PageFrameAllocator::PageFrameAllocator() {}