#include "CLI.h"
#include <cstdarg>

void CLI::start() {
    GlobalRenderer->Clear(0x00000000);
    GlobalRenderer->SetCursor(0, 0);
    requestInput();
}

void CLI::execute(char* command, ...) {
    va_list args;
    va_start(args, command);
    if(strcmp(command, "albie") == 0) {
        BMP_IMAGE* albie = va_arg(args, BMP_IMAGE*);
        GlobalRenderer->DrawBMP(albie, 0, 0);
    }
    va_end(args);
}

void CLI::requestInput() {
    if(GlobalRenderer->CursorPosition.X != 0) {
        GlobalRenderer->NewLine();
    }
}