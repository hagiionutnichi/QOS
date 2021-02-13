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
    uint32_t MouseCursorBuffer[16 * 16];
    uint32_t MouseCursorBufferAfter[16 * 16];
    void PutChar(unsigned int colour, char chr, unsigned int xOff, unsigned int yOff);
    void PrintChar(char chr, unsigned int colour = 0xffffffff);
    void PutPixel(unsigned int colour, unsigned int x, unsigned int y);
    void Print(const char* str, unsigned int colour = 0xffffffff);
    void PrintLn(const char* str, unsigned int colour = 0xffffffff);
    void DrawBMP(BMP_IMAGE* image, unsigned int xOff, unsigned int yOff);
    void SetCursor(unsigned int x, unsigned int y);
    uint32_t GetPix(uint32_t x, uint32_t y);
    void Clear(uint32_t colour);
    void NewLine();
    void Backspace();
    void SetClearColour(uint32_t colour);
    void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour);
    void ClearMouseCursor(uint8_t* mouseCursor, Point position);
    bool MouseDrawn;
    void SetPrimaryColour(uint32_t colour);
    void SetSecondaryColour(uint32_t colour);
    uint32_t GetPrimaryColour();
    uint32_t GetSecondaryColour();

    private:
    uint32_t ClearColour = 0;
    uint32_t PrimaryColour = 0;
    uint32_t SecondaryColour = 0;
};

extern BasicRenderer* GlobalRenderer;