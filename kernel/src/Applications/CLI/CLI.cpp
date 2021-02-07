#include "CLI.h"
#include <cstdarg>

CLI* GlobalCLI;
char* commandBuffer;
uint8_t bufferIndex;
void CLI::start() {
    commandBuffer = (char*) GlobalAllocator.RequestPage();
    bufferIndex = 0;
    GlobalRenderer->Clear(0x00000000);
    GlobalRenderer->SetCursor(0, 0);
    requestInput();
}

void CLI::execute(char* command, ...) {
    va_list args;
    va_start(args, command);
    if(strcmp(command, "albie") == 0) {
        // BMP_IMAGE* albie = va_arg(args, BMP_IMAGE*);
        // GlobalRenderer->DrawBMP(albie, 0, 0);
        GlobalRenderer->Print("My name's albie and you can't beat it");
    }
    va_end(args);
    GlobalRenderer->NewLine();
}

void CLI::requestInput() {
    if(GlobalRenderer->CursorPosition.X != 0) {
        GlobalRenderer->NewLine();
    }
    GlobalRenderer->PrintChar('>');
}

void CLI::KeyHandle(char c) {
    if(c == '\n') {

        GlobalRenderer->Print("Executing ");
        GlobalRenderer->Print(commandBuffer);
        execute(commandBuffer);
        bufferIndex = 0;
    } else {
        GlobalRenderer->Print("Adding");
        commandBuffer[bufferIndex++] = c;
    }
}