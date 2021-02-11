#pragma once
#include "../../Graphics/BasicRenderer.h"
#include "../../string.h"
#include "../../numstring.h"
#include "../../ACPI/acpi.h"
#include "../../kernelUtil.h"

class CLI {
    public:
    void start();
    void execute(char* command, ...);
    void KeyHandle(char c);
    void Backspace();
    void SetBootInfo(BootInfo* bootInfo);

    private:
    void requestInput();
    BootInfo* bootInfo;
};

extern CLI* GlobalCLI;