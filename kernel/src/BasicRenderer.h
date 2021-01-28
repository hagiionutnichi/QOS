#pragma once
#include "Framebuffer.h"
#include "PSF_1.h"
#include "BMP.h"
#include "math.h"

class BasicRenderer {
    public:
    BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* font);
    Point CursorPosition;
    FrameBuffer* Framebuffer;
    PSF1_FONT* font;
    void PutChar(unsigned int colour, char chr, unsigned int xOff, unsigned int yOff);
    void PutPixel(unsigned int colour, unsigned int x, unsigned int y);
    void Print(unsigned int colour, char* str);
    void DrawBMP(BMP_IMAGE* image, unsigned int xOff, unsigned int yOff);
    void SetCursor(unsigned int x, unsigned int y);
};