#include "BasicRenderer.h"
#include "Framebuffer.h"
#include "PSF_1.h"
#include "BMP.h"
#include "math.h"
#include "numstring.h"
#include "EfiMemory.h"
#include "memory.h"
#include "bitmap.h"

struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

extern "C" void _start(BootInfo* bootInfo, BMP_IMAGE* albie)  {

    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);
    renderer.SetCursor(0, 0);
    for (int t = 0; t < 50; t+=1){
        renderer.Print("Hello Kernel");
    }
    
    renderer.DrawBMP(albie, 320, 200);

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

    uint8_t bytes[20];
    Bitmap bitmap = Bitmap(&bytes[0], (size_t) 20);
    bitmap.Set(0, false);
    bitmap.Set(1, true);
    bitmap.Set(2, false);
    bitmap.Set(3, true);
    bitmap.Set(4, false);
    bitmap.Set(5, true);
    bitmap.Set(6, false);
    bitmap.Set(7, true);
    bitmap.Set(8, true);
    bitmap.Set(9, true);
    bitmap.Set(10, true);
    bitmap.Set(11, true);

    for(size_t t = 0; t < 20; t++) {
        renderer.Print(bitmap[t] ? "true" : "false");
        renderer.CursorPosition.X = 320;
        renderer.CursorPosition.Y += 16;
    }
    return ;
}