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

    PageFrameAllocator newAllocator;
    newAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    newAllocator.LockPages(&_KernelStart, kernelPages);

    renderer.CursorPosition = {370, renderer.CursorPosition.Y + 16};
    renderer.Print("Free RAM: ");
    renderer.Print(to_string(newAllocator.GetFreeRAM() / 1024));
    renderer.Print(" KB ");
    renderer.CursorPosition = {370, renderer.CursorPosition.Y + 16};

    renderer.Print("Used RAM: ");
    renderer.Print(to_string(newAllocator.GetUsedRAM() / 1024));
    renderer.Print(" KB ");
    renderer.CursorPosition = {370, renderer.CursorPosition.Y + 16};

    renderer.Print("Reserved RAM: ");
    renderer.Print(to_string(newAllocator.GetReservedRAM() / 1024));
    renderer.Print(" KB ");
    renderer.CursorPosition = {370, renderer.CursorPosition.Y + 16};

    for (int t = 0; t < 20; t++){
        void* address = newAllocator.RequestPage();
        renderer.Print(to_hstring((uint64_t)address));
        renderer.CursorPosition = {370, renderer.CursorPosition.Y + 16};
    }
    return ;
}