#include "kernelUtil.h"
#include "GDT/gdt.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Interrupts.h"
#include "io.h"
#include "Keyboard/mouse.h"

KernelInfo kernelInfo;

void PrepareMemory(BootInfo* bootInfo) {
    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    g_PageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t+= 0x1000){
        g_PageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;

    //Lock the page in which the Framebuffer resides
    GlobalAllocator.LockPages((void*)fbBase, fbSize/0x1000 + 1);

    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
        g_PageTableManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

    kernelInfo.pageTableManager = &g_PageTableManager;
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

    IDTDescriptorEntry* int_Mouse = (IDTDescriptorEntry *)(idtr.offset + 0x2C * sizeof(IDTDescriptorEntry));
    int_Mouse->setOffset((uint64_t)MouseInterrupt_Handler);
    int_Mouse->types_attributes = IDT_TA_InterruptGate;
    int_Mouse->selector = 0x08;

    asm("lidt %0" : : "m" (idtr));

    RemapPIC();
}

BasicRenderer r = BasicRenderer(NULL, NULL);
KernelInfo InitialiseKernel(BootInfo* bootInfo) {
    r = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);
    GlobalRenderer = &r;
    GlobalRenderer->SetCursor(0, 0);

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t) &DefaultGDT;
    LoadGDT(&gdtDescriptor);

    PrepareMemory(bootInfo);

    //Clear screen
    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);

    PrepareInterrupts(); 
    InitPS2Mouse();
    outb(PIC1_DATA, 0b11111001);
    outb(PIC2_DATA, 0b11101111);
    asm("sti");

    InitialiseXSDT(bootInfo->rsdp_ext);

    return kernelInfo;
}