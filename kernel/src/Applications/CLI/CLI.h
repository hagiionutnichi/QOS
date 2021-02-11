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
    void SetBootInfo(struct BootInfo* bootInfo);

    private:
    void requestInput();
    struct BootInfo* bootInfo;
};

extern CLI* GlobalCLI;