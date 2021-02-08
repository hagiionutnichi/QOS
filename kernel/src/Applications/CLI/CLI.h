#pragma once
#include "../../Graphics/BasicRenderer.h"
#include "../../string.h"
#include "../../numstring.h"

class CLI {
    public:
    void start();
    void execute(char* command, ...);
    void KeyHandle(char c);
    void Backspace();

    private:
    void requestInput();
};

extern CLI* GlobalCLI;