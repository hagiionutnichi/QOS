#include "kernelUtil.h"

CLI c = CLI();
extern "C" void _start(BootInfo* bootInfo, BMP_IMAGE* albie)  {
    KernelInfo kernelInfo = InitialiseKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->CursorPosition = {0, 0};
    GlobalRenderer->Print("Kernel Initialised");

    // GlobalCLI = &c;
    // GlobalCLI->start();
    // cli.execute("albie", albie);
    GlobalRenderer->NewLine();
    GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.checksum);
    // GlobalRenderer->NewLine();
    // GlobalRenderer->Print(to_string((uint64_t)bootInfo->rsdp->length));
    GlobalRenderer->NewLine();
    GlobalRenderer->Print(to_string((uint64_t)bootInfo->rsdp_ext->rsdp.revision));
    while(true);
}