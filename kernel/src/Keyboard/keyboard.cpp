#include "keyboard.h"

void HandleKeyboard(uint8_t scancode) {
    char result = QWERTY_US_MAC::Translate(scancode, false);
    if(result != 0)
        GlobalRenderer->PrintChar(result);
}