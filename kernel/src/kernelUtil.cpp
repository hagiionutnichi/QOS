#include "kernelUtil.h"
#include "GDT/gdt.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Interrupts.h"
#include "io.h"

KernelInfo kernelInfo;
PageTableManager pageTableManager  = NULL;
void PrepareMemory(BootInfo* bootInfo) {
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    pageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t+= 0x1000){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;

    //Lock the page in which the Framebuffer resides
    GlobalAllocator.LockPages((void*)fbBase, fbSize/0x1000 + 1);

    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
        pageTableManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    kernelInfo.pageTableManager = &pageTableManager;
}

IDTR idtr;
void PrepareInterrupts() {
    idtr.limit = 0x0fff;
    idtr.offset = (uint64_t)GlobalAllocator.RequestPage();

    IDTDescriptorEntry* int_PageFault = (IDTDescriptorEntry *)(idtr.offset + 0xE * sizeof(IDTDescriptorEntry));
    int_PageFault->setOffset((uint64_t)PageFault_Handler);
    int_PageFault->types_attributes = IDT_TA_InterruptGate;
    int_PageFault->selector = 0x08;

    IDTDescriptorEntry* int_DoubleFault = (IDTDescriptorEntry *)(idtr.offset + 0x8 * sizeof(IDTDescriptorEntry));
    int_DoubleFault->setOffset((uint64_t)DoubleFault_Handler);
    int_DoubleFault->types_attributes = IDT_TA_InterruptGate;
    int_DoubleFault->selector = 0x08;

    IDTDescriptorEntry* int_GeneralFault = (IDTDescriptorEntry *)(idtr.offset + 0xD * sizeof(IDTDescriptorEntry));
    int_GeneralFault->setOffset((uint64_t)GeneralProtectionFault_Handler);
    int_GeneralFault->types_attributes = IDT_TA_InterruptGate;
    int_GeneralFault->selector = 0x08;

    IDTDescriptorEntry* int_Keyboard = (IDTDescriptorEntry *)(idtr.offset + 0x21 * sizeof(IDTDescriptorEntry));
    int_Keyboard->setOffset((uint64_t)KeyboardInterrupt_Handler);
    int_Keyboard->types_attributes = IDT_TA_InterruptGate;
    int_Keyboard->selector = 0x08;

    IDTDescriptorEntry* int_Mouse = (IDTDescriptorEntry *)(idtr.offset + 0x22 * sizeof(IDTDescriptorEntry));
    int_Mouse->setOffset((uint64_t)MouseInterrupt_Handler);
    int_Mouse->types_attributes = IDT_TA_InterruptGate;
    int_Mouse->selector = 0x08;

    asm("lidt %0" : : "m" (idtr));

    RemapPIC();

    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);
    asm("sti");
}

void InitialiseXSDT(RSDP_Ext* rsdp_ext) {
    GlobalRenderer->SetCursor(0, 0);

    if(rsdp_ext->rsdp.revision != 2) {
        GlobalRenderer->PrintLn("Unable to initialise XSDT");
    }

    SDTHeader* xsdt = (SDTHeader *) rsdp_ext->xsdt_address;
    GlobalRenderer->PrintLn(to_string(rsdp_ext->xsdt_address));

    uint64_t entries = (xsdt->Length - sizeof(SDTHeader)) / 8;
    GlobalRenderer->Print(to_string(entries));
    GlobalRenderer->PrintLn(" entries in XSDT");

    GlobalRenderer->PrintLn(to_string(rsdp_ext->xsdt_address));
    GlobalRenderer->PrintLn(to_string((uint64_t)rsdp_ext->rsdp.revision));
    GlobalRenderer->PrintChar(rsdp_ext->rsdp.signature[0]);
    GlobalRenderer->PrintChar(rsdp_ext->rsdp.signature[1]);
    GlobalRenderer->PrintChar(rsdp_ext->rsdp.signature[2]);
    GlobalRenderer->PrintChar(rsdp_ext->rsdp.signature[3]);
    GlobalRenderer->PrintChar(rsdp_ext->rsdp.signature[4]);
    GlobalRenderer->PrintChar(rsdp_ext->rsdp.signature[5]);
    GlobalRenderer->PrintChar(rsdp_ext->rsdp.signature[6]);
    GlobalRenderer->PrintChar(rsdp_ext->rsdp.signature[7]);
    GlobalRenderer->NewLine();

    GlobalRenderer->PrintChar(xsdt->signature[0]);
    GlobalRenderer->PrintChar(xsdt->signature[1]);
    GlobalRenderer->PrintChar(xsdt->signature[2]);
    GlobalRenderer->PrintChar(xsdt->signature[3]);
    
}

BasicRenderer r = BasicRenderer(NULL, NULL);
KernelInfo InitialiseKernel(BootInfo* bootInfo) {
    r = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);
    GlobalRenderer = &r;


    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t) &DefaultGDT;
    LoadGDT(&gdtDescriptor);
    PrepareMemory(bootInfo);
    PrepareInterrupts(); 
    //Clear screen
    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);
    InitialiseXSDT(bootInfo->rsdp_ext);

    return kernelInfo;
}