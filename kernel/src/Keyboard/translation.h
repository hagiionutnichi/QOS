#pragma once
#include <stdint.h>

namespace QWERTY_US_MAC {
    #define LEFT_SHIFT      0x2A
    #define RIGHT_SHIFT     0x36
    #define ENTER           0x1C 
    #define BACKSPACE       0x0E
    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}