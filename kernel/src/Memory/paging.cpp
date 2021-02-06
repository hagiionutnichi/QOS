#include "paging.h"

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool value){
    uint64_t bitSelector = (uint64_t)1 << flag;
    Value &= ~bitSelector;
    if(value) Value |= bitSelector;
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag){
    uint64_t bitSelector = (uint64_t)1 << flag;
    return Value & bitSelector > 0 ? true : false;
}

void PageDirectoryEntry::SetAddress(uint64_t address){
    address &= 0x000000ffffffffff;
    Value &= 0xfff0000000000fff;
    Value |= address << 12;
}

uint64_t PageDirectoryEntry::GetAddress(){
    return (Value & 0x000ffffffffff000) >> 12;
}