#include "Interrupts.h"
#include "../Panic/panic.h"

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame) {
    Panic("Page Fault detected");
    while(true);
}