#include "CLI.h"

void CLI::start() {
    GlobalRenderer->Clear(0x00000000);
    GlobalRenderer->SetCursor(0, 0);
    if(strcmp("abc", "abc") == 0) {
        GlobalRenderer->Print("abc");
    }
}

void CLI::execute(char* command) {

}

void CLI::requestInput() {
    if(GlobalRenderer->CursorPosition.X != 0) {
        GlobalRenderer->NewLine();
    }
}