#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo, BMP_IMAGE* albie)  {
    KernelInfo kernelInfo = InitialiseKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;
    BasicRenderer renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);

    renderer.CursorPosition = {0, 0};
    renderer.Print("Kernel Initialised");

    while(true);
}