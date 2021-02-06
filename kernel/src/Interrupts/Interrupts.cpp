#include "Interrupts.h"
#include "../Graphics/BasicRenderer.h"
#include "../Panic/panic.h"
#include "../io.h"
#include <stdint.h>

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame) {
    Panic("Page Fault detected");
    while(true);
}

__attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame* frame) {
    Panic("Double Fault detected");
    while(true);
}
__attribute__((interrupt)) void GeneralProtectionFault_Handler(struct interrupt_frame* frame) {
    Panic("General Fault detected");
    while(true);
}

__attribute__((interrupt)) void KeyboardInterrupt_Handler(struct interrupt_frame* frame) {
    GlobalRenderer->Print("Keyboard");
}

void RemapPIC() {
    uint8_t a1, a2;
    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086); 
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait(); 

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
}