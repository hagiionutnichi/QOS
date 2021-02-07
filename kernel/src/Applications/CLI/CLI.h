#pragma once
#include "../../Graphics/BasicRenderer.h"
#include "../../string.h"

class CLI {
    public:
    void start();

    private:
    void requestInput();
    void execute(char* command);
};