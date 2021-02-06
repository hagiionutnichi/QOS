#include "Graphics/BasicRenderer.h"
#include "Graphics/Framebuffer.h"
#include "FileFormats/PSF_1.h"
#include "FileFormats/BMP.h"
#include "math.h"
#include "numstring.h"
#include "Memory/EfiMemory.h"
#include "Memory/memory.h"
#include "Memory/Bitmap.h"
#include "Memory/PageFrameAllocator.h"
#include "Memory/PageTableManager.h"

struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

uint8_t bytes[20];
extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
extern "C" void _start(BootInfo* bootInfo, BMP_IMAGE* albie)  {

    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);
    renderer.SetCursor(0, 0);
    renderer.DrawBMP(albie, 0, 0);

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    for (int i = 0; i < mMapEntries; i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescSize));
        renderer.CursorPosition = {0, renderer.CursorPosition.Y + 16};
        renderer.Print(EFI_MEMORY_TYPE_STRINGS[desc->type]);
        renderer.Print(" ");
        renderer.Print(to_string(desc->numPages * 4096 / 1024), 0xffffff00);
        renderer.Print(" KB", 0xffffff00);
    }

    renderer.CursorPosition = {320, 48};
    renderer.Print(to_string(GetMemorySize(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize)));

    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    PageTableManager pageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t+= 0x1000){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    pageTableManager.MapMemory((void*)0x600000000, (void*)0x80000);

    uint64_t* test = (uint64_t*)0x600000000;
    *test = 26;
    
    renderer.CursorPosition = {320, 64};
    renderer.Print(to_string(*test));
}