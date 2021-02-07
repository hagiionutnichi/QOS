#include "keyboard.h"

bool isLeftShiftPressed;
bool isRightShiftPressed;
void HandleKeyboard(uint8_t scancode) {
    switch(scancode) {
        case LEFT_SHIFT:
            isLeftShiftPressed = true;
            break;
        case LEFT_SHIFT + 0x80:
            isLeftShiftPressed = false;
            break;
        case RIGHT_SHIFT:
            isRightShiftPressed = true;
            break;
        case RIGHT_SHIFT + 0x80:
            isRightShiftPressed = false;
            break;
        case ENTER:
            GlobalRenderer->NewLine();
            break;
        case BACKSPACE:
            GlobalRenderer->Backspace();
            break;
        default:
            break;
    }
    char result = QWERTY_US_MAC::Translate(scancode, false);
    if(result != 0)
        GlobalRenderer->PrintChar(result);
}