#include "CLI.h"
#include <cstdarg>

CLI* GlobalCLI = NULL;
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
        GlobalRenderer->PrintLn("Available Commands:");
        GlobalRenderer->PrintLn("-------------------");
        GlobalRenderer->PrintLn("albie - prints a cute message");
        GlobalRenderer->PrintLn("clear <R 0-255> <G 0-255> <B 0-255> - clears the screen with the specified colour. Each colour defaults to 0.");
        GlobalRenderer->PrintLn("ACPI - lists ACPI Tables");
        GlobalRenderer->Print("help - lists available commands");
    } else if(begins(command, "clear")) {
        //If no argument was passed, clear with black
        if(strlen(command) == strlen("clear")) {
            GlobalRenderer->Clear(0x00000000);
        } else {
            uint32_t r = strargval(command, 1);
            uint32_t g = strargval(command, 2);
            uint32_t b = strargval(command, 3);
            uint32_t col = (0xFF << 24) + (r << 16) + (g << 8) + (b);
            GlobalRenderer->Clear(col);

            GlobalRenderer->Print("Cleared with ");
            GlobalRenderer->Print(to_hstring((uint32_t) col));
            // GlobalRenderer->Print(" at counter ");
            // GlobalRenderer->Print(to_string((uint64_t) counter));
        }

    } else if (strcmp(command, "ACPI") == 0) {
        if(bootInfo) {
            InitialiseXSDT(bootInfo->rsdp_ext);
        } else {
            GlobalRenderer->PrintLn("Boot info has not been set in CLI");
        }
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

void CLI::SetBootInfo(BootInfo* bootInfo) {
    this->bootInfo = bootInfo;
}