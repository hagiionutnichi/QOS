#include "kernelUtil.h"

CLI c = CLI();
extern "C" void _start(BootInfo* bootInfo, BMP_IMAGE* albie)  {
    KernelInfo kernelInfo = InitialiseKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    // GlobalRenderer->CursorPosition = {0, 0};
    // GlobalRenderer->PrintLn("Kernel Initialised");

    // GlobalRenderer->PrintLn(to_string(bootInfo->rsdp_ext->xsdt_address));
    // GlobalRenderer->PrintLn(to_string((uint64_t)bootInfo->rsdp_ext->rsdp.revision));
    // GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.signature[0]);
    // GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.signature[1]);
    // GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.signature[2]);
    // GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.signature[3]);
    // GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.signature[4]);
    // GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.signature[5]);
    // GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.signature[6]);
    // GlobalRenderer->PrintChar(bootInfo->rsdp_ext->rsdp.signature[7]);
    // GlobalRenderer->NewLine();

    GlobalCLI = &c;
    GlobalCLI->start();
    GlobalCLI->SetBootInfo(bootInfo);
    while(true){
        ProcessMousePacket();
    }
    // cli.execute("albie", albie);
    while(true);
}