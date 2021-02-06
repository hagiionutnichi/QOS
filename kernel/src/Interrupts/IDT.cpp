#include "IDT.h"


void IDTDescriptorEntry::setOffset(uint64_t offset){
    offset0 = (uint16_t)(offset & 0x000000000000ffff);
    offset1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    offset2 = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}
uint64_t IDTDescriptorEntry::getOffset(){
    return (uint64_t)((offset2 << 32) | (offset1 << 16) | offset0);
}