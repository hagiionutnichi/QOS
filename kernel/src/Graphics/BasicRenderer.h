#pragma once
#include "Framebuffer.h"
#include "../FileFormats/PSF_1.h"
#include "../FileFormats/BMP.h"
#include "../math.h"
#include <stdint.h>
#include "../Memory/memory.h"
#include "../numstring.h"

class BasicRenderer {
    public:
    BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* font);
    Point CursorPosition;
    FrameBuffer* Framebuffer;
    PSF1_FONT* font;
    void PutChar(unsigned int colour, char chr, unsigned int xOff, unsigned int yOff);
    void PrintChar(char chr, unsigned int colour = 0xffffffff);
    void PutPixel(unsigned int colour, unsigned int x, unsigned int y);
    void Print(const char* str, unsigned int colour = 0xffffffff);
    void PrintLn(const char* str, unsigned int colour = 0xffffffff);
    void DrawBMP(BMP_IMAGE* image, unsigned int xOff, unsigned int yOff);
    void SetCursor(unsigned int x, unsigned int y);
    void Clear(uint32_t colour);
    void NewLine();
    void Backspace();
    void SetClearColour(uint32_t colour);

    private:
    uint32_t ClearColour = 0;
};

extern BasicRenderer* GlobalRenderer;