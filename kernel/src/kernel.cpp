#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo, BMP_IMAGE* albie)  {
    KernelInfo kernelInfo = InitialiseKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->CursorPosition = {0, 0};
    GlobalRenderer->Print("Kernel Initialised");

    CLI cli = CLI();
    cli.start();

    while(true);
}