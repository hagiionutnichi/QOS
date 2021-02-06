#include "panic.h"
#include "../Graphics/BasicRenderer.h"

void Panic(const char* panicMessage) {
    GlobalRenderer->Clear(0x008b0000);
    GlobalRenderer->SetCursor(0, 0);
    GlobalRenderer->Print("Kernel Panic");
    GlobalRenderer->NewLine();
    GlobalRenderer->Print(panicMessage);
}