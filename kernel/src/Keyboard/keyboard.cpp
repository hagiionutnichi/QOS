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
            // GlobalRenderer->NewLine();
            // if(GlobalCLI)
            //     GlobalCLI->KeyHandle('\n');
            return;
        case BACKSPACE:
            // GlobalRenderer->Backspace();
            // if(GlobalCLI)
            //     GlobalCLI->Backspace();
            return;
        case SPACEBAR:
            // GlobalRenderer->PrintChar(' ');
            // if(GlobalCLI)
            //     GlobalCLI->KeyHandle(' ');
            return;
        default:
            break;
    }
    char result = QWERTY_US_MAC::Translate(scancode, isLeftShiftPressed || isRightShiftPressed);
    if(result != 0) {
        // GlobalRenderer->PrintChar(result);
        // if(GlobalCLI)
        //     GlobalCLI->KeyHandle(result);
    }
}