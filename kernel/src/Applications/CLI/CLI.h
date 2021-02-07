#pragma once
#include "../../Graphics/BasicRenderer.h"
#include "../../string.h"

class CLI {
    public:
    void start();
    void execute(char* command, ...);

    private:
    void requestInput();
};