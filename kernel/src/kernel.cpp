#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo, BMP_IMAGE* albie)  {
    KernelInfo kernelInfo = InitialiseKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->CursorPosition = {0, 0};
    GlobalRenderer->Print("Kernel Initialised");

    // asm("int $0x0e");
    int* test = (int *) 0x80000000000;
    *test = 2;

    while(true);
}