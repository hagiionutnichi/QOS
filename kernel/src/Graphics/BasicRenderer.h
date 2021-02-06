#pragma once
#include "Framebuffer.h"
#include "../FileFormats/PSF_1.h"
#include "../FileFormats/BMP.h"
#include "../math.h"
#include <stdint.h>

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
    void DrawBMP(BMP_IMAGE* image, unsigned int xOff, unsigned int yOff);
    void SetCursor(unsigned int x, unsigned int y);
    void Clear(uint32_t colour);
    void NewLine();
};

extern BasicRenderer* GlobalRenderer;