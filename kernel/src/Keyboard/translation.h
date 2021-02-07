#pragma once
#include <stdint.h>

namespace QWERTY_US_MAC {
    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}