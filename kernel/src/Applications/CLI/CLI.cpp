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
        GlobalRenderer->Print("Available Commands:");
        GlobalRenderer->NewLine();
        GlobalRenderer->Print("-------------------");
        GlobalRenderer->NewLine();
        GlobalRenderer->Print("albie - prints a cute message");
        GlobalRenderer->NewLine();
        GlobalRenderer->Print("clear <decimal colour - default black> - clears the screen with the specified colour. The format of the colour is 0xAARRGGBB, and it needs to be converted into decimal");
        GlobalRenderer->NewLine();
        GlobalRenderer->Print("help - lists available commands");
    } else if(begins(command, "clear")) {
        char *str = strtok(command, " ");
        size_t counter = 0;
		while(str){
            counter++;
            //If it found an argument, clear with it and ignore the rest
            if(counter == 2) {
                uint64_t num = myAtoi(str);
                GlobalRenderer->Clear(num);
                // GlobalRenderer->Print("Cleared with ");
                // GlobalRenderer->Print(to_string((uint64_t) num));
                // GlobalRenderer->Print(" at counter ");
                // GlobalRenderer->Print(to_string((uint64_t) counter));
                break;
            }
			str = strtok(0, " ");
		}
        //If no argument was passed, clear with black
        if(counter == 1) {
            GlobalRenderer->Clear(0x00000000);
            // GlobalRenderer->Print("Cleared with 0");
        }

        GlobalRenderer->SetCursor(0, 0);
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