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
    } else if(strcmp(command, "help") == 0) {
        GlobalRenderer->Print("Available Commands:");
        GlobalRenderer->NewLine();
        GlobalRenderer->Print("-------------------");
        GlobalRenderer->NewLine();
        GlobalRenderer->Print("albie - prints a cute message");
        GlobalRenderer->NewLine();
        GlobalRenderer->Print("help - lists available commands");
        GlobalRenderer->NewLine();
    } else {
        GlobalRenderer->Print("Unknown Command '");
        GlobalRenderer->Print(command);
        GlobalRenderer->Print("'.Use 'help' to list available commands.");
    }
    va_end(args);
    requestInput();
}

void CLI::requestInput() {
    bufferIndex = 0;
    if(GlobalRenderer->CursorPosition.X != 0) {
        GlobalRenderer->NewLine();
    }
    GlobalRenderer->PrintChar('>');
    GlobalRenderer->PrintChar(' ');
}

void CLI::KeyHandle(char c) {
    if(c == '\n') {
        GlobalRenderer->NewLine();
        commandBuffer[bufferIndex++] = '\0';
        execute(commandBuffer);
        bufferIndex = 0;
    } else {
        GlobalRenderer->PrintChar(c);
        commandBuffer[bufferIndex++] = c;
    }
}

void CLI::Backspace() {
    if(bufferIndex == 0) { return; }
    GlobalRenderer->Backspace();
    bufferIndex--;
    if(bufferIndex <= 0) bufferIndex = 0;
}