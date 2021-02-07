#include "keyboard.h"

bool isLeftShiftPressed;
bool isRightShiftPressed;
void HandleKeyboard(uint8_t scancode) {
    switch(scancode) {
        case LEFT_SHIFT:
            isLeftShiftPressed = true;
            return;
        case LEFT_SHIFT + 0x80:
            isLeftShiftPressed = false;
            return;
        case RIGHT_SHIFT:
            isRightShiftPressed = true;
            return;
        case RIGHT_SHIFT + 0x80:
            isRightShiftPressed = false;
            return;
        case ENTER:
            GlobalRenderer->NewLine();
            return;
        case BACKSPACE:
            GlobalRenderer->Backspace();
            return;
        case SPACEBAR:
            GlobalRenderer->PrintChar(' ');
            return;
        default:
            break;
    }
    char result = QWERTY_US_MAC::Translate(scancode, isLeftShiftPressed || isRightShiftPressed);
    if(result != 0) {
        GlobalRenderer->PrintChar(result);
        GlobalCLI->KeyHandle(result);
    }
}