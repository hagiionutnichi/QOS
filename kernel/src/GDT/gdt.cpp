#include "gdt.h"

__attribute__((aligned(0x1000)))
GDT DefaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0}, //Null
    {0, 0, 0, 0x9a, 0xa0, 0}, //Kernel Code
    {0, 0, 0, 0x92, 0xa0, 0}, //Kernel Data
    {0, 0, 0, 0x00, 0x00, 0}, //User Null
    {0, 0, 0, 0x9a, 0xa0, 0}, //User Code
    {0, 0, 0, 0x92, 0xa0, 0}, //User Data
};